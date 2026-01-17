#include <stdio.h>
#include <stdlib.h>
#include "matr.h"
#include "input.h"
#include "line.h"


int make_matrix(int n, Matrix *matr) 
{
    ErrorCode result = SUCCESS; 
    if (n == 0)
    	matr->linep = NULL;
    else
    {
    	matr->linep =  (Line *) calloc(n, sizeof(Matrix));
    	if (matr->linep == NULL)
    	{
			result = OOM; //возвращает сообщение об ошибке
			matr->lines = 0;
    	}
    }
    matr->lines = n;
    return result;
}


int input_matrix(Matrix *rm)
{
	int error = 0; //для сообщения об ошибке в функциях мейк_
    int test; //cюда кладется значение возвращаемое getInt-ом
    int m; //число для строк и элементов
    //int *p = NULL; //указатель, через который получаем доступ к ячейкам строки
    printf("Enter number of lines: --> ");

    char *str1 = "lines";
	test = getInt_right(&m, str1); //считываем число строк
	if (test == END_OF_FILE)
		return END_OF_FILE;

    if (rm == NULL)
            return OOM;
            
    error = make_matrix(m, rm); 
    if (error == OOM)
    	return OOM;
    
    
    for (int i = 0; i < rm->lines; i++) //инициализируем строчки
    {
    	printf("Enter number of items in line %d: --> ", i + 1);
    	error = input_line(rm->linep+i);
    	if (error == OOM) //встретился EOF
    	{
    		erase_matrix(rm);
    		return OOM;
    	}
    } 
    return 0;
}


int output_matrix(const Matrix *rm)
{
    if ((rm == NULL) || (rm->lines == 0))
    {
        printf("It's void matrix\n");
        return 0;
    }
    printf("\n"); 
    
    for (int i = 0; i < rm->lines; i++)
    {
    	output_line((rm->linep)+i); //вывожу строки
    }
    return 0;
}


int erase_matrix(Matrix *M)
{
    if (M == NULL)
        return 0;

    for (int i = 0; i < M->lines; i++)
    	erase_line((M->linep)+i);
    	
    free(M->linep);
    M->linep = NULL;

    return 0;
}

Line *individual(Matrix *rm)
{
	int match; //количество совпадающих элементов с строках
	int error = 0;
    if ((rm == NULL) || (rm->lines == 0)) //ПРОВЕРКА ЧТО ДАНА АДЕКВАТНАЯ МАТРИЦА
    {
        printf("It's void matrix");
        return NULL;
    }
    //int flag = 0; //1 - если элемент i-й строки встретился в (i+1)-й
    int t = 0; //счетчик для нижней строки

	Line *b = (Line *) calloc(1, sizeof(Line));
    if (b == NULL)
    	return NULL;

    error = make_line(rm->lines, b);
    if (error == OOM)
    	return NULL;

    for(int i = 0; i < rm->lines; i++)
    {
        if (i == rm->lines - 1)
            t = 0;
        else
            t = i + 1;

		match = compare_lines(rm->linep + i, rm->linep + t); //сравниваем строки
		b->a[i] = (rm->linep+i)->n - match;

    }
    return b;
}


