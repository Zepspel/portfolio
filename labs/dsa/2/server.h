#ifndef _SERVER_H_
#define _SERVER_H_

#include "queue.h"

typedef struct Item {
	int angle;
	double sin;
} Item;


typedef struct Table {
	Item *item;
	int size;
	int low_range;
} Table;


typedef struct Host {
	int count_nodes;
	double p1, p3;
	Table table;
	Queue **qanswers;	//массив очередей для полученных данных
} Host;


typedef struct Slave {
	double p2;
	Queue *qslave;
} Slave;



double calc_sin(int angle, int N);	//N - количество слагаемых
int fact(int n);

Host *h_make(int size_queue, int count_nodes, int low_range, int high_range, double p1, double p3);
ErrorCode h_erase(Host *ph);
Slave *slave_make(int size_queue, double p2);
ErrorCode slave_erase(Slave *ps);
//ErrorCode print_table(Host host, int low_range, int high_range);

ErrorCode t_make(Table *pt, int low_range, int high_range);
ErrorCode t_erase(Table *pt);
ErrorCode t_print(Table table);
ErrorCode t_write(Table *pt, int angle, double sin);	//записываем по ключу angle значение sin


ErrorCode emergency(Host *ph, Slave **workers);



#endif 
