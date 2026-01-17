#ifndef _TIMING_H_
#define _TIMING_H_

#include "tree.h"

unsigned int timing_insert(Tree *tree, size_t size_init, size_t count);
unsigned int timing_delete(Tree *tree, size_t size_init, size_t count);
unsigned int timing_search(Tree *tree, size_t size_init, size_t count);
unsigned int timing_closest(Tree *tree, size_t size_init, size_t count);
unsigned int timing_traversial(Tree *tree, size_t size_init, size_t count);

ErrorCode treeTraversalSpecial(Tree *tree, unsigned low);


#endif
