#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "tree_privat.h"



Tree *tree_make()
{
	Tree *tree = (Tree *) calloc(1, sizeof(Tree));
	if (tree == NULL)
		return NULL;

	tree->root = NULL;

	return tree;
}

ErrorCode tree_clean(Tree *tree)
{
	if (tree == NULL)
		return FAIL;
	

	Node *ptr = tree_maximum(tree);
	if (ptr == NULL)
		return FAIL;
	
	Node *temp;
	while (ptr != NULL)
	{
		free(ptr->key);
		free(ptr->info);
		
		ptr->key = NULL;
		ptr->info = NULL;
		ptr->left = NULL;
		ptr->right = NULL;
		ptr->par = NULL;
				
		temp = ptr;
		ptr = ptr->prev;
		temp->prev = NULL;
		free(temp);
	}

	tree->root = NULL;	

	return OK;
}

ErrorCode tree_import(Tree *tree, FILE *file)				////////////////////////////////////////////////////
{
	if ((tree == NULL) || (file == NULL))
		return FAIL;
	
	tree_clean(tree);

	char key[256];
	unsigned int info;
	int test;
	
	while (1)
	{
		test = fscanf(file, "%255s", key);
		if (test == EOF)
			return OK;
		else if ((test >= 255) || (test == 0))
			return FAIL;
	
			
		test = fscanf(file, "%d", &info);
		if (test == EOF)
			return FAIL;
		else if (test == 0)
			return FAIL;
			
		test = tree_insert(tree, key, info);
		if (test != OK)
			return FAIL;
	}
}

ErrorCode tree_erase(Tree *tree)
{
	if (tree == NULL)
		return FAIL;

	Node *ptr = tree_maximum(tree);
	if (ptr == NULL)
	{
		free(tree);
		return OK;
	}	

	Node *temp;
	while (ptr != NULL)
	{
		free(ptr->key);	
		free(ptr->info);
		
		temp = ptr;
		ptr = ptr->prev;
		free(temp);
	}
	
	free(tree);

	return OK;
}


Node *tree_find_place(Tree *tree, char *clef, unsigned int *release)
{	
	if (tree == NULL)
		return NULL;

	Node *ptr = tree->root;
	Node *par = NULL;
	
	int compare;
	*release = 0;

	while (ptr != NULL)
	{
		par = ptr;
		
		compare = strcmp(clef, ptr->key);
		if (compare == 0)
		{
			if (ptr->release > *release)	
				*release = ptr->release;
		}
		if (compare < 0)
			ptr = ptr->left;
		else  
			ptr = ptr->right;
	}
	*release += 1;	

	return par;
}		





ErrorCode tree_delete(Tree *tree, char *clef)
{
	if ((tree == NULL) && (clef == NULL))
		return FAIL;	

	Node **vector = tree_search(tree, clef);
	if (vector == NULL)
		return FAIL;

	size_t i = 0;
	Node *ptr = NULL;
	while (vector[i] != NULL)
	{	
		ptr = vector[i];
		i++;	
	}
	printf("key: %s, release: %d\n", ptr->key, ptr->release);

	Node *changed = ptr;
	Node *del;

	if ((ptr->left == NULL) || (ptr->right == NULL))
		del = ptr;
	else
	{
		ptr = ptr->right;
		while (ptr != NULL)					//min elem in right subtree
		{
			del = ptr;
			ptr = ptr->left;
		}
	}		
	
	Node *subtree = NULL;
	if (del->left != NULL)
		subtree =  del->left;
	else if (del->right !=NULL)
		subtree = del->right;

	if (tree->root == del)						//if we delete root
		tree->root = subtree;
	else if (subtree != NULL)				///////////////////////////////////////////////////////////////
		subtree->par = del->par;



	if (changed != del)						//copy data from deal deleted to which we wanted delete at first
	{
		size_t len = strlen(del->key);
		changed->key = (char *) realloc(changed->key, len+1);		
		if (changed->key == NULL)
			return OOM;

		strcpy(changed->key, del->key);
		*(changed->info) = *(del->info);
		changed->release = del->release;

	}	
	
							//find next elem after del to change its prev value
	Node *next;
	ptr = del;
	if (ptr->right != NULL)			
	{
		ptr = ptr->right;
		while (ptr != NULL)			//min elem in right subtree
		{
			next = ptr;
			ptr = ptr->left;
		}
		next->prev = del->prev;
	}	
	else
	{
		next = ptr->par;					//next elem
		
		while ((next != NULL) && (ptr == next->right))		//while elem lays on the right from par
		{
			ptr = ptr->par;
			next = next->par;
		}
		if (next != NULL)
		{
			next->prev = del->prev;
		}
	}				
		
	
	if (del->par != NULL)					//if it isn't root
	{
		if (del->par->left == del)			//if deleted elem on the left subtree
			del->par->left = subtree;
		else
			del->par->right = subtree;
	}
	else if ((del->left == NULL) && (del->right == NULL)) 	//if it's last elem
		tree->root = NULL;				//////////////////////////////////////////////////////////////



	free(del->key);
	free(del->info);
	free(del);

	free(vector);
	return OK;
}

