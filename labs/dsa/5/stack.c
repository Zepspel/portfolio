#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "dataStructures.h"


Stack *stackMake()
{
	Stack *stack = (Stack *) calloc(1, sizeof(Stack));
	if (stack == NULL)
		return NULL;

	stack->top = NULL;	
	return stack;
}

ErrorCode stackErase(Stack *stack)
{
	if (stack == NULL)
		return OOM;
		
	Elem *elem = stack->top;
	Elem *temp;
	while (elem != NULL)
	{
		temp = elem->next;
		free(elem);
		elem = temp;
	}
	free(stack);
	return OK;
}


ErrorCode stackPush(Stack *stack, KeySpace *ks)
{
	Elem *x = (Elem *) calloc(1, sizeof(Elem));
	if (x == NULL)
		return OOM;
	x->ks = ks;
	x->next = stack->top;
	stack->top = x;
	
	return OK;
}

ErrorCode stackPop(Stack *stack, KeySpace **ks)
{
	if (stack->top == NULL)
		return FAIL;
	Elem *x = stack->top;
	stack->top = stack->top->next;
	x->next = NULL;
	*ks = x->ks;
	free(x);
	
	return 0;
}


