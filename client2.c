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

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget client2");
        exit(1);
    }

    // Enviar petición al Servidor (mtype = 21)
    buf.mtype = 21;
    strcpy(buf.mtext, "Peticion enviada desde Cliente 2");
    msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);

    // Escuchar ÚNICAMENTE en su canal privado (mtype = 20)
    if (msgrcv(msqid, &buf, MSGSZ, 20, 0) < 0) {
        perror("msgrcv client2");
        exit(1);
    }

    printf("Cliente 2 recibio respuesta: %s\n", buf.mtext);
    return 0;
}
