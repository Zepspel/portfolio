#ifndef _GRAPH_PRIVAT_H_
#define _GRAPH_PRIVAT_H_

#include "graph.h"

typedef enum CellState { 
	EMPTY, 
	OCCUPIED, 
	DELETED 
} CellState;

typedef enum Colors { 
	WHITE = 4, 
	GREY = 5, 
	BLACK = 6 
} Colors;



typedef struct List List;

typedef struct List {
	char *drain;
	short weight;
	List *next;
} List;


typedef struct KeySpace {
	CellState state;
	char *key;	
	List *list;	
} KeySpace;


typedef struct Graph {
	KeySpace *ks;			
	unsigned int msize;
	unsigned int csize;
} Graph;


#endif
