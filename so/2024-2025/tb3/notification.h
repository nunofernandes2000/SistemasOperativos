#ifndef NOTIFICATION_H
#define NOTIFICATION_H

/* define of status for each request */
#define INVALID_DISH         -1
#define REQUESTED             1
#define PREPARATION           2
#define READY                 3

/* structure of a notification from central server and cooks to client */
typedef struct {
    long type; // equal to the pid of the client (corresponde ao pid do cliente)/
    int status; // status of the request, corresponde ao estado */
} NOTIFICATION_STATUS;

void send_notification(int msg_id, int pid, int status);

#endif