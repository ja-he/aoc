CXXFLAGS=-O0 --std=c++20 -Wall -Wextra -Wpedantic -Wunused-function

all: solve

solve: a.out
	./a.out < ../../input

a.out: main.cc
	g++ $(CXXFLAGS) main.cc

clean:
	rm a.out
