/* logserver.c -- implementation of the log server */
#include <signal.h>
#include "logservice.h"

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
        if (msgrcv(msqid, &rbuf, MSGCHARS, 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        /*
         * Print the answer.
         */
        printf("%s\n", rbuf.message);
    } while (1);

    return 0;
}
