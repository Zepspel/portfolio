#ifndef _TREE_PRIVAT_H_
#define _TREE_PRIVAT_H_


typedef struct Node {
	char *key;
	unsigned int *info;
	Node *left, *right;
	Node *par;
	Node *prev;
	unsigned int release;
} Node;


typedef struct Tree {
	Node *root;
} Tree;






#endif
