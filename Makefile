CC  = gcc
CFLAGS  = -g
RM  = rm -f

all: main

main: mnw.c main.c
	$(CC) $(CFLAGS) -o mnw mnw.c
	$(CC) $(CFLAGS) -o main main.c

clean:
	$(RM) mnw
	$(RM) main
