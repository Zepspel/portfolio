CFLAGS = -Wall -Wextra -g
SRCS = main.c dialog.c table.c
SRCS_ITER = main.c dialog_iter.c table.c iterator.c

all: build/basic						#default purpose

build/basic: $(SRCS)
	gcc $(CFLAGS) $^ -o $@

build/iter: $(SRCS_ITER)	
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -rf build/*
