CC  = gcc
CFLAGS  = -g
RM  = rm -f

all: main

main: mnw.c main.c
	$(CC) $(CFLAGS) -o mnw mnw.c
	$(CC) $(CFLAGS) -o main main.c
	$(CC) -shared -Wl,-install_name,main.so -o main.so -fPIC main.c
	$(CC) $(CFLAGS) -o test test.c
	$(CC) -shared -Wl,-install_name,test.so -o test.so -fPIC test.c

clean:
	$(RM) mnw
	$(RM) main
	$(RM) main.so
	$(RM) test
	$(RM) test.so
