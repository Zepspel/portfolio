#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H



typedef struct Elem Elem;

typedef struct Elem {
	KeySpace *ks;
	Elem *next;
} Elem;

typedef struct Stack {
	Elem *top;
} Stack;

typedef struct Queue {
	Elem *head;
	Elem *tail;
} Queue;	


#endif

