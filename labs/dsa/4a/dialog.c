#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "tree.h"
#include "dialog.h"
#include "timing.h"


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
	
	
	return OK;
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
					return OK;
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
	
	return OK;	
}
	

void case_help()
{
	printf("options:\nh - help\ni - insert element into the tree\nd - delete element from the tree\np - print the tree in reverse order\n");
	printf("r - print elements in range [a, b] in reverse order\nt - print tree like thee\ns - search for an element by key\nm - max elements\n");
	printf("imp - import the tree from text file\nexp - export the tree to dot file\ntime - timing\nc - clean the tree\nq - quite\n\n");
}	

ErrorCode case_insert_element(Tree *tree)
{
	char key[256];
	unsigned int info;

	printf("for insertion enter key value -> ");
	int test = get_word_right(key);
	if (test == EOF)
		return EOF;

	printf("enter info -> ");
	test = getInt_right(&info, "info", 0, UINT_MAX);
	if (test == EOF)
		return EOF;

			
	test = tree_insert(tree, key, info);
	switch (test)
	{
		case OOM:
			printf("out of memory\n");
			break;
		case OK:
			printf("insertion completed\n");
			break;
		default:
			printf("unknown error\n");
	}
	return OK;
}	


ErrorCode case_delete_element(Tree *tree)
{
	char key[256];

	printf("for deletion enter key value -> ");

	int test = get_word_right(key);
	if (test == EOF)
		return EOF;
	
	test = tree_delete(tree, key);
	switch (test)
	{	
		case OOM:
			printf("out of memory\n");
			break;
		case FAIL:
			printf("element with such key doesn't exist\n");
			break;
		case OK:
			printf("element was deleted\n");
			break;
		default:
			printf("unknown error\n");

	}
	return OK;
}

void case_print_tree(Tree *tree)
{
	int test = tree_traversal(tree);
	switch (test)
	{	
		case OOM:
			printf("out of memory\n");
			break;
		case FAIL:
			printf("empty tree\n");
			break;
		case OK:
			break;
		default:
			printf("unknown error\n");

	}
}

ErrorCode case_range_tree(Tree *tree)
{
	printf("enter range [a b] -> ");
	char first[256];
	char last[256];
	int test = scanf("%255s", first);
	if (test == EOF)
		return EOF;
		
	int next_char = getchar();
	if (next_char == EOF)
		return EOF;
    	if ((next_char != '\n') && (next_char != ' ') && (next_char!= 't'))
        {
		printf("Too large key\n");
		scanf("%*[^\n]");						//clear buffer
		return OK;
	}

	test = scanf("%255s", last);
	if (test == EOF)
		return EOF;
		
	next_char = getchar();
	if (next_char == EOF)
		return EOF;
    	if ((next_char != '\n') && (next_char != ' ') && (next_char!= 't'))
        {
		printf("Too large key\n");
		scanf("%*[^\n]");						//clear buffer
		return OK;
	}

	test = strcmp(first, last);
	if (test >= 0)
	{
		printf("invalid range\n");
		return OK;
	}

	test = tree_range(tree, first, last);
	if (test == FAIL)
		printf("there are no such elements\n");

	
	return OK;
}

ErrorCode case_search_element(Tree *tree)
{
	unsigned int index;
	char key[256];

	printf("for searching enter key value -> ");

	int test = get_word_right(key);
	if (test == EOF)
		return EOF;
	
	tree_search_print(tree, key);

	
	return OK;
}

void case_max_tree(Tree *tree)
{
	tree_max_print(tree);
}


ErrorCode case_export_tree(Tree *tree)
{
	char file_name[256];
	char command[1024];
	
	printf("\nenter file name -> ");
	int test = scanf("%255s", file_name);
	if (test == EOF)
		return EOF;
	
	test = tree_todot(tree, file_name);
	if (test == FAIL)
	{
		printf("export error\n");
		return OK;
	}
	else
		printf("export to dot the file complited successfully\n");


    	snprintf(command, sizeof(command), "dot -Tpng %s -o tree.png", file_name);
    	test = system(command);
	if (test == OK)
		printf("png file was created\n");
	else
		printf("error creating png file\n");

	return OK;
}
	

void case_clean_tree(Tree *tree)
{
	int test = tree_clean(tree);
	if (test == FAIL)
		printf("tree doesn't have any elements\n");
	else if (test == OK)
		printf("table has been cleaned\n");
	else
		printf("out of memory\n");
	
}

ErrorCode case_import_tree(Tree *tree)
{
	char file_name[256];
	printf("enter file name -> ");
	int test = scanf("%255s", file_name);
	if (test == EOF)
		return EOF;
	else if (test >= 255)
	{
		printf("file name too long\n");
		return FAIL;
	}
	
	FILE *file = fopen(file_name,"r");
	if (file == NULL)
	{
		printf("file doesn't exist\n");
		return FAIL;
	}

	test = tree_import(tree, file);
	fclose(file);
	if (test == OOM)
	{
		printf("out of memory\n");
		return OOM;
	}
	else if (test == FAIL)
	{
		printf("can't import full tree\n");
		return FAIL;
	}
	else 
	{
		printf("tree has been imported successfully\n");
		return OK;
	}
}

