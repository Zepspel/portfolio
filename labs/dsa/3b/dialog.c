#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "table.h"
#include "dialog.h"


int getInt(unsigned int *intp, unsigned int low, unsigned int high)
{
	unsigned int n;
	int test = scanf("%d", &n);
    
	if (test == EOF) 
		return EOF;

	if ((test == 0) || ((n < low) || (n > high)))
		return FAIL;


	int c = getchar();
    
	if ((c != ' ') && (c != '\n'))
		return FAIL;
	else 
	{
		*intp = n; 
		return 0; 
	}
}

int getInt_right(unsigned int *m, char *c, unsigned int low, unsigned int high)
{
	int test;
	while (1) 
    	{
		test = getInt(m, low, high); 
		if (test == EOF) { 
		    return EOF;
		}
		else if (test == FAIL)
		{
		    printf("\nRepeat %s value correctly -> ", c);
		    scanf("%*[^\n]");
		}
		else
		    break;
	}
	return 0;
}



int get_word(char *choice)
{
	int test = scanf("%255s", choice);
	if (test == EOF)
		return EOF;	

	if (test == 0)
		return FAIL;

	int c = getchar();
	if (c == EOF)
		return EOF;
	
	else if ((c != ' ') && (c != '\n') && (c != '\t'))
		return FAIL;
	
	
	return SUCCESS;
}


int get_word_pattern(char *choice, const char * const *option, int amount_option)
{
	int test;
	while (1) 
    	{
		test = get_word(choice); 
		if (test == EOF) 
		    return EOF;
		
		else if (test == FAIL)
		{
		    printf("\nRepeat action correctly -> ");
		    scanf("%*[^\n]");
		}
		
		else 
		{
			for (int i = 0; i < amount_option; i++)
			{	
				if (strcmp(choice, option[i]) == 0)
					return SUCCESS;
			}			
			printf("\nRepeat action correctly -> ");
		}
	
	}
}

int get_word_right(char *word)
{
	int test;
	while (1)
	{
		test = get_word(word);
		if (test == EOF)
			return EOF;

		else if (test == FAIL)
		{
		    printf("\nRepeat action correctly -> ");
		    scanf("%*[^\n]");
		}
		else
			break;
	}
	
	return SUCCESS;	
}
	

void case_help()
{
	printf("options:\nh - help\ni - insert element into the table\nd - delete element from the table\np - print the table\n");
	printf("s - search for an element by key\nimp - import the table from binary file\n");
	printf("exp - export the table to binary file\nc - clean\nq - quite\n\n");
}	

int case_insert_element(Table **ptable)
{
	Table *table = *ptable;

	unsigned int key;
	char info[256];

	printf("for insertion enter key value -> ");
	int test = getInt_right(&key, "key", 0, UINT_MAX);
	if (test == EOF)
		return EOF;

	printf("enter info -> ");
	test = get_word_right(info);
	if (test == EOF)
		return EOF;

			

	test = table_insert(&table, key, info);
	*ptable = table;
	switch (test)
	{
		case DUBBED:
			printf("keys are dubbed\n");
			break;
		case FAIL:
			printf("key cann't be '0'\n");
			break;
		case OOM:
			printf("hash function couldn't find place\n");
			break;
		case SUCCESS:
			printf("insertion completed\n");
			break;
		default:
			printf("unknown error\n");
	}
	return 0;
}	


int case_delete_element(Table *table)
{
	int test;
	unsigned int key;
	printf("for deletion enter key value -> ");
	test = getInt_right(&key, "key", 0, UINT_MAX);
	if (test == EOF)
		return EOF;
	test = table_delete(table, key);
	switch (test)
	{
		case FAIL:
			printf("element with such key doesn't exist\n");
			break;
		case SUCCESS:
			printf("element was deleted\n");
			break;
	}
	return 0;
}

void case_print_table(Table *table)
{
	int test = table_print_ti(table);
	if (test == FAIL)
		printf("table is empty\n");
}


