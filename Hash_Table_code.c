/* Реализация хэш-таблицы, объявленной в Hash_Table.h */
#include <stdlib.h>
#include <stdio.h>

#include "Hash_Table.h"

/* ----- статические функции и переменные ----- */
static enum HT_INC {HT_INSERT = +1, HT_ERASE = -1};

/* внутренняя функция для пересчета переменных */
static void HTRECALC(HashTable *HT, int hc, int inc)
{
	/* т.к. мы полностью контролируем вызовы, то без проверки аргументов */
	if (inc == HT_INSERT)
	{
		// при вставке в нулевую строку
		if (HT->qarray[hc]++ == 0)
		{
			HT->empty--;
			HT->occupied++;
			HT->load_ratio = (int) ((100UL * (unsigned long) HT->occupied) / (unsigned long) HT->capacity); 	
		}
		HT->entries++;
	}
	else if (inc == HT_ERASE)
	{
		// при удалении последнего элемента из строки
		if (HT->qarray[hc]-- == 1)
		{
			HT->empty++;
			HT->occupied--;
			HT->load_ratio = (int) ((100UL * (unsigned long) HT->occupied) / (unsigned long) HT->capacity); 	
		}
		HT->entries--;
	}
	return;			
}  

/* внутренняя функция для проверки таблицы и значения, подсчитывает и возвращает хэш-индекс массива */
static int HTCHECK(HashTable *HT, Value *v)
{
	// проверка аргументов
	if (HT == NULL || v == NULL)
		return -1;
	
	// проверка наличия ёмкости, массивов значений, хеш-функции
	if (HT->barray == NULL || HT->qarray == NULL || HT->capacity <= 0 || HT->hash_func == NULL)
		return -1;

	// подсчет хеш-индекса
	int hc;
	hc = HT->hash_func(v);
	return hc >= HT->capacity ? -1 : hc;
}  
/* ----- конец статических функций ----- */


/* Вставляет в хеш-таблицу HT новое значение v. Успех HT, неудача NULL */
HashTable *HTINSERT(HashTable *HT, Value *v)
{
	// подсчет хеш-функции, проверка аргументов и таблицы
	int hc;
	hc = HTCHECK(HT, v);
	if (hc < 0)
		return NULL;

	// вставка в таблицу и изменение переменных
	List *L, *R;
	R = HT->barray[hc];
	L = LUINSERT(R, v, HT->unique);
	if (L == NULL)
		return NULL;

	// если у нас был нулевой указатель
	if (R == NULL)
		HT->barray[hc] = L;

	// пересчет переменных
	HTRECALC(HT, hc, HT_INSERT);

	return HT;
}

/* Ищет величину v в таблице, если она есть, то возвращает указатель на неё. Иначе NULL */
Value *HTFIND(HashTable *HT, Value *v)
{
	// подсчет хеш-функции, проверка аргументов и таблицы
	int hc;
	hc = HTCHECK(HT, v);
	if (hc < 0)
		return NULL;

	// поиск в таблице
	List *L;
	if ((L = LFIND(HT->barray[hc], v)) == NULL)
		return NULL;
	
	return &(L->v);
}

/* Находит и удаляет из хэш-таблицы HT одно значение v. Успех HT, неудача NULL */
HashTable *HTERASE(HashTable *HT, Value *v)
{
	// подсчет хеш-функции, проверка аргументов и таблицы
	int hc;
	hc = HTCHECK(HT, v);
	if (hc < 0)
		return NULL;
	
	// просмотр, есть ли что-то в строке с таким хэш-кодом
	List *R, *L, *L2;
	R = HT->barray[hc];
	if (R == NULL)
		return NULL;

	// поиск элемента списка с таким значением в строке с хэш-кодом
	L = LFIND(R, v);
	if (L == NULL)
		return NULL;

	// удаление элемента списка 
	L2 = LDELETE(L);

	// если мы удалили последнюю запись в строке, либо был удален первый элемент
	if (L == R)
		HT->barray[hc] = L2;

	// пересчет переменных
	HTRECALC(HT, hc, HT_ERASE);
	
	return HT;
}

