#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "treePrivat.h"
#include "stack.h"


Tree *treeMake()
{
	Tree *tree = (Tree *) calloc(1, sizeof(Tree));
	if (tree == NULL)
		return NULL;

	tree->root = NULL;

	return tree;
}

ErrorCode treeErase(Tree *tree)
{
	if (tree == NULL)
		return FAIL;

	Node *elem = NULL;
	Node *ptr = tree->root;

	Stack *stack = stackMake();
	
	ErrorCode test;

	List *list, *temp;
	while (1)
	{
		while (ptr != NULL)
		{
			stackPush(stack, ptr);
			ptr = ptr->right;
		}
		
		test = stackPop(stack, &elem);
		if (test == FAIL)
			break;
		
		
		if (elem->left != NULL)
			ptr = elem->left;
	
		list = elem->list;
		while (list != NULL)
		{
			temp = list->next;
			free(list->info);
			free(list);
			list = temp;
		}
			
		free(elem);

	}

	stackErase(stack);
	free(tree);

	return OK;
}

ErrorCode treeClean(Tree *tree)
{
	if (tree == NULL)
		return FAIL;
	

	Node *elem = NULL;
	Node *ptr = tree->root;

	Stack *stack = stackMake();
	
	ErrorCode test;
	List *list, *temp;

	while (1)
	{
		while (ptr != NULL)
		{
			stackPush(stack, ptr);
			ptr = ptr->right;
		}
		
		test = stackPop(stack, &elem);
		if (test == FAIL)
			break;
		
		
		if (elem->left != NULL)
			ptr = elem->left;

		list = elem->list;
		while (list != NULL)
		{
			temp = list->next;
			free(list->info);
			free(list);
			list = temp;
		}
	
		free(elem);

	}

	tree->root = NULL;
	
	stackErase(stack);

	return OK;
}

