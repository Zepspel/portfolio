#ifndef _DIALOG_H_
#define _DIALOG_H_




int getInt(unsigned int *intp, unsigned int low, unsigned int high);
int getInt_right(unsigned int *m, char *c, unsigned int low, unsigned int high);

int get_word(char *choice);
int get_word_pattern(char *choice, const char * const *option, int amount_option);
int get_word_right(char *word);


void case_help();

ErrorCode case_insert_element(Tree *tree);
ErrorCode case_delete_element(Tree *tree);

ErrorCode case_print_tree(Tree *tree);
ErrorCode case_search_element(Tree *tree);
ErrorCode case_closest_tree(Tree *tree);

void case_export_tree(Tree *tree);
ErrorCode case_import_tree(Tree *tree);

void case_clean_tree(Tree *tree);

ErrorCode case_timing_tree(Tree *tree);

ErrorCode dialog(Tree *tree);




#endif
