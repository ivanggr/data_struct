/* Bin_Search_Tree.h
Реализация структуры данных двоичное дерево поиска (Binary Search Tree, BST).
Хранимый тип данных: Value.
Объявлены операции:  INSERT, FIND, 
                     MAX, MIN, SUCCESSOR, PREDECESSOR,
                     ERASE, DELETE, SEPARATE,
                     TRAVERSE, PRINT
*/

#ifndef BIN_SEARCH_TREE_H
#define BIN_SEARCH_TREE_H

#include "Stored_Value.h"

/* структура двоичное дерево */
typedef struct Tree Tree;
struct Tree
{
	Tree *parent;  // родитель
	Tree *left;    // левый потомок с величиной, меньшей текущей величины
	Tree *right;   // правый потомок с величиной, большей текущей величины
	Value v;       // хранимая величина
};


/* Вставка узла с хранимой величиной v в T, родитель при этом - P */
Tree *TINSERT(Tree **T, Value *v, Tree *P);

/* Поиск узла по величине */
Tree *TFIND(Tree *T, Value *v);

/* Поиск узла с наибольшей величиной */
Tree *TMAX(Tree *T);

/* Поиск узла с наименьшей величиной */
Tree *TMIN(Tree *T);

/* Поиск узла в дереве и его потомках с величиной, следующей сразу после заданного узла. */
Tree *TSUCCESSOR(Tree *T);

/* Поиск узла в дереве и его потомках с величиной, предшествующей заданному узлу. */
Tree *TPREDECESSOR(Tree *T);

/* Удаление в дереве T узла с величиной v с последующим перемещением поддеревьев */
void TERASE(Tree *T, Value *v);

/* Рекурсивное удаление узла T и всех его потомков */
void TDELETE(Tree *T);

/* Отделение дерева от родителя путем обнуления указателей */
void TSEPARATE(Tree *T);

/* Рекурсивный обход дерева T с вызовом функции f() с передачей ей в качестве аргумента каждого узла дерева */
/* Порядок обхода order: 
 < 0  префиксный порядок: узел, левое поддерево, правое поддерево.  TTRAVERSE_PREFIX
== 0  инфиксный порядок: левое поддерево, узел, правое поддерево.   TTRAVERSE_INFIX
 > 0  постфиксный порядок: левое поддерево, правое поддерево, узел. TTRAVERSE_POSTFIX
*/
#define TTRAVERSE_PREFIX   (-1)
#define TTRAVERSE_INFIX     (0)
#define TTRAVERSE_POSTFIX  (+1)
void TTRAVERSE(Tree *T, void (*f) (Tree *), int order);

/* Печать узла и его потомков */
void TPRINT(Tree *T);



#endif