#include <stdio.h>
#include <limits.h>
#include "table.h"
#include "dialog.h"

int main()
{

	Table *table = table_make(23);
	if (table == NULL)
	{
		printf("lack of memory\n");
		return 0;
	}

	dialog(&table);

	table_erase(table);

	return 0;
}
