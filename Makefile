CC=gcc
CFLAGS=-fPIC
LDFLAGS=-shared 
LDLIBS=-ldl

default: libfakeopen.so
fakeopen: libfakeopen.so

libfakeopen.so:
	$(CC) $(CFLAGS) -c -o libfakeopen.o libfakeopen.c
	$(CC) $(LDFLAGS) -o $@ libfakeopen.o $(LDLIBS)

clean:
	rm -f *.so *.o
