#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <errno.h>


void usage(void)
{
    fprintf(stderr, "usage: sem [forks] [sem_value]\n");
    exit(1);
}

int sem_lock(int sem_id, unsigned short sem_num)
{
    struct sembuf sem_buf = { .sem_num = sem_num, .sem_op = -1, .sem_flg = 0 };
    return semop(sem_id, &sem_buf, 1);
}

int sem_unlock(int sem_id, unsigned short sem_num)
{
    struct sembuf sem_buf = { .sem_num = sem_num, .sem_op = 1, .sem_flg = 0 };
    return semop(sem_id, &sem_buf, 1);
}


int main (int argc, char **argv)
{
    int forks = 5;
    int sem_value = 1;
    // Check the arguments
    if (argc > 1) {
        forks = atoi(argv[1]);
        if (forks <= 0) {
            fprintf(stderr, "ERR: forks has to be a number > 0\n");
            usage();
        }
    }
    if (argc > 2) {
        sem_value = atoi(argv[2]);
        if (sem_value <= 0) {
            fprintf(stderr, "ERR: sem_value has to be a number > 0\n");
            usage();
        }
    }
    if (argc > 3) {
        usage();
    }
    // Make the IPC key and use sem program as key file
    key_t key = ftok("sem", 'R');
    printf("key=0x%x\n", key);
    // Initialize a shared variable in shared memory
    int shm_id = shmget(key, sizeof(int), 0644 | IPC_CREAT);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }
    // Attach p to shared memory
    int *p = shmat(shm_id, NULL, 0);
    *p = 0;
    printf ("p=%d is allocated in shared memory.\n", *p);
    // Create a semaphore for the shared memory
    int sem_id = semget(key, 1, 0666 | IPC_CREAT);
    if (sem_id < 0) {
        perror ("semget");
        exit(1);
    }
    // Initialize semaphore
    semctl(sem_id, 0, SETVAL, sem_value);
    // Fork child processes
    pid_t pid;
    int i;
    for (i = 0; i < forks; i++) {
        pid = fork();
        // Check for error
        if (pid < 0) {
            printf("Fork error.\n");
        } else if (pid == 0) {
            // Child processes
            break;
        }
    }
    if (pid != 0){
        /* PARENT PROCESS */
        // Wait for all children to exit
        while (waitpid(-1, NULL, 0)) {
            if (errno == ECHILD) {
                break;
            }
        }
        printf ("Parent: All children have exited.\n");
        // Detach and remove shared memory
        shmdt(p);
        shmctl(shm_id, IPC_RMID, 0);
        // Remove semaphore
        semctl(sem_id, 0, IPC_RMID);
        exit(0);
    } else {
        /* CHILD PROCESS */
        sem_lock(sem_id, 0);
        printf("Child(%d) is in critical section.\n", i);
        int x = *p;
        sleep(1);
        *p = x + 1;
        printf("Child(%d) new value of p=%d.\n", i, *p);
        sem_unlock(sem_id, 0);
        exit(0);
    }
}
