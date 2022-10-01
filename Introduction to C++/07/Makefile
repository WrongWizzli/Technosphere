CC = g++
FLAGS = -std=c++17 -O2 -Wunused-variable


test: all task
	./task


all: main clean


main: test.o my_vector.o
	$(CC) $(FLAGS) test.o my_vector.o -o task


test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -c


my_vector.o: my_vector.cpp my_vector.hpp
	$(CC) $(FLAGS) my_vector.cpp -c


clean:
	rm -f *.o