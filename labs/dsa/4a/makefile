CFLAGS = -g
SRCS = main.c dialog.c tree.c timing.c

all: build/a.out						#default purpose

build/a.out: $(SRCS)
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -rf build/*
