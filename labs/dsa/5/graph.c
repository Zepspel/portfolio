#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "table.h"
#include "queue.h"
#include "stack.h"
#include "graphPrivat.h"
#include "graph.h"


Graph *graphMake()
{
	return tableMake(23);
}
	

ErrorCode graphErase(Graph *graph)
{
	if (graph == NULL)
		return OOM;

	List *list, *temp;
	for (unsigned int i = 0; i < graph->msize; i++)
	{	
		list = graph->ks[i].list;
		while (list != NULL)
		{
			temp = list->next;
//			free(list->drain);
			free(list);
			list = temp;	
		}
	}

	return tableErase(graph);
}



ErrorCode graphClean(Graph *graph)
{
	if (graph == NULL)
		return OOM;

	List *list, *temp;
	for (unsigned int i = 0; i < graph->msize; i++)
	{	
		list = graph->ks[i].list;
		while (list != NULL)
		{
			temp = list->next;
//			free(list->drain);
			free(list);
			list = temp;	
		}
		graph->ks[i].list = NULL;
	}

	return tableClean(graph);
}



ErrorCode graphInsertNode(Graph **graph, const char *node)
{
	return tableInsert(graph, node, NULL);
}	


ErrorCode graphChangeNode(Graph **graph, const char *old, const char *new)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(*graph, old);
	if (i == FAIL)
		return FAIL;
	
	List *list = (*graph)->ks[i].list;
	

	ErrorCode test = tableInsert(graph, new, list);
	if (test == OOM)
		return OOM;

	unsigned a = tableSearch(*graph, new);			//теперь мы должны пройтись по всему списку и для каждой вершины в нем
								//изменить название нашего узла
	unsigned j;
	List *list2;
	while (list != NULL)
	{
		j = tableSearch(*graph, list->drain);
		list2 = (*graph)->ks[j].list;

		while (strcmp(list2->drain, old) != 0)
		{
			list2 = list2->next;
		}	

		list2->drain = (*graph)->ks[a].key;

		list = list->next;
	}

	tableDelete(*graph, old);	
	(*graph)->ks[i].list = NULL;

	
	return OK;
}	

ErrorCode graphChangeEdge(Graph *graph, const char *source, const char *drain, short weight)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(graph, source);
	if (i == FAIL)
		return FAIL;

	unsigned j = tableSearch(graph, drain);
	if (j == FAIL)
		return FAIL;

	List *list = graph->ks[i].list;
	while (strcmp(list->drain, drain) != 0)
		list = list->next;

	list->weight = weight;

	
	list = graph->ks[j].list;
	while (strcmp(list->drain, source) != 0)
		list = list->next;

	list->weight = weight;


	return OK;
}

ErrorCode graphDeleteNode(Graph *graph, const char *node)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(graph, node);
	if (i == FAIL)
		return FAIL;
	

	List *list, *temp;
	List *list2, *temp2;
	list = graph->ks[i].list;
	unsigned j;	

	while (list != NULL)
	{
		temp = list->next;
									//delete this edge from second node
		j = tableSearch(graph, list->drain);
		list2 = graph->ks[j].list;


		if (strcmp(list2->drain, node) == 0)
		{
			graph->ks[j].list = list2->next;
//			free(list2->drain);
			free(list2);
		}	
		else
		{
			while (list2->next != NULL)
			{
				temp2 = list2;
				list2 = list2->next;
				if (strcmp(list2->drain, node) == 0)
				{
					temp2->next = list2->next;
//					free(list2->drain);
					free(list2);
					break;
				}
			}
		}

//		free(list->drain);
		free(list);
		list = temp;	
	}
	graph->ks[i].list = NULL;
	tableDelete(graph, node);

	return OK;
}

ErrorCode graphDeleteEdge(Graph *graph, const char *source, const char *drain)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(graph, source);
	if (i == FAIL)
		return FAIL;

	unsigned j = tableSearch(graph, drain);
	if (j == FAIL)
		return FAIL;
	

	List *list, *temp;
	list = graph->ks[i].list;

	if (strcmp(list->drain, drain) == 0)
	{
		graph->ks[i].list = list->next;
//		free(list->drain);
		free(list);
	}	
	else
	{
		while (list->next != NULL)
		{
			temp = list;
			list = list->next;
			if (strcmp(list->drain, drain) == 0)
			{
				temp->next = list->next;
//				free(list->drain);
				free(list);
				break;
			}
		}
	}


	list = graph->ks[j].list;

	if (strcmp(list->drain, source) == 0)
	{
		graph->ks[j].list = list->next;
//		free(list->drain);
		free(list);
	}	
	else
	{
		while (list->next != NULL)
		{
			temp = list;
			list = list->next;
			if (strcmp(list->drain, source) == 0)
			{
				temp->next = list->next;
//				free(list->drain);
				free(list);
				break;
			}
		}
	}


	return OK;
}



