#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf buf;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget P2");
        exit(1);
    }

    // Paso 1: Recibir mensaje inicial de Process 1 (mtype = 1)
    if (msgrcv(msqid, &buf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv P2");
        exit(1);
    }
    printf("Process 2 recibio: \"%s\"\n", buf.mtext);

    // Paso 2: Responder "Loud and clear" (mtype = 2)
    buf.mtype = 2;
    strcpy(buf.mtext, "Loud and clear");
    if (msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0) < 0) {
        perror("msgsnd P2");
        exit(1);
    }
    printf("Process 2 respondio: \"%s\"\n", buf.mtext);

    // Paso 3: Recibir réplica final de Process 1 (mtype = 1)
    if (msgrcv(msqid, &buf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv P2 replica");
        exit(1);
    }
    printf("Process 2 recibio replica: \"%s\"\n", buf.mtext);

    return 0;
}
