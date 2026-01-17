#ifndef _TREE_H_
#define _TREE_H_

typedef enum {
	OK = 0,          	  
   	OOM = -1,    		  
	FAIL = -2,		  
} ErrorCode;


typedef struct Tree Tree;
typedef struct Node Node;

Tree *tree_make();
ErrorCode tree_erase(Tree *tree);
ErrorCode tree_clean(Tree *tree);

Node *tree_find_place(Tree *tree, char *clef, unsigned int *release);
ErrorCode tree_insert(Tree *tree, char *clef, unsigned int data);

ErrorCode tree_delete(Tree *tree, char *clef);

ErrorCode tree_traversal_recursive(Tree *tree);
ErrorCode work(Node *ptr);
ErrorCode tree_traversal(Tree *tree);
ErrorCode tree_range(Tree *tree, char *first, char *last);
void tree_print_bashshell(Tree *tree);
void tree_print_bash(Node *ptr, int space, int gap);

ErrorCode tree_todot(Tree *tree, char *file_name);
void tree_export_recursion(Node *ptr, FILE *file);
ErrorCode tree_import(Tree *tree, FILE *file);

void tree_search_print(Tree *tree, char *clef);
Node **tree_search(Tree *tree, char *clef);

Node *tree_maximum(Tree *tree);
void tree_max_print(Tree *tree);
Node **tree_max(Tree *tree);


Node *find_next(Node *ptr);

#endif
