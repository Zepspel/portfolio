#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

typedef struct Elem Elem;

typedef struct Elem {
	Node *ptr;
	Elem *next;
} Elem;

typedef struct Stack {
	Elem *top;
} Stack;


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

	Elem *ptr = stack->top;
	Elem *temp;

	while (ptr != NULL)
	{
		temp = ptr;
		ptr = ptr->next;
		free(temp);
	}

	free(stack);

	return OK;
}

Stack *stackCopy(const Stack *stack)
{
	if (stack == NULL)
		return NULL;

	Stack *copy = stackMake();
	Stack *copy2 = stackMake();
	
	Elem *elem;
	elem = stack->top;
	while (elem != NULL)
	{
		stackPush(copy, elem->ptr);
		elem = elem->next;
	}

	Node *node;
	stackPop(copy, &node);
	while (node != NULL)
	{
		stackPush(copy2, node);
		stackPop(copy, &node);
	}

	stackErase(copy);
	return copy2;
}


ErrorCode stackPush(Stack *stack, Node *ptr)
{
	if (ptr == NULL)
		return FAIL;

	Elem *x = (Elem *) calloc(1, sizeof(Elem));
	if (x == NULL)
		return OOM;
	x->ptr = ptr;
	x->next = stack->top;
	stack->top = x;
	
	return OK;
}

ErrorCode stackPop(Stack *stack, Node **ptr)
{
	if (stack->top == NULL)
	{
		*ptr = NULL;
		return FAIL;
	}
	Elem *x = stack->top;
	stack->top = stack->top->next;
	x->next = NULL;
	*ptr = x->ptr;
	free(x);
	
	return OK;
}


