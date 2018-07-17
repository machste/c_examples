/* XSI shared memory facility
 *
 * Helpful tools:
 *  * ipcs
 *  * ipcrm
 * 
 * http://pubs.opengroup.org/onlinepubs/009696899/basedefs/sys/shm.h.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>


#define SHM_SIZE 1024


int main(int argc, char *argv[])
{
    // Check the arguments
    if (argc > 2) {
        fprintf(stderr, "usage: shm [data]\n");
        exit(1);
    }
    // Make the IPC key and use shm program as key file
    key_t key = ftok("shm", 'R');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    printf("Key: %i\n", key);
    /* Set shared mermory flags
     *
     * Bit      Meaning
     * ----     -------
     * 0400     Read by user.
     * 0200     Write by user.
     * 0040     Read by group.
     * 0020     Write by group.
     * 0004     Read by others.
     * 0002     Write by others.
     *
     * IPC_CREAT    Create a new segment.  If this flag is not used, then
     *              shmget() will find the segment associated with key and
     *              check to see if the user has permission to access the
     *              segment.
     */
    int shmflg = 0644 | IPC_CREAT;
    // Get the shared memory identifier (shmid) associated with key
    int shmid = shmget(key, SHM_SIZE, shmflg);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    printf("ID: %i\n", shmid);
    // Attach to the segment to get a pointer to it
    char *data = shmat(shmid, NULL, 0);
    if ((void *)data < NULL) {
        perror("shmat");
        exit(1);
    }
    printf("Segement: %p\n", data);
    // Modify the data of the shared memory segment, if new data are given
    if (argc == 2) {
        strncpy(data, argv[1], SHM_SIZE);
    }
    printf("Data: '%s'\n", data);
    // Detach from the segment
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }
    return 0;
}
