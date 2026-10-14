#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf
{
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main()
{
    int msqid;
    key_t key;
    message_buf sbuf;
    size_t buf_length;

    // Usamos la misma clave 1234 para conectarnos a la misma cola
    key = 1234;

    // msgget con IPC_CREAT: crea la cola si no existe, o la abre si ya fue creada
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    // Preparamos el mensaje
    sbuf.mtype = 1; // Debe coincidir con el mtype = 1 que espera msgrcv
    strcpy(sbuf.mtext, "Are you hearing me?");
    buf_length = strlen(sbuf.mtext) + 1; // Incluye el carácter nulo '\0'

    // Enviamos el mensaje a la cola
    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    printf("Mensaje enviado con exito: \"%s\"\n", sbuf.mtext);
    exit(0);
}
