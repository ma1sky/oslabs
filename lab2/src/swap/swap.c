#include "swap.h"

void swap(char *left, char *right)
{
	char buf = *left;
	*left = *right;
	*right = buf;
}