ErrorCode tree_insert(Tree *tree, char *clef, unsigned int data)
{
	if (tree == NULL)
		return OOM;

	size_t len = strlen(clef);

	char *key = (char *) calloc(len + 1, sizeof(char));
	if (key == NULL)
		return OOM;

	strcpy(key, clef);

	unsigned int *info = (unsigned int *) calloc(1, sizeof(int));
	if (info == NULL)
		return OOM;

	*info = data;

	Node *elem = (Node *) calloc(1, sizeof(Node));
	if (elem == NULL)
		return OOM;
 


	unsigned int release;
	Node *par = tree_find_place(tree, clef, &release);
	
	*elem = (Node) {key, info, NULL, NULL, NULL, NULL, release};
	
	if (par == NULL)				//if void tree
	{
		tree->root = elem;
		return OK;
	}	

	int compare = strcmp(clef, par->key);
	if (compare < 0)
	{	
		par->left = elem;
		elem->par = par;
	}
	else 
	{
		par->right = elem;
		elem->par = par;
	}
	
	Node *child = elem;					//firmware
	par = child->par;					//previous elem

	while ((par != NULL) && (child == par->left))		//while elem lays on the left from par
	{
		child = child->par;
		par = par->par;
	}
	elem->prev = par;
	
	


	child = elem;
	par = child->par;					//next elem
	
	while ((par != NULL) && (child == par->right))		//while elem lays on the right from par
	{
		child = child->par;
		par = par->par;
	}
	if (par != NULL)
	{
		par->prev = elem;
	}
	

	return OK;
}

Node *find_next(Node *ptr)
{
	if (ptr == NULL)
		return NULL;

				
	Node *par = NULL;
	if (ptr->right != NULL)					//if left child exists
	{
		ptr = ptr->right;
		while (ptr != NULL)
		{
			par = ptr;
			ptr = ptr->left;
		}
		ptr = par;
	}
	else
	{
		Node *child = ptr;					
		par = ptr->par;
		
		while ((par != NULL) && (child == par->right))		//while elem lays on the left from par
		{
			child = child->par;
			par = par->par;
		}
		ptr = par;
	}
	
	return ptr;
}



ErrorCode work(Node *ptr)
{
	if (ptr == NULL)
		return OK;
	
	work(ptr->right);
	printf("key: %s, info: %d\n", ptr->key, *(ptr->info));	
	work(ptr->left);

}

ErrorCode tree_traversal_recursive(Tree *tree)
{
	if (tree == NULL)
		return FAIL;
	
	work(tree->root);
	
	return OK;
}

void tree_search_print(Tree *tree, char *clef)
{
	Node **vector = tree_search(tree, clef);
	if (vector == NULL)
	{
		printf("no elements with such key\n");	
		return;
	}

	size_t i = 0;
	printf("\n");
	while (vector[i] != NULL)
	{
		printf("key: %s, info: %u\n", vector[i]->key, *(vector[i]->info));
		i++;
	}

	free(vector);
}	


Node **tree_search(Tree *tree, char *clef)
{
	if (tree == NULL)
		return NULL;

	Node *ptr = tree->root;
	Node *rem = NULL;

	int compare;
	while (ptr != NULL)
	{
		compare = strcmp(clef, ptr->key);
		if (compare == 0)			//remember last elem with such key
		{
			rem = ptr;
			ptr = ptr->right;
		}
		else if (compare < 0)
			ptr = ptr->left;
		else
			ptr = ptr->right;
	}
	
	if (rem == NULL)
		return NULL;			//no elem with such key	


	size_t count = 0;
	Node *temp = rem;

	while (temp != NULL)
	{
		compare = strcmp(clef, temp->key);
		if (compare != 0)
			break;
		count++;
		temp = temp->prev;
	}


	Node **vector_pointers = (Node **) calloc(count+1, sizeof(Node*));
	size_t i = 0;
	while (i < count)
	{
		vector_pointers[i] = rem;
		rem = rem->prev;
		i++;
	}
	
	vector_pointers[i] = NULL;

	return vector_pointers;
}

