CC = g++
FLAGS = -std=c++17 -O2 -pthread -Wunused-variable


test: all task
	./task


all: main clean


main: test.o my_thrsort.o
	$(CC) $(FLAGS) test.o my_thrsort.o -o task


test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -c


my_thrsort.o: my_thrsort.cpp
	$(CC) $(FLAGS) my_thrsort.cpp -c


clean:
	rm -f *.o