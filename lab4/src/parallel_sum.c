#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <sys/time.h>
#include <unistd.h>

#include "sum_lib.h"
#include "utils.h"

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *)args;
    return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
    int threads_num = -1;
    int array_size = -1;
    int seed = -1;

    while (1) {
        static struct option options[] = {
            {"threads_num", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"seed", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1) break;

        switch (option_index) {
            case 0:
                threads_num = atoi(optarg);
                break;
            case 1:
                array_size = atoi(optarg);
                break;
            case 2:
                seed = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Unknown option\n");
                return 1;
        }
    }

    if (threads_num <= 0 || array_size <= 0 || seed <= 0) {
        fprintf(stderr, "Usage: %s --threads_num num --seed num --array_size num\n", argv[0]);
        return 1;
    }

    // --- Генерация массива ---
    int *array = malloc(sizeof(int) * array_size);
    if (!array) {
        perror("malloc");
        return 1;
    }
    GenerateArray(array, array_size, seed);

    // --- Засекаем время ---
    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    pthread_t threads[threads_num];
    struct SumArgs args[threads_num];

    int block_size = array_size / threads_num;

    for (int i = 0; i < threads_num; i++) {
        args[i].array = array;
        args[i].begin = i * block_size;
        args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * block_size;

        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
            fprintf(stderr, "Error: pthread_create failed\n");
            return 1;
        }
    }

    int total_sum = 0;
    for (int i = 0; i < threads_num; i++) {
        int sum = 0;
        pthread_join(threads[i], (void **)&sum);
        total_sum += sum;
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    printf("Total sum: %d\n", total_sum);
    printf("Elapsed time: %.3f ms\n", elapsed_time);

    free(array);
    return 0;
}