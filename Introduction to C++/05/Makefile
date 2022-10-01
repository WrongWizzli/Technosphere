CC = g++
FLAGS = -std=c++17 -O2 -Wunused-variable


test: all task
	./task


all: main clean


main: test.o
	$(CC) $(FLAGS) test.o -o task


test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -c


clean:
	rm -f *.o