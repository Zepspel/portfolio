#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "graph.h"
#include "dialog.h"

ErrorCode getInt(short *intp, short low, short high)
{
	short n;
	int test = scanf("%hd", &n);
	if (test == EOF) 
		return EOF;

	if ((test == 0) || ((n < low) || (n > high)))
		return FAIL;


	int c = getchar();
    
	if ((c != ' ') && (c != '\n'))
		return FAIL;
	else 
	{
		*intp = n; 
		return OK; 
	}
}

ErrorCode getIntRight(short *m, char *c, short low, short high)
{
	int test;
	while (1) 
    	{
		test = getInt(m, low, high); 
		if (test == EOF) { 
		    return EOF;
		}
		else if (test == FAIL)
		{
		    printf("\nRepeat %s value correctly -> ", c);
		    scanf("%*[^\n]");
		}
		else
		    break;
	}
	return 0;
}



ErrorCode getWord(char *choice)
{
	int test = scanf("%255s", choice);
	if (test == EOF)
		return EOF;	

	if (test == 0)
		return FAIL;

	int c = getchar();
	if (c == EOF)
		return EOF;
	
	else if ((c != ' ') && (c != '\n') && (c != '\t'))
		return FAIL;
	
	
	return OK;
}


ErrorCode getWordPattern(char *choice, const char * const *option, int amount_option)
{
	int test;
	while (1) 
    	{
		test = getWord(choice); 
		if (test == EOF) 
		    return EOF;
		
		else if (test == FAIL)
		{
		    printf("\nRepeat action correctly -> ");
		    scanf("%*[^\n]");
		}
		
		else 
		{
			for (int i = 0; i < amount_option; i++)
			{	
				if (strcmp(choice, option[i]) == 0)
					return OK;
			}			
			printf("\nRepeat action correctly -> ");
		}
	
	}
}

ErrorCode getWordRight(char *word)
{
	int test;
	while (1)
	{
		test = getWord(word);
		if (test == EOF)
			return EOF;

		else if (test == FAIL)
		{
		    printf("\nRepeat action correctly -> ");
		    scanf("%*[^\n]");
		}
		else
			break;
	}
	
	return OK;	
}
	

void caseHelp()
{
	printf("options:\nh - help\n+node - insert node into the graph\n+edge - insert edge into the graph\n");
	printf("s - search for an element by key\n-node - delete node from the graph\n-enge - delete edge from the graph\np - print the graph\n");
	printf("bfs - breadth-first search\nexp - export the graph to dot file\nc - clean the graph\nq - quite\n");
	printf("short - shortest pass between two nodes\ngroup - splitting into groups\nchange - change node key or edge weight\n\n");
}	

ErrorCode caseInsertNode(Graph **graph)
{
	if ((graph == NULL) || (*graph == NULL))
		return OOM;

	char key[256];
	printf("for insertion enter key value -> ");
	ErrorCode test = getWordRight(key);
	if (test == EOF)
		return EOF;

	
	test = graphInsertNode(graph, key);
	switch (test)
	{
		case OOM:
			printf("out of memory\n");
			break;
		case OK:
			printf("insertion completed\n");
			break;
		default:
			printf("unknown error\n");
	}
	return OK;
}	

ErrorCode caseInsertEdge(Graph *graph)
{
	if (graph == NULL)
		return OOM;

	char first[256];
	printf("for insertion enter first node value -> ");
	ErrorCode test = getWordRight(first);
	if (test == EOF)
		return EOF;

	char second[256];
	printf("enter second node value -> ");
	test = getWordRight(second);
	if (test == EOF)
		return EOF;


	short weight;
	printf("enter weight value -> ");
	test = getIntRight(&weight, "weight", -10, 10);
	if (test == EOF)
		return EOF;
	

	test = graphInsertEdge(graph, first, second, weight);
	switch (test)
	{
		case OOM:
			printf("out of memory\n");
			break;
		case OK:
			printf("insertion completed\n");
			break;
		case FAIL:
			printf("error, check correctness of the nodes\n");
			break;
		default:
			printf("unknown error\n");
	}
	return OK;
}	




