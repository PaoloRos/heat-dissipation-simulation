GCC = g++

all: seq par

seq: seq_main.o matrix.o
	$(GCC) -o seq seq_main.o matrix.o

seq_main.o: seq_main.cc
	$(GCC) -c seq_main.cc

matrix.o: matrix.cc matrix.hh
	$(GCC) -c matrix.cc

srun:
	./seq $(N)

fig:
	python3 display_mxm.py

clean:
	rm -f *.out *.o starting_mat.txt seq par
	rm -rf .vscode