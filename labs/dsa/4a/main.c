#include <stdio.h>
#include "tree.h"
#include "dialog.h"

int main()
{
	Tree *tree = tree_make();
	if (tree == NULL)
	{
		printf("out of memory\n");
		return 0;
	}

	dialog(tree);

	tree_erase(tree);

	return 0;
}
