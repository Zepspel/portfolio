#include <stdio.h>
#include "iterator.h"



Iterator i_make(Table *table, unsigned int index)
{
	Iterator iter;
	if (table == NULL) 
		iter = (Iterator) {NULL, NULL, FAIL};
	else if ((table->csize == 0) || (index > table->csize - 1))
		iter = (Iterator) {NULL, NULL, FAIL};
	else
		iter = (Iterator) {table, table->ks + index, SUCCESS};
	return iter;
}

int i_compare(Iterator iter1, Iterator iter2)
{
	if (iter1.current == iter2.current)
		return 1;
	
	return 0;
}

int i_next(Iterator *iter)
{
	//if (iter->current == iter->end)
	//	return 0;
	if (iter->current == iter->table->ks + iter->table->csize - 1)
		return 0;

	iter->current++;
	return 1;
}

int i_prev(Iterator *iter)
{
	//if (iter->current == iter->start)
	//	return 0;
	if (iter->current == iter->table->ks)
		return 0;	

	iter->current--;
	return 1;
}	

unsigned int i_get_key(Iterator iter)
{
	return iter.current->key;
}


unsigned int i_get_par(Iterator iter) 
{
	return iter.current->par;
}


unsigned int i_get_info(Iterator iter)
{
	return iter.current->info;
}

//set of function for work with table by iterator


Iterator t_insert_i(Table *table, unsigned int key, unsigned int par, unsigned int info)
{

	if (table->msize == table->csize)	
		return (Iterator) {NULL, NULL, OOM};


	if (key == 0)
		return (Iterator) {NULL, NULL, FAIL};


	Iterator iter = t_search_key_i(table, key);
	if (iter.flag == SUCCESS)
		return (Iterator) {NULL, NULL, DUBBED};
			

	if (par != 0)
	{
		iter = t_search_key_i(table, par);
		if (iter.flag == FAIL)
			return (Iterator) {NULL, NULL, NO_PARENT};
	}

	KeySpace field;					//create field that we'll insert
	field.key = key;
	field.par = par;
	field.info = info;
							//we will check table starting from its tail

	Iterator iter_old = i_make(table, table->csize - 2);		//points to the last elem
	if (iter_old.flag != FAIL)					//if table doesn't have elements
	{			
		unsigned int p;
		p = i_get_par(iter_old);
		while (p > par)
		{
			*(iter_old.current + 1) = *(iter_old.current);				
			if (i_prev(&iter_old) == 0)			//if we went to the start
				break;
			
			p = i_get_par(iter_old);
		}
		*(iter_old.current + 1) = field;
		table->csize++;
		i_next(&iter_old);
	}
	else
	{
		table->ks[0] = (KeySpace) {key, par, info};
		table->csize++;
		iter_old = i_make(table, 0);
	}

	return iter_old;	
}





Iterator t_search_key_i(Table *table, unsigned int key)
{
	Iterator iter = {NULL, NULL, FAIL}; 

	if (key == 0)
		return iter;

	iter = i_make(table, 0);
	if (iter.flag == FAIL)
		return (Iterator) {NULL, NULL, FAIL};
	
	unsigned int k;
	do {
		k = i_get_key(iter);		
		if (k == key)
			return iter;

	} while (i_next(&iter) == 1);	

	iter = (Iterator) {NULL, NULL, FAIL};

	return iter;
}

int t_print_i(Table *table)
{
	if (table == NULL)
		return FAIL;
	if (table->csize == 0)
		return FAIL;

	unsigned int key, par, info;
	Iterator iter = i_make(table, 0);
	printf("\nTABLE: size = %d, max size = %d\n", table->csize, table->msize);
	printf("\nkey		info		parent\n");
	do {
		key = i_get_key(iter);
		par = i_get_par(iter);
		info = i_get_info(iter);
		printf("%-15u %-15u %-15u\n", key, info, par);
	} while (i_next(&iter) == 1);

	return SUCCESS;
}
	
Iterator t_delete_i(Table *table, unsigned int key)
{
	//check if it's not empty directory (its is parent for some elem)
	Iterator iter = t_search_child_i(table, key);
	if (iter.flag != FAIL)
		return (Iterator) {NULL, NULL, NOT_EMPTY};

	//search by this key
	iter = t_search_key_i(table, key);
	if (iter.flag == FAIL)
		return (Iterator) {NULL, NULL, FAIL};

	int flag = 0;
	unsigned int par;
	Iterator iter2 = iter;
	Iterator iter_return = iter;
	i_next(&iter2);
	while (1) 
	{
		par = i_get_par(iter2);
		while (par == i_get_par(iter2))
		{
			if (i_next(&iter2) == 0)
			{
				flag = 1;			//if we reach the end
				break;
			}
		}
		if (flag == 1)
		{
			*(iter.current) = *(iter2.current);
			iter.current = iter2.current;
			break;
		}
		else
		{
			*(iter.current) = *(iter2.current - 1);
			iter.current = iter2.current - 1;
		}
			
	} 
	
	table->csize--;	

	return iter_return;
}

Iterator t_search_child_i(Table *table, unsigned int par)		//it searchs first child in the table
{
	if (table == NULL)
		return (Iterator) {NULL, NULL, FAIL};

	int start = 0;
	int end = table->csize - 1;
	int middle;
	while (start <= end) 
	{
		middle = (end + start) / 2;
		if (par == table->ks[middle].par)
		{
			while(par == table->ks[middle].par)
			{
				middle--;
			}
			middle++;
			Iterator iter = i_make(table, middle);
			return iter;				//it will be the first one
		}	
		if (par < table->ks[middle].par)
			end = middle - 1;
		else
			start = middle +1;		

	}

	
	return (Iterator) {NULL, NULL, FAIL};
}