int case_search_element(Table *table)
{
	int test;
	unsigned int index;
	unsigned int key;
	printf("for searching enter key value -> ");
	test = getInt_right(&key, "key", 0, UINT_MAX);
	if (test == EOF)
		return EOF;
	else
	{
		index = table_search_index(table, key);	
		if (index == FAIL)
			printf("element with such key doesn't exist\n");
		else		
			element_print(table, index);
	}
	return 0;
}

int case_export_table(Table *table)
{
	char file_name[256];

	printf("\nenter file name -> ");
	scanf("%255s", file_name);

	FILE* file = fopen(file_name, "wb");
	if (file == NULL) 
	{
    		perror("file opening error\n");
    		return FAIL;
	}
	
	int test = table_export(table, file);
	if (test == FAIL)
		printf("export error");
	else
		printf("export complited successfully\n");

	fclose(file);
}
	
int case_import_table(Table **ptable)
{
	char file_name[256];

	printf("\nenter file name -> ");
	scanf("%255s", file_name);

	FILE* file = fopen(file_name, "rb");
	if (file == NULL) 
	{
    		perror("file opening error\n");
    		return FAIL;
	}
	
	int test = table_import(ptable, file);
	if (test == FAIL)
		printf("import error\n");
	else if (test == OOM)
	{
		printf("lack of memory\n");
		return EOF;
	}
	else if (test == SUCCESS)
		printf("import complited successfully\n");
	else if (test = DUBBED)
		printf("inapropriate file\n");
	else
		printf("unknown error\n");



	fclose(file);
	
	return 0;
}
	
int case_clean_table(Table *table)
{
	int test = table_clean(table);
	if (test == FAIL)
		printf("table doesn't exist\n");
	else
		printf("table has been cleaned\n");
	
	return 0;
}


int dialog(Table **ptable)
{
	Table *table = *ptable;
	int test;
	char choice[256];	
	const char * const option[] = {"help", "h", "insert", "i", "delete", "d", "clean", "c", "print", "p", "search", "s", "imp", "import", "exp", "export", "quite", "q"};
	int amount_option = sizeof(option) / sizeof(option[0]);

	case_help();

	while(1)
	{
		test = get_word_pattern(choice, option, amount_option);
		if (test == EOF)
		{
			*ptable = table;
			return EOF;
		}

		if ((strcmp(choice, "help") == 0) || (strcmp(choice, "h") == 0))
		{
			case_help();
		}

		else if ((strcmp(choice, "insert") == 0) || (strcmp(choice, "i") == 0))
		{
			test = case_insert_element(&table);
			if (test == EOF)
			{
				*ptable = table;
				return EOF;
			}
		}

		else if ((strcmp(choice, "delete") == 0) || (strcmp(choice, "d") == 0))
		{
			test = case_delete_element(table);
			if (test == EOF)
			{
				*ptable = table;
				return EOF;		
			}
		}

		else if ((strcmp(choice, "print") == 0) || (strcmp(choice, "p") == 0))
		{
			case_print_table(table);
		}	
		else if ((strcmp(choice, "search") == 0) || (strcmp(choice, "s") == 0))
		{
			test = case_search_element(table);
			if (test == EOF)
			{
				*ptable = table;
				return EOF;	
			}
		}
		else if ((strcmp(choice, "import") == 0) || (strcmp(choice, "imp") == 0))
		{
			test = case_import_table(&table);
			if (test == EOF)
			{
				*ptable = table;
				return EOF;
			}
		}
		else if ((strcmp(choice, "export") == 0) || (strcmp(choice, "exp") == 0))
		{
			test = case_export_table(table);
		}
		else if ((strcmp(choice, "clean") == 0) || (strcmp(choice, "c") == 0))
		{
			test = case_clean_table(table);
		}		
				
		else if ((strcmp(choice, "quite") == 0) || (strcmp(choice, "q") == 0))
		{
			break;
		}
		else
		{
			printf("unknown error\n");
		}
	}
	
	*ptable = table;
	return SUCCESS;
}