Node *tree_maximum(Tree *tree)
{
	if (tree == NULL)
		return NULL;
	
	Node *ptr = tree->root;
	Node *par = NULL;
	while (ptr != NULL)
	{
		par = ptr;
		ptr = ptr->right;
	}

	return par;	
}

void tree_max_print(Tree *tree)
{
	Node **vector = tree_max(tree);
	if (vector == NULL)
	{
		printf("void tree\n");	
		return;
	}

	printf("\n");
	size_t i = 0;
	while (vector[i] != NULL)
	{
		printf("key: %s, release: %u, info: %u\n", vector[i]->key, vector[i]->release, *(vector[i]->info));
		i++;
	}

	free(vector);

	return;
}	



Node **tree_max(Tree *tree)
{
	if (tree == NULL)
		return NULL;

	Node *ptr = tree->root;
	Node *par = NULL;

	if (ptr == NULL)
		return NULL;		//if void tree

	while (ptr != NULL)
	{
		par = ptr;
		ptr = ptr->right;
	}

	size_t count = 0;
	char *max = par->key;
	int compare = strcmp(par->key, max);
	Node *temp = par;

	while (compare == 0)
	{
		count++;
		ptr = temp;				/////////////////////////////////////////
		temp = temp->prev;
		if (temp == NULL)
			break;
		compare = strcmp(temp->key, max);
	}

	
	Node **vector_pointers = (Node **) calloc(count+1, sizeof(Node*));
	size_t i = 0;
	while (i < count)
	{
		vector_pointers[i] = par;
		par = par->prev;
		i++;
	}
	
	
	vector_pointers[i] = NULL;
	
	return vector_pointers;
}



ErrorCode tree_range(Tree *tree, char *first, char *last)	//работает так, как будто мы хотим вставить элемент, а потом ищем для него
{								//ближайший предыдущий
	unsigned int trash;
	Node *ptr = tree_find_place(tree, last, &trash);
	if (ptr == NULL)
		return FAIL;	
								//find previous elem

	int compare = strcmp(first, ptr->key);
	while (ptr != NULL)
	{
		compare = strcmp(first, ptr->key);
		if (compare > 0)
			break;
		printf("key: %s, release: %d, info: %d\n", ptr->key, ptr->release, *(ptr->info));	
		ptr = ptr->prev;
	}


	return OK;
}



ErrorCode tree_traversal(Tree *tree)
{
	if (tree == NULL)
		return OOM;
	
	if (tree->root == NULL)
	{
		printf("void tree\n");
		return OK;
	}


//	Node **vector = tree_max(tree);
//	if (vector == NULL)
//		return FAIL;

//	Node *ptr = vector[0];	

	Node *ptr = tree_maximum(tree);
		
	while (ptr != NULL)
	{
		printf("key: %s, release: %d, info: %d\n", ptr->key, ptr->release, *(ptr->info));	
		ptr = ptr->prev;
	}


//	free(vector);
	return OK;
}

void tree_export_recursion(Node *ptr, FILE *file)
{
	if (ptr == NULL)
		return;
	
	if (ptr->left != NULL) 
	{
        	fprintf(file, "\t\"%s v%d\" -> \"%s v%d\";\n", ptr->key, ptr->release, ptr->left->key, ptr->left->release);
        	tree_export_recursion(ptr->left, file);
    	}

	if (ptr->right != NULL) 
	{
        	fprintf(file, "\t\"%s v%d\" -> \"%s v%d\";\n", ptr->key, ptr->release, ptr->right->key, ptr->right->release);
        	tree_export_recursion(ptr->right, file);
    	}

}

ErrorCode tree_todot(Tree *tree, char *file_name)
{
	if ((tree == NULL) || (tree->root ==NULL))
		return FAIL;	

	FILE *file = fopen(file_name, "w");
	if (file == NULL)
		return FAIL;

	fprintf(file, "digraph BinaryTree {\n");
  	fprintf(file, "    node [shape=box];\n");

	fprintf(file, "\t\"%s v%d\";\n", tree->root->key, tree->root->release);
	
	tree_export_recursion(tree->root, file);

									//then firmware
	Node *ptr = tree_maximum(tree);
	while (ptr->prev != NULL)
	{
        	fprintf(file, "\t\"%s v%d\" -> \"%s v%d\" [color=\"red\", constraint=false];\n", ptr->key, ptr->release, ptr->prev->key, ptr->prev->release);
		ptr = ptr->prev;
	}
	
	fprintf(file, "}\n");
	fclose(file);
	return OK;
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
	
	printf("%s\n", ptr->key);
		

    	tree_print_bash(ptr->left, space + gap, gap);

}
