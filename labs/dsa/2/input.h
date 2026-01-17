#ifndef _INPUT_H_
#define _INPUT_H_

#include "queue.h"

int getInt(int *intp, int low, int high);
int getInt_right(int *m, char *c, int low, int high);
int get_probably(double *m, char *s);
int input(int *size_queue, int *count_nodes, int *low_range, int *high_range, double *p1, double *p2, double *p3);

#endif
