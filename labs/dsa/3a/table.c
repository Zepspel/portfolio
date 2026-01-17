#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "table_privat.h"

int t_clear(Table *table)
{
	table->csize = 0;

	return 0;
}

Table *t_make(unsigned int msize)
{
	Table *table = (Table *) calloc(1, sizeof(Table));
	if (table == NULL)
		return NULL;

	table->msize = msize;
	table->csize = 0;
	table->ks = (KeySpace *) calloc(msize, sizeof(KeySpace));
	if (table->ks == NULL)
	{	
		free(table);
		table = NULL;
	}
	return table;	
}

int t_erase(Table *table)
{
	if (table == NULL)
		return FAIL;
	
	if (table->ks != NULL)
	{
		free(table->ks);
		table->ks = NULL;
	}
	
	free(table);
	table = NULL;

	return SUCCESS;	
}

Table *t_kids_make(Table *table, unsigned int par)
{
	int start = t_search_child(table, par);
	if (start == FAIL)
		return NULL;						//empty t_kids
		
	//then we count all kids
	int counter = 1;
	int i = start + 1;
	
	while (table->ks[i].par == par)
	{
		counter++;	
		i++;
	}
	
	Table *table_kids = t_make(counter);
	if (table_kids == NULL)
		return NULL;
	
	i = start;
	for (int s = 0; s < counter; s++)
		table_kids->ks[s] = table->ks[i++];

	table_kids->csize = counter;
	
	return table_kids;
}

int t_search_key(Table *table, unsigned int key)		//it'll return position
{
	if (key == 0)
		return FAIL;
	
	for (unsigned int i = 0; i < table->csize; i++)
	{
		if (key == table->ks[i].key)
			return i;
	}

	return FAIL;
}

int t_search_child(Table *table, unsigned int par)		//it searchs first child in the table
{
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
			return middle;				//it will be the first one
		}	
		if (par < table->ks[middle].par)
			end = middle - 1;
		else
			start = middle +1;		

	}

	
	return FAIL;
}


int t_insert(Table *table, unsigned int key, unsigned int par, unsigned int info)
{
	//for insertion elem we need to follow 2 conditions
	//1) key isn't dubbed
	//2) elem has parent (par == some key)
	
	if (table->msize == table->csize)	//if table is full
		return OOM;

	if (key == 0)
		return FAIL;

	int test = t_search_key(table, key);
	if (test >= 0)
		return DUBBED;		

	if (par != 0)
	{
		test = t_search_key(table, par);
		if (test == FAIL)
			return NO_PARENT;
	}
	
							//then we need insert elem
	KeySpace field;					//create field that we'll insert
	field.key = key;
	field.par = par;
	field.info = info;
							//we will check table starting from its tail
	int i = table->csize - 1;

	while ((i >= 0) && (table->ks[i].par > par))	//then we move forward by 1 i elem 
	{
		table->ks[i+1] = table->ks[i];
		i--;
	}
	table->ks[i+1] = field;
	table->csize++;
	

	return SUCCESS;	
}

int t_delete(Table *table, unsigned int key)
{
	//check if it's not empty directory (its is parent for some elem)
	int i = t_search_child(table, key);
	if (i >= 0)
		return NOT_EMPTY; 
	
	//search by this key
	i = t_search_key(table, key);
	if (i == FAIL)
		return FAIL;

//we need to find last cell with such par (or par+1 if it last sample)
	unsigned int par;
	unsigned int k = i;			

	
	while (k != table->csize - 1)					//we'll repeat it until we reach last elem in the table [csize-1]		
	{
		par = table->ks[k+1].par;						//with such par we'll seek
		while ((par == table->ks[k+1].par) && (k != table->csize - 1))
		{
			k++;
		}
		table->ks[i] = table->ks[k];						//so we replace [key] elem with [par], which is last 
		i = k;
	}	


	table->csize--;	

	return SUCCESS;
}

int t_print(Table *table)
{
	if (table == NULL)
		return FAIL;
	if (table->csize == 0)
		return FAIL;

	printf("\nTABLE: size = %d, max size = %d\n", table->csize, table->msize);
	printf("\nkey		info		parent\n");
	for (unsigned int i = 0; i < table->csize; i++)
		printf("%-15u %-15u %-15u\n", table->ks[i].key, table->ks[i].info, table->ks[i].par);


	return SUCCESS;
}

int t_elem_print(Table *table, unsigned int i)
{
	if (table == NULL)
		return FAIL;

	if (i > table->csize)
		return FAIL;

	printf("%-15u %-15u %-15u\n", table->ks[i].key, table->ks[i].info, table->ks[i].par);

	return SUCCESS;	
}






















