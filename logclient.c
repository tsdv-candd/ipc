/* logclient.c -- implements a simple log service client */
#include "logservice.h"

int main(int argc, char**argv)
{
    int msqid;
    int result = -1;

    /*
     * User must input the message when launching client
     */
    if(argc < 2) {
        printf("Please input message to send to server\n");
        printf("%s \"message\"\n", argv[0]);
        exit (1);
    }
    printf("Make me useful too!\n");

    /*
     * Attach the message queue
     */
    msqid = logServiceInit();
    if(msqid == -1 ) {
        printf("CLIENT: ERROR logServiceInit failed[%d]\n", msqid);
        exit(1);
    }

    /*
     * Sent the message to the server.
     */
    result = logMessage(msqid, argv[1]);
    if(result == -1 ) {
        printf("CLIENT: ERROR logMessage failed[%d]\n", result);
        exit (1);
    }
    return 0;
}

