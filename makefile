CC=gcc
CFLAGS=-g

all:    mtfencode mtfdecode

mtfencode: mtfencode.o mtfcoding.o
	$(CC) -o mtfencode mtfencode.o mtfcoding.o

mtfdecode: mtfdecode.o mtfcoding.o
	$(CC) -o mtfdecode mtfdecode.o mtfcoding.o

mtfcoding.o: mtfcoding.c mtfcoding.h
	$(CC) $(CFLAGS) -c mtfcoding.c

mtfencode.o: mtfencode.c mtfcoding.h
	$(CC) $(CFLAGS) -c mtfencode.c

mtfdecode.o: mtfdecode.c mtfcoding.h
	$(CC) $(CFLAGS) -c mtfdecode.c

clean:
	rm -rf mtfencode.o mtfdecode.o mtfcoding.o mtfencode mtfdecode
