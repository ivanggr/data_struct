/* Реализация двоичного дерева поиска, объявленного в Bin_Search_Tree.h */
#include <stdlib.h>
#include <stdio.h>

#include "Bin_Search_Tree.h"
#include "Stored_Value.h"

/* ----- статические функции ----- */
/* функция освобождения памяти одного узла  */
static inline void TFREE(Tree *T)
{
	if (T != NULL)
		free((void *) T);
	return;
}

/* печать одного узла */
static inline void TPRNODE(Tree *T)
{
	if (T != NULL)
		VPRINT(&(T->v));
	return;
}
/* ----- конец статических функций ----- */

/* вставка узла с хранимой величиной v в T, родитель при этом - P */
Tree *TINSERT(Tree **T, Value *v, Tree *P)
/* Вставка узла c записью по указателю T. 
Параметры:
   **T      -  место, куда будет записываться дерево
   v        -  величина
   *P       -  указатель на родитель
возвращаемое значение:
   == NULL  -  операция вставки неуспешна
   != NULL  -  операция вставки успешна
*/
{
	int r;
	/* если место для хранения узла дерева не выделено и узел, таким образом, пуст */
	if (*T == NULL)
	{
		*T = (Tree *) malloc(sizeof(Tree));
		if ((*T) == NULL)
			return NULL;
		VSET(&(*T)->v, v);
		(*T)->left = (*T)->right = NULL;
		(*T)->parent = P;
		return (*T);
	}
	/* если узел есть */
	else
	{
		// сравнение записываемой и текущей величины
		r = VCMP(v, &((*T)->v));
		/* если они равны, то величина не записывается */
		if (r == 0)
			return NULL;
		/* если записываемая величина меньше текущей величины, то она вставляется в левое поддерево */
		else if (r < 0)
			TINSERT(&((*T)->left), v, *T);
		/* если записываемая величина больше текущей величины, то она вставляется в правое поддерево */
		else
			TINSERT(&((*T)->right), v, *T);
	}
	return NULL;
}

/* поиск узла по величине */
Tree *TFIND(Tree *T, Value *v)
{
	int r;
	if (T == NULL || v == NULL)
		return NULL;
	// сравнение
	r = VCMP(v, &(T->v));  
	if (r == 0)
		return T;  // если равны, мы наши
	else if (r < 0)
		return TFIND(T->left, v);   // если меньшей текущей, идём влево
	else
		return TFIND(T->right, v);  // если больше текущей, идём вправо
}

/* поиск узла с наибольшей величиной */
Tree *TMAX(Tree *T)
{
	// идём вправо, пока указатель на правый потомок не нулевой
	Tree *max;
	if (T == NULL)
		return NULL;
	max = T;
	while (max->right != NULL)
		max = max->right;
	return max;
}

/* поиск узла с наименьшей величиной */
Tree *TMIN(Tree *T)
{
	// идём влево, пока указатель на левый потомок не нулевой
	Tree *min;
	if (T == NULL)
		return NULL;
	min = T;
	while (min->left != NULL)
		min = min->left;
	return min;
}

/* Поиск узла в дереве и его потомках с величиной, следующей сразу после заданного узла. */
Tree *TSUCCESSOR(Tree *T)
{
	if (T == NULL)
		return NULL;
	// если у узла есть правый потомок, то наименьший в его ветви
	if (T->right != NULL)
		return TMIN(T->right);
	// если у узла нет правого потомка, то идем вверх по родителям, 
	// пока текущий узел не будет их левым потомком и возвращаем родителя
	while (T->parent != NULL)
	{
		if (T->parent->left == T)
			break;
		else
			T = T->parent;
	}
	return T->parent;
}

/* Поиск узла в дереве и его потомках с величиной, предшествующей заданному узлу. */
Tree *TPREDECESSOR(Tree *T)
{
	if (T == NULL)
		return NULL;
	// если у узла есть левый потомок, то наибольший в его ветви
	if (T->left != NULL)
		return TMAX(T->left);
	// если у узла нет левого потомка, то идем вверх по родителям, 
	// пока текущий узел не будет их правым потомком и возвращаем родителя
	while (T->parent != NULL)
	{
		if (T->parent->right == T)
			break;
		else
			T = T->parent;
	}
	return T->parent;
}