ErrorCode caseDeleteNode(Graph *graph)
{
	char key[256];
	printf("for insertion enter key value -> ");
	ErrorCode test = getWordRight(key);
	if (test == EOF)
		return EOF;

	test = graphDeleteNode(graph, key);
	switch (test)
	{	
		case OOM:
			printf("out of memory\n");
			break;
		case FAIL:
			printf("element with such key doesn't exist\n");
			break;
		case OK:
			printf("element was deleted\n");
			break;
		default:
			printf("unknown error\n");

	}
	return OK;
}

ErrorCode caseDeleteEdge(Graph *graph)
{
	char first[256];
	printf("for deletion enter first node value -> ");
	ErrorCode test = getWordRight(first);
	if (test == EOF)
		return EOF;

	char second[256];
	printf("for deletion enter second node value -> ");
	test = getWordRight(second);
	if (test == EOF)
		return EOF;


	test = graphDeleteEdge(graph, first, second);
	switch (test)
	{	
		case OOM:
			printf("out of memory\n");
			break;
		case FAIL:
			printf("element with such key doesn't exist\n");
			break;
		case OK:
			printf("element was deleted\n");
			break;
		default:
			printf("unknown error\n");

	}
	return OK;
}




void casePrintGraph(Graph *graph)
{
	ErrorCode test = graphPrint(graph);
	switch (test)
	{	
		case OOM:
			printf("out of memory\n");
			break;
		case FAIL:
			printf("empty graph\n");
			break;
		case OK:
			break;
		default:
			printf("unknown error\n");

	}
	return;
}


void caseCleanGraph(Graph *graph)
{
	ErrorCode test = graphClean(graph);
	if (test == FAIL)
		printf("graph doesn't have any elements\n");
	else if (test == OK)
		printf("graph has been cleaned\n");
	else
		printf("out of memory\n");
	
}

ErrorCode caseSearchNode(Graph *graph)
{
	char key[256];
	printf("for seraching enter key value -> ");
	ErrorCode test = getWordRight(key);
	if (test == EOF)
		return EOF;

//	graphSearchPrint(tree, key);

	
	return OK;
}



void caseExportGraph(Graph *graph)
{
	char command[1024];
	

	int test = graphDot(graph, "graph.dot");
	if (test == FAIL)
	{
		printf("export error\n");
		return;
	}
	else
		printf("export to dot the file complited successfully\n");


    	snprintf(command, sizeof(command), "dot -Tpng %s -o graph.png", "graph.dot");
    	test = system(command);
	if (test == 0)
		printf("png file was created\n");
	else
		printf("error creating png file\n");

	return;
}




ErrorCode caseBFS(Graph *graph)
{
	if (graph == NULL)
		return OOM;

	
	short num;
	printf("enter number of handshakes -> ");
	ErrorCode test = getIntRight(&num, "number", 0, SHRT_MAX);
	if (test == EOF)
		return EOF;
	

	char node[256];
	printf("enter node -> ");
	test = getWordRight(node);
	if (test == EOF)
		return EOF;

	test = graphBFS(graph, node, num);
	switch (test)
	{	
		case OOM:
			printf("out of memory\n");
			break;
		case FAIL:
			printf("element with such key doesn't exist\n");
			break;
		case OK:
			break;
		default:
			printf("unknown error\n");

	}
	
	return OK;
}


ErrorCode caseShortestPass(Graph *graph)
{
	char first[256];
	printf("for searching shortest pass enter first node -> ");
	ErrorCode test = getWordRight(first);
	if (test == EOF)
		return EOF;

	char second[256];
	printf("enter second node -> ");
	test = getWordRight(second);
	if (test == EOF)
		return EOF;


//	test = graphDeleteEdge(graph, first, second);
	List *list = listMake();
	short weight;
	test = graphShortestPass(graph, &weight, first, second, &list);
	switch (test)
	{	
		case OOM:
			listErase(list);
			printf("out of memory\n");
			break;
		case FAIL:
			listErase(list);
			printf("can not find shortest path\n");
			break;
		case OK:
			printf("total weight = %d\n", weight);
			listPrint(list);
			listErase(list);
			break;
		default:
			listErase(list);
			printf("unknown error\n");

	}
	return OK;
}


