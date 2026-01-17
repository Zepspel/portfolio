#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include "table_privat.h"
#include "table.h"

typedef struct Iterator {
	Table *table;
	KeySpace *current;
	ErrorCode flag;
} Iterator;



Iterator i_make(Table *table, unsigned int index);
int i_compare(Iterator iter1, Iterator iter2);
int i_next(Iterator *iter);
int i_prev(Iterator *iter);

unsigned int i_get_key(Iterator iter);
unsigned int i_get_par(Iterator iter);
unsigned int i_get_info(Iterator iter);




Iterator t_insert_i(Table *table, unsigned int key, unsigned int par, unsigned int info);
Iterator t_delete_i(Table *table, unsigned int key);
Iterator t_search_key_i(Table *table, unsigned int key);
Iterator t_search_child_i(Table *table, unsigned int par);
int t_print_i(Table *table);


#endif
