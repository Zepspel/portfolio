CFLAGS = -g
SRCS = main.c dialog.c stack.c timing.c tree.c

all: build/a.out						#default purpose

build/a.out: $(SRCS)
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -rf build/*
