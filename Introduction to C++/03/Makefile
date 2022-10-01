CC = g++
FLAGS = -std=c++17 -O2 -Wall -Wunused-variable

all: main clean


main: test.o my_matrix.o
	$(CC) $(FLAGS) test.o my_matrix.o -o 03


test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -c


my_parser.o: my_matrix.cpp my_matrix.hpp
	$(CC) $(FLAGS) my_matrix.cpp -c


test: 03
	./03


clean:
	rm -f *.o