ErrorCode treeDelete(Tree *tree, unsigned key, unsigned number)
{
	if (tree == NULL)
		return OOM;

	if (number == 0)
		return FAIL;

	Stack *stack = treeFindPlace(tree, key);
	Node *ptr, *par;
	ErrorCode test = stackPop(stack, &ptr);		//changed elem
	if ((test == FAIL) || (ptr->key != key))
	{
		stackErase(stack);
		return FAIL;
	}
	stackPop(stack, &par);				//parent of changed elem

	
	List *list = ptr->list;
	List *prev = NULL;
	unsigned count = ptr->count;
							//if there are few elems in node
	if (count > 1)
	{
		if (number > count)
		{
			stackErase(stack);
			return FAIL;
		}
		
		while ((list != NULL) && (number != 1))
		{
			prev = list;
			list = list->next;
			number--;
		}
		prev->next = list->next;
		free(list->info);
		free(list);
		stackErase(stack);
		return OK;
	}
									//if count == 1
	if (number != 1)
	{
		stackErase(stack);
		return FAIL;
	}
									//if we need to delete node
	Node *changed = ptr;
	Node *del = ptr;

	if ((ptr->left == NULL) || (ptr->right == NULL))		//if elem has 0 or 1 chind then delete him
		del = ptr;
	else
	{
		stackPush(stack, ptr);
		ptr = ptr->right;
		while (ptr != NULL)					//min elem in right subtree
		{
			stackPush(stack, ptr);				//save path
			ptr = ptr->left;
		}
		stackPop(stack, &del);
		stackPop(stack, &par);
	}		
		
	Node *subtree = NULL;
	if (del->left != NULL)
		subtree =  del->left;
	else if (del->right !=NULL)
		subtree = del->right;


	if (tree->root == del)						//if we delete root
		tree->root = subtree;

		
	list = changed->list;
	List *temp;
	while (list != NULL)
	{
		temp = list->next;
		free(list->info);
		free(list);
		list = temp;
	}
	

	if (changed != del)						//copy data from real deleted to which we wanted delete at first
	{
		changed->key = del->key;
		changed->list = del->list;
		changed->count = del->count;	
	}	

	if (par != NULL)
	{
		if (par->left == del)
		{	
			par->balance += 1;
			par->left = subtree;
		}
		else 
		{
			par->balance -= 1;
			par->right = subtree;
		}
	}

	free(del);
									//correction of balance
									//par - parent of deleted elem, now parent of x (subtree)
	Node *x = subtree;
	Node *axis, *parent;

	while (par != NULL)
	{
//		if (x == par->left)
//			par->balance += 1;
//		else
//			par->balance -= 1;
		
											//CORRECTION TREE
		if (par->balance == 2)							//if we deleted from left subtree
		{
			if (par->right->balance == 1)					//case1
			{
				axis = par;
				par = axis->right;					//it will be new par
				axis->balance = 0;
				axis->right->balance = 0;
				test = stackPop(stack, &parent);
				stackPush(stack, parent);
				if (test == FAIL)
				{
					tree->root = axis->right;
					treeTurnLeft(axis, NULL);
				}
				else
					treeTurnLeft(axis, parent);			//A, par(A)
				
				
				 
			}
			else if (par->right->balance == -1)				//case2
			{
				if (par->right->left->balance == 1)			//case2a
				{
					par->balance = -1;
					par->right->balance = 0;
					par->right->left->balance = 0;
				}
				else if (par->right->left->balance == -1)			//case2b
				{
					par->balance = 0;
					par->right->balance = 1;
					par->right->left->balance = 0;
				}
				else 
				{
					par->balance = 0;
					par->right->balance = 0;
					par->right->left->balance = 0;
				}
	
				treeTurnRight(par->right, par);				//B, A

				axis = par;
				par = axis->right->left;				//it will be new par
				test = stackPop(stack, &parent);
				stackPush(stack, parent);
				if (test == FAIL)
				{
					tree->root = axis->right;
					treeTurnLeft(axis, NULL);
				}
				else
					treeTurnLeft(axis, parent);			//A, par(A)
			}
			else if (par->right->balance == 0)				//case3
			{
				axis = par;
				par = axis->right;
				axis->balance = 1;
				axis->right->balance = -1;
				test = stackPop(stack, &parent);
				stackPush(stack, parent);
				if (test == FAIL)
				{
					tree->root = axis->right;
					treeTurnLeft(axis, NULL);
				}
				else
					treeTurnLeft(axis, parent);
			}
		}	

		else if (par->balance == -2)						//if we deleted from right subtree
		{
			if (par->left->balance == -1)					//case1
			{
				axis = par;
				par = axis->left;					//it will be new par
				axis->balance = 0;
				axis->left->balance = 0;
				test = stackPop(stack, &parent);
				stackPush(stack, parent);
				if (test == FAIL)
				{
					tree->root = axis->right;
					treeTurnRight(axis, NULL);
				}
				else
					treeTurnRight(axis, parent);			//A, par(A)
				
				
				 
			}
			else if (par->left->balance == 1)				//case2
			{
				if (par->left->right->balance == -1)			//case2a
				{
					par->balance = 1;
//					par->balance = -1;

					par->left->balance = 0;
					par->left->right->balance = 0;
				}
				else if (par->left->right->balance == 1)		//case2b
				{
					par->balance = 0;
					par->left->balance = -1;
					par->left->right->balance = 0;
				}
				else 
				{
					par->balance = 0;
					par->left->balance = 0;
					par->left->right->balance = 0;
				}
	
//				treeTurnLeft(par->right, par);				//B, A
				treeTurnLeft(par->left, par);				//B, A

				axis = par;
				par = axis->left->right;				//it will be new par
				test = stackPop(stack, &parent);
				stackPush(stack, parent);
				if (test == FAIL)
				{
					tree->root = axis->left;
					treeTurnRight(axis, NULL);
				}
				else
					treeTurnRight(axis, parent);			//A, par(A)
			}
			else if (par->left->balance == 0)				//case3
			{
				axis = par;
				par = axis->left;
				axis->balance = -1;
				axis->left->balance = 1;
				test = stackPop(stack, &parent);
				stackPush(stack, parent);
				if (test == FAIL)
				{
					tree->root = axis->right;
					treeTurnRight(axis, NULL);
				}
				else
					treeTurnRight(axis, parent);
			}
		}

		if (par->balance != 0)
		{
			stackErase(stack);
			return OK;
		}
		else
		{
			x = par;
			stackPop(stack, &par);
		}

		if (par == NULL)
			break;

		if (x == par->left)
			par->balance += 1;
		else
			par->balance -= 1;
	}

	stackErase(stack);
	return OK;
}


