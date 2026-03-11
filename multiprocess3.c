#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/* -------- Shared Data -------- */

double mean_value = 0;
double median_value = 0;
double stddev_value = 0;

int *data;      // array to store numbers
int n;          // number of elements


/* -------- Mean Calculation Thread -------- */

void* find_mean(void *arg)
{
    double sum = 0;

    for(int i = 0; i < n; i++)
    {
        sum = sum + data[i];
    }

    mean_value = sum / n;

    pthread_exit(NULL);
}


/* -------- Function used by qsort to compare numbers -------- */

int compare_numbers(const void *a, const void *b)
{
    int x = *(int*)a;
    int y = *(int*)b;

    return x - y;
}


/* -------- Median Calculation Thread -------- */

void* find_median(void *arg)
{
    /* sort numbers first */
    qsort(data, n, sizeof(int), compare_numbers);

    if(n % 2 == 0)   // even number of elements
    {
        median_value = (data[n/2 - 1] + data[n/2]) / 2.0;
    }
    else             // odd number of elements
    {
        median_value = data[n/2];
    }

    pthread_exit(NULL);
}


/* -------- Standard Deviation Thread -------- */

void* find_stddev(void *arg)
{
    double sum = 0;

    for(int i = 0; i < n; i++)
    {
        double diff = data[i] - mean_value;
        sum = sum + pow(diff, 2);
    }

    stddev_value = sqrt(sum / n);

    pthread_exit(NULL);
}


/* -------- Main Function -------- */

int main(int argc, char *argv[])
{
    /* check if numbers are given */
    if(argc < 2)
    {
        printf("Usage: %s number1 number2 number3 ...\n", argv[0]);
        return 1;
    }

    /* number of integers given */
    n = argc - 1;

    /* allocate memory for numbers */
    data = malloc(n * sizeof(int));

    /* convert command line arguments to integers */
    for(int i = 1; i < argc; i++)
    {
        data[i-1] = atoi(argv[i]);
    }

    pthread_t mean_thread;
    pthread_t median_thread;
    pthread_t stddev_thread;

    /* create threads */
    pthread_create(&mean_thread, NULL, find_mean, NULL);
    pthread_create(&median_thread, NULL, find_median, NULL);

    /* wait for mean (needed for std deviation) */
    pthread_join(mean_thread, NULL);

    pthread_create(&stddev_thread, NULL, find_stddev, NULL);

    /* wait for remaining threads */
    pthread_join(median_thread, NULL);
    pthread_join(stddev_thread, NULL);

    /* print results */
    printf("\nResults\n");
    printf("Mean = %.2f\n", mean_value);
    printf("Median = %.2f\n", median_value);
    printf("Standard Deviation = %.2f\n", stddev_value);

    free(data);

    return 0;
}
