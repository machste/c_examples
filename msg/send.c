#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>

#include "message.h"


int main (int argc, char **argv)
{
    // Check the arguments
    if (argc > 2) {
        fprintf(stderr, "usage: send [text]\n");
        exit(1);
    }
    const char *text;
    if (argc == 2) {
        text = argv[1];
    } else {
        text = "Default Message!";
    }
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
    // Construct message
    Message msg;
    size_t msg_size = strlen(text) + 1;
    if (msg_size > MESSAGE_MAX_SIZE) {
        fprintf(stderr, "Text message is too big!\n");
        exit(1);
    }
    msg.mtype = 1;
    strcpy(msg.data, text);
    // Send message
    if (msgsnd(msq_id, &msg, msg_size, IPC_NOWAIT) < 0) {
        printf("Unable to send: %s, (%zu)\n", msg.data, msg_size);
        perror("msgsnd");
        exit(1);
    } else { 
        printf("Message: \"%s\" sent.\n", msg.data);
    }
    exit(0);
}