ErrorCode graphInsertEdge(Graph *graph, const char *source, const char *drain, short weight)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(graph, drain);
	if (i == FAIL)
		return FAIL;

	unsigned index = tableSearch(graph, source);
	if (index == FAIL)
		return FAIL;

	List *list = graph->ks[index].list;
	List *temp = list;
	List *temp1 = graph->ks[i].list;
									//check if this Edge already in graph
	while (list != NULL)
	{
		if (strcmp(drain, list->drain) == 0)
			return FAIL;
		list = list->next;
	}		

	List *newList = (List *) calloc(1, sizeof(List));
	graph->ks[index].list = newList;
	newList->next = temp;	
	newList->weight = weight;
	
	newList->drain = graph->ks[i].key;

	
	List *list1 = (List *) calloc(1, sizeof(List));
	graph->ks[i].list = list1;
	list1->next = temp1;
	list1->weight = weight;

	list1->drain = graph->ks[index].key;


	return OK;
}


ErrorCode graphPrint(Graph *graph)
{
	if (graph == NULL)
		return FAIL;

	List *list;
	short flag = 0;
	for (unsigned int i = 0; i < graph->msize; i++)
	{
		if (graph->ks[i].state == OCCUPIED)
		{
			flag++;
			printf("\nnode: %s, neighbors: ", graph->ks[i].key);

			list = graph->ks[i].list;
			while (list != NULL)
			{
				printf("%s (weight = %d), ", list->drain, list->weight);
				list = list->next;
			}
		}
	}
	if (flag == 0)
		return FAIL;

	printf("\n");

	return OK;
}




ErrorCode graphDot(Graph *graph, char *file_name)
{
	if (graph == NULL)
		return OOM;	

	FILE *file = fopen(file_name, "w");
	if (file == NULL)
		return FAIL;

	fprintf(file, "graph G {\n");


	for (unsigned i = 0; i < graph->msize; i++) 
	{
		if (graph->ks[i].state == OCCUPIED)
        	{
			fprintf(file, "\t%s;\n", graph->ks[i].key);
		}
    	}


	List *list;
	char *key;
	
	for (unsigned i = 0; i < graph->msize; i++)
	{
		if (graph->ks[i].state == OCCUPIED)
		{
			key = graph->ks[i].key;
			list = graph->ks[i].list;
			while (list != NULL)
			{
				if (strcmp(key, list->drain) >= 0)
                			fprintf(file, "\t%s -- %s [label=\"%d\"];\n", key, list->drain, list->weight);
				
				list = list->next;
			}
		}
	}
	

    	fprintf(file, "}\n");
    	fclose(file);


	return OK;
}



#define INF SHRT_MAX - 100

