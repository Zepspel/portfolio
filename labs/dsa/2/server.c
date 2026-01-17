#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "queue.h"
#include "server.h"

ErrorCode emergency(Host *ph, Slave **workers)
{
	if (workers != NULL)
	{
		for (int i = 0; i < ph->count_nodes; i++)
			slave_erase(workers[i]);
		
		free(workers);
		workers = NULL;
	}

	if (ph != NULL)
		h_erase(ph);
	
	return OK;
}	
	
Host *h_make(int size_queue, int count_nodes, int low_range, int high_range, double p1, double p3)
{
	Host *ph = (Host *) calloc(1, sizeof(Host));
	if (ph == NULL)
		return NULL;
	
	ph->p1 = p1;
	ph->p3 = p3;
	
	ph->count_nodes = count_nodes;
	
	/*ph->table = (double *) calloc(91, sizeof(double));
	if (ph->table == NULL)
	{
		free(ph);
		return NULL;
	}	

	for (int angle = low_range; angle <= high_range; angle++) 	//положить в массив значения углов, а sin = 2
		ph->table[angle] = 2;
	*/
	
	int test = t_make(&(ph->table), low_range, high_range);				//сoздаем таблицу
	if (test == OOM)
		return NULL;	

	ph->qanswers = (Queue **) calloc(count_nodes, sizeof(Queue*)); 	//выделяем массив очередей для ответов
	if (ph->qanswers == NULL)
	{
		t_erase(&(ph->table));
		free(ph);
		return NULL;
	}

	for (int i = 0; i < count_nodes; i++) 				//инициализируем массив очередей
		ph->qanswers[i] = q_make(size_queue);
	
	
	return ph;
}

ErrorCode h_erase(Host *ph)
{
	/*if (ph->table != NULL)
		free(ph->table);*/
	t_erase(&(ph->table));

	for (int i = 0; i < ph->count_nodes; i++)
		q_erase(ph->qanswers[i]);	
	
	if (ph->qanswers != NULL)
		free(ph->qanswers);

	free(ph);
	
	return OK;
} 

Slave *slave_make(int size_queue, double p2)
{
	Slave *S = (Slave *) calloc (1, sizeof(Slave));
	S->qslave = q_make(size_queue);
	S->p2 = p2;
	return S;
}

ErrorCode slave_erase(Slave *ps)
{
	q_erase(ps->qslave);	

	if (ps != NULL)
		free(ps);

	return OK;
}

ErrorCode t_make(Table *pt, int low_range, int high_range)
{
	int n = high_range - low_range + 1;
	pt->item = (Item *) calloc(n, sizeof(Item));	//выделяем память под n элементов таблицы
	if (pt->item == NULL)
		return OOM;
	
	int angle = low_range;
	for(int i = 0; i < n; i++)
	{
		pt->item[i].angle = angle;
		//pt->item[i].sin = NAN;
		angle++;
	}
	pt->size = n;
	pt->low_range = low_range;
	return OK;
}

ErrorCode t_erase(Table *pt)
{	
	if (pt == NULL)
		return OK;
	if (pt->item == NULL)
		return OK;
	
	free(pt->item);
	pt->item = NULL;
	return OK;
}	

ErrorCode t_print(Table table)
{
	if (table.item == NULL)
		return OK;

	for(int i = 0; i < table.size; i++)
		printf("\nsin %d = %lf", table.item[i].angle, table.item[i].sin);

	return OK;	
}

ErrorCode t_write(Table *pt, int angle, double sin)	//записываем по ключу angle значение sin
{
	if (pt->item == NULL)
		return OOM;
	
	int i = angle - pt->low_range;
	pt->item[i].sin = sin;
	
	return OK;
}

double calc_sin(int angle, int N)	//N - количество слагаемых
{
	while (angle < -90)		//приводим угол к [-90; 269]
		angle += 360;
	while (angle > 270)
		angle -= 360;

	if (angle > 90)			//тк sin(180 - a) = sin a
		angle = 180 - angle;


	double sin = 0;
	double rad = angle * 2 * M_PI / 360;
	
	for (int n = 1; n <= N; n++)
		sin += pow(-1, n + 1) * pow(rad, 2*n - 1) / fact(2*n - 1);
	
	return sin;
}


int fact(int n)
{
	int res = 1;
	for (int i = 2; i <= n; i++)
		res = res * i;

	return res;
}






