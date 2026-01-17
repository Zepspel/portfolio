#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "graph.h"

typedef struct KeySpace KeySpace;
typedef struct Elem Elem;
typedef struct Queue Queue;



Queue *queueMake();
ErrorCode queueErase(Queue *queue);
ErrorCode queueWrite(Queue *queue, KeySpace *ks);
ErrorCode queueRead(Queue *queue, KeySpace **ks);



#endif
