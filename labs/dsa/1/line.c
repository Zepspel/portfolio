#include <stdio.h>
#include <stdlib.h>
#include "line.h"
#include "input.h"

int make_line(int n, Line *ln) //не через указатель, чтобы они не дублировались, к тому же я не передаю ее в другие функции
{
	ErrorCode result = SUCCESS;
	if (n == 0)
		ln->a = NULL;
	else
	{
		ln->a = (int *) calloc(n, sizeof(int));
		if (ln->a == NULL)
    	{
    		result = OOM;
    		ln->n = 0;
    	}
	}
	ln->n = n;
	return result;
}

int input_line(Line *ln) //i - номер строки
{
	int error = 0;
	int error2 = 0; //для мейклайн
	int test; //cюда кладется значение возвращаемое getInt-ом
	int *p = NULL;
	int m = 0; //кол-во элементов в строке

	
	char *str1 = "items";
	test = getInt_right(&m, str1); //считываем число элементов в строке


	error2 = make_line(m, ln); //делаем строку, выделяя память под каждый элемент
	if ((test == END_OF_FILE) || (error2 == OOM)) //если EOF или не создалась строка
	    return OOM;

	p = ln->a;
    printf("Enter items for line:\n");

	for (int j = 0; j < m; ++j, ++p) //m раз считается число (m чисел в данной строке)
    {
        test = getInt(p, -1); //подходят и отрицательные числа
        if (test == 0)
        {
            error = 1;
            return error;
        }
        else if (test == -1)
        {
            scanf("%*[^\n]");
            printf("\nWrite only numbers, repeat string correctly please -> \n");
            p = (p - j - 1); //указатель назад в начало строки
            j = -1; //счетчик -1 //такие значения, а не на 1 больше потому, что цикл сразу сделает j++, p++
        }
    }
	return error;	
}


int output_line(const Line *ln)
{
	int *p = NULL;
	for (int j = 0; j < ln->n; j++) //выводим строки
	{
	    p = ln->a + j;
	    printf("%5d", *p);
	}
	printf("\n");

	return 0;
}

int erase_line(Line *L)
{
	if (L == NULL) 
        return 0;

	free(L->a);
	L->a = NULL;

	return 0;
}

int compare_lines(const Line *L, const Line *J) //возвращает сколько элементов строки L
{				//содержится в строке J
	int count = 0;
	for(int j = 0; j < L->n; j++)
	{
		for(int k = 0; k < J->n; k++)
		{
			if (*(L->a + j) == *(J->a + k))
			{
				count++;
				break;
			}
		}
	}
	return count;	
}

