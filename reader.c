#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t readCountLock;      // protects readerCount variable
sem_t resourceLock;       // lock for shared resource (reading/writing)
sem_t serviceQueue;       // keeps order so writers get priority

int readerCount = 0;      // number of readers currently reading


void *reader(void *arg)
{
    int id = *(int *)arg;

    sem_wait(&serviceQueue);      // wait in queue
    sem_wait(&readCountLock);     // lock readerCount

    readerCount++;

    if(readerCount == 1)
        sem_wait(&resourceLock);  // first reader locks resource

    sem_post(&readCountLock);
    sem_post(&serviceQueue);

    // Reading section
    printf("Reader %d is reading\n", id);
    sleep(1);
    printf("Reader %d finished reading\n", id);

    sem_wait(&readCountLock);
    readerCount--;

    if(readerCount == 0)
        sem_post(&resourceLock);  // last reader unlocks resource

    sem_post(&readCountLock);

    return NULL;
}


void *writer(void *arg)
{
    int id = *(int *)arg;

    sem_wait(&serviceQueue);      // wait in queue
    sem_wait(&resourceLock);      // lock resource for writing

    printf("Writer %d is writing\n", id);
    sleep(2);
    printf("Writer %d finished writing\n", id);

    sem_post(&resourceLock);
    sem_post(&serviceQueue);

    return NULL;
}


int main()
{
    pthread_t r[5], w[5];
    int id[5];

    sem_init(&readCountLock, 0, 1);
    sem_init(&resourceLock, 0, 1);
    sem_init(&serviceQueue, 0, 1);

    for(int i=0;i<5;i++)
    {
        id[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &id[i]);
        pthread_create(&w[i], NULL, writer, &id[i]);
    }

    for(int i=0;i<5;i++)
    {
        pthread_join(r[i], NULL);
        pthread_join(w[i], NULL);
    }

    return 0;
}
