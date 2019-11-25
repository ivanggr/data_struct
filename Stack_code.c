/* Реализация стека, объявленого в Stack.h */
#include <stdlib.h>
#include "Stack.h"
#include "Stored_Value.h"

// Поместить в стек величину *v. Вернуть новый указатель sp; если < 0 стек полон или ошибка
int SPUSH(Stack *stack, Value *v)
{
	if (stack == NULL || v == NULL)
		return -1;
	// проверка на выход за пределы допустимой вершины стека
	if (stack->sp > stack->capacity)    
	{
		stack->sp = stack->capacity;
		return STACK_FULL;
	}
	else if (stack->sp == stack->capacity)
		return STACK_FULL;
	else
		VSET(&stack->sarray[stack->sp++], v);
	return stack->sp;
}

// Вернуть из стека в величину *v. Вернуть новый указатель sp; если < 0 стек пуст или ошибка
int SPOP(Stack *stack, Value *v)
{
	if (stack == NULL || v == NULL)
		return -1;
	if (stack->sp <= 0)  // проверка на выход ниже дна стека
		return STACK_EMPTY;
	else
	{
		// проверка на выход за пределы допустимой вершины стека
		if (stack->sp > stack->capacity)  
			stack->sp = stack->capacity;
		VSET(v, &stack->sarray[--stack->sp]);
		return stack->sp;
	}
}

// Проверка, заполнен ли стек. Если заполнен, то возвращает STACK_FULL, если нет ноль. 
// Если ошибка, то возврат меньше ноля
int SFULL(Stack *stack)
{
	if (stack == NULL)
		return -1;
	if (stack->capacity > 0 && stack->sarray != NULL && stack->sp == stack->capacity - 1)
		return STACK_FULL;
	else
		return 0;
}

// Инициализировать стек. Если он не пустой, удаляет содержимое. 
// Возвращает новый размер стека, если <= 0 - ошибка
int SINIT(Stack *stack, int capacity)
{
	// проверка аргументов
	if (stack == NULL)
		return -1;

	// очистка памяти, занятой массивом стека
	if (stack->sarray != NULL)
		free(stack->sarray);

	// выделение памяти под массив стека
	stack->sarray = (Value *) calloc((size_t) capacity, sizeof(Value));
	if (stack->sarray == NULL)
	{
		stack->capacity = 0;
		stack->sp = 0;		
		return 0;
	}

	// установка величин и возврат
	stack->capacity = capacity;
	stack->sp = 0;
	return stack->capacity;
}

/* Изменить размер стека, вернуть sp. 
Если ошибка, то возвращает число меньше ноля, изменение массива не меняется.
Если новый размер больше или равен предыдущему, то sp останется неизменным.
Если меньше и указатель вершины выше новой границы, то стек будет полон и sp == capacity - 1 */
int SRESIZE(Stack *stack, int newcapacity)
{
	if (stack == NULL || newcapacity <= 0)
		return -1;
	Value *newsarray;

	// просто выделение памяти, если указатель был пуст
	if (stack->sarray == NULL)
	{
		stack->sarray = (Value *) calloc((size_t) newcapacity, sizeof(Value));
		if (stack->sarray == NULL)
		{	
			stack->capacity = 0;
			stack->sp = 0;
			return -2;
		}
	}
	// перевыделение памяти, если указатель не был пуст
	else
	{
		newsarray = (Value *) realloc((void *) stack->sarray, newcapacity * sizeof(Value));
		if (newsarray == NULL)
			return -2;
		// копируется новый указатель на массив
		stack->sarray = newsarray;
	}

	// если ёмкость уменьшилась и указатель вершины превзашёл допустимую величину
	if (stack->sp > newcapacity)
		stack->sp = newcapacity;

	// установить ёмкость и вернуть указатель стека
	stack->capacity = newcapacity;
	return stack->sp;
}

// Удалить массив стека и сбросить переменные. Вернуть новую ёмкость == 0, иначе ошибка
int SCLEAR(Stack *stack)
{
	if (stack == NULL)
		return -1;
	if (stack->sarray != NULL)
	{
		free(stack->sarray);
		stack->sarray = NULL;
	}
	stack->sp = 0;
	stack->capacity = 0;
	return 0;
}

// Печать стека, возврат число пройденных элементов стека
int SPRINT(Stack *stack)
{
	if (stack == NULL)
		return -1;
	if (stack->sarray == NULL)
		return 0;
	int j;
	for (j = 0; j < stack->capacity; j++)
		VPRINT(&stack->sarray[j]);
	return j;
}