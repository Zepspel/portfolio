#ifndef _TABLE_PRIVAT_H_
#define _TABLE_PRIVAT_H_


typedef struct KeySpace {
	unsigned int key;	
	unsigned int par;		
	unsigned int info;	
} KeySpace;

typedef struct Table {
	KeySpace *ks;			
	unsigned int msize;
	unsigned int csize;
} Table;


#endif
