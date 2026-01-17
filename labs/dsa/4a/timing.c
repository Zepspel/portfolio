#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include "timing.h"
#include "tree_privat.h"


#define MC 1000000


void generate_word(char *word, size_t length) 
{
	const char letters[] = "123456789abcdefghijklmnopqrstuvwxyz"; 
	size_t len = strlen(letters);
	
	for (size_t i = 0; i < length - 1; i++) 
	{
        	size_t index = rand() % len; 
        	word[i] = letters[index];
    	}

    	word[length-1] = '\0';
}


unsigned int timing_insert(Tree *tree, size_t size_init, size_t count)
{
	tree_clean(tree);

	char key[20]; 
	size_t length;
	unsigned int num;

	
	for (size_t i = 0; i < size_init; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		num = rand() % UINT_MAX;
		
		tree_insert(tree, key, num);
	} 

	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);

//	clock_t tic = clock();

	for (size_t i = 0; i < count; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		num = rand() % UINT_MAX;
		
		tree_insert(tree, key, num);
	} 

	gettimeofday(&toc, NULL);
  //  	clock_t toc = clock();
	
//	double time = ((double)(toc - tic)) / CLOCKS_PER_SEC;
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;

	tree_clean(tree);

	return time;
}		



unsigned int timing_delete(Tree *tree, size_t size_init, size_t count)
{
	tree_clean(tree);

	char key[20]; 
	size_t length;
	unsigned int num;

	
	for (size_t i = 0; i < size_init; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		num = rand() % UINT_MAX;
		
		tree_insert(tree, key, num);
	} 



//	clock_t tic = clock();
	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);


	for (size_t i = 0; i < count; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		
		tree_delete(tree, key);
	} 

//    	clock_t toc = clock();
	
	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
//	double time = ((double)(toc - tic)) / CLOCKS_PER_SEC;
	tree_clean(tree);

	return time;
}		

unsigned int timing_search(Tree *tree, size_t size_init, size_t count)
{
	tree_clean(tree);

	char key[20]; 
	size_t length;
	unsigned int num;

	
	for (size_t i = 0; i < size_init; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		num = rand() % UINT_MAX;
		
		tree_insert(tree, key, num);
	} 



//	clock_t tic = clock();
	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);


	for (size_t i = 0; i < count; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		
		tree_search(tree, key);
	} 

//    	clock_t toc = clock();
	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
//	
//	double time = ((double)(toc - tic)) / CLOCKS_PER_SEC;
	tree_clean(tree);

	return time;
}		


unsigned int timing_maximum(Tree *tree, size_t size_init, size_t count)
{
	tree_clean(tree);

	char key[20]; 
	size_t length;
	unsigned int num;

	
	for (size_t i = 0; i < size_init; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		num = rand() % UINT_MAX;
		
		tree_insert(tree, key, num);
	} 



	Node **vector;
//	clock_t tic = clock();
	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);


	for (size_t i = 0; i < count; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		
		vector = tree_max(tree);
		free(vector);
	} 

//    	clock_t toc = clock();
	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
//	
//	double time = ((double)(toc - tic)) / CLOCKS_PER_SEC;
	tree_clean(tree);

	return time;
}		




unsigned int timing_traversial(Tree *tree, size_t size_init)
{
	tree_clean(tree);

	char key[20]; 
	size_t length;
	unsigned int num;

	
	for (size_t i = 0; i < size_init; i++)
	{
		length = 5 + rand() % 15;	
		generate_word(key, length);
		num = rand() % UINT_MAX;
		
		tree_insert(tree, key, num);
	} 

//	clock_t tic = clock();
	struct timeval  tic, toc;
	gettimeofday(&tic, NULL);


	Node *ptr = tree_maximum(tree);
		
	while (ptr != NULL)
		ptr = ptr->prev;


//    	clock_t toc = clock();
	gettimeofday(&toc, NULL);
	unsigned int time = (unsigned int) (toc.tv_usec - tic.tv_usec) + (unsigned int) (toc.tv_sec - tic.tv_sec) * MC;
	
//	double time = ((double)(toc - tic)) / CLOCKS_PER_SEC;
	tree_clean(tree);

	return time;
}		


