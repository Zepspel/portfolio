#ifndef _STACK_H_
#define _STACK_H_

#include "graph.h"

typedef struct Elem Elem;
typedef struct Stack Stack;



Stack *stackMake();
ErrorCode stackErase(Stack *stack);
ErrorCode stackPush(Stack *stack, KeySpace *ks);
ErrorCode stackPop(Stack *stack, KeySpace **ks);







#endif
