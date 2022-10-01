CC = g++
FLAGS = -std=c++17 -O2 -Wall -Wunused-variable


test: all task
	./task


all: main clean


main: test.o my_bigint.o
	$(CC) $(FLAGS) test.o my_bigint.o -o task


test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -c


my_parser.o: my_bigint.cpp my_bigint.hpp
	$(CC) $(FLAGS) my_bigint.cpp -c


clean:
	rm -f *.o