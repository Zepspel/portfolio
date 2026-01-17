#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "graphPrivat.h"

ErrorCode tableExpansion(Graph **ptable)
{
	if (ptable == NULL)
		return OOM;

	Graph *table = *ptable;
	if (table == NULL)
		return OOM;

	unsigned int prime[] = {23, 43, 101, 211, 401, 601, 997, 1999, 4001, 8009, 12007};
	int size_prime = sizeof(prime)/sizeof(prime[0]);
	
	int i = 0;
	while (prime[i] != table->msize)
	{
		i++;
	}
	if (i == size_prime - 1)			//if its bigger
		return OOM;

	unsigned int new_size = prime[i+1];
	
							//now we'll copy all elements
	Graph *new_table = tableMake(new_size);
	unsigned int key;
	
	for (int i = 0; i < table->msize; i++)
	{
								//get one element
		if (table->ks[i].state == OCCUPIED)
			tableInsert(&new_table, table->ks[i].key, table->ks[i].list);

	}

	tableErase(table);
	*ptable = new_table;
	
	return OK;
}
	
		
	

Graph *tableMake(unsigned int msize)
{
	if (msize == 0)
		return NULL;

	Graph *table = (Graph *) calloc(1, sizeof(Graph));
	if (table == NULL)
		return NULL;

	table->msize = msize;
	table->csize = 0;
	table->ks = (KeySpace *) calloc((size_t)msize, sizeof(KeySpace));
	if (table->ks == NULL)
	{	
		free(table);
		return NULL;
	}
	
	for (unsigned int i = 0; i < msize; i++)
	{
		table->ks[i].state = EMPTY;	
		table->ks[i].key = NULL;
		table->ks[i].list = NULL;
	}

	return table;	
}


ErrorCode tableClean(Graph *table)
{
	if (table == NULL)
		return OOM;

	for (unsigned int i = 0; i < table->msize; i++)
//		if (table->ks[i].info != NULL)
		if (table->ks[i].state == OCCUPIED)
		{
			table->ks[i].state = EMPTY;
			free(table->ks[i].key);
			table->ks[i].key = NULL;
		}

	table->csize = 0;
	
	return OK;
}



ErrorCode tableErase(Graph *table)
{
	if (table == NULL)
		return OOM;

	List *list, *temp;	

	for (unsigned int i = 0; i < table->msize; i++)
	{	
		if ((table->ks[i].state == OCCUPIED) && (table->ks[i].key != NULL))
			free(table->ks[i].key);	
	}


	if (table->ks != NULL)
	{
		free(table->ks);
		table->ks = NULL;
	}
	
	free(table);

	return OK;	
}

static inline unsigned long hash1(const char *key) {
	unsigned long hash = 5381;
	while (*key)
	{
		hash = ((hash << 5) + hash) + (unsigned char)*key;
		key++;	
	}

	return hash*2654435761;
}

static inline unsigned long hash2(const char *key) {

	unsigned long hash = 0;
	while (*key) 
	{
		hash = (hash << 16) - hash - (unsigned char)*key;
		key++;
	}

    	return hash*7340033;
}


ErrorCode tableSearch(Graph *table, const char *key)
{
	unsigned int n = 0;							//counter for number of elements that we wiewed

	unsigned int index = hash1(key) % table->msize;
	while ((table->ks[index].state != EMPTY) && (n < table->msize))
	{
//		if ((table->ks[index].key == key) && (table->ks[index].state != DELETED))
		if ((strcmp(table->ks[index].key, key) == 0) && (table->ks[index].state != DELETED))
			return index;
		n++;
		index = (index + hash2(key)) % table->msize;
	}
	return FAIL;
}


ErrorCode tableInsert(Graph **ptable, const char *clef, List *list)				//we consider DELETED like free
{	
	Graph *table = *ptable;

	if (table == NULL)
		return OOM;


	unsigned int test = tableSearch(table, clef);
	if (test != FAIL)
		return DUBBED;

	if (table->csize > 0.7 * table->msize)
	{
		int test = tableExpansion(&table);
		if (test == OOM)
			return OOM;
	
		*ptable = table;						//because we've changed the table
	}
								//we didn't find element with such key, so that we'll insert element
	size_t len = strlen(clef);

	char *key = (char *) calloc(len + 1, sizeof(char));
	if (key == NULL)
		return OOM;

	strcpy(key, clef);

	KeySpace elem;
	elem.state = OCCUPIED;
	elem.key = key;
	elem.list = list;
//	if (elem.info == NULL)
//		return OOM;
	
	strcpy(elem.key, clef);

	unsigned int n = 0;
	unsigned int index = hash1(key) % table->msize;
	while (n < table->msize)
	{
		if (table->ks[index].state != OCCUPIED)
		{
			table->ks[index] = elem;
			table->csize++;
			return OK;
		}
		n++;
		index = (index + hash2(key)) % table->msize;
	}
	
	free(elem.key);
	elem.key = NULL;
	return OOM;								//but we'll never reach it
}

ErrorCode tableDelete(Graph *table, const char *key)
{
	if (table == NULL)
		return OOM;

	
	unsigned int index = tableSearch(table, key);
	if (index == FAIL)
		return FAIL;
	
	table->ks[index].state = DELETED;
	free(table->ks[index].key);
	table->ks[index].key = NULL;
	table->csize--;

	return OK;							
}


	

/*int tablePrint(Table *table)
{
	if (table == NULL) 
		return FAIL;
	if (table->csize == 0)
		return FAIL;

	printf("\nTABLE: size = %d, max size = %d\n", table->csize, table->msize);
	printf("_______________________________________________________________________________\n");
	printf("          index          |            key           |	       info\n");
	printf("_______________________________________________________________________________\n");
	for (unsigned int i = 0; i < table->msize; i++)
		if (table->ks[i].state == OCCUPIED)
		{
			printf("%-25u| %s| %d", i, table->ks[i].key, table->ks[i].info);
			printf("_______________________________________________________________________________\n");
		}

	return SUCCESS;
}


int element_print(Table *table, unsigned int index)
{
	if (table == NULL)
		return FAIL;
	
	if (table->ks[index].state != OCCUPIED)
		return FAIL;
		 
	printf("_______________________________________________________________________________\n");
	printf("          index          |            key           |	       info\n");
	printf("_______________________________________________________________________________\n");
	printf("%-25u| %s| %d\n", index, table->ks[index].key, table->ks[index].info);
	printf("_______________________________________________________________________________\n");


	return SUCCESS;
}
*/

