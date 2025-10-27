CC = gcc
CFLAGS = -I.

all: sequential_min_max parallel_min_max run_sequential

sequential_min_max: utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max: utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -c utils.c -o utils.o $(CFLAGS)

find_min_max.o: find_min_max.c find_min_max.h utils.h
	$(CC) -c find_min_max.c -o find_min_max.o $(CFLAGS)

run_sequential: run_sequential.c
	$(CC) -o run_sequential run_sequential.c $(CFLAGS)

clean:
	rm -f *.o sequential_min_max parallel_min_max run_sequential
