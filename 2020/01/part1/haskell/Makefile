all: solve

solve: Main
	./Main < ../../input

Main: Main.hs
	ghc --make Main

clean:
	rm -rf Main Main.o Main.hi
