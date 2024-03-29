/* Хранимая структурой данных величина. 
Операции присвоения, сравнения, вывода  */

#ifndef STORED_VALUE_H
#define STORED_VALUE_H

#include <stdio.h>

// Хранимая структурой данных величин. 
// Её состав можно подкорректировать совместно с функциями, 
// так мы получим некое подобие обобщенного программирования.
// Манипуляции с величиной только косвенные, через указатели
typedef struct Value Value;
struct Value
{
	int index;
	char c;
};

void VSET(Value *d, Value *s);  // операция присвоения d = s
int  VCMP(Value *a, Value *b);  // операция сравнения. результаты: 0 (a == b), +1 (a > b), -1 (a < b)
int VPRINT(Value *);           // печать величины в стандартное устройство вывода
int VFPRINT(FILE *, Value *);  // печать величины в файл




#endif