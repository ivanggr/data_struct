/* Реализация двусвязного списка, объявленного в List.h */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "List.h"
#include "Stored_Value.h"

/* Константа условной компиляции реализации
#define	LIST_TRAVERSE_NEXT - при обходе списка (TRAVERSE), перед вызовом переданной функции f() 
сохраняется указатель L->next, после вызова переходит к элементу L->next.
Так реализуется защита от изменения в f() указателя на следующий элемент.
*/

// вставка элемента с проходом вперёд, вставка идёт по-умолчанию
List *LINSERT(List *L, Value *v)
{
	if (v == NULL)
		return NULL;
	List *parent; // указатель для хранения предыдущего элемента
	parent = NULL;
	int i;
	// добавим ограничение на число элементов в списке
	for (i = 0; i < INT_MAX; i++)
	{
		// если текущий элемент пуст
		if (L == NULL)
		{
			L = (List *) malloc(sizeof(List));
			if (L == NULL)
				return NULL;
			L->prev = parent; // устанавливаем указатель на предыдущий элемент
			if (parent != NULL)
				parent->next = L; // в предыдущем элементе также устанавливаем указатель на L
			L->next = NULL;
			VSET(&(L->v), v);
			return L;
		}
		// если текущий элемент не пуст
		else
		{
			/* Проверка на уникальность хранимой величины посредством условной компиляции
			#ifdef LIST_UNIQUE_VALUES
			if (VCMP(&(L->v), v) == 0)
				return NULL;
			#endif */
			// проверка на уникальность при необходимости
			if (LIST_DEFAULT == LIST_UNIQUE)
			{
				if (VCMP(&(L->v), v) == 0)
					return NULL;
			}
			parent = L;
			L = L->next;
		}
	}
	return NULL;
}

/* вставка элемента с проходом вперёд, вставка согласно значению order: 
int == LIST_UNIQUE только уникальные, != LIST_UNIQUE любые */
List *LUINSERT(List *L, Value *v, int order)
{
	if (v == NULL)
		return NULL;
	List *parent; // указатель для хранения предыдущего элемента
	parent = NULL;
	int i;
	// добавим ограничение на число элементов в списке
	for (i = 0; i < INT_MAX; i++)
	{
		// если текущий элемент пуст
		if (L == NULL)
		{
			L = (List *) malloc(sizeof(List));
			if (L == NULL)
				return NULL;
			L->prev = parent; // устанавливаем указатель на предыдущий элемент
			if (parent != NULL)
				parent->next = L; // в предыдущем элементе также устанавливаем указатель на L
			L->next = NULL;
			VSET(&(L->v), v);
			return L;
		}
		// если текущий элемент не пуст
		else
		{
			// проверка на уникальность
			if (order == LIST_UNIQUE)
			{
				if (VCMP(&(L->v), v) == 0)
					return NULL;
			}
			parent = L;
			L = L->next;
		}
	}
	return NULL;
}


// удаляет первый найденный проходом вперёд элемент по значению
// после удаления возвращает указатель на предыдущий элемент, если его нет, то на следующий
List *LDELETE(List *L)
{
	if (L == NULL)
		return NULL;
	List *N, *P; // next, prev
	N = L->next;
	P = L->prev;
	if (P != NULL)
		P->next = N;
	if (N != NULL)
		N->prev = P;
	free(L);
	// вернем указатель на предыдущий элемент, 
	// если он нулевой, то вернем указатель на следующий элемент
	return P != NULL ? P : N;
}

// удаляет первый найденный проходом вперёд элемент по значению
inline List *LERASE(List *L, Value *v)
{
	return LDELETE(LFIND(L, v));
}

// поиск по значению (индексу) вперёд
List *LFIND(List *L, Value *v)
{
	while (L != NULL)
	{
		if (VCMP(&(L->v), v) == 0)
			return L;
		else
			L = L->next;
	}
	return NULL;
}

// проход по списку вперёд с применением к каждому элементу функции f(List *), возврат числа пройденных элементов
int LTRAVERSE(List *L, void (*f)(List *))
{
	int count = 0;  // сколько элементов пройдено	
	if (L == NULL || f == NULL) return -1;  // проверка аргументов

	#ifdef LIST_TRAVERSE_NEXT
	    List *next;
	#endif

	while (L != NULL)  // проход вперед
	{
		#ifdef LIST_TRAVERSE_NEXT
		    next = L->next;
		    f(L);
		    L = next;
		#else
		    f(L);
		    L = L->next;
		#endif
		count++;
	}
	return count;  // возврат количества пройденных элементов
}

// удалить все элементы списка с проходом вперёд, возврат числа удалённых
int LDELETE_ALL(List *L)
{
	List *next;
	int count = 0;  // сколько элементов пройдено	
	while (L != NULL)  // проход вперед
	{
		next = L->next;
		free(L);
		L = next;
		count++;
	}
	return count;  // возврат количества пройденных элементов
}

static void Lprint_node(List *L)
{
	if (L != NULL)
		VPRINT(&L->v);
	return;
}

// отпечать список с проходом вперёд, возврат числа отпечатанных
int LPRINT(List *L)
{
	if (L == NULL)
	{
		printf("<empty>\n");
		return 0;
	}
	else
		return LTRAVERSE(L, &Lprint_node);
}

// обращение списка, возврат нового корня списка
List *LROTATE(List *L)
{
	List *NL, *NEWR = L;
	while (L != NULL)
	{
		NEWR = L;
		NL = L->next;
		L->next = L->prev;
		L->prev = NL;
		L = NL;
	}
	return NEWR;
}