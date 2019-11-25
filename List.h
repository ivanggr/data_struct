/* List.h
Реализация структуры данных двунаправленный связный список.
Хранимый тип данных Value.
Объявлены операции: INSERT, DELETE, ERASE, FIND, TRAVERSE, DELETE_ALL, PRINT.
*/

#ifndef LIST_H
#define LIST_H

#include "Stored_Value.h"

typedef struct List List;
struct List
{
	List *next;  // следующий элемент
	List *prev;  // предыдущий элемент (родитель)
	Value v;     // хранимое значение
};

#define	LIST_UNIQUE      0                 // вставка только уникальных значений
#define	LIST_NON_UNIQUE  1                 // вставка любых значений
#define LIST_DEFAULT     LIST_NON_UNIQUE   // порядок вставки по-умолчанию: значения с повторами допустимы

// вставка элемента с проходом вперёд, вставка идёт по-умолчанию: значения с повторами допустимы
List *LINSERT(List *, Value *);    

// вставка элемента с проходом вперёд, вставка согласно значению int == LIST_UNIQUE только уникальные, != LIST_UNIQUE любые
List *LUINSERT(List *, Value *, int);    

// удаление по указателю одной записи
List *LDELETE(List *);

// удаляет первый найденный проходом вперёд элемент по значению
// после удаления возвращает указатель на предыдущий элемент, если его нет, то на следующий
List *LERASE(List *, Value *);

// поиск по значению (индексу) вперёд
List *LFIND(List *, Value *);       

// проход по списку вперёд с применением к каждому элементу функции f(List *), возврат числа пройденных элементов
int LTRAVERSE(List *, void (*f)(List *));  

// удалить все элементы списка с проходом вперёд, возврат числа удалённых
int LDELETE_ALL(List *);

// отпечать список с проходом вперёд, возврат числа отпечатанных записей
int LPRINT(List *);

// обращение списка, возврат нового корня списка
List *LROTATE(List *L);

/* Константа условной компиляции реализации
#define	LIST_TRAVERSE_NEXT - при обходе списка (TRAVERSE), перед вызовом переданной функции f() 
сохраняется указатель L->next, после вызова переходит к элементу L->next.
Так реализуется защита от изменения в f() указателя на следующий элемент.
*/


#endif