/*
ErrorCode graphDFS(Graph *graph, const char *node, short hand)
{
	if (graph == NULL)
		return OOM;

//	short k = 0;

	unsigned i = tableSearch(graph, node);
	if (i == FAIL)
		return FAIL;

	unsigned msize = graph->msize;
	
	short *dist = (short *) malloc(msize * sizeof(short));								//строим массив для колор
	short *color = (short *) malloc(msize * sizeof(short));
	for (unsigned a = 0; a < msize; a++)
	{
		color[a] = WHITE;
		dist[a] = INF; 
	}
	color[i] = GREY;
	dist[i] = 0;

	Stack *stack = stackMake();
	if (stack == NULL)
		return OOM;

	List *list;
	KeySpace *ptr = graph->ks + i;
	ErrorCode test;

//	stackPush(stack, ptr);

	list = ptr->list;
	while (list != NULL) 
	{
		i = tableSearch(graph, list->drain);
		dist[i] = k;
		
		list = list->next;
	}

	short k;
	while (1)
	{
		list = ptr->list;		

		i = tableSearch(graph, list->drain);
		k = ptr - graph->ks;
		while ((color[i] == GREY) && (dist[i] <= dist[k] + 1))		//скипаем уже серые вершины
		{
			list = list->next;
			if (list == NULL)
				break;
			i = tableSearch(graph, list->drain);
		}			

		if (list != NULL)						//если нашли белую вершину
		{
			color[i] = GREY;
			stackPush(stack, ptr);					//сохраняем предыдущий, когда переходим на следующий
//			k++;

			list = ptr->list;
			k = ptr - graph->ks;
			while (list != NULL) 
			{
				i = tableSearch(graph, list->drain);
//				if (dist[i] > k)
//					dist[i] = k;

				if (dist[i] > dist[k] + 1)
					dist[i] = dist[k] + 1;
							
				list = list->next;
			}

			ptr = graph->ks + i;					//переходим в неё
		//	printf("%s\n", ptr->key);
		}
		else
		{
			test = stackPop(stack, &ptr);				//если не нашли, то выходим на уровень назад
//			k--;
			if (test == FAIL)
				break;
		}
	}	
	
	for (i = 0; i < msize; i++)
	{
		if ((dist[i] <= hand) && (dist[i] != 0))
		{
			printf("%s\n", graph->ks[i].key);	
		}
	}	
	

	stackErase(stack);
	free(color);
	free(dist);	
	return OK;
}
*/





ErrorCode BFS(Graph *graph, const char *node, short *visited)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(graph, node);
	if (i == FAIL)
		return FAIL;


	KeySpace *s = graph->ks + i;

	unsigned msize = graph->msize;
										//строим массив для колор
	short *color = (short *) calloc(msize, sizeof(short));
	for (unsigned i = 0; i < msize; i++)
		color[i] = WHITE;
	unsigned a = (s - graph->ks);
	color[a] = GREY;
	visited[a] = 1;


	Queue *queue = queueMake();
	if (queue == NULL)
		return OOM;
	queueWrite(queue, s);

	
	List *list;
	KeySpace *par, *child;
	while (queueRead(queue, &par) == OK)					//мы прочитали очередную вершину из очереди
	{									//для каждой смежной с ней вершины
		list = par->list;
		while (list != NULL)
		{
			i = tableSearch(graph, list->drain);
			if ((color[i] == WHITE) && (list->weight > 0))
			{
				color[i] = GREY;
				visited[i] = 1;
				child = graph->ks + i;	
				queueWrite(queue, child);
			}
			list = list->next;
		}
		a = (par - graph->ks);
//		color[a] = BLACK;
		printf("%s\n", graph->ks[a].key);
	}
	
	queueErase(queue);	
	free(color);
	return OK;
}

#define INF SHRT_MAX - 100

ErrorCode graphBFS(Graph *graph, const char *node, short hands)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(graph, node);
	if (i == FAIL)
		return FAIL;


	KeySpace *s = graph->ks + i;


	unsigned msize = graph->msize;
									//строим массив для колор
	short *color = (short *) malloc(msize * sizeof(short));
	short *dist = (short *) malloc(msize * sizeof(short));
	for (unsigned i = 0; i < msize; i++)
	{
		color[i] = WHITE;
		dist[i] = INF;
	}
	unsigned a = (s - graph->ks);
	color[a] = GREY;
	dist[a] = 0;


	Queue *queue = queueMake();
	if (queue == NULL)
		return OOM;
	queueWrite(queue, s);

	List *list;
	KeySpace *par, *child;
	while (queueRead(queue, &par) == OK)					//мы прочитали очередную вершину из очереди
	{									//для каждой смежной с ней вершины
		list = par->list;
		a = (par - graph->ks);
		while ((list != NULL) && (dist[a] < hands))
		{
			i = tableSearch(graph, list->drain);
			if (color[i] == WHITE)
			{
				if (dist[i] > dist[a] + 1)
				{
					dist[i] = dist[a] + 1;
				}
			
				color[i] = GREY;
				child = graph->ks + i;	
				queueWrite(queue, child);
			}
			list = list->next;
		}
//		a = (par - graph->ks);
//		color[a] = BLACK;
//		printf("%s\n", graph->ks[a].key);
		if (dist[a] != 0)
			printf("%s\n", graph->ks[a].key);
	}
	
	queueErase(queue);	
	free(color);
	free(dist);
	return OK;
}



