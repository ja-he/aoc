CXXFLAGS=-O0 --std=c++20 -Wall -Wextra -Wpedantic

all: solve

solve: a.out
	./a.out < ../../input

a.out: main.cc
	g++ $(CXXFLAGS) main.cc

clean:
	rm a.out
