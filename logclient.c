/* logclient.c -- implements a simple log service client */
#include "logservice.h"

int main(int argc, char**argv)
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct message sbuf;
    size_t buf_length;
    if(argc < 2) {
        printf("Please input message to send to server\n");
        printf("%s \"message\"\n", argv[0]);
        exit (1);
    }
    printf("Make me useful too!\n");
    key = KEY;

    msqid = logServiceInit();
    if(msqid == -1 ) {
        printf("logServiceInit failed[%d]\n", msqid);
    }

    logMessage(msqid, argv[1]);


    return 0;
}