ErrorCode treeInsert(Tree *tree, unsigned key, unsigned data)
{
	if (tree == NULL)
		return OOM;


	unsigned *info = (unsigned *) calloc(1, sizeof(unsigned));
	if (info == NULL)
		return OOM;

	*info = data;

	Node *elem = (Node *) calloc(1, sizeof(Node));
	if (elem == NULL)
		return OOM;

	List *list = (List *) calloc(1, sizeof(List));
	if (list == NULL)
		return OOM;
 

	Stack *stack = treeFindPlace(tree, key);
	Node *par;
	ErrorCode test = stackPop(stack, &par);
	*elem = (Node) {key, list, NULL, NULL, 0, 1};
	*list = (List) {info, NULL};

	
	
	if (test == FAIL)					//if void tree
	{
		tree->root = elem;
		stackErase(stack);
		return OK;
	}	


	if (key < par->key)
		par->left = elem;
	else if (key > par->key)
		par->right = elem;
	else
	{
		list->next = par->list;
		par->list = list;
		par->count++;
		stackErase(stack);
		free(elem);
		return OK;
	}

							//correction of balance
	Node *x = elem;

	while ((test != FAIL) && (par->balance == 0))		//while it isn't root
	{
		if (x == par->left)
			par->balance = -1;
		else
			par->balance = 1;
		
		x = par;
		test = stackPop(stack, &par);
	}

	if (test == FAIL)					//if it's end of tree
	{
		stackErase(stack);
		return OK;
	}

	if (((x == par->left) && (par->balance == 1)) || ((x == par->right) && (par->balance == -1)))
	{
		par->balance = 0;
		stackErase(stack);
		return OK;
	}


											//correction of the tree
											//now we on A (+1+1)
	Node *axis;

//	if (par->balance == 2)
//	{
		if ((par->right != NULL) && (par->right->balance == 1))				//case 1 (RR)
		{
			axis = par;
			axis->balance = 0;
			axis->right->balance = 0;

			test = stackPop(stack, &par);
			if (test == FAIL)							//if it's root
			{
				tree->root = axis->right;
				treeTurnLeft(axis, NULL);
			}
			else
				treeTurnLeft(axis, par);
			
		}

		else if ((par->right != NULL) && (par->right->balance == -1))			//case 2 (RL)
		{
	//		if ((par->right->left == NULL) || (par->right->left->balance == 0))
/*			if (par->right->left == NULL)
			{
				par->balance = 0;
				par->right->balance = 0;
	//			par->right->left->balance = 0;
			}
*/			
			if (par->right->left->balance == -1)				//case RLL
			{
				par->balance = 0;
				par->right->balance = 1;
				par->right->left->balance = 0;	
					
			}
			else if (par->right->left->balance == 1)				//case RLR
			{
				par->balance = -1;
				par->right->balance = 0;
	//			par->right->right->balance = 0;	
				par->right->left->balance = 0;	
			}
			else 
			{
				par->balance = 0;
				par->right->balance = 0;
				par->right->left->balance = 0;
			}
	
			treeTurnRight(par->right, par);				//B, A	
			
			axis = par;
			test = stackPop(stack, &par);
			if (test == FAIL)
			{
				tree->root = axis->right;
				treeTurnLeft(axis, NULL);
			}
			else
				treeTurnLeft(axis, par);			//A, par(A)
		}
//	}

//	else if (par->balance == -2)
//	{
		
		else if ((par->left != NULL) && (par->left->balance == 1))				//case 3 (LR)
		{

	//		if ((par->left->right == NULL) || (par->left->right->balance == 0))
/*			if (par->left->right == NULL)
			{
				par->balance = 0;
				par->left->balance = 0;
	//			par->left->right->balance = 0;
			}
*/
			if (par->left->right->balance == 1)		//case LRR
			{
				par->balance = 0;
				par->left->balance = -1;
	//			par->left->left->balance = 0;	
				par->left->right->balance = 0;	
					
			}
			else if (par->left->right->balance == 1)		//case LRL
			{
				par->balance = 1;
				par->left->balance = 0;
				par->left->right->balance = 0;	
			}
			else 
			{
				par->balance = 0;
				par->left->balance = 0;
				par->left->right->balance = 0;
			}
	

			treeTurnLeft(par->left, par);			//B, A	
			
			axis = par;
			test = stackPop(stack, &par);
			if (test == FAIL)
			{
				tree->root = axis->left;
				treeTurnRight(axis, NULL);
			}
			else
				treeTurnRight(axis, par);		//A, par(A)
		

		}

		else if ((par->left != NULL) && (par->left->balance == -1))			//case 4 (LL)
		{
			axis = par;
			axis->balance = 0;
			axis->left->balance = 0;

			test = stackPop(stack, &par);
			if (test == FAIL)				//if it's root
			{
				tree->root = axis->left;
				treeTurnRight(axis, NULL);
			}
			else
				treeTurnRight(axis, par);
		
		}
//	}

	stackErase(stack);

	return OK;
}


