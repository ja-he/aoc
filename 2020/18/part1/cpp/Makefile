CXXFLAGS=-Og -g --std=c++20 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=leak

all: solve

solve: a.out
	./a.out < ../../input

a.out: main.cc
	clang++ $(CXXFLAGS) main.cc

for_clangd:
	clang++ --std=c++20 -fno-delayed-template-parsing main.cc

clean:
	rm a.out
