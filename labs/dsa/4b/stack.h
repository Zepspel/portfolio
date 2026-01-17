#ifndef _STACK_H_
#define _STACK_H_


typedef enum {
	OK = 0,          	  
   	OOM = -1,    		  
	FAIL = -2,		  
} ErrorCode;

typedef struct Stack Stack;
typedef struct Node Node;

Stack *stackMake();
ErrorCode stackErase(Stack *stack);
Stack *stackCopy(const Stack *stack);

ErrorCode stackPush(Stack *stack, Node *elem);
ErrorCode stackPop(Stack *stack, Node **elem);


#endif
