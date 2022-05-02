CC=g++
CFLAGS=-Iinclude -std=c++11 -g
LIBS=-lglut -lGLEW -lGL -lGLU
# Default target executed when no arguments are given to make.
default_target: as1
.PHONY : default_target

as1: as1.o InitShader.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

as1.o: as1.cc
	$(CC) $(CFLAGS) -c $^

InitShader.o: common/InitShader.cc
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f as1 *~ *.o
