#include <sys/ipc.h>
#include <sys/msg.h>
#include "notification.h"

void send_notification(int msg_id, int pid, int status) {
    NOTIFICATION_STATUS notification;

    notification.type = pid;
    notification.status = status;
    status = msgsnd(msg_id, &notification, sizeof(notification) - sizeof(long), 0); // message send
    // tammanho da estrutura - o type (sizeof(notification) - sizeof(long))
}