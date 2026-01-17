#ifndef _TABLE_H_
#define _TABLE_H_

typedef enum {
	SUCCESS = 0,          	  // Успешное выполнение
   	OOM = -1,    		  // out of memory
	FAIL = -2,		  //не получилось выполнить операцию
	DUBBED = -3,
	NO_PARENT = -4,
	NOT_EMPTY = -5,
	EXIT = -6,
} ErrorCode;


typedef struct Table Table;
typedef struct KeySpace KeySpace;


Table *t_make(unsigned int msize);
int t_clear(Table *table);
Table *t_kids_make(Table *table, unsigned int par);
int t_erase(Table *table);

int t_search_key(Table *table, unsigned int key);					//it'll return position
int t_search_child(Table *table, unsigned int par);					//it'll return position

int t_insert(Table *table, unsigned int key, unsigned int par, unsigned int info);
int t_delete(Table *table, unsigned int key);

int t_print(Table *table);
int t_elem_print(Table *table, unsigned int i);


#endif
