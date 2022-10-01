CC = g++
FLAGS = -std=c++17 -O2 -Wall -Wunused-variable

all: main clean


main: test.o my_parser.o
	$(CC) $(FLAGS) test.o my_parser.o -o 02


test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -c


my_parser.o: my_parser.cpp my_parser.hpp
	$(CC) $(FLAGS) my_parser.cpp -c


test: 02
	./02


clean:
	rm -f *.o