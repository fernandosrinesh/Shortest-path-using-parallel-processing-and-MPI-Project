#ifndef BITONIC_SORT_H_
#define BITONIC_SORT_H_
#include <mpi.h>
#include "utils.h"

void bitSwap(int*, int*, int);
void bitMerge(int*, int, int, int);
void bitSort(int*, int, int, int, int);
void bitSortSer(int*, int, int, int);

#endif
