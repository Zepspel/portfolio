#ifndef _DIALOG_H__
#define _DIALOG_H_

#include <stdio.h>
#include "table.h"


int getInt(unsigned int *intp, unsigned int low, unsigned int high);
int getInt_right(unsigned int *m, char *c, unsigned int low, unsigned int high);
int get_word(char *choice);
int get_word_pattern(char *choice, const char * const *option, int amount_option);
int get_word_right(char *word);


void case_help();
int case_insert_element(Table **ptable);
int case_delete_element(Table *table);
void case_print_table(Table *table);
int case_search_element(Table *table);
int case_export_table(Table *table);
int case_import_table(Table **ptable);
int case_clean_table(Table *table);

int dialog(Table **ptable);






#endif
