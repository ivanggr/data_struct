/* Hash_Table.h
Реализация хэш-таблицы с цепочечным методом разрешения коллизий.
Хранимый тип данных: Value.
Объявлены операции: INSERT, FIND, ERASE, LOOK, 
                    INIT, CLEAR, PRINT
*/

/* Реализация использует два массива:
- в первом массиве хранятся указатели на начальные элементы связного списка;
- во втором массиве записана целая типа int, которая показывает, сколько элементов в каждой из ячеек,
если она == 0 ячейка свободна, > 0 ячейка занята и сколько элементов в списке */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "List.h"
#include "Stored_Value.h"

typedef struct HashTable HashTable;
struct HashTable
{
	int (*hash_func)(Value *);   // указатель на хеш-функцию, используемую для формирования индекса массива	
	int capacity;       // ёмкость таблицы (число строк хэш-значений) == max_bucket_count
	int empty;          // число свободных строк хэш-значений
	int occupied;       // число занятых строк хэш-значений
	int load_ratio;     // коэффициент загрузки в процентах (от 0 до 100) = 100UL * occupied / capacity
	long entries;       // общее число элементов, внесенных в таблицу
	int unique;         // вставлять в таблицу значения без повторений (не должно быть VCMP(&v1,&v2) == 0)
	List **barray;      // первый массив, непосредственая таблица указателей на начальные элементы списка
	int   *qarray;      // второй массив, в котором хранятся количества элементов списков
};

// константы требуют включения заголовка List.h
#define	HT_UNIQUE      LIST_UNIQUE        // вставка в таблицу только уникальных значений
#define	HT_NON_UNIQUE  LIST_NON_UNIQUE    // вставка в таблицу любых значений
#define HT_DEFAULT     HT_UNIQUE          // порядок вставки по-умолчанию: только уникальные значения

/* Вставляет в хеш-таблицу HT новое значение v. Успех HT, неудача NULL. */
HashTable *HTINSERT(HashTable *HT, Value *v); 

/* Ищет величину v в таблице, если она есть, то возвращает указатель на неё. Иначе NULL */
Value *HTFIND(HashTable *HT, Value *v);

/* Находит и удаляет из хэш-таблицы HT одно значение v. Успех HT, неудача NULL */
HashTable *HTERASE(HashTable *HT, Value *v); 

/* Смотрит, есть ли в таблице HT элементы с хэш-кодом hc и возвращает их количество */
int HTLOOK(HashTable *HT, int hc);

/* Инициализирует хэш-таблицу HT ёмкостью в строках capacity с использованием хеш-функции hash_func().
Если HT == NULL, то создаёт новую таблицу. 
Если HT != NULL, то перезаписывает все переменные структуры, освобождает память указателей в исх.структуре !
Если инициализация успешна, то возвращает указатель на HT, иначе NULL. */
HashTable *HTINIT(HashTable *HT, int capacity, int (*hash_func)(Value *), int unique);   

/* Сбрасываем хэш-таблицу, обнуляем массивы, указатель на хэш-функцию, устанавливаем ёмкость в ноль и т.д. */ 
HashTable *HTCLEAR(HashTable *HT);

/* Функция, реализующая операцию RESIZE требует объявления и определения. 
 Изменяет число строк хэш-таблицы HT на новое newcapacity. Возвращает новую ёмкость 
int HTRESIZE(HashTable *HT, int newcapacity);
 */

/* Печатает хэш-таблицу HT. Возвращает число отпечатанных хранимых элементов */
int HTPRINT(HashTable *HT);


#endif