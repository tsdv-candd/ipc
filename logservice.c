/* logservice.c -- implementation of the log service */
#include "logservice.h"

int logServiceInit()
{
    int id;
    key_t key;
    int msgflg = IPC_CREAT | 0666;

    key = KEY;

    fprintf(stderr, "logServiceInit: Start(%#lx,%#o)\n", key, msgflg);

    /*
     * Get the message queue id for the
     * key = , which was created by
     * the server.
     */
    if ((id = msgget(key, msgflg )) < 0) {
        perror("logServiceInit: msgget");
        return (-1);
    }

    return id;
}

int logMessage(int serviceId, char *message)
{
    int rv;
    size_t sbuf_len;
    struct message sbuf;

    /*
     * Create message to send.
     */
    memset(sbuf.message, 0, MSGCHARS + 1);

    strcpy(sbuf.message, message);
    sbuf_len = strlen(sbuf.message);
    //sbuf.type = getpid();
    sbuf.type = 1;

    /*
     * Send a message.
     */
    if ((rv = msgsnd(serviceId, &sbuf, sbuf_len, IPC_NOWAIT)) < 0) {
        printf ("%d, %d, %s, %d\n", serviceId, sbuf.type, sbuf.message, sbuf_len);
        perror("logMessage: msgsnd");
        return (-1);
        //exit(1);
    }

    printf("Message: \"%s\" Sent\n", sbuf.message);
    return rv;
}
