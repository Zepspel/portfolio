#include <stdio.h>
#include <stdlib.h>
#include "input.h"

int getInt(int *intp, int flag)
{
    int n;
    int test = scanf("%d", &n);
    
    if (test == -1) //если EOF
        return END_OF_FILE;

    if ((test == 0) || ((flag != -1) && (n <= 0))) //если фигня, то не делаем гетчар
        return -1;


    int c = getchar();
    
    if ((c != ' ') && (c != '\n'))
        return -1;
    else //то число считалось успешно
    {
        *intp = n; 
        return 1; 
    }
}

int getInt_right(int *m, char *c)
{
	int test2;
	while (1) //НАДО ЛИ ЗАСУНУТЬ ЭТОТ ЦИКЛ В ОТДЕЛЬНУЮ ФУНКЦИЮ?
    {
        test2 = getInt(m, 1); //flag 1 значит, что отрицательные числа не подходят
        if (test2 == END_OF_FILE) { //если EOF, то инпут не удался
            return END_OF_FILE;
        }
        else if (test2 == -1)
        {
            printf("\nRepeat number of %s correctly ->\n", c);
            scanf("%*[^\n]");
        }
        else
            break;
    }
	    return 0;
}



