#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int galton(void) {//gets galton's cell
    int sum = 0;
    for (int i = 0; i < 19; i++) {
        sum += rand() % 2;
    }
    return sum;
}

void *cell();

int array[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//galton cells
pthread_mutex_t mutex_array[20];// mutex for every cell

void print_cells() {//prints galton cells
    printf("Cell:  Value\n------------\n");
    for (int i = 0; i < 20; i++) {
        if (i < 10) printf("   %i:  %i\n", i, array[i]);
        else printf("  %i:  %i\n", i, array[i]);
    }
}

int sumOfCells() {//gets sum of galton cells
    int sum = 0;
    for (int i = 0; i < 20; i++) {
        sum += array[i];
    }
    return sum;
}

int main() {
    for (int i = 0; i < 20; i++) {//initilazes mutexes
        pthread_mutex_init(&mutex_array[i], NULL);
    }
    int threads = 1000000;//number of threads
    pthread_t tids[threads];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int decision;
    int success = 0;
    srand(time(0));

    for (int i = 0; i < threads; i++) { // creates 1M threads

        decision = pthread_create(&tids[i], &attr, &cell, NULL);

        if (decision == 0) {
            success++;//number of created successfully threads
        }
    }

    for (int j = 0; j < success; j++) {//end of created threads
        pthread_join(tids[j], NULL);
    }

    print_cells();
    printf("Successfully created threads: %i\n", success);
    printf("Sum of values in the cell: %i", sumOfCells());
    return 0;
}

void *cell() {
    int index = galton();
    while (pthread_mutex_trylock(&mutex_array[index]) != 0);//blocks process same index
    array[index]++;
    pthread_mutex_unlock(&mutex_array[index]);

}