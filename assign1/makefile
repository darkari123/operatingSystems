all: pre sort ctp uni

pre: pre.o
	g++ -o pre pre.o

sort: sort.o
	g++ -o sort sort.o

ctp: ctp.o
	g++ -o ctp ctp.o

uni: uni.o
	g++ -o uni uni.o

.PHONY: clean

clean:
	rm -f *.o pre sort ctp uni
