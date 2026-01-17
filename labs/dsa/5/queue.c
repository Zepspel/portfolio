#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "dataStructures.h"

Queue *queueMake()
{
	Queue *queue = (Queue *) calloc(1, sizeof(Queue));
	if (queue == NULL)
		return NULL;

	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}
	
ErrorCode queueErase(Queue *queue)
{
	if (queue == NULL)
		return OOM;
	
	Elem *elem = queue->head;
	Elem *temp;
	while (elem != NULL)
	{
		temp = elem->next;
		free(elem);
		elem = temp;
	}	
	
	free(queue);
	return OK;	
}	

//ErrorCode queueIsEmpty(Queue *queue)

	


ErrorCode queueWrite(Queue *queue, KeySpace *ks)
{
	Elem *x = (Elem *) calloc(1, sizeof(Elem));
	if (x == NULL)
		return OOM;
	x->ks = ks;
	x->next = NULL;
	
	if (queue->head == NULL)
		queue->head = x;
	else
		queue->tail->next = x;
	queue->tail = x;

	return OK;
}

ErrorCode queueRead(Queue *queue, KeySpace **ks)
{
	if (queue->head == NULL)
	{
		*ks = NULL;
		return FAIL;
	}

	Elem *x = queue->head;
	*ks = x->ks;
	queue->head = queue->head->next;
	if (queue->head == NULL)
		queue->tail = NULL;

	x->next = NULL;
//	*info = x->info;
	free(x);

	return OK;
}



