CFLAGS = -g
SRCS = main.c dialog.c stack.c queue.c table.c graph.c

all: build/a.out						#default purpose

build/a.out: $(SRCS)
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -rf build/*
