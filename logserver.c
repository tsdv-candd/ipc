/* logserver.c -- implementation of the log server */
#include <signal.h>
#include <errno.h>
#include "logservice.h"

/*
 * Client process ID get by logservice
 * Refer here to make sure get the message from specific client
 */
extern int client_pid;
static volatile int running = 1;

/*
 * Handler the
 */
void ctl_and_C_handler(int dummy) {
    running = 0;
}

int main()
{
    int msqid;
    int result = 0;
    /*
     * For creating message queue
     */
    int msgflg = IPC_CREAT | 0666;
    struct message rbuf;

    /*
     * For handling Ctr + C key
     */
    signal(SIGINT, ctl_and_C_handler);

    printf("Please make me useful!\n");

    /*
     * Create the message queue
     */
    if ((msqid = msgget(KEY, msgflg)) < 0) {
        perror("SERVER: ERROR msgget");
        exit(1);
    }

    /*
     * Get message from client and display
     */
    printf("Start receive the message from client!\n");
    do {
        /*
         * Make sure the receiving buffer is empty.
         */
        memset(rbuf.message, 0, MSGCHARS + 1);

        /*
         * Receive answer of message with the type is client_pid (process ID)
         * Block until the message is sent from client.
         */
        if (msgrcv(msqid, &rbuf, MSGCHARS, client_pid, 0) < 0) {
            perror("SERVER: ERROR msgrcv");
            break;
            //exit(1);
        }

        /*
         * Print the answer to output terminal
         */
        printf("Receive message \"%s\" from the client which PID = [%ld]\n", rbuf.message, rbuf.type);
    } while (running);

    result = msgctl(msqid, IPC_RMID, NULL);
    if (result < 0) {
        perror( strerror(errno) );
        printf("msgctl (return queue) failed, result=%d\n", result);
        return 1;
    }

    printf("Stop server program\n");
    return 0;
}
