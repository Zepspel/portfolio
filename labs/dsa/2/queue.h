#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef enum {
	OK = 0,          	  // Успешное выполнение
   	OOM = 1,    		  // out of memory
	FAIL = 2,		  //не получилось выполнить операцию
	END = 3,
} ErrorCode;

typedef struct Info{
	int angle;
	double sin;
} Info;

typedef struct Queue Queue;

Queue *q_make(int n);
ErrorCode q_erase(Queue *pq);

ErrorCode q_write(Queue *pq, int angle, double sin);
ErrorCode q_read(Queue *pq, int *angle, double *sin);

ErrorCode q_print(Queue *pq);



#endif

