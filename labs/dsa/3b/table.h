#ifndef _TABLE_H_
#define _TABLE_H_


typedef enum {
	SUCCESS = 0,          	  
   	OOM = -1,    		  
	FAIL = -2,		  
	DUBBED = -3,
	SIZE_ERROR = -4,
} ErrorCode;


typedef struct Table Table;
typedef struct KeySpace KeySpace;


Table *table_make(unsigned int msize);
int table_erase(Table *table);
int table_clean(Table *table);

unsigned int hash_function(unsigned int msize, unsigned int key, unsigned int i);

KeySpace table_search_elem(Table *table, unsigned int key);				
int table_search_index(Table *table, unsigned int key);
int table_insert(Table **ptable, unsigned int key, char *info);			
int table_delete(Table *table, unsigned key);

int table_print(Table *table);
int table_print_ti(Table *table);
int element_print(Table *table, unsigned int index);

int table_export(Table *table, FILE *file);
int table_import(Table **ptable, FILE *file);



#endif
