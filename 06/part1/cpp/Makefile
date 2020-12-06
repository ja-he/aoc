CXXFLAGS=-O3 --std=c++20 -Wall -Wextra

all: solve

solve: a.out
	./a.out < ../../input

a.out: main.cc
	g++ $(CXXFLAGS) main.cc

clean:
	rm a.out
