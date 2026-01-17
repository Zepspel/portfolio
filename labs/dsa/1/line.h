#ifndef _LINE_H_
#define _LINE_H_

typedef struct Line {
    int n; // количество элементов в строке матрицы
    int *a; // массив элементов
} Line;


int make_line(int n, Line *ln);
int input_line(Line *ln);

int output_line(const Line *ln);
int erase_line(Line *L);

int compare_lines(const Line *L, const Line *J);

#endif
