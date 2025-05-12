GCC = g++

seq: main.o matrix.o
	$(GCC) -o ...

clean:
	rm -f *.out starting_mat.txt