ErrorCode graphShortestPass(Graph *graph, short *weight, const char *node, const char *drain, List **note)
{
	if (graph == NULL)
		return OOM;

	unsigned i = tableSearch(graph, node);
	if (i == FAIL)
		return FAIL;

	unsigned k = tableSearch(graph, drain);
	if (k == FAIL)
		return FAIL;


	unsigned msize = graph->msize;
									//строим массив для дистанций
	short *dist = (short *) malloc(msize * sizeof(short));
	KeySpace **pred = (KeySpace **) malloc(msize * sizeof(KeySpace *));
	for (unsigned a = 0; a < msize; a++)
	{
		dist[a] = INF;
		pred[a] = NULL;
	}
	dist[i] = 0;


	List *list, *elem, *temp;
	size_t len;
	unsigned v;
	for (unsigned a = 0; a < graph->csize - 1; a++)
	{
		for (unsigned u = 0; u < msize; u++)				//обрабатываем все ребра для каждой вершины u (индекс)
		{
			if (dist[u] == INF)					//если еще не знаем как добраться, то пропускаем
				continue;
			
			list = graph->ks[u].list;
			while (list != NULL)					//для каждого ребра из данной вершины
			{
				v = tableSearch(graph, list->drain);
				if (dist[v] > dist[u] + list->weight)
				{
					dist[v] = dist[u] + list->weight;
					pred[v] = graph->ks + u;

				}
				list = list->next;	
			}		
		
		}
	}
										//проверка на отрицательный цикл				

	for (unsigned u = 0; u < msize; u++)					//обрабатываем все ребра для каждой вершины u (индекс)
	{
		list = graph->ks[u].list;
		while (list != NULL)						//для каждого ребра из данной вершины
		{
			v = tableSearch(graph, list->drain);
			if (dist[v] > dist[u] + list->weight)
			{
				free(dist);
				free(pred);
				return FAIL;					/////////////////////////////////////////////////
			}
			list = list->next;	
		}		
	
	}

	*weight = dist[k];
	
	unsigned q = k;
	unsigned w;

	if (pred[q] == NULL)
	{
		free(dist);
		free(pred);
		return FAIL;
	}

	elem = (List *) malloc(1 * sizeof(List));
	len = strlen(pred[q]->key) + 1;
	elem->drain = (char *) malloc(len * sizeof(char));

	strcpy(elem->drain, graph->ks[q].key);
	elem->weight = dist[q];

	*note = elem;
	elem->next = NULL;



	while (pred[q] != NULL)
	{
		elem = (List *) malloc(1 * sizeof(List));
		len = strlen(pred[q]->key) + 1;
		elem->drain = (char *) malloc(len * sizeof(char));

		strcpy(elem->drain, pred[q]->key);
		w = pred[q] - graph->ks;
		elem->weight = dist[w];

		temp = *note;
		*note = elem;
		elem->next = temp;

		q = w;
	}



	free(dist);
	free(pred);
	return OK;
}



ErrorCode graphGroup(Graph *graph)
{

	unsigned msize = graph->msize;
	short *visited = (short *) malloc(msize * sizeof(short));
	for (int a = 0; a < msize; a++)
	{
		visited[a] = 0;
	}

	int i;
	int j = 0;
	while (1)
	{
		j += 1;
		i = 0;
		while ((graph->ks[i].state != OCCUPIED) || (visited[i] == 1))
		{
			i++;
			if (i == msize)
				break;
		}		

		if (i == msize)
			break;
		
		printf("Group number %d:\n", j);
		BFS(graph, graph->ks[i].key, visited);
	}	

	free(visited);	
	return OK;
}



List *listMake()
{
	List *list = NULL;
	return list;
}

ErrorCode listErase(List *list)
{
	if (list == NULL)
		return OOM;

	List *ptr = list;
	List *temp;
	while (ptr != NULL)
	{
		temp = ptr->next;
		free(ptr->drain);
		free(ptr);
		ptr = temp;
	}
	
//	free(list);
	return OK;
}

ErrorCode listPrint(List *list)
{
	while (list != NULL)
	{
		printf("%s: %d\n", list->drain, list->weight);
		list = list->next;
	}
	return OK;
}
	


