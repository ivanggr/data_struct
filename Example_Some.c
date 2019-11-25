/* Пример работы с хэш-таблицей и деревом поиска */ 

#include <stdlib.h>
#include <stdio.h>

#include "Bin_Search_Tree.h"
#include "Hash_Table.h"
#include "Stored_Value.h"

/* Пример работы с деревом */
static void bin_tree()
{	
	Tree *T;
	Value v;
	int j;
	T = NULL;
	
	// заполнение дерева
	printf("\nBinary search tree\n");
        printf("\nGenerating pseudo-random pairs...\n\n");
	for (v.c = 'A', j = 0; j < 10; j++, v.c++)
	{
		v.index = (rand() * 100)/RAND_MAX * 10;
		TINSERT(&T, &v, T);
	}

	TPRINT(T);
        
        v.index = 800;
        printf("\nErasing value with index: %d.\n", v.index);
	TERASE(T, &v);
        
        printf("\nTree after erase:\n");        
	TPRINT(T);
	TDELETE(T);

	return;
}

/* Хэш-функция */
int hash_func(Value *v)
{
	return (v->c - 'A') % 11;
}

/* Пример работы с хэш-таблицей */
static void hash_table()
{
	HashTable *HT;
	HT = HTINIT(NULL, 13, &hash_func, HT_DEFAULT);

	Value v, w;
	int j, z = 0;
        printf("Hash table\n");
        
	// генерация букв
        printf("\nGenerating pseudo-random pairs...\n");
	for (j = 0; j < 20; j++)
	{
		v.index = (rand() * 100)/RAND_MAX * 10;
		v.c = (rand() * 11)/RAND_MAX + 'A';
		z++; if (z == 10) {VSET(&w,&v);}
		HTINSERT(HT, &v);
	}

	HTPRINT(HT);

	printf("-----------------\nErasing value: \n");
	VPRINT(&w);
	printf("-----------------\n");

	HashTable *T;
	T = HTERASE(HT, &w);
	if (T == NULL)
		printf("NULL\n");

	w.c = 'D';
	w.index = 510;
	int k = 3;
	printf("Inserting value %d times: \n", k);
	VPRINT(&w);
	printf("-----------------\n");
	for (j = 0; j < k; j++)
	{
		HTINSERT(HT, &w);
		//w.c++;
	}

	HTPRINT(HT);

	j = 4;
	printf("Hash Index[%d] = %d\n", j, HTLOOK(HT, j));

	HTCLEAR(HT);
	free(HT);
	return;
}

int main()
{
	bin_tree();
        putchar('\n');
        hash_table();
	return (EXIT_SUCCESS);
}