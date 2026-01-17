#ifndef _TREE_PRIVAT_H_
#define _TREE_PRIVAT_H_


typedef struct Node Node;
typedef struct List List;


typedef struct List {
	unsigned *info;
	List *next;
} List;


typedef struct Node {
	unsigned key;
	List *list;
	Node *left, *right;
	int balance;
	unsigned count;
} Node;


typedef struct Tree {
	Node *root;
} Tree;






#endif
