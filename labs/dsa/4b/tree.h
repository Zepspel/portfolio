#ifndef _TREE_H_
#define _TREE_H_

#include "stack.h"


typedef struct Tree Tree;
typedef struct Node Node;
typedef struct List List;


Tree *treeMake();
ErrorCode treeErase(Tree *tree);

ErrorCode treeInsert(Tree *tree, unsigned key, unsigned data);
ErrorCode treeDelete(Tree *tree, unsigned key, unsigned number);

ErrorCode treeTurnLeft(Node *axis, Node *par);
ErrorCode treeTurnRight(Node *axis, Node *par);

Stack *treeFindPlace(Tree *tree, unsigned key);
ErrorCode treeTraversal(Tree *tree, unsigned low);

void tree_print_bashshell(Tree *tree);
void tree_print_bash(Node *ptr, int space, int gap);

ErrorCode treeDot(Tree *tree, char *file_name);
ErrorCode treeImport(Tree *tree, FILE *file);
ErrorCode treeClean(Tree *tree);
Node *treeSearch(Tree *tree, unsigned key);
void treeSearchPrint(Tree *tree, unsigned key);


Node *treeClosest(Tree *tree, unsigned target);
void treeClosestPrint(Tree *tree, unsigned target);
unsigned mod(unsigned a, unsigned b);
Node *treeFindNext(Node *ptr, const Stack *stack);
Node *treeFindPrev(Node *ptr, const Stack *stack);

#endif
