#
# Makefile
#
CFLAGS = -g -O0

treeop:
	gcc $(CFLAGS) treeop.c -o treeop

clean:
	rm -f treeop

all: treeop

.PHONY: clean

.PHONY: treeop all
