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

    // Generate same key
    key = ftok("progfile", 65);

    // Access message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Receive message
    msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);

    char str[100];
    strcpy(str, message.msg_text);

    int i, len, flag = 0;

    len = strlen(str);

    for(i = 0; i < len/2; i++)
    {
        if(str[i] != str[len-i-1])
        {
            flag = 1;
            break;
        }
    }

    if(flag == 0)
        printf("'%s' is a Palindrome\n", str);
    else
        printf("'%s' is NOT a Palindrome\n", str);

    // Remove message queue
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
