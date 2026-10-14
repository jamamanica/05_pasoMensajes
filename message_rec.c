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

    // Crear o abrir la cola de mensajes
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // Paso 1: Enviar "Are you hearing me?" (mtype = 1)
    buf.mtype = 1;
    strcpy(buf.mtext, "Are you hearing me?");
    if (msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0) < 0) {
        perror("msgsnd P1");
        exit(1);
    }
    printf("Process 1 envio: \"%s\"\n", buf.mtext);

    // Paso 2: Esperar respuesta de Process 2 (mtype = 2)
    if (msgrcv(msqid, &buf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv P1");
        exit(1);
    }
    printf("Process 1 recibio: \"%s\"\n", buf.mtext);

    // Paso 3: Replicar "I can hear you too" (mtype = 1)
    buf.mtype = 1;
    strcpy(buf.mtext, "I can hear you too");
    if (msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0) < 0) {
        perror("msgsnd P1 replica");
        exit(1);
    }
    printf("Process 1 replico: \"%s\"\n", buf.mtext);

    return 0;
}
