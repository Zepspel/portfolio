#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "table.h"
#include "dialog.h"

int getInt_file(FILE *file, unsigned int *intp, unsigned int low, unsigned int high)
{
	unsigned int n;
	int test = fscanf(file, "%d", &n);
    
	if (test == -1) //если EOF
		return EOF;

	if ((test == 0) || ((n < low) || (n > high))) //если фигня, то не делаем гетчар
		return FAIL;


	int c = fgetc(file);
    
	if ((c != '\n') && (c != '\t'))
		return FAIL;
	else //то число считалось успешно
	{
		*intp = n; 
		return 0; 
	}
}
int getInt(unsigned int *intp, unsigned int low, unsigned int high)
{
	unsigned int n;
	int test = scanf("%d", &n);
    
	if (test == -1) //если EOF
		return EOF;

	if ((test == 0) || ((n < low) || (n > high))) //если фигня, то не делаем гетчар
		return FAIL;


	int c = getchar();
    
	if ((c != ' ') && (c != '\n'))
		return FAIL;
	else //то число считалось успешно
	{
		*intp = n; 
		return 0; 
	}
}

int getInt_right(unsigned int *m, char *c, unsigned int low, unsigned int high)
{
	int test2;
	while (1) 
    	{
		test2 = getInt(m, low, high); //flag 1 значит, что отрицательные числа не подходят
		if (test2 == EOF) { //если EOF, то инпут не удался
		    return EOF;
		}
		else if (test2 == FAIL)
		{
		    printf("\nRepeat %s value correctly -> ", c);
		    scanf("%*[^\n]");
		}
		else
		    break;
	}
	return 0;
}

int case_insert_element(Table *table)
{
	unsigned int key, par, info;
	printf("for insertion enter key value -> ");
	int test = getInt_right(&key, "key", 1, UINT_MAX);
	if (test == EOF)
		return EOF;

	printf("enter par value -> ");	
	test = getInt_right(&par, "par", 0, UINT_MAX);
	if (test == EOF)
		return EOF;
	

	printf("enter info value -> ");
	test = getInt_right(&info, "info", 0, UINT_MAX);
	if (test == EOF)
		return EOF;
	printf("\n");
	test = t_insert(table, key, par, info);
	switch (test)
	{
		case DUBBED:
			printf("keys are dubbed\n");
			break;
		case NO_PARENT:
			printf("such parent doesn't exist, you can use '0' parent\n");
			break;
		case FAIL:
			printf("key cann't be '0'\n");
			break;
		case OOM:
			printf("table is full, delete some elements at first\n");
			break;
		case SUCCESS:
			printf("insertion complited\n");
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
	test = getInt_right(&key, "key", 1, UINT_MAX);
	if (test == EOF)
		return EOF;
	test = t_delete(table, key);
	switch (test)
	{
		case NOT_EMPTY:
			printf("this element have children, delete them at first\n");
			break;
		case FAIL:
			printf("element with such key doesn't exist\n");
			break;
		case SUCCESS:
			printf("element was deleted\n");
			break;
	}
	return 0;
}	

int case_search_bykey(Table *table)
{
	int test;
	unsigned int key;
	printf("for serching enter key value -> ");
	test = getInt_right(&key, "key", 1, UINT_MAX);
	if (test == EOF)
		return EOF;
	else
	{
		test = t_search_key(table, key);	
		if (test == FAIL)
			printf("element with such key doesn't exist\n");
		
		printf("\nkey		*info		parent\n");
		t_elem_print(table, test);
	}
	return 0;
}

void case_print_table(Table *table)
{
	int test = t_print(table);
	if (test == FAIL)
		printf("table is empty\n");
}

int case_search_children(Table *table)
{
	unsigned int par;
	printf("for serching children enter par value -> ");
	int test = getInt_right(&par, "par", 1, UINT_MAX);
	if (test == EOF)
		return EOF;
	Table *table_kids = t_kids_make(table, par);
	if (table_kids == NULL)
		printf("There are no children for this key\n");
	else
	{
		t_print(table_kids);
		t_erase(table_kids);
		table_kids = NULL;
	}
	
	return 0;
}


void case_help()
{
	printf("\nChoose one of them:");
	printf("\n1: insert element\n2: delete element by key\n3: search element by key\n");
	printf("4: print table\n5: search childrens by key\n6: import from the file\n7: clear the table\n8: help\n9: exit\n\n");
}	


int case_import_file(Table *table)
{
	t_clear(table);

	int counter = 0;
	int test = 0;
	int flag = 1;
	unsigned int key, par, info;
	char file_name[256];

	printf("\nenter name of file -> ");
	scanf("%s", file_name);

	FILE *file = fopen(file_name,"r");
	if (file == NULL)
	{
		printf("\nerror opening file\n");
		return 0;
	}

//firstly we are looking data if its correct
	while (test != EOF)
	{
		test = getInt_file(file, &key, 1, UINT_MAX);
		if (test == EOF)
		{
			flag = 0;
			break;
		}
		else if (test == FAIL)
			break;
			
		test = getInt_file(file, &par, 0, UINT_MAX);
		if (test == EOF)
			break;
		else if (test == FAIL)
			break;
		
		test = getInt_file(file, &info, 0, UINT_MAX);
		if (test == EOF)
			break;
		else if (test == FAIL)
			break;
		
		counter++;				//it will be increased when we red cell for table correctly
	}
	if (flag == 1)
	{			
		printf("\nincorrect format of data");
		fclose(file);
		return 0;
	}

//then we put data onto the table
	test = 0;
	rewind(file);		//go to the beginning of file	
	while (counter != 0)
	{
		getInt_file(file, &key, 1, UINT_MAX);
		getInt_file(file, &par, 0, UINT_MAX);
		getInt_file(file, &info, 0, UINT_MAX);
		test = t_insert(table, key, par, info);
		switch (test)
		{
			case DUBBED:
				printf("keys are dubbed\n");
				break;
			case NO_PARENT:
				printf("some parent doesn't exist, you can use '0' parent\n");
				break;
			case FAIL:
				printf("key cann't be '0'\n");
				break;
			case OOM:
				printf("exceeding size of the table\n");
				break;
			case SUCCESS:
				break;////////default
			default:
				printf("unknown error\n");
		}
		if (test != SUCCESS)
		{
			t_clear(table);
			fclose(file);
			return 0;
		}
		counter--;
	}
	printf("the recording from the file has been complited\n");	
	fclose(file);
	return 0;
}


int dialog(Table *table)
{
	unsigned int choice;
	int test;
	int flag = 0;			//for exit

	case_help();

	while (1)
	{	
		test = getInt_right(&choice, "action", 1, 9);
		if (test == EOF)
			return EOF;
		
		switch(choice)
		{
			case 1:
				//we'll ask key, par and info
				test = case_insert_element(table);
				if (test == EOF)
					return EOF;
				break;
			
			case 2:
				test = case_delete_element(table);
				if (test == EOF)
					return EOF;
				break;
			
			case 3:						
				test = case_search_bykey(table);
				if (test == EOF)
					return EOF;
				break;
		
			case 4:						
				case_print_table(table);
				break;
			case 5:
				test = case_search_children(table);
				if (test == EOF)
					return EOF;
				break;
			
			case 6:
				case_import_file(table);
				break;	
			case 7:
				t_clear(table);
				break;
			case 8:
				case_help();
				break;
			
			case 9:						
				flag = EXIT;
				break;
			default:
				printf("unknown error\n");
		}
		if (flag == EXIT)
			break;
	}
	return 0;
}




