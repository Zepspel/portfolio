#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct Queue { 
	int head, tail;
	int size, capacity;
	Info *cells;		//указатель на вектор
} Queue;



Queue *q_make(int n)
{	
	Queue *pq = (Queue *) calloc(1, sizeof(Queue));
	if (pq == NULL)
		return NULL;
		
	pq->head = 0;
	pq->tail = 0;
	pq->size = 0;
	
	pq->capacity = n;
	pq->cells = (Info *) calloc(n, sizeof(Info));
	if (pq->cells == NULL)
	{
		free(pq);
		return NULL;
	}

	return pq;
}

ErrorCode q_erase(Queue *pq)
{
	if (pq == NULL)
		return OK;
	

	free(pq->cells);
	pq->cells = NULL;
	
	free(pq);
	pq = NULL;
	
	return OK;
}

ErrorCode q_write(Queue *pq, int angle, double sin)
{
	if (pq == NULL)
		return OOM;
	if (pq->size == pq->capacity)
		return FAIL;

	int i = pq->tail;
	pq->cells[i].angle = angle;
	pq->cells[i].sin = sin;
	pq->tail = (pq->tail + 1) % (pq->capacity);
	pq->size++;

	return OK;
}

ErrorCode q_read(Queue *pq, int *angle, double *sin)
{
	if (pq == NULL)
		return OOM;
	if (pq->size == 0)
		return FAIL;

	int i = pq->head;
	*angle = pq->cells[i].angle;
	*sin = pq->cells[i].sin;
	pq->head = (pq->head + 1) % (pq->capacity);
	pq->size--;

	return OK;
}	

ErrorCode q_print(Queue *pq)
{
	if (pq == NULL)
		return OOM;
	
	int n = pq->size;
	int i = pq->head;
	
	while(n != 0 )
	{	
		printf("\n%d, ", pq->cells[i].angle);
		printf("%lf", pq->cells[i].sin);
		i++;
		n--;
	}

	

	return OK;
} 












