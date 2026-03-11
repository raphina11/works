#include <stdio.h>

int main() {

    // Number of processes and resources
    int n = 3;   // P0 P1 P2
    int m = 2;   // R0 R1

    // Allocation matrix
    int allocation[3][2] = {
        {1,0},   // P0
        {0,1},   // P1
        {1,1}    // P2
    };

    // Maximum need
    int max[3][2] = {
        {1,1},
        {1,1},
        {2,2}
    };

    // Available resources
    int available[2] = {0,0};

    // Need matrix
    int need[3][2];

    // Process finished or not
    int finish[3] = {0,0,0};

    // Work = temporary available resources
    int work[2];

    // Safe sequence
    int safeSeq[3];

    int count = 0;

    /* ---------------------------
       STEP 1 : Calculate NEED
       need = max - allocation
    ----------------------------*/

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    /* ---------------------------
       STEP 2 : work = available
    ----------------------------*/

    for(int i=0;i<m;i++){
        work[i] = available[i];
    }

    /* ---------------------------
       STEP 3 : Try to find safe sequence
    ----------------------------*/

    while(count < n){

        int found = 0;

        for(int i=0;i<n;i++){

            if(finish[i] == 0){

                int canRun = 1;

                for(int j=0;j<m;j++){
                    if(need[i][j] > work[j]){
                        canRun = 0;
                        break;
                    }
                }

                if(canRun){

                    for(int k=0;k<m;k++){
                        work[k] = work[k] + allocation[i][k];
                    }

                    safeSeq[count] = i;
                    count++;

                    finish[i] = 1;

                    found = 1;
                }
            }
        }

        if(found == 0)
            break;
    }

    /* ---------------------------
       STEP 4 : Result
    ----------------------------*/

    if(count == n){

        printf("System is SAFE\n");

        printf("Safe sequence: ");

        for(int i=0;i<n;i++)
            printf("P%d ", safeSeq[i]);

    }
    else{

        printf("System is in DEADLOCK\n");

        printf("Deadlocked processes: ");

        for(int i=0;i<n;i++){
            if(finish[i] == 0)
                printf("P%d ", i);
        }

    }

    return 0;
}
