all: pgmToMatrix separateChannel histogram

pgmToMatrix: pgmToMatrix.c
	gcc -o pgmToMatrix pgmToMatrix.c 

separateChannel: separateChannel.c
	gcc -o separateChannel separateChannel.c

histogram: histogram.c
	gcc -o histogram histogram.c

clean:
	rm *.out
