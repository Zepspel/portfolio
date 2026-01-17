#ifndef _MATR_H_
#define _MATR_H_

#include "line.h"

typedef enum {
    SUCCESS = 0,          // Успешное выполнение
    OOM = 1,    		  // out of memory
    END_OF_FILE = 2,    // Недостаточно памяти
    UNKNOWN_ERROR = 4     // Неизвестная ошибка
} ErrorCode;

typedef struct Matrix {
    int lines; // количество строк матрицы
    Line *linep; // массив строк матрицы
} Matrix;

int make_matrix(int n, Matrix *matr);
int input_matrix(Matrix *rm);

int output_matrix(const Matrix *rm);
int erase_matrix(Matrix *M);

Line *individual(Matrix *rm);

#endif
