#ifndef _TIMING_H_
#define _TIMING_H_

#include "tree.h"

void generate_word(char *word, size_t length);
unsigned int timing_insert(Tree *tree, size_t size_init, size_t count);
unsigned int timing_delete(Tree *tree, size_t size_init, size_t count);
unsigned int timing_search(Tree *tree, size_t size_init, size_t count);
unsigned int timing_maximum(Tree *tree, size_t size_init, size_t count);
unsigned int timing_traversial(Tree *tree, size_t size_init);





#endif
