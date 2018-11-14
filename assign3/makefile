all: withoutMutex  withMutex

withoutMutex: withoutMutex.c
	gcc -o withoutMutex withoutMutex.c -lpthread -lm

withMutex: withMutex.c
	gcc -o withMutex withMutex.c -lpthread -lm



.PHONY: clean

clean:
	rm -rf *.o withMutex withoutMutex
