#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {

    key_t key;
    int msgid;
    struct msg_buffer message;

    // Generate unique key
    key = ftok("progfile", 65);

    // Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    message.msg_type = 1;

    printf("Enter a string: ");
    scanf("%s", message.msg_text);

    // Send message
    msgsnd(msgid, &message, sizeof(message.msg_text), 0);

    printf("Message sent: %s\n", message.msg_text);

    return 0;
}
