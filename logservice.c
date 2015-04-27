/* logservice.c -- implementation of the log service */
#include "logservice.h"

int client_pid;

/*
 * logServiceInit : Initialise the message queue to log message to
 * Return : returning an id if successful, and -1 on error
 * NOTE: This function should not attempt to create the message queue,
 * only attach it to the process, nor should it take an argument to
 * specify the queue, there shall be only one log queue
 */
int logServiceInit()
{
    int id;
    /*
     * Not create again the message queue
     */
    int msgflg = 0666;

    fprintf(stderr, "logServiceInit: Start(KEY [%#x],flag[%#o])\n", KEY, msgflg);

    /*
     * Get the message queue id for the
     * which was created by the server.
     * (Mapping by the same KEY)
     */
    if ((id = msgget(KEY, msgflg )) < 0) {
        perror("logServiceInit: msgget");
        return (-1);
    }

    return id;
}

/*
 * logMessage : This function logs the message passed as
 * the string message to the log service id.
 * serviceId : ID of the receiving message queue.
 * message : pointer to the message will be sent.
 * Return : return 0 on success and -1 on error.
 * NOTE: If the message is too long (i.e. longer than MSGCHARS),
 * Only send sending MSGCHARS characters to the server, remaining characters will be through away.
 */
int logMessage(int serviceId, char *message)
{
    int rv = 0;
    size_t sbuf_len;
    struct message sbuf;
    int copy_len = 0;

    /*
     * Create message to send.
     */
    memset(sbuf.message, 0, MSGCHARS + 1);
    copy_len = strlen(message) > MSGCHARS + 1 ? (MSGCHARS) : (strlen(message) + 1);
    strncpy(sbuf.message, message, copy_len);
    sbuf_len = strlen(sbuf.message);

    /*
     * Assign message type as client's ProcessID
     */
    client_pid = getpid();
    sbuf.type = client_pid;

    /*
     * Send a message to the server
     */
    if ((rv = msgsnd(serviceId, &sbuf, sbuf_len, IPC_NOWAIT)) < 0) {
        printf ("%d, %ld, %s, %d\n", serviceId, sbuf.type, sbuf.message, sbuf_len);
        perror("logMessage: msgsnd");
        return (-1);
    }

    printf("Message: \"%s\" is sent\n", sbuf.message);
    return rv;
}