ErrorCode treeTurnLeft(Node *axis, Node *par)
{
	if (par != NULL)
	{
		if (par->left == axis)
			par->left = axis->right;	
		else
			par->right = axis->right;
	}
				
		
	Node *temp = axis->right;
	axis->right = axis->right->left;
	temp->left = axis;

	return OK;
}


ErrorCode treeTurnRight(Node *axis, Node *par)
{
	if (par != NULL)
	{
		if (par->left == axis)
			par->left = axis->left;	
		else
			par->right = axis->left;
	}
	
	Node *temp = axis->left;
	axis->left = axis->left->right;
	temp->right = axis;

	return OK;
}



Stack *treeFindPlace(Tree *tree, unsigned key)
{	
	if (tree == NULL)
		return NULL;

	Node *ptr = tree->root;
	Node *par = NULL;
	

	Stack *stack = stackMake();
	if (stack == NULL)
		return NULL;

	while (ptr != NULL)
	{
		par = ptr;
	
		stackPush(stack, ptr); 
	
		if (key == ptr->key)
			break;

		if (key < ptr->key)
			ptr = ptr->left;
		else  
			ptr = ptr->right;
	}	

	return stack;
}		

ErrorCode treeTraversal(Tree *tree, unsigned low)
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
		
		printf("\nkey: %u, qty %u, info: ", elem->key, elem->count);
		
		List *list, *temp;
		list = elem->list;
		
		while (list != NULL)
		{
			printf("%u  ", *(list->info));
			list = list->next;
		}
		
		if (elem->left != NULL)
			ptr = elem->left;

	}

	printf("\n");
	stackErase(stack);

	return OK;
}



ErrorCode treeDot(Tree *tree, char *file_name)
{
	if ((tree == NULL) || (tree->root ==NULL))
		return FAIL;	

	FILE *file = fopen(file_name, "w");
	if (file == NULL)
		return FAIL;

	fprintf(file, "digraph BinaryTree {\n");
  	fprintf(file, "    node [shape=box];\n");

	fprintf(file, "\t\"%u qty %u\";\n", tree->root->key, tree->root->count);
	

	Node *elem = NULL;
	Node *ptr = tree->root;

	Stack *stack = stackMake();
	ErrorCode test;
	
	while (1)
	{
		while (ptr != NULL)
		{
			if (ptr->left != NULL)	
				fprintf(file, "\t\"%u qty %u\" -> \"%u qty %u\";\n", ptr->key, ptr->count, ptr->left->key, ptr->left->count);
		
			stackPush(stack, ptr);
			ptr = ptr->left;
		}
		
		test = stackPop(stack, &elem);
		if (test == FAIL)
			break;

		if (elem->right != NULL)	
			fprintf(file, "\t\"%u qty %u\" -> \"%u qty %u\";\n", elem->key, elem->count, elem->right->key, elem->right->count);


		if (elem->right != NULL)
			ptr = elem->right;

	}


	stackErase(stack);


	fprintf(file, "}\n");
	fclose(file);
	return OK;
}


ErrorCode treeImport(Tree *tree, FILE *file)		
{
	if ((tree == NULL) || (file == NULL))
		return FAIL;
	
	treeClean(tree);

	unsigned key, info;
	int test;
	
	while (1)
	{
		test = fscanf(file, "%d", &key);
		if (test == EOF)
			return OK;
		else if ((test >= 255) || (test == 0))
			return FAIL;
	
			
		test = fscanf(file, "%d", &info);
		if (test == EOF)
			return FAIL;
		else if (test == 0)
			return FAIL;
			
		test = treeInsert(tree, key, info);
		if (test != OK)
			return FAIL;
	}
}

void treeSearchPrint(Tree *tree, unsigned key)
{
	if (tree == NULL)
		return;

	Node *ptr = treeSearch(tree, key);
	if (ptr == NULL)
	{
		printf("no elements with such key\n");	
		return;
	}

	printf("key: %u, info: ", key);
	List *list = ptr->list;
	while (list != NULL)
	{
		printf("%u ", *(list->info));
		list = list->next;
	}
	printf("\n");

}	