ErrorCode caseChange(Graph **graph)
{
	const char * const option[] = {"node", "edge"};
	
	printf("What do you want to change (node/edge) ?\n");
	
	char choice[256];
	char new[256];
	ErrorCode test = getWordPattern(choice, option, 2);

	if (strcmp(choice, "node") == 0)
	{
		printf("Enter old node name -> ");
		test = getWordRight(choice);
		if (test == EOF)
			return EOF;

		printf("Enter new node name -> ");
		test = getWordRight(new);
		if (test == EOF)
			return EOF;


		test = graphChangeNode(graph, choice, new);
		if (test == OOM)
			printf("out of memory\n");
	}
	else
	{
		printf("for changing enter first node value -> ");
		ErrorCode test = getWordRight(choice);
		if (test == EOF)
			return EOF;

		printf("enter second node value -> ");
		test = getWordRight(new);
		if (test == EOF)
			return EOF;


		short weight;
		printf("enter new weight value -> ");
		test = getIntRight(&weight, "weight", -10, 10);
		if (test == EOF)
			return EOF;
		

		test = graphChangeEdge(*graph, choice, new, weight);
		if (test == FAIL)
			printf("such edge doesn't exist\n");
	}
	return OK;
}


ErrorCode dialog(Graph *graph)
{
	int test;
	char choice[256];	
	const char * const option[] = {"help", "h", "+node", "+edge", "-node", "-edge", "delete", "d", "clean", "c", "print", "p", "search", "s", "exp", "export", "quite", "q", "imp", "import", "bfs", "short", "group", "change"};
	int amount_option = sizeof(option) / sizeof(option[0]);

	caseHelp();

	while(1)
	{
		test = getWordPattern(choice, option, amount_option);
		if (test == EOF)
			return EOF;

		if ((strcmp(choice, "help") == 0) || (strcmp(choice, "h") == 0))
			caseHelp();


		else if (strcmp(choice, "+node") == 0)
		{
			test = caseInsertNode(&graph);
			if (test == EOF)
				return EOF;
		}

		else if (strcmp(choice, "+edge") == 0)
		{
			test = caseInsertEdge(graph);
			if (test == EOF)
				return EOF;
		}

		else if (strcmp(choice, "-node") == 0)
		{
			test = caseDeleteNode(graph);
			if (test == EOF)
				return EOF;
		}

		else if (strcmp(choice, "-edge") == 0)
		{
			test = caseDeleteEdge(graph);
			if (test == EOF)
				return EOF;
		}

		else if ((strcmp(choice, "print") == 0) || (strcmp(choice, "p") == 0))
			casePrintGraph(graph);

		else if ((strcmp(choice, "search") == 0) || (strcmp(choice, "s") == 0))
		{
			test = caseSearchNode(graph);
			if (test == EOF)
				return EOF;	
		}

		else if (strcmp(choice, "bfs") == 0)
		{
			test = caseBFS(graph);
			if (test == EOF)
				return EOF;	
		}

		else if (strcmp(choice, "short") == 0)
		{
			test = caseShortestPass(graph);
			if (test == EOF)
				return EOF;	
		}
		else if (strcmp(choice, "change") == 0)
		{
			test = caseChange(&graph);
			if (test == EOF)
				return EOF;	
		}
				
		else if (strcmp(choice, "group") == 0)
			graphGroup(graph);

		else if ((strcmp(choice, "export") == 0) || (strcmp(choice, "exp") == 0))
			caseExportGraph(graph);

		else if ((strcmp(choice, "clean") == 0) || (strcmp(choice, "c") == 0))
			caseCleanGraph(graph);

		else if ((strcmp(choice, "quite") == 0) || (strcmp(choice, "q") == 0))
			break;
		else
			printf("unknown error\n");
	}
	
	return EOF;
}
