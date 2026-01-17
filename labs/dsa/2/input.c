#include <stdio.h>
#include "input.h"

int getInt(int *intp, int low, int high)
{
    int n;
    int test = scanf("%d", &n);
    
    if (test == -1) //если EOF
        return EOF;

    if ((test == 0) || ((n < low) || (n > high))) //если фигня, то не делаем гетчар
        return FAIL;

    //if ((test == 0) || ((flag == 1) && (n <= 0))) //если фигня, то не делаем гетчар
      //  return FAIL;

    int c = getchar();
    
    if ((c != ' ') && (c != '\n'))
        return FAIL;
    else //то число считалось успешно
    {
        *intp = n; 
        return 0; 
    }
}

int getInt_right(int *m, char *c, int low, int high)
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
		    printf("\nRepeat number of %s correctly ->\n", c);
		    scanf("%*[^\n]");
		}
		else
		    break;
	}
	    return 0;
}

int get_probably(double *m, char *s)
{
	int test;
	int c;
	
	while (1)
	{
		test = scanf("%lf", m);
		if (test == EOF)
			return EOF;
		if ((*m <= 0.0) || (*m > 1.0))
		{
			printf("Write %s  correctly ->", s);
			scanf("%*[^\n]");
			getchar();			
			continue;
		}
		c = getchar();
		if ((c != ' ') && (c != '\n'))
		{
			printf("Write %s correctly ->", s);
			scanf("%*[^\n]");
			getchar();
		}
		else
		{
			break;
		}
	}
	return 0;
}			

int input(int *size_queue, int *count_nodes, int *low_range, int *high_range, double *p1, double *p2, double *p3)
{
	int test;
	*low_range = 1, *high_range = 0;

	printf("\nprint size of queues ->\n");
	test = getInt_right(size_queue, "items in queues", 1, 2147483647);
	if (test == EOF)
		return EOF;  
	
	printf("\nprint number of nodes ->\n");
	test = getInt_right(count_nodes, "nodes", 1, 2147483647); 		//подходят только положительные
	if (test == EOF)
		return EOF;

	printf("\nprint boundaries ->\n");
	while (*low_range > *high_range)
	{
		test = getInt_right(low_range, "low boundary", -2147483647, 2147483647); 	
		if (test == EOF)
			return EOF;		
		test = getInt_right(high_range, "high boundary", -2147483647, 2147483647);
		if (test == EOF)
			return EOF;
		
		if (*low_range > *high_range)
			printf("\nrepeat range within [low, high] ->\n");
		
	}
	
	printf("\nprint probabilities ->\n");
	test = get_probably(p1, "p1");
	if (test == EOF)
		return EOF;

	test = get_probably(p2, "p2");
	if (test == EOF)
		return EOF;

	test = get_probably(p3, "p3");
	if (test == EOF)
		return EOF;

	printf("\ncount_nodes = %d,\nlow boundary = %d, \nhigh boundary = %d,\n", *count_nodes, *low_range, *high_range);	
	printf("\np1 = %lf,\np2 =  %lf,\np3 = %lf,", *p1, *p2, *p3);


	return 0;	
}








