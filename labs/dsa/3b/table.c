#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "table_privat.h"
#include "table_file.h"

#define hash1(key) ((((key * 0x9e3779b9) ^ (key >> 12) ^ (key << 4)) * 0x85ebca6b) ^ ((key * 0x9e2679b9) >> 10))
	
#define hash2(key) (key * 0xb5f1a359)


int table_expansion(Table **ptable)
{
	if (ptable == NULL)
		FAIL;

	Table *table = *ptable;
	if (table == NULL)
		FAIL;

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
	Table *new_table = table_make(new_size);
	unsigned int key;
	
	for (int i = 0; i < table->msize; i++)
	{
								//get one element
		if (table->ks[i].state == OCCUPIED)
			table_insert(&new_table, table->ks[i].key, table->ks[i].info);

	}

	table_erase(table);
	*ptable = new_table;
	
	return SUCCESS;
}
	
		
		



Table *table_make(unsigned int msize)
{
	if (msize == 0)
		return NULL;

	Table *table = (Table *) calloc(1, sizeof(Table));
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
		table->ks[i].info = NULL;
	}

	return table;	
}


int table_clean(Table *table)
{
	if (table == NULL)
		return FAIL;

	for (unsigned int i = 0; i < table->msize; i++)
		if (table->ks[i].info != NULL)
		{
			table->ks[i].state = EMPTY;
			free(table->ks[i].info);
			table->ks[i].info = NULL;
		}

	table->csize = 0;
	
	return SUCCESS;
}



int table_erase(Table *table)
{
	if (table == NULL)
		return FAIL;
	
	for (unsigned int i = 0; i < table->msize; i++)
		if (table->ks[i].info != NULL)
		{
			free(table->ks[i].info);
			table->ks[i].info = NULL;
		}


	if (table->ks != NULL)
	{
		free(table->ks);
		table->ks = NULL;
	}
	
	free(table);
	table = NULL;

	return SUCCESS;	
}


int table_search_index(Table *table, unsigned int key)
{
	unsigned int n = 0;							//counter for number of elements that we wiewed
	//unsigned int index = hash_function(table->msize, key, n);
	unsigned int index = hash1(key) % table->msize;
	while ((table->ks[index].state != EMPTY) && (n < table->msize))
	{
		if ((table->ks[index].key == key) && (table->ks[index].state != DELETED))
			return index;
		n++;
		//index = hash_function(table->msize, key, n);
		index = (index + hash2(key)) % table->msize;
	}
	return FAIL;
}


int table_insert(Table **ptable, unsigned int key, char *info)			//we consider DELETED like free
{	
	Table *table = *ptable;

	if (table == NULL)
		return FAIL;


	unsigned int test = table_search_index(table, key);
	if (test != FAIL)
		return DUBBED;

	if (table->csize > 0.7 * table->msize)
	{
		int test = table_expansion(&table);
		if (test == OOM)
			return OOM;
	
		*ptable = table;						//because we've changed the table
	}
										//we didn't find element with such key, so that we'll insert element
	KeySpace elem;
	elem.state = OCCUPIED;
	elem.key = key;
	elem.info = (char *) calloc(strlen(info) + 1, sizeof(char));		//allocating memory for info
	if (elem.info == NULL)
		return OOM;
	
	strcpy(elem.info, info);

	unsigned int n = 0;
	//unsigned int index = hash_function(table->msize, key, n);
	unsigned int index = hash1(key) % table->msize;
	while (n < table->msize)
	{
		if (table->ks[index].state != OCCUPIED)
		{
			table->ks[index] = elem;
			table->csize++;
			return SUCCESS;
		}
		n++;
		//index = hash_function(table->msize, key, n);
		index = (index + hash2(key)) % table->msize;
	}
	
	free(elem.info);
	elem.info = NULL;
	return OOM;								//but we'll never reach it
}

int table_delete(Table *table, unsigned key)
{
	if (table == NULL)
		return FAIL;

	
	unsigned int index = table_search_index(table, key);
	if (index == FAIL)
		return FAIL;
	
	table->ks[index].state = DELETED;
	free(table->ks[index].info);
	table->ks[index].info = NULL;
	table->csize--;

	return SUCCESS;							
}


	

int table_print(Table *table)
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
			printf("%-25u| %-25u| %-25s\n", i, table->ks[i].key, table->ks[i].info);
			//printf("%-25u| %-25u| %-25s\n", i, table->ks[i].key, "Hello");
			printf("_______________________________________________________________________________\n");
		}

	return SUCCESS;
}

int table_print_ti(Table *table)
{
	if (table == NULL) 
		return FAIL;
	if (table->csize == 0)
		return FAIL;

	printf("\nTABLE: size = %d, max size = %d\n", table->csize, table->msize);
	printf("____________________________________________________________________________________________________________\n");
	printf("          index          | 	       busy	     |            key           |	       info\n");
	printf("____________________________________________________________________________________________________________\n");
	for (unsigned int i = 0; i < table->msize; i++)
		printf("%-25u| %-25u | %-25u| %-25s\n", i, table->ks[i].state, table->ks[i].key, table->ks[i].info);				
		printf("_______________________________________________________________________________\n");

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
	printf("%-25u| %-25u| %-25s\n", index, table->ks[index].key, table->ks[index].info);
	printf("_______________________________________________________________________________\n");


	return SUCCESS;
}



