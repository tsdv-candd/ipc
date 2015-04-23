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
/* 	initialises the log service client,
	returns a service id (queue ID of the receive queue).
*/

int logMessage(int serviceId, char* message);
/* 	logs the message message to the log service serviceID
	serviceId 	-> queue ID of the receive queue
	message		-> message will be send to the queue (queue ID = serviceID)
	Note: If the length of the message greater than MSGCHARS,
	Only MSGCHARS character is sent to the message queue.
*/

#endif /* ifndef LOGSERVICE_H */
