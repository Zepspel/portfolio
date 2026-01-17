#ifndef _TABLE_PRIVAT_H_
#define _TABLE_PRIVAT_H_


typedef enum CellState { 
	EMPTY, 
	OCCUPIED, 
	DELETED 
} CellState;

typedef struct KeySpace {
	CellState state;
	unsigned int key;	
	char *info;	
} KeySpace;

typedef struct Table {
	KeySpace *ks;			
	unsigned int msize;
	unsigned int csize;
} Table;


#endif
