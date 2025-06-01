GCC = g++
OMP = -fopenmp

seq: seq_main.o matrix.o std_libraries.o
	$(GCC) $(OMP) -o seq seq_main.o matrix.o std_libraries.o

par: par_block.o matrix.o std_libraries.o
	$(GCC) $(OMP) -o par par_block.o matrix.o std_libraries.o

dim: size.o matrix.o std_libraries.o
	$(GCC) -o dim size.o matrix.o std_libraries.o

seq_main.o: seq_main.cc
	$(GCC) $(OMP) -c seq_main.cc

par_block.o: par_block.cc
	$(GCC) $(OMP) -c par_block.cc

dim.o: size.cc
	$(GCC) -c size.cc

matrix.o: matrix.cc matrix.hh
	$(GCC) $(OMP) -c matrix.cc

std_libraries.o: std_libraries.cc std_libraries.hh
	$(GCC) $(OMP) -c std_libraries.cc

# n: matrix size; t: used threads; s: calculation steps; r: execution repetition; s: calculation steps
srun:
	./seq $(n) $(t) $(s) $(r)

fig:
	python3 display_mxm.py $(f)
# $(f) -> se ci aggiungo il file

clean:
	rm -f *.out *.o seq par
	rm -rf .vscode

clear:
	> output.txt > starting_mat.txt > statistics.txt
