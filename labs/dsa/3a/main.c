#include <stdio.h>
#include "table.h"
#include "dialog.h"

int main()
{
	int size;
	printf("\nEnter size of table -> ");
	int test = scanf("%d", &size);
	if (test == EOF)
		return 0;
	Table *table = t_make(size);
	if (table == NULL)
		return 0;

	dialog(table);

	t_erase(table);

	return 0;
}
