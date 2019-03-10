#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <zconf.h>
int galton(void){
    int sum = 0;
    for (int i = 0; i < 19 ; i++) {
        sum +=rand()%2;
    }
    return sum;
}
pthread_rwlock_t  rwlock = PTHREAD_RWLOCK_INITIALIZER;
void *cell();
int array[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
pthread_mutex_t mutex_array[20];
void print_cells(){
    printf("Cell:  Value\n------------\n");
    for (int i = 0; i < 20 ; i++) {
        if (i < 10) printf("   %i:  %i\n",i,array[i]);
        else printf("  %i:  %i\n",i,array[i]);
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
    for(int i = 0; i<20;i++){
        pthread_mutex_init(&mutex_array[i], NULL);
    }

    pthread_rwlock_init(&rwlock, NULL);
    pthread_t tids[1000000];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int decision;
    int success = 0;
    srand(time(0));

    for(int i=0;i<1000000;i++) {

        decision = pthread_create(&tids[i], &attr , &cell, NULL);

        if (decision == 0){
            success++;
        }
    }

    for (int j = 0; j < success ; j++) {
        pthread_join(tids[j], NULL);
    }

    print_cells();
    printf("Successfully created threads: %i\n", success);
    printf("Sum of values in the cell: %i", sumOfCells());
    return 0;
}
void *cell(){
    int index = galton();
    while (pthread_mutex_trylock(&mutex_array[index])!=0);
            array[index]++;
            pthread_mutex_unlock(&mutex_array[index]);

}