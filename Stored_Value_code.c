/* Реализация хранимой величины */

#include "Stored_Value.h"

inline void VSET(Value *d, Value *s)
{
	if (d != NULL && s != NULL)
		*d = *s;
	return;
}

inline int VCMP(Value *a, Value *b)
{
	if (a->index == b->index) 
		return 0;
	else if (a->index < b->index ) 
		return -1;
	else 
		return +1;
}

inline int VFPRINT(FILE *fp, Value *v)
{
	if (fp == NULL || v == NULL)
		return -1;
	else
		return fprintf(fp, "Index: %3d.  Char: %c.\n", v->index, v->c);
}

inline int VPRINT(Value *v)
{
	if (v == NULL)
		return -1;
	else
		return printf("Index: %3d.  Char: %c.\n", v->index, v->c);
}