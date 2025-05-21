GCC = g++
OMP = -fopenmp

all: seq par

seq: seq_main.o matrix.o std_libraries.o
	$(GCC) $(OMP) -o seq seq_main.o matrix.o std_libraries.o

seq_main.o: seq_main.cc
	$(GCC) $(OMP) -c seq_main.cc

matrix.o: matrix.cc matrix.hh
	$(GCC) $(OMP) -c matrix.cc

std_libraries.o: std_libraries.cc std_libraries.hh
	$(GCC) $(OMP) -c std_libraries.cc

# n: matrix size; s: calculation steps; r: execution repetition 
srun:
	./seq $(n) $(r)

fig:
	python3 display_mxm.py
# $(f) -> se ci aggiungo il file

clean:
	> output.txt > starting_mat.txt > statistics.txt
	rm -f *.out *.o seq par
	rm -rf .vscode

clear:
	> output.txt > starting_mat.txt
