/* Решение известной задачи о корректности постановки скобок () в строке 
 и вывода приоритета вычисления скобочек.

----------------------
Задача обеспечения правильного соотношения и вложенности открывающихся и закрывающихся скобок в строке.
Дана строка, в ней может быть последовательность скобок:
	((())())()
Необходимо осуществить проверку правильности порядка следования скобок и,
при необходимости, определить положение первой неправильной скобки.
Желательно построить список с последовательностью вычислений скобок, 
 где будут указаны интервалы символов для вычисления.

----------------------
Предварительные замечания к решению:
При проходе строки в каждой текущей позиции число пройденных левых скобок '(' 
должно быть больше или равно числу пройденных скобок ')'.
После окончания прохода общее число пройденных скобок '(' должно быть равно числу пройденных скобок ')'. 
Отсюда следует, что общее число скобок должно быть чётным.
*/

#include <stdlib.h>
#include <stdio.h>

#include "List.h"
#include "Stack.h"
#include "Stored_Value.h"

/* Проход по строке и проверка корректности положения скобок.
Запись списка приоритета вычислений. */
int brackets(char *s, List **R)
/* Параметры функции:
	*s    - строка текста, которая может содержать последовательность скобок
	**R   - указатель на указатель элемента двусвязного списка последовательности вычислений,
	если R == NULL, то строить список приоритетов не нужно.	
Возвращаемое значение:
	== 0  - последовательность скобок корректна
	 > 0  - последовательность скобок некорректна, (индекс + 1) некорректного элемента
	 < 0  - какая-то либо ошибка, кроме как в последовательности скобок
*/
{
	int   i;   // индекс текущего символа в s
	char  c;   // текущий символ
	int   B;   // количество незакрытых скобок '('
	List *L;   // список последовательности вычислений
	Value v;   // переменная для записи последовательности вычислений
	Stack stack; // стек для хранения левых скобок '('

	/* проверка аргумента */
	if (s == NULL)
		return -1;

	// Инициализация стека для хранения левых скобок '('
	stack.sarray = NULL;
	if (SINIT(&stack, 0xFFF) != 0xFFF)
		return -1;
	
	// зафиксируем нулевой элемент списка
	if (R != NULL)
		L = *R; 
	else
	// если R == NULL в список вносить не требуется 
		L = NULL; // Здесь нам L не нужна. Она проинициализирована, чтобы скомпилировался код

	// проход по списку
	for (B = 0, i = 0; (c=s[i]) != '\0'; i++)
	{
		// левая скобка
		if (c == '(')
		{
			B++;           // увеличиваем сумму незакрытых скобок '('
			v.c = '(';     // запоминаем индекс последних скобок '(' в стеке
			v.index = i;
			if (SPUSH(&stack, &v) < 0)
			{
				SCLEAR(&stack);
				return -1;
			}
		}

		// правая скобка 
		else if (c == ')')    
		{
			// если незакрытых скобок '(' нет, тогда в этом символе ошибка
			if (B == 0) 
			{
				SCLEAR(&stack);
				return (i + 1);			
			}
            // если незакрытые скобки '(' есть
			else      
			{
				B--;   // уменьшаем количество незакрытых скобок '(' на единицу
				if (SPOP(&stack, &v) < 0)  // извлекаем из стека величину '('
				{
					SCLEAR(&stack);
					return -1;
				}
				/* при R != NULL добавляем пару () индексами
					в список последовательности вычислений */
				if (R != NULL)
				{
					if (L == *R)
					{
						L = LINSERT(*R, &v);
						*R = L;
					}
					else
						L = LINSERT(L, &v);
					v.index = i;
					v.c = ')';
					L = LINSERT(L, &v);
				}
			}
		}

		// если не скобка, пропустить
		else
			continue;
	}

	// Строка пройдена.

	// Если есть незакрытые скобки '(' - ошибка, возращаем индекс+1 последней
	if (B > 0)
	{
		// стек со скобками должен быть не пуст
		i = SPOP(&stack, &v); 
		SCLEAR(&stack);
		if (i < 0)
			return i; // если были ошибки в извлечении скобок из стека, то вернем ошибку
		else
			return v.index + 1; // иначе вернем индекс+1 элемента
	}

	// в другом случае освободим стек и вернем B, если все хорошо, то B == 0
	else
	{
		SCLEAR(&stack);
		return B;
	}
}

/* Функция для ввода строки для функции brackets() и 
 последующий вывод результатов. */
int test_brackets()
{
	char s[5000];
	int r;

	// считать строку
	printf("Please enter the string:\n");
	if (gets(s) == NULL)
	{
		printf("Error in the entering the string.\n");
		return -1;
	}

	// выделение памяти под нулевой узел списка последовательности вычислений
	List **R = (List **) malloc(sizeof(List *)); 
	if (R != NULL)
		*R = NULL;
	else
		return -1;

	// разбор скобочек
	r = brackets(s, R);

	// вывод результатов разбора скобочек
	printf("\nThe brackets sequence is correct:\n");
	if (r == 0)
		printf("TRUE.\n");
	else if (r > 0)
		printf("FALSE.\nNon correct char index: %d.\n", r-1);
	else
		printf("UKNOWN ERROR.\n");

	// вывод списка вычислений и освобождение памяти
	printf("\nBrackets calc sequence:\n");
	LPRINT(*R);
	LDELETE_ALL(*R);
	free(R); 

	return 0;
}


int main()
{
	test_brackets();
	return (EXIT_SUCCESS);
}