#define SIZE 100
#define	SIZE_M 1000000

ErrorCode case_timing_tree(Tree *tree)
{
	if (tree == NULL)
		return FAIL;

	
	printf("wait a few minutes, please\n");

	srand(time(NULL));
 	unsigned int time;

	size_t count = 10;
	size_t size = SIZE;

	FILE *file = fopen("timing/insert.csv", "w");
	if (file == NULL)
		return OOM;
	fprintf(file, "N, Time\n");

	while (size < SIZE_M)
	{
		time = timing_insert(tree, size, count);
		fprintf(file, "%ld, %u\n", size, time);
		size *= 2;
	}
	fclose(file);
	size  = SIZE;
	
	file = fopen("timing/delete.csv", "w");
	if (file == NULL)
		return OOM;
	fprintf(file, "N, Time\n");

	while (size < SIZE_M)
	{
		time = timing_delete(tree, size, count);
		fprintf(file, "%ld, %u\n", size, time);
		size *= 2;
	}
	fclose(file);
	size = SIZE;

	file = fopen("timing/search.csv", "w");
	if (file == NULL)
		return OOM;
	fprintf(file, "N, Time\n");

	while (size < SIZE_M)
	{
		time = timing_search(tree, size, count);
		fprintf(file, "%ld, %u\n", size, time);
		size *= 2;
	}
	fclose(file);
	size = SIZE;


	file = fopen("timing/maximum.csv", "w");
	if (file == NULL)
		return OOM;
	fprintf(file, "N, Time\n");

	while (size < SIZE_M)
	{
		time = timing_maximum(tree, size, count);
		fprintf(file, "%ld, %u\n", size, time);
		size *= 2;
	}
	fclose(file);
	size = SIZE;

	file = fopen("timing/traversial.csv", "w");
	if (file == NULL)
		return OOM;
	fprintf(file, "N, Time\n");

	while (size < SIZE_M)
	{
		time = timing_traversial(tree, size);
		fprintf(file, "%ld, %u\n", size, time);
		size *= 2;
	}
	fclose(file);			
	
	printf("timing is completed successfully\n");
	return OK;
}

ErrorCode dialog(Tree *tree)
{
	int test;
	char choice[256];	
	const char * const option[] = {"help", "h", "tree", "t", "insert", "i", "delete", "d", "m", "max", "clean", "c", "r", "range", "print", "p", "search", "s", "exp", "export", "imp", "import", "c", "clean", "time", "quite", "q"};
	int amount_option = sizeof(option) / sizeof(option[0]);

	case_help();

	while(1)
	{
		test = get_word_pattern(choice, option, amount_option);
		if (test == EOF)
			return EOF;

		if ((strcmp(choice, "help") == 0) || (strcmp(choice, "h") == 0))
			case_help();


		else if ((strcmp(choice, "insert") == 0) || (strcmp(choice, "i") == 0))
		{
			test = case_insert_element(tree);
			if (test == EOF)
				return EOF;
		}

		else if ((strcmp(choice, "delete") == 0) || (strcmp(choice, "d") == 0))
		{
			test = case_delete_element(tree);
			if (test == EOF)
				return EOF;		
		}

		else if ((strcmp(choice, "print") == 0) || (strcmp(choice, "p") == 0))
			case_print_tree(tree);
		
		else if ((strcmp(choice, "search") == 0) || (strcmp(choice, "s") == 0))
		{
			test = case_search_element(tree);
			if (test == EOF)
				return EOF;	
		}
		else if ((strcmp(choice, "export") == 0) || (strcmp(choice, "exp") == 0))
		{
			test = case_export_tree(tree);
			if (test == EOF)
				return EOF;	
		}
		else if ((strcmp(choice, "import") == 0) || (strcmp(choice, "imp") == 0))		
		{
			test = case_import_tree(tree);
			if (test == EOF)
				return EOF;
		}
		else if ((strcmp(choice, "clean") == 0) || (strcmp(choice, "c") == 0))
			case_clean_tree(tree);
		else if ((strcmp(choice, "range") == 0) || (strcmp(choice, "r") == 0))		
		{
			test = case_range_tree(tree);
			if (test == EOF)
				return EOF;
		}
		else if ((strcmp(choice, "max") == 0) || (strcmp(choice, "m") == 0))		
			case_max_tree(tree);

		else if ((strcmp(choice, "tree") == 0) || (strcmp(choice, "t") == 0))		
			tree_print_bashshell(tree);

		else if (strcmp(choice, "time") == 0)
		{	
			test = case_timing_tree(tree);
			if (test == EOF)
				return EOF;
		}

		else if ((strcmp(choice, "quite") == 0) || (strcmp(choice, "q") == 0))
			break;
		else
			printf("unknown error\n");
	}
	
	return EOF;
}
