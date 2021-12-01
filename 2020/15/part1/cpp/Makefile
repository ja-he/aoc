CXXFLAGS=-O3 -g --std=c++20 -Wall -Wextra -Wpedantic

all: solve

solve: a.out
	./a.out < ../../input

a.out: main.cc
	clang++ $(CXXFLAGS) main.cc

for_clangd:
	clang++ --std=c++20 -fno-delayed-template-parsing main.cc

clean:
	rm a.out
