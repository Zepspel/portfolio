#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct Box Box;


typedef struct Queue {
	Box *tail;
} Queue;

typedef struct Box {
	Info cells;
	Box *next;
} Box;



Queue *q_make(int n)	//передаем пустой параметр для полиморфизма
{
	Queue *pq = (Queue *) calloc(1, sizeof(Queue));
	if (pq == NULL)
		return NULL;	
	pq->tail = NULL;

	return pq;
}

ErrorCode q_erase(Queue *pq) //по сути считывем, пока не tail = null
{
	if (pq == NULL)
		return 0;

	while(pq->tail != NULL)
	{
		Box *x = pq->tail->next;
		if(x == x->next)
			pq->tail = NULL;
		else
			pq->tail->next = x->next;
		free(x);
		x = NULL;
	}

	free(pq);
	pq = NULL;

	return OK;
}


Box *b_make(int angle, double sin)
{
	Box *x = (Box *) calloc(1, sizeof(Box));
	x->cells.angle = angle;
	x->cells.sin = sin;
	
	return x;
}

ErrorCode q_write(Queue *pq, int angle, double sin)
{
	if (pq == NULL)
		return OOM;
	
	Box *x = b_make(angle, sin);
		
	if (pq->tail == NULL)	//если очередь пуста
		x->next = x; 	//бокс будет указывать сам на себя
	else
	{
		x->next = pq->tail->next;
		pq->tail->next = x;

	}
	pq->tail = x;
	
	return OK;
} 

ErrorCode q_read(Queue *pq, int *angle, double *sin)
{
	if (pq == NULL)
		return OOM;
	if (pq->tail == NULL)	//очередь пуста
		return FAIL;

	//Info x = pq->tail->next->cells;
	Box *x = pq->tail->next;	

	if (x == pq->tail)	//если последний элемент
		pq->tail = NULL;
	else
		pq->tail->next = x->next;
	
	x->next = NULL;
	
	*angle = x->cells.angle;
	*sin = x->cells.sin;

	free(x);	
	x = NULL;	

	return OK;
}

ErrorCode q_print(Queue *pq)
{
	if (pq == NULL)
		return OOM;
		
	Box *x = pq->tail->next;	//идем в голову
	
	while (x != pq->tail)	//пока не дойдем до хвоста
	{
		printf("\nangle = %d", x->cells.angle);
		printf("\nsin = %lf", x->cells.sin);
		x = x->next;	//переходим к следующему
	}
	printf("\nangle = %d", x->cells.angle);	//выводим последний
	printf("\nsin = %lf", x->cells.sin);	
	
	return OK;
}











