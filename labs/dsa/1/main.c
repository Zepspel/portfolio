#include <stdio.h>
#include <stdlib.h>
#include "matr.h"
#include "line.h"
#include "input.h"

int main()
{

    Matrix *obj = (Matrix *) calloc(1, sizeof(Matrix));
    ErrorCode result = input_matrix(obj);
    if ((result == OOM) || (result == END_OF_FILE)) //если матрица была введена не корректно (EOF встретился)
    {
        printf("\nIncorrest input of matrix\n");
        free(obj);
        return 0;
    }

    printf("Matrix: ");
    output_matrix(obj);

    //индивидуальное задание
    Line *vector = individual(obj);

    printf("Vector b:\n");
    output_line(vector);

    erase_matrix(obj);
    free(obj);
    erase_line(vector);
    free(vector);

    return 0;
}
