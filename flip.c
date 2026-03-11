#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <ctype.h>

int main()
{
    int shmid;
    char *str;

    // create shared memory
    shmid = shmget(1234, 100, 0666 | IPC_CREAT);

    // attach shared memory
    str = (char*) shmat(shmid, NULL, 0);

    printf("Enter a string: ");
    fgets(str, 100, stdin);

    // flip case
    for(int i = 0; str[i] != '\0'; i++)
    {
        if(isupper(str[i]))
            str[i] = tolower(str[i]);
        else if(islower(str[i]))
            str[i] = toupper(str[i]);
    }

    printf("Case flipped string: %s", str);

    // detach shared memory
    shmdt(str);

    // delete shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