Node *treeSearch(Tree *tree, unsigned key)
{
	if (tree == NULL)
		return NULL;

	Node *ptr = tree->root;

	while (ptr != NULL)
	{
		if (key == ptr->key)		
			return ptr;
		else if (key < ptr->key)
			ptr = ptr->left;
		else
			ptr = ptr->right;
	}
	
	return NULL;
}


void treeClosestPrint(Tree *tree, unsigned target)
{
	if (tree == NULL)
		return;

	Node *ptr = treeClosest(tree, target);
	if (ptr == NULL)
	{
		printf("empty tree\n");	
		return;
	}

	printf("key: %u, info: ", ptr->key);
	List *list = ptr->list;
	while (list != NULL)
	{
		printf("%u ", *(list->info));
		list = list->next;
	}
	printf("\n");

}	



Node *treeClosest(Tree *tree, unsigned target)
{
	if (tree == NULL)
		return NULL;
	
	Node *ptr = tree->root;
	Node *prev, *next;
	Node *closest = NULL;
	unsigned value = mod(ptr->key, target);

	Stack *stack = stackMake();
	if (stack == NULL)
		return NULL;
	
	while (ptr != NULL)
	{		
		stackPush(stack,ptr);

		if (target == ptr->key)
		{		
			next = treeFindNext(ptr, stack);
			prev = treeFindPrev(ptr, stack);

			if ((prev == NULL) && (next == NULL))
			{
				stackErase(stack);
				return NULL;
			}
			else if (prev == NULL)
			{	
				stackErase(stack);
				return next;
			}
			else if (next == NULL)
			{	
				stackErase(stack);
				return prev;
			}
			if (next->key - target >= target - prev->key)
			{
				stackErase(stack);
				return prev;
			}
			else
			{
				stackErase(stack);
				return next;	
			}
		}
		else if (target <= ptr->key)
		{
			
			ptr = ptr->left;
		}
		else
		{
			ptr = ptr->right;
		}
		
		if ((ptr != NULL) && (value >= mod(ptr->key, target)))
		{
			value = mod(ptr->key, target);
			closest = ptr;
		}

	}	

	stackErase(stack);
	return closest;
}

Node *treeFindNext(Node *ptr, const Stack *stack)
{
	if (ptr == NULL)
		return NULL;
	
	Node *next, *par;
	Stack *copy = stackCopy(stack);

	if (ptr->right != NULL)
	{
		ptr = ptr->right;

		while (ptr != NULL)
		{
			next = ptr;
			ptr = ptr->left;		
		}
		stackErase(copy);
		return next;
	}
	else 
	{
		stackPop(copy, &par);
		
		while ((par != NULL) && (par->left != ptr))
		{
			ptr = par;
			stackPop(copy, &par);
		}
//		if (par != NULL)
//			return ptr;
//		else
//			return NULL;
		stackErase(copy);
		return par;
	}
}

Node *treeFindPrev(Node *ptr, const Stack *stack)
{
	if (ptr == NULL)
		return NULL;
	
	Node *prev, *par;
	Stack *copy = stackCopy(stack);

	if (ptr->left != NULL)
	{
		ptr = ptr->left;

		while (ptr != NULL)
		{
			prev = ptr;
			ptr = ptr->right;		
		}
		stackErase(copy);
		return prev;
	}
	else 
	{
		stackPop(copy, &par);
		
		while ((par != NULL) && (par->right != ptr))
		{
			ptr = par;
			stackPop(copy, &par);
		}
		
//		if (par != NULL)
//			return ptr;
//		else
//			return NULL;
		
		stackErase(copy);
		return par;	
	}
}



unsigned mod(unsigned a, unsigned b)
{
	if (a >= b)
		return a - b;
	else
		return b - a;
}



void tree_print_bashshell(Tree *tree)
{
	if (tree->root == NULL)
	{
		printf("void tree\n");
		return;
	}
	
	printf("\n");	
	tree_print_bash(tree->root, 0, 4);
}


void tree_print_bash(Node *ptr, int space, int gap)
{
	if (ptr == NULL)
		return;

	tree_print_bash(ptr->right, space + gap, gap);

	
	for (int i = 0; i < space; i++)
		printf(" ");
	
	printf("%u\n", ptr->key);
		

    	tree_print_bash(ptr->left, space + gap, gap);

}
