#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define MESSAGE_MAX_SIZE 128

typedef struct {
    long mtype;
    char data[MESSAGE_MAX_SIZE];
} Message;

#endif /* _MESSAGE_H_ */
