CXXFLAGS=-O3 -g --std=c++20 -Wall -Wextra -Wpedantic

all: solve

run-test: test
	./test

solve: a.out
	./a.out < ../../input

a.out: main.o tile.o
	g++ $(CXXFLAGS) $^ -o $@

test: tile.o test.o
	g++ $(CXXFLAGS) $^ -o $@

%.o: %.cc
	g++ $(CXXFLAGS) -c $^ -o $@

for_clangd:
	g++ --std=c++20 -fno-delayed-template-parsing main.cc

clean:
	rm -rf a.out *.o core
