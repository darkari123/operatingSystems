all: pre sort ctp uni sssh

pre: pre.o
	g++ -o pre pre.o

sort: sort.o
	g++ -o sort sort.o

ctp: ctp.o
	g++ -o ctp ctp.o

uni: uni.o
	g++ -o uni uni.o

sssh: sssh.o
	g++ -o sssh sssh.o


.PHONY: clean

clean:
	rm -f *.o pre sort ctp uni sssh
