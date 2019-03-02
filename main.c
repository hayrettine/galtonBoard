#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

int galton(void){
    int sum = 0;
    for (int i = 0; i < 19 ; i++) {
        sum +=rand()%2;
    }
    return sum;
}

void *cell(void*);
int array[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void print_cells(){
    for (int i = 0; i < 20 ; i++) {
        printf("\t%i:\t%i\n",i,array[i]);
    }
}

int sumOfCells(){
    int sum = 0;
    for (int i = 0; i < 20 ; i++) {
        sum += array[i];
    }
    return sum;
}

int main () {
    pthread_t tids[1000000];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int decision;
    int success = 0;
    srand(time(0));

    for(int i=0;i<1000000;i++) {

        decision = pthread_create(&tids[i], &attr , &cell, (void*)&i);

        if (decision == 0){
            success++;
        }
    }

    for (int j = 0; j < success ; j++) {
        pthread_join(tids[j], NULL);
    }

    print_cells();
    printf("Success:*****%i****\n", success);
    printf("Sum of values in the cell: %i", sumOfCells());
    return 0;
}
pthread_mutex_t mutex;
void *cell(void* arg){
    int index = galton();
    pthread_mutex_lock(&mutex);
    //TODO stop just reaching same time same index!
    array[index]++;
    pthread_mutex_unlock(&mutex);
}