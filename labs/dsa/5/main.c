#include <stdio.h>
#include "graph.h"
#include "dialog.h"

int main()
{
	Graph *graph = graphMake();
	if (graph == NULL)
	{
		printf("out of memory\n");
		return 0;
	}

	dialog(graph);

	graphErase(graph);

	return 0;
}
