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

void print_cells() {//prints galtonBoard boards cells
    printf("Cell:  Value\n------------\n");
    for (int i = 0; i < 20; i++) {
        if (i < 10) printf("   %i:  %i\n", i, array[i]);
        else printf("  %i:  %i\n", i, array[i]);
    }
}

int sumOfCells() {//gets sum of galtonBoard board cells
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
    printf("Sum of values in the cell: %i\n", sumOfCells());
    printf("COMMENT\n");
    printf("\tI chose 0 for left side, and 1 for right side. When red ball arrives to green balls, red ball chooses a random number. After red ball passes all green balls, the red ball\n"
           "adds every choice to itself as sum value. The sum value gives us red ball's cell's index. In code, I wrote a galtonBoard method that returns an integer value, also\n"
           "creates an array which has 20 sizes, besides creates a mutex array which is has 20 sizes. In main method, I created 1 million threads after that I checked how many\n"
           "threads are created successfully. Cell method is called with threads, and is called galtonBoard method too. GaltonBoard method's that is inside the cell method\n"
           "returns a value to inside index variable. Subsequently, the method calls try_lock function in a while loop. If the function doesn't return zero, it means there is an\n"
           "another thread that does process inside the index. So the thread waits in while loop, until other thread finishes own job. After all these, all threads are finished with\n"
           "join function to prints cells, created successful threads and sum of index values.");
    return 0;
}

void *cell() {
    int index = galtonBoard();
    while (pthread_mutex_trylock(&mutex_array[index]) != 0);//blocks process same index
    array[index]++;
    pthread_mutex_unlock(&mutex_array[index]);
}