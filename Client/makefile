CFLAGS=-c

all:
	$(CROSS_COMPILE)gcc main.c -pthread -o client

rebuild:
	make clean
	make

clean:
	rm -rf *.o client
