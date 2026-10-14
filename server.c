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
    key_t key = 5678;
    message_buf buf;

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget server");
        exit(1);
    }

    printf("Servidor iniciado. Esperando mensajes de los clientes...\n");

    // Recibir solicitud de Cliente 1 (mtype = 11)
    if (msgrcv(msqid, &buf, MSGSZ, 11, 0) >= 0) {
        printf("Servidor recibio de Cliente 1: %s\n", buf.mtext);
        // Responder en el canal privado de Cliente 1 (mtype = 10)
        buf.mtype = 10;
        strcpy(buf.mtext, "Hola Cliente 1, este es un mensaje PRIVADO para ti.");
        msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);
    }

    // Recibir solicitud de Cliente 2 (mtype = 21)
    if (msgrcv(msqid, &buf, MSGSZ, 21, 0) >= 0) {
        printf("Servidor recibio de Cliente 2: %s\n", buf.mtext);
        // Responder en el canal privado de Cliente 2 (mtype = 20)
        buf.mtype = 20;
        strcpy(buf.mtext, "Hola Cliente 2, este es un mensaje PRIVADO para ti.");
        msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);
    }

    printf("Servidor finalizo la atencion.\n");
    return 0;
}
