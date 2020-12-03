CXXFLAGS=-O3 --std=c++20 -Wall -Wextra

all: test

test: p
	./p < ../input

p: main.cc
	g++ $(CXXFLAGS) main.cc -o p

clean:
	rm p
