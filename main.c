#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int galtonBoard() {//gets galtonBoard's cell
    int sum = 0;
    for (int i = 0; i < 19; i++) {
        sum += rand() % 2;
    }
    return sum;
}

void *cell();

int array[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//galtonBoard cells
pthread_mutex_t mutex_array[20];// mutex for every cell

void print_cells() {//prints galtonBoard's cells
    printf("Cell:  Value\n------------\n");
    for (int i = 0; i < 20; i++) {
        if (i < 10) printf("   %i:  %i\n", i, array[i]);
        else printf("  %i:  %i\n", i, array[i]);
    }
}

int sumOfCells() {//gets sum of galtonBoard cells
    int sum = 0;
    for (int i = 0; i < 20; i++) {
        sum += array[i];
    }
    return sum;
}

int main() {
    for (int i = 0; i < 20; i++) {//initializes mutexes
        pthread_mutex_init(&mutex_array[i], NULL);
    }

    int threadSize = 1000000;//number of threads
    pthread_t tids[threadSize];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int decision;
    int success = 0;
    srand(time(0));

    for (int i = 0; i < threadSize; i++) { // creates 1M threads

        decision = pthread_create(&tids[i], &attr, &cell, NULL);

        if (decision == 0) {
            success++;//number of created threads successfully
        }
    }

    for (int j = 0; j < success; j++) {//end of created threads
        pthread_join(tids[j], NULL);
    }

    print_cells();
    printf("Successfully created threads: %i\n", success);
    printf("Sum of values in the cell: %i\n\n\n", sumOfCells());

    return 0;
}

void *cell() {
    int index = galtonBoard();
    while (pthread_mutex_trylock(&mutex_array[index]) != 0);//blocks process same index
    array[index]++;
    pthread_mutex_unlock(&mutex_array[index]);
}