/*
Compile este programa asi

gcc message_send.c -o send
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

/**
Esta estructura define los elementos que tiene un mensaje
*/
typedef struct msgbuf
{
    long mtype;
    char mtext[MSGSZ]; // Un mensaje de longitud MSGSZ
} message_buf;

main()
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;

    key = 1234;

    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }

    sbuf.mtype = 1;

    (void) strcpy(sbuf.mtext, "Are you hearing me?");

    buf_length = strlen(sbuf.mtext) + 1 ;

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       perror("msgsnd");
        exit(1);
    }

    printf("Message : \"%s\" Sent\n", sbuf.mtext);
    
    exit(0);
}


