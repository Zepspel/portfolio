#ifndef _TABLE_FILE_H_
#define _TABLE_FILE_H_

typedef struct FileTable {
	unsigned int msize;
	unsigned int csize;
	int length;
	int magic;
} FileTable;


typedef struct FileElem {
	CellState state;
	unsigned int key;
	unsigned int offset;
} FileElem;




#endif
