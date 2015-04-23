/* logserver.c -- implementation of the log server */
#include <signal.h>
#include "logservice.h"

extern int global_variable;

int main()
{
    int msqid;
    key_t key;
    struct message rbuf;

    key = KEY;

    printf("Please make me useful!\n");
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    do {
        memset(rbuf.message, 0, MSGCHARS + 1);

        /*
         * Receive an answer of message type 1.
         */
        if (msgrcv(msqid, &rbuf, MSGCHARS, global_variable, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        /*
         * Print the answer.
         */
        printf("Receive message %s from process PID [%d]\n", rbuf.message, rbuf.type);
    } while (1);

    return 0;
}
