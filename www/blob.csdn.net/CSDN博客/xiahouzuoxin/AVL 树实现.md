# AVL 树实现 - xiahouzuoxin - CSDN博客





2012年11月12日 19:27:41[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2625








        AVL树是一种带平衡条件的查找树。一棵AVL树是其每个节点的左子树和右子树的高度最多差1的二叉查找树。前一篇文章给出了二叉树查找树的基本实现，AVL树的大部分实现都与普通的二叉查找树相同。

        二叉查找树的实现：http://blog.csdn.net/xiahouzuoxin/article/details/8159320


        参考《数据结构与算法分析——C语言描述》。


avlTree.h



```cpp
/*
 * =====================================================================================
 *
 *       Filename:  avlTree.h
 *
 *    Description:  AVL tree implemention
 *
 *        Version:  1.0
 *        Created:  11/09/2012 08:44:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xhzuoxin (), xiahouzuoxin@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _AVLTREE_H
#define _AVLTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

typedef UINT16 EleType;
struct AvlNode
{
	EleType ele;
	struct AvlNode* left;
	struct AvlNode* right;
	UINT16 height;
};
typedef struct AvlNode* Position;
typedef struct AvlNode* AvlTree;

extern AvlTree MakeEmpty(AvlTree T);
extern Position Find(EleType X, AvlTree T);
extern Position FindMax(AvlTree T);
extern Position FindMin(AvlTree T);
extern AvlTree Insert(EleType X, AvlTree T);
extern AvlTree Delete(EleType X, AvlTree T);

#endif
```



avlTree.c


```cpp
/*
 * =====================================================================================
 *
 *       Filename:  avlTree.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/09/2012 08:51:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xhzuoxin (), xiahouzuoxin@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "avlTree.h"


#define MAX(A, B)           ((A > B) ? (A) : (B))
#define MIN(A, B)           ((A < B) ? (A) : (B))
/*-----------------------------------------------------------------------------
 *  local functions
 *-----------------------------------------------------------------------------*/
static int Height(Position P);
static Position SRL(Position K2);
static Position DRL(Position K3);
static Position SRR(Position K2);
static Position DRR(Position K3);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  MakeEmpty
 *  Description:  Init AvlTree as NULL
 * =====================================================================================
 */
AvlTree MakeEmpty(AvlTree T)
{
	if(T != NULL)
	{
		MakeEmpty(T->left);
		MakeEmpty(T->right);
		free(T);
	}

	return T;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Find
 *  Description:  Find element X from AvlTree, return pointer of the element
 * =====================================================================================
 */
Position Find(EleType X, AvlTree T)
{
	if(T == NULL)
	{
		return NULL;
	}
	else if(X < T->ele)
	{
		return Find(X, T->left);
	}
	else if(X > T->ele)
	{
		return Find(X, T->right);
	}
	else
	{
		return T;
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  FindMax
 *  Description:  Find max element's pointer in a AvlTree
 * =====================================================================================
 */
Position FindMax(AvlTree T)
{
	if(T == NULL)
	{
		return NULL;
	}
	else if(T->right == NULL)
	{
		return T;
	}
	else
	{
		return FindMax(T->right);
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  FindMin
 *  Description:  Find min element's pointer in a AvlTree
 * =====================================================================================
 */
Position FindMin(AvlTree T)
{
	if(T == NULL)
	{
		return NULL;
	}
	else if(T->left == NULL)
	{
		return T;
	}
	else 
	{
		return FindMin(T->left);
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Insert
 *  Description:  Insert a element to the AvlTree, be sure to keep the banlance of tree
 * =====================================================================================
 */
AvlTree Insert(EleType X, AvlTree T)
{
	if(T == NULL)
	{
		/* create and return a one-node tree */
		T = (AvlTree)malloc(sizeof(struct AvlNode));
		if(T == NULL)
		{
			printf("error:alloc space failure!\n");
			return NULL;
		}
		T->ele = X;
		T->height = 0;
		T->left = T->right = NULL;
	}
	else if(X < T->ele)
	{
		T->left = Insert(X, T->left);
		if((Height(T->left) - Height(T->right)) >= 2)
		{
			if(X < T->left->ele)
			{
				T = SRL(T);
			}
			else
			{
				T = DRL(T);
			}
		}
	
	}	
	else if(X > T->ele)
	{
		T->right = Insert(X, T->right);
		if((Height(T->right) - Height(T->left)) >= 2)
		{
			if(X > T->right->ele)
			{
				T = SRR(T);
			}
			else
			{
				T = DRR(T);
			}
		}
	}
	else
	{
		// do nothing for the Element is already in the tree
	}

	T->height = MAX(Height(T->left), Height(T->right)) + 1;

	return T;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Height
 *  Description:  reurn height of position P
 * =====================================================================================
 */
static int Height(Position P)
{
	if(P == NULL)
	{
		return -1;
	}
	else
	{
		return P->height;
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SRL
 *  Description:  Single Rotate with Left
 *  K2 - the node that not keep balance
 *  K1 - new root repalce K2
 * =====================================================================================
 */
static Position SRL(Position K2)
{
	Position K1 = NULL;

	K1 = K2->left;
	K2->left = K1->right;
	K1->right = K2;

	K2->height = MAX(Height(K2->left), Height(K2->right)) + 1;
	K1->height = MAX(Height(K1->left), Height(K2)) + 1;

	return K1;  /* new root */
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SRR
 *  Description:  Single Rotate With Right
 *  K2 - the node that not keep balance after insert
 *  K1 - return new root replace K2
 * =====================================================================================
 */
static Position SRR(Position K2)
{
	Position K1 = NULL;

	K1 = K2->right;
	K2->right = K1->left;
	K1->left = K2;

	K2->height = MAX(Height(K2->left), Height(K2->right)) + 1;
	K1->height = MAX(Height(K2), Height(K1->right)) + 1;

	return K1;  /* new root */
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DRL
 *  Description:  This function can be called only if K3 has a left child 
 *  and K3's left child has a right child.
 *  Do the left-right double rotation.
 *  Update heights, then return new root.
 * =====================================================================================
 */
static Position DRL(Position K3)
{
#if 0
	/* Rotate between K1 and K2 */
	K3->left = SRR(K3->left);

	/* Rotate between K3 and K2 */
	return SRL(K3);
#else
	Position K1 = NULL;
	Position K2 = NULL;

	K1 = K3->left;
	K2 = K1->right;
	K1->right = K2->left;
	K2->left = K1;
	K3->left = K2->right;
	K2->right = K3;

	return K2;
#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DRL
 *  Description:  This function can be called only if K3 has a right child 
 *  and K3's right child has a left child.
 *  Do the left-right double rotation.
 *  Update heights, then return new root.
 * =====================================================================================
 */
static Position DRR(Position K3)
{
#if 0
	/* Rotate between K1 and K2 */
	K3->right = SRL(K3->right);

	/* Rotate between K3 and K2 */
	return SRR(K3);
#else
	Position K1 = NULL;
	Position K2 = NULL;

	K1 = K3->right;
	K2 = K1->left;
	K1->left = K2->right;
	K2->right = K1;
	K3->right = K2->left;
	K2->left = K3;

	return K2;
#endif
}
```



test.c





```cpp
/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  test avlTree file
 *
 *        Version:  1.0
 *        Created:  11/12/2012 06:37:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xhzuoxin (), xiahouzuoxin@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "avlTree.h"

void PrintTree(AvlTree T)
{
	if(T != NULL)
	{
		PrintTree(T->left);
		printf("h=%d, e=%d\n", T->height, T->ele);
		PrintTree(T->right);
	}
}

int main(void)
{
	AvlTree T = NULL;

	T = MakeEmpty(T);
	T = Insert(3, T);
	T = Insert(2, T);
	T = Insert(1, T);
	T = Insert(4, T);
	T = Insert(5, T);
	T = Insert(6, T);
	T = Insert(7, T);
	T = Insert(16, T);
	T = Insert(15, T);
	T = Insert(14, T);
	T = Insert(13, T);
	T = Insert(12, T);
	T = Insert(11, T);
	T = Insert(10, T);
	T = Insert(8, T);
	T = Insert(9, T);

	PrintTree(T);

	return 0;
}
```



gcc编译所使用的makefile文件如下：



```
SRC=avlTree.c avlTree.h test.c

all:$(SRC)
	gcc -g -Wall $^ -o $@

.PHONY:clean tags run
clean:
	rm *.o all *~
tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q
run:
	./all
```
最终输出结果（h表示节点高度，e表示节点元素值）：



h=0, e=1

h=1, e=2

h=0, e=3

h=2, e=4

h=0, e=5

h=1, e=6

h=4, e=7

h=0, e=8

h=1, e=9

h=0, e=10

h=2, e=11

h=0, e=12

h=3, e=13

h=0, e=14

h=1, e=15

h=0, e=16



