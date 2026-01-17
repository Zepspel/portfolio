CC = gcc
CFLAGS = -g -Wall -fPIC
LDFLAGS = -lm
AR = ar rcs

QUEUE ?= vector
LIB ?= static

ifeq ($(QUEUE), vector)
    QUEUE_SRC = queue_vector.c
else ifeq ($(QUEUE), list)
    QUEUE_SRC = queue_list.c
else
    $(error Unknown QUEUE value: $(QUEUE))
endif

QUEUE_OBJ = $(QUEUE_SRC:.c=.o)

STATIC_LIB = libqueue.a
SHARED_LIB = libqueue.so

ifeq ($(LIB), static)
    LIBQUEUE = $(STATIC_LIB)
else ifeq ($(LIB), shared)
    LIBQUEUE = $(SHARED_LIB)
else
    $(error Unknown LIB value: $(LIB))
endif

all: build/a.out clean0

$(STATIC_LIB): $(QUEUE_OBJ)
	$(AR) $@ $^

$(SHARED_LIB): $(QUEUE_OBJ)
	$(CC) -shared -o $@ $^

SRCS = main.c input.c server.c
OBJS = $(SRCS:.c=.o)

build/a.out: $(LIBQUEUE) $(OBJS)
ifeq ($(LIB_TYPE), static)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lqueue $(LDFLAGS)
else
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lqueue $(LDFLAGS) -Wl,-rpath=.
endif

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean0:
	rm -f *.o $(STATIC_LIB)

clean: 
	rm -rf build/* $(SHARED_LIB)



