#ifndef _TABLE_H_
#define _TABLE_H_

#include "graph.h"

/*typedef enum {
	SUCCESS = 0,          	  
   	OOM = -1,    		  
	FAIL = -2,		  
	DUBBED = -3,
	SIZE_ERROR = -4,
} ErrorCode;
*/


typedef struct List List;
typedef struct Graph Graph;
typedef struct KeySpace KeySpace;


Graph *tableMake(unsigned int msize);
ErrorCode tableErase(Graph *table);
ErrorCode tableClean(Graph *table);


//KeySpace table_search_elem(Table *table, const char *key);				
ErrorCode tableSearch(Graph *table, const char *key);
ErrorCode tableInsert(Graph **ptable, const char *key, List *list);			
ErrorCode tableDelete(Graph *table, const char *key);

//int table_print(Table *table);
//int element_print(Table *table, unsigned int index);



#endif
