#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "queue.h"
#include "server.h"
#include "input.h"


int main() 
{
	int test;	
	
	int size_queue;
	int count_nodes;
	int low_range;
	int high_range;
	
	double p1, p2, p3;
	double p; 			//для рандома

	test = input(&size_queue, &count_nodes, &low_range, &high_range, &p1, &p2, &p3);
	printf("\n");	
	if (test == EOF)
	{
		printf("\nEOF\n");
		return 0;
	}
	
	int angle;	
	double sin;
	
	int task = low_range; 		//счетчик заданий в очереди слейвов
	int calculate = low_range; 	//счетчик значений, которые посчитанны и отправлены в очереди ответов
	int done = low_range; 		//счетчик записанных в таблицу значений	

	srand(time(NULL));

	Host *ph = h_make(size_queue, count_nodes, low_range, high_range, p1, p3);
	if (ph == NULL)
	{
		printf("\nНедостаточно памяти");
		return 0;
	}

	Slave **workers = (Slave **) calloc (count_nodes, sizeof(Slave*));	//выделяем масив рабов
	if (workers == NULL)
	{
		h_erase(ph);
		return 0;
	}
	
	for (int i = 0; i < count_nodes; i++)
		workers[i] = slave_make(size_queue, p2);



	//теперь хост берет угол из таблицы и с вероятностью p1 помещает его в очередь первого слейва
	//и так он делает N раз, где N - это количество слейвов
	
	while (done <= high_range) 		//пока не посчитаем все углы
	{	
		if (task <= high_range) 	//если еще не все задания раздали
		{
			for (int i = 0; i < count_nodes; i++)	
			{	
				if (task == high_range+1) //если все задания разданы
					break;

				p = (double)rand() / RAND_MAX; 
				if (p <= ph->p1) 		  //если вероятность не прокнет, то ничего не произойдет
				{
					test = q_write(workers[i]->qslave, task, NAN);
					if (test == OOM)			//если не смогли выделить элемент в списке
					{
						emergency(ph, workers);
						return 0;
					} 
					else if (test != FAIL)			//если записалось нормально (если не записалось в вектор из-за его конечных размеров, то не увеличиваем task)
						task++; 	  		//плюс одно задание слейву
				}
			}
		}		
		//теперь ведомые узлы с вероятностью p2 должны считать и посчитать
		if (calculate <=  high_range)
		{
			for (int i = 0; i < count_nodes; i++)
			{	
				if (calculate ==  high_range+1)
					break;
			
				p = (double)rand() / RAND_MAX; 
				
				if (p <= workers[i]->p2)
				{ 
					test = q_read(workers[i]->qslave, &angle, &sin);
					if (test == FAIL)
						continue; 	//если очередь слейва была пуста, то пропускаем
					
					if (isnan(sin))	
						sin = calc_sin(angle, 15);
					
					test = q_write(ph->qanswers[i], angle, sin);	
					if (test == OOM)
					{
						emergency(ph, workers);
						return 0;
					}
					else if (test == FAIL)	//тогда кладем назад в очередь воркера
						q_write(workers[i]->qslave, angle, sin);	////////////////////////////////////////////////////////////////
					else
						calculate++;
				}
			}
		}	
		//теперь пункт 3
		//считываем результаты из qunswer и кладем в итоговую таблицу
		for (int i = 0; i < count_nodes; i++)
		{
			if (done == high_range+1)
				break;
		 
			p = (double)rand() / RAND_MAX;
			if (p <= ph->p3)
			{
				test = q_read(ph->qanswers[i], &angle, &sin); 	//если очередь данного слейва, то ничего не делаем
				if (test == OK)
				{
					printf("%5d", angle);
					t_write(&(ph->table), angle, sin);
					done++;					//заполнили еще 1 ячейку в итоговой таблице
				}
			}
		}
			
	}
	
	t_print(ph->table);

	for (int i = 0; i < count_nodes; i++) 	//почистим слейвов
		slave_erase(workers[i]);
	
	free(workers);

	h_erase(ph);
	
	return 0;
}



