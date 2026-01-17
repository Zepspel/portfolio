#include <stdio.h>
#include "tree.h"
#include "dialog.h"

int main()
{
	Tree *tree = treeMake();
	if (tree == NULL)
	{
		printf("out of memory\n");
		return 0;
	}

	dialog(tree);

	treeErase(tree);

	return 0;
}
