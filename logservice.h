/* logservice.h -- definitions for the  log service */
#ifndef LOGSERVICE_H /* prevent multiple inclusion */
#define LOGSERVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* key for the queue */
#define KEY ftok("logservice.h", 'a')

extern int client_pid;

/* message structure */
#define MSGCHARS 255
/* MSGCHARS is the number of characters in the message!*/
struct message
{
    long type;
    char message[MSGCHARS+1]; /* allow it to be a string! */
};

/* function prototypes */
int logServiceInit();
/* initialises the log service client, returns a service id
 * Return : returning an id if successful, and -1 on error
 * NOTE: This function should not attempt to create the message queue,
 * only attach it to the process, nor should it take an argument to
 * specify the queue, there shall be only one log queue
 */

int logMessage(int serviceId, char* message);
/* logs the message message to the log service serviceID
 * -> serviceId : ID of the receiving message queue.
 * -> message : pointer to the message will be sent.
 * Return : return 0 on success and -1 on error.
 * NOTE: If the message is too long (i.e. longer than MSGCHARS),
 * Only sending MSGCHARS characters to the server, remaining characters will be through away.
 */
#endif /* ifndef LOGSERVICE_H */
