#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>

#include "message.h"

int main (int argc, char **argv)
{
    // Make the IPC key and use the 'common.h' header file as key file
    key_t key = ftok("message.h", 'R');
    printf("key: 0x%x\n", key);
    // Create message queue
    int msq_id = msgget(key, 0666 | IPC_CREAT);
    if (msq_id < 0) {
        perror("msgget");
        exit(1);
    } else {
        printf("msq_id: %i\n", msq_id);
    }
    // Receive a message type 1
    Message msg;
    if (msgrcv(msq_id, &msg, MESSAGE_MAX_SIZE, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    //Print the answer.
    printf("%s\n", msg.data);
    exit(0);
}