/* Смотрим, есть ли в таблице HT элементы с хэш-кодом hc и возвращаем их количество */
int HTLOOK(HashTable *HT, int hc)
{
	// проверка аргументов
	if (HT == NULL || hc < 0)
		return -1;
	
	// проверка наличия ёмкости, массивов значений, хеш-функции
	if (HT->barray == NULL || HT->qarray == NULL || HT->capacity <= 0)
		return -1;
	if (hc >= HT->capacity)
		return -1;

	if (HT->barray[hc] != NULL)
		return HT->qarray[hc];
	else
		return -1;
}

/* Инициализирует хэш-таблицу HT ёмкостью capacity с использованием хеш-функции hash_func().
Если HT == NULL, то создаёт новую таблицу. 
Если HT != NULL, то перезаписывает все переменные структуры, освобождает память указателей в исх.структуре !
Если инициализация успешна, то возвращает указатель на HT, иначе NULL. */
HashTable *HTINIT(HashTable *HT, int capacity, int (*hash_func)(Value *), int unique)
{
	/* проверка аргументов */
	if (hash_func == NULL || capacity <= 0)
		return NULL;

	/* создание временной таблицы */
	HashTable *T;
	T = (HashTable *) malloc(sizeof(HashTable));
	if (T == NULL)
		return NULL;
	
	/* запрос памяти для первого массива */
	T->barray = (List **) calloc((size_t) capacity, sizeof(List *));
	if (T->barray == NULL)
	{
		free(T);
		return NULL;
	}

	/* запрос памяти для второго массива */
	T->qarray = (int *) calloc((size_t) capacity, sizeof(int));
	if (T->qarray == NULL)
	{
		free(T->barray);
		free(T);
		return NULL;
	}

	/* заполнение памяти массивов в два прохода, а не в один */
	int i;
	for (i = 0; i < capacity; i++)
		T->barray[i] = NULL;
	for (i = 0; i < capacity; i++)
		T->qarray[i] = 0;

	/* задание переменных */
	T->capacity = capacity;
	T->empty = capacity;
	T->entries = 0;
	T->hash_func = hash_func;
	T->load_ratio = 0;
	T->occupied = 0;
	T->unique = unique;

	/* возврат */	
	if (HT == NULL)
		return T;    // если нам был передан нулевой указатель HT, вернем временную таблицу
	else
	{
		free(HT->barray);  // освобождаем память двух массивов в текущей структуре
		free(HT->qarray);  
		*HT = *T;    // копирование временной структуры при необходимости
		free(T);     // освобождение памяти, занятой временной структурой
		return HT;    
	}
}

/* Сбрасываем хэш-таблицу, обнуляем массивы, указатель на хэш-функцию, устанавливаем ёмкость в ноль и т.д. */ 
HashTable *HTCLEAR(HashTable *HT)
{
	if (HT == NULL)
		return NULL;
	free(HT->barray); HT->barray = NULL;
	free(HT->qarray); HT->qarray = NULL;
	HT->capacity = 0;
	HT->empty = 0;
	HT->entries = 0;
	HT->hash_func = NULL;
	HT->load_ratio = 0;
	HT->occupied = 0;
	HT->unique = HT_DEFAULT;
	return HT;
}

/* Изменяет число строк хэш-таблицы HT на новое newcapacity. Возвращает новую ёмкость */
//int HTRESIZE(HashTable *HT, int newcapacity);
/* Пока не реализовано */

/* Печатает хэш-таблицу HT. Возвращает число отпечатанных хранимых элементов */
int HTPRINT(HashTable *HT)
{
	if (HT == NULL)
		return 0;

	printf("\nHash Table\n");
	if (HT->barray == NULL || HT->qarray == NULL || HT->capacity == 0 || HT->entries == 0 || HT->occupied == 0)
	{
		printf("<empty>\n");
		return 0;
	}

	printf("Capacity: %d. Total entries: %d.\n", HT->capacity, HT->entries);
	int j, count;
	for (count = 0, j = 0; j < HT->capacity; j++)
	{
		putchar('\n');
		printf("- Hash index: %d. Quantity: %d.\n", j, HT->qarray[j]);		
		if (LPRINT(HT->barray[j]) > 0)
			count++;
	}

	putchar('\n');
	return count;
}