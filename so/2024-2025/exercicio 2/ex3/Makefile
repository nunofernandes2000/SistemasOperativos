# Makefile
#
# It uses implicit rules to compile .c files using the
# values assigned to macros CC and CFLAGS
#

CC = gcc
CFLAGS = -g -Wall


counter: counter.o
	gcc -o counter counter.o -lpthread

clean:
	rm -f *.o counter
