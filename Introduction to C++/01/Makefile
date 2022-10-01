CC = g++
FLAGS = -std=c++17 -O2 -Wall -Wunused-variable

all: main clean


main: test.o my_allocator.o
	$(CC) $(FLAGS) test.o my_allocator.o -o 01


test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -c


my_allocator.o: my_allocator.cpp my_allocator.hpp
	$(CC) $(FLAGS) my_allocator.cpp -c


test: 01
	./01


clean:
	rm -f *.o