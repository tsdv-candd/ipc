/* logserver.c -- implementation of the log server */
#include <signal.h>
#include "logservice.h"

extern int client_pid;
static volatile int running = 1;
void intHandler(int dummy) {
    running = 0;
}

int main()
{
    int msqid;
    key_t key;
    struct message rbuf;

    key = KEY;

    signal(SIGINT, intHandler);

    printf("Please make me useful!\n");
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    do {
        memset(rbuf.message, 0, MSGCHARS + 1);

        /*
         * Receive an answer of message type client_pid.
         */
        if (msgrcv(msqid, &rbuf, MSGCHARS, client_pid, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        /*
         * Print the answer.
         */
        printf("Receive message %s from process PID [%d]\n", rbuf.message, rbuf.type);
    } while (running);

    printf("Exit server program\n");
    return 0;
}
