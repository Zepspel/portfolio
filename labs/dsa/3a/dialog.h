#ifndef _DIALOG_H__
#define _DIALOG_H_

#include <stdio.h>
#include "table.h"

int getInt_file(FILE *file, unsigned int *intp, unsigned int low, unsigned int high);
int getInt(unsigned int *intp, unsigned int low, unsigned int high);
int getInt_right(unsigned int *m, char *c, unsigned int low, unsigned int high);
int dialog(Table *table);

int case_insert_element(Table *table);
int case_delete_element(Table *table);
int case_search_bykey(Table *table);
void case_print_table(Table *table);
int case_search_children(Table *table);
void case_help();
int case_import_file(Table *table);



#endif
