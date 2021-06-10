all:
	gcc studentsseq.c -o studentsseq -fopenmp -lm -g
run:
	./studentsseq 3 4 6 7