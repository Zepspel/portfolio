#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include "timing.h"
#include "treePrivat.h"


#define MC 1000000


unsigned int timing_insert(Tree *tree, size_t size_init, size_t count)
{
	treeClean(tree);

	unsigned num;
	unsigned key;
	
	for (size_t i = 0; i < size_init; i++)
	{
		key = rand() % UINT_MAX;
		num = rand() % UINT_MAX;
		treeInsert(tree, key, num);
	} 

	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);

	for (size_t i = 0; i < count; i++)
	{
		key = rand() % UINT_MAX;
		num = rand() % UINT_MAX;
		treeInsert(tree, key, num);
	} 

	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;

	treeClean(tree);

	return time;
}		



unsigned int timing_delete(Tree *tree, size_t size_init, size_t count)
{
	treeClean(tree);

	unsigned num;
	unsigned key;

	
	for (size_t i = 0; i < size_init; i++)
	{
		key = rand() % UINT_MAX;
		num = rand() % UINT_MAX;
		treeInsert(tree, key, num);
	} 

	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);

	for (size_t i = 0; i < count; i++)
	{
		key = rand() % UINT_MAX;
		treeDelete(tree, key, 1);
	} 

	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
	treeClean(tree);

	return time;
}		

unsigned int timing_search(Tree *tree, size_t size_init, size_t count)
{
	treeClean(tree);

	unsigned num;
	unsigned key;

	
	for (size_t i = 0; i < size_init; i++)
	{
		key = rand() % UINT_MAX;
		num = rand() % UINT_MAX;
		
		treeInsert(tree, key, num);
	} 

	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);

	for (size_t i = 0; i < count; i++)
	{
		key = rand() % UINT_MAX;
		num = rand() % UINT_MAX;
		
		treeSearch(tree, key);
	} 

	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
	treeClean(tree);

	return time;
}		


unsigned int timing_closest(Tree *tree, size_t size_init, size_t count)
{
	treeClean(tree);

	unsigned num;
	unsigned key;

	
	for (size_t i = 0; i < size_init; i++)
	{
		num = rand() % UINT_MAX;
		key = rand() % UINT_MAX;
		
		treeInsert(tree, key, num);
	} 



	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);


	for (size_t i = 0; i < count; i++)
	{
		key = rand() % UINT_MAX;
		treeClosest(tree, key);
	} 

	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
	treeClean(tree);

	return time;
}		




unsigned int timing_traversial(Tree *tree, size_t size_init, size_t count)
{
	treeClean(tree);

	unsigned key, num;

	
	for (size_t i = 0; i < size_init; i++)
	{
		num = rand() % UINT_MAX;
		key = rand() % UINT_MAX;
		treeInsert(tree, key, num);
	} 

	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);


	for (size_t i = 0; i < count; i++)
		treeTraversalSpecial(tree, 0);



	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
	
	treeClean(tree);

	return time;
}		


ErrorCode treeTraversalSpecial(Tree *tree, unsigned low)
{
	if (tree == NULL)
		return OOM;

	Node *elem = NULL;
	Node *ptr = tree->root;

	Stack *stack = stackMake();
	
	ErrorCode test;

	while (1)
	{
		while (ptr != NULL)
		{
			stackPush(stack, ptr);
			ptr = ptr->right;
		}
		
		
		
		test = stackPop(stack, &elem);
		if ((test == FAIL) || (elem->key < low))
			break;
		
//		printf("\nkey: %u, qty %u, info: ", elem->key, elem->count);
		
		List *list, *temp;
		list = elem->list;
		
		while (list != NULL)
		{
//			printf("%u  ", *(list->info));
			list = list->next;
		}
		
		if (elem->left != NULL)
			ptr = elem->left;

	}

//	printf("\n");
	stackErase(stack);

	return OK;
}

