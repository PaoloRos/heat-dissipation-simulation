GCC = g++

all: seq par

seq: seq_main.o matrix.o
	$(GCC) -o seq seq_main.o matrix.o

seq_main.o: seq_main.cc
	$(GCC) -c seq_main.cc

matrix.o: matrix.cc matrix.hh
	$(GCC) -c matrix.cc

# n: matrix size; s: calculation steps; r: execution repetition 
srun:
	./seq $(n) $(s) $(r)

fig:
	python3 display_mxm.py
# $(f) -> se ci aggiungo il file

clean:
	> output.txt > starting_mat.txt
	rm -f *.out *.o seq par
	rm -rf .vscode

clear:
	> output.txt > starting_mat.txt