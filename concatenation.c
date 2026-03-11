#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
    int shmid;
    char *str;
    char str2[50];

    // create shared memory
    shmid = shmget(1234, 100, 0666 | IPC_CREAT);

    // attach shared memory
    str = (char*) shmat(shmid, NULL, 0);

    printf("Enter first string: ");
    fgets(str, 50, stdin);

    printf("Enter second string: ");
    fgets(str2, 50, stdin);

    // remove newline
    str[strcspn(str, "\n")] = '\0';
    str2[strcspn(str2, "\n")] = '\0';

    // concatenate strings
    strcat(str, str2);

    printf("Concatenated string: %s\n", str);

    // detach shared memory
    shmdt(str);

    // remove shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