/* удаление в дереве T узла с величиной v с последующим перемещением поддеревьев */
void TERASE(Tree *T, Value *v)
{
	Tree *TR;
	int r;

	if (T == NULL)
		return;

	/* сравнение ключей */
	r = VCMP(v, &(T->v));
	if (r < 0)
		TERASE(T->left, v);   // удаление в левом поддереве
	else if (r > 0)
		TERASE(T->right, v);  // удаление в правом поддереве

	/* если ключи равны, мы нашли узел для удаления */
	else
	{
		/* Случай первый. Если у удаляемого узла нет потомков. */
		if (T->left == NULL && T->right == NULL)
		{
			TSEPARATE(T);  // обнуление указатель на удаляемый узел у его родителя
			TFREE(T);      // освобождение памяти, занимаемой удаляемым узлом
			return;
		}

		/* Случай второй. Если у удаляемого узла один потомок */
		else if ((T->left != NULL && T->right == NULL) || (T->left == NULL && T->right != NULL))
		{
			// переносим данные потомка в искомый узел, а затем удаляем потомка
			// определим, кто потомок
			TR = T->left != NULL ? T->left : T->right; // TR - потомок удаляемого узла

			// а потом заменим хранимую величину и указатели на указатели потомка
			VSET(&(T->v), &(TR->v)); 
			T->left = TR->left;
			T->right = TR->right;

			// родитель поддеревьев потомков теперь будет T
			if (TR->left != NULL) TR->left->parent = T;
			if (TR->right != NULL) TR->right->parent = T;
			// у родителя узла Т менять указатель не требуется

			// освобождение памяти, занимаемой потомком
			TFREE(TR);
			return;
		}

		/* Случай третий. Если у удаляемого узла два потомка.
		/* взамен удаляемого узла необходимо поставить самое левое поддерево правого потомка */
		else
		{
			/* если левое поддерево у правого потомка отсутствует */
			if (T->right->left == NULL)
			{
				TR = T->right;           // TR правый потомок
		        VSET(&(T->v), &(TR->v)); // копируем хранимую величину
				T->left = TR->left;      // NULL
				T->right = TR->right;
				if (TR->right != NULL) TR->right->parent = T; // у правого поддерева потомка меняем указатель на родителя
				// освобождение памяти, занимаемой потомком
				TFREE(TR);
				return;
			}
			/* если всё же есть левое поддерево у правого потомка */
			else
			{
			    // ищем самое левое поддерево TR
				TR = TMIN(T->right->left);
				// копируем хранимую величину в Т
			    VSET(&(T->v), &(TR->v));
				// рекурсивное удаление самого левого элемента
				TERASE(TR, &(TR->v));
				return;
			}
		}	
	}
	return;
}

/* рекурсивное удаление узла T и всех его потомков */
void TDELETE(Tree *T)
{
	if (T != NULL)
	{
		/* обнуляем указатель у родителя узла */
		TSEPARATE(T);
		/* освобождаем память, занимаемую узлом и его поддеревьями путём обхода */
		TTRAVERSE(T, &TFREE, TTRAVERSE_POSTFIX);
		return;
	}
	return;
}

/* отделение дерева от родителя путем обнуления указателей */
void TSEPARATE(Tree *T)
{
	if (T != NULL)
	{
		/* если есть указатель на родителя */		
		if (T->parent != NULL)	
		{
			/* обнуление указателя у родителя */
			if (T->parent->left == T)
				T->parent->left = NULL;
			else
				T->parent->right = NULL;
			/* обнуление указателя на родителя */
			T->parent = NULL;
		}
	}
	return;
}

/* Рекурсивный обход дерева T с вызовом функции f() с передачей ей в качестве аргумента каждого узла дерева */
/* Порядок обхода order: 
 < 0  префиксный порядок: узел, левое поддерево, правое поддерево.
== 0  инфиксный порядок: левое поддерево, узел, правое поддерево.
 > 0  постфиксный порядок: левое поддерево, правое поддерево, узел.
*/
void TTRAVERSE(Tree *T, void (*f) (Tree *), int order)
{
	if (T != NULL && f != NULL)
	{
		if (order < 0)
		{
			f(T);
			TTRAVERSE(T->left, f, order);
			TTRAVERSE(T->right, f, order);
		}
		else if (order == 0)
		{
			TTRAVERSE(T->left, f, order);
			f(T);
			TTRAVERSE(T->right, f, order);
		}
		else
		{
			TTRAVERSE(T->left, f, order);
			TTRAVERSE(T->right, f, order);
			f(T);
		}	
	}
	return;
}

/* Печать узла и его потомков */
void TPRINT(Tree *T)
{
	TTRAVERSE(T, &TPRNODE, TTRAVERSE_INFIX);
	return;
}