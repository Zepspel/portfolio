CFLAGS = -Wall -Wextra -g
SRCS = main.c matr.c line.c input.c

all: build/a.out

build/a.out: $(SRCS)
	mkdir -p build
	gcc $(CFLAGS) $^ -o build/a.out

clean:
	rm -rf build/*