int table_export(Table *table, FILE *file)
{
	if ((table == NULL) || (file == NULL))
		return FAIL;

	
	int magic = 0xb5f1a359;
		
									//now we'll write info
	int length = 0;
	for (int i = 0; i < table->msize; i++)				//count size of array for info
	{
		if (table->ks[i].info != NULL)
			length = length + strlen(table->ks[i].info);
	}
	
	FileElem felem[table->msize];
	char finfo[length];						//array for info	
	int len = 0;
	int offset = 0;
	for (int i = 0; i < table->msize; i++)
	{
		if (table->ks[i].info != NULL)
			len = strlen(table->ks[i].info);
		else
			len = 0;
		
		if (len > 0)
			strcpy(finfo + offset, table->ks[i].info);
		
		felem[i] = (FileElem) {table->ks[i].state, table->ks[i].key, sizeof(FileTable) + table->msize * sizeof(FileElem) + 1 + offset};
		offset += len;
	}	


	FileTable ftable = (FileTable) {table->msize, table->csize, length, magic};
	int test = fwrite(&ftable, sizeof(FileTable), 1, file);	
	if (test != 1)						
		return FAIL;


	test = fwrite(felem, sizeof(FileElem), table->msize, file);
	if (test != table->msize)
		return FAIL;


	test = fwrite(finfo, sizeof(char), length, file);
	if (test != length)
		return FAIL;

	return SUCCESS;
}


int table_import(Table **ptable, FILE *file)
{
	Table *table = *ptable;
	if (file == NULL)
		return FAIL;
	
	FileTable ftable;
	int test = fread(&ftable, sizeof(FileTable), 1, file);	
	if (test != 1)
		return FAIL;
	if (ftable.magic != 0xb5f1a359)
		return DUBBED;

	FileElem felem[table->msize];
	test = fread(felem, sizeof(FileElem), table->msize, file);
	if (test != table->msize)
		return FAIL;


	char finfo[ftable.length];
	test = fread(finfo, sizeof(char), ftable.length, file);
									//by this point we've filled buffers

	table_erase(table);	
	table = table_make(ftable.msize);	
	if (table == NULL)
		return OOM;
	table->csize = ftable.csize;


	int len;
	int offset = 0;
	int letter = 0;
	for (unsigned int i = 0; i < table->msize; i++)
	{
		table->ks[i].state = felem[i].state;
		table->ks[i].key = felem[i].key;


		if (i != table->msize - 1)
			len = felem[i+1].offset - felem[i].offset;
		else
			len = sizeof(FileTable) + table->msize * sizeof(FileElem) + ftable.length  + 1 - felem[i].offset;
		

		if (len > 0)
		{
			table->ks[i].info = (char *) calloc(len+1, sizeof(char));
			if (table->ks[i].info == NULL)
				return OOM;
			
			while (letter < len)
			{
				table->ks[i].info[letter] = finfo[offset+letter];
				letter++;
			}
			table->ks[i].info[letter] = 0;						//it is '\0'
			letter = 0;
			offset += len;
		}	
	}

	*ptable = table;

	return SUCCESS;
}


/*int table_import(Table **ptable, FILE *file)
{
	Table *table = *ptable;
	if (file == NULL)
		return FAIL;
	

	unsigned int msize;
	int test = fread(&msize, sizeof(unsigned int), 1, file);
	if (test == 0)							//nothing was read
		return FAIL;
	
	table_erase(table);	
	table = table_make(msize);	
	if (table == NULL)
		return OOM;

	fread(&(table->csize), sizeof(unsigned int), 1, file);

	int len;
	for (unsigned int i = 0; i < table->msize; i++)
	{
		fread(&(table->ks[i].state), sizeof(CellState), 1, file);
		fread(&(table->ks[i].key), sizeof(unsigned int), 1, file);
		fread(&len, sizeof(int), 1, file);
		if (len > 0)
		{
			table->ks[i].info = (char *) calloc(len+1, sizeof(char));
			if (table->ks[i].info == NULL)
				return OOM;
			
			fread(table->ks[i].info, sizeof(char), len, file);
			*(table->ks[i].info+len) = 0;						//it's '\0'
			
		}
	}
	
	*ptable = table;

	

	return SUCCESS;
}



int table_export(Table *table, FILE *file)
{
	if ((table == NULL) || (file == NULL))
		return FAIL;

	int len;
	fwrite(&(table->msize), sizeof(unsigned int), 1, file);
	fwrite(&(table->csize), sizeof(unsigned int), 1, file);
	for (unsigned int i = 0; i < table->msize; i++)
	{
		fwrite(&(table->ks[i].state), sizeof(CellState), 1, file);
		fwrite(&(table->ks[i].key), sizeof(unsigned int), 1, file);
		if (table->ks[i].info != NULL)
			len = strlen(table->ks[i].info);
		else
			len = 0;
			
		fwrite(&len, sizeof(int), 1, file);
		if (len > 0)
			fwrite(table->ks[i].info, sizeof(char), len, file);
	}
	
	return SUCCESS;
}
*/


