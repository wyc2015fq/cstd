# 树形结构 AVL树 - PeterBishop - CSDN博客





2018年08月01日 23:20:30[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：263
个人分类：[数据结构与算法](https://blog.csdn.net/qq_40061421/article/category/7898328)









它是最先发明的自平衡二叉查找树，也被称为高度平衡树。相比于"二叉查找树"，它的特点是：AVL树中任何节点的两个子树的高度最大差别为1。有四种不平衡的情况会出现：**AVL树的插入，单旋转的第一种情况---右旋：**

![](https://images0.cnblogs.com/i/566545/201403/312132459386335.png)

由上图可知：在插入之前树是一颗AVL树，而插入之后结点T的左右子树高度差的绝对值不再 < 1,此时AVL树的平衡性被破坏，我们要对其进行旋转。由上图可知我们是在结点T的左结点的左子树上做了插入元素的操作，我们称这种情况为左左情况，我们应该进行右旋转(只需旋转一次，故是单旋转)。具体旋转步骤是：

T向右旋转成为L的右结点，同时，Y放到T的左孩子上。这样即可得到一颗新的AVL树，旋转过程图如下：

![](https://images0.cnblogs.com/i/566545/201403/311851389533418.png)



左左情况的右旋举例：

![](https://images0.cnblogs.com/i/566545/201403/311901354227875.png)

**AVL树的插入，单旋转的第一种情况---左旋：**

![](https://images0.cnblogs.com/i/566545/201403/312134280473397.png)

由上图可知：在插入之前树是一颗AVL树，而插入之后结点T的左右子树高度差的绝对值不再 < 1,此时AVL树的平衡性被破坏，我们要对其进行旋转。由上图可知我们是在结点T的右结点的右子树上做了插入元素的操作，我们称这种情况为右右情况，我们应该进行左旋转(只需旋转一次，故事单旋转)。具体旋转步骤是：

T向右旋转成为R的左结点，同时，Y放到T的左孩子上。这样即可得到一颗新的AVL树，旋转过程图如下：

![](https://images0.cnblogs.com/i/566545/201403/311917183918990.png)

右右情况的左旋举例：

![](https://images0.cnblogs.com/i/566545/201403/311932227973692.png)

以上就是插入操作时的单旋转情况！我们要注意的是：谁是T谁是L，谁是R还有谁是X,Y,Z!T始终是开始不平衡的左右子树的根节点。显然L是T的左结点，R是T的右节点。X、Y、Y是子树当然也可以为NULL.NULL归NULL，但不能破坏插入时我上面所说的左左情况或者右右情况。

**AVL树的插入，双旋转的第一种情况---左右(先左后右)旋：**

**![](https://images0.cnblogs.com/i/566545/201403/312206523286886.png)**



由上图可知，我们在T结点的左结点的右子树上插入一个元素时，会使得根为T的树的左右子树高度差的绝对值不再 < 1，如果只是进行简单的右旋，得到的树仍然是不平衡的。我们应该按照如下图所示进行二次旋转：

![](https://images0.cnblogs.com/i/566545/201403/312226224694371.png)

左右情况的左右旋转实例：

![](https://images0.cnblogs.com/i/566545/201403/312241527975291.png)

**AVL树的插入，双旋转的第二种情况---右左(先右后左)旋：**

**![](https://images0.cnblogs.com/i/566545/201403/312208352506363.png)**

由上图可知，我们在T结点的右结点的左子树上插入一个元素时，会使得根为T的树的左右子树高度差的绝对值不再 < 1，如果只是进行简单的左旋，得到的树仍然是不平衡的。我们应该按照如下图所示进行二次旋转：

![](https://images0.cnblogs.com/i/566545/201403/312152446726456.png)

右左情况的右左旋转实例：

![](https://images0.cnblogs.com/i/566545/201403/312151521573646.png)

```cpp
#include <iostream>

using namespace std;

#define DataType int

/**

定义AVL树的结构体，链式

**/

typedef struct AvlNode {

	DataType    data;

	AvlNode    * m_pLeft;

	AvlNode    * m_pRight;

	int height;

}AvlTree, Position, AvlNode;

//求两个数的最大值

int Max(int a, int b)

{

	return a>b ? a : b;

}

//求树的高度

int Height(AvlTree T)

{

	if (NULL == T)

		return -1;

	else

		return T->height;

}

//单旋转右旋

AvlTree singleRotateWithRight(AvlTree T)

{

	AvlTree L = T->m_pLeft;

	T->m_pLeft = L->m_pRight;

	L->m_pRight = T;

	T->height = Max(Height(T->m_pLeft), Height(T->m_pRight)) + 1;

	L->height = Max(Height(L->m_pLeft), Height(L->m_pRight)) + 1;

	return L;    //此时L成为根节点了（可参考AVL的插入的左左情况的右旋图）

}

//单旋转左旋

AvlTree singleRotateWithLeft(AvlTree T)

{

	AvlTree R = T->m_pRight;

	T->m_pRight = R->m_pLeft;

	R->m_pLeft = T;

	T->height = Max(Height(T->m_pLeft), Height(T->m_pRight)) + 1;

	R->height = Max(Height(R->m_pLeft), Height(R->m_pRight)) + 1;

	return R;    //此时R成为根节点了（可参考AVL的插入的左左情况的左旋图）

}

//双旋转，先左后右

AvlTree doubleRotateWithLeft(AvlTree T)        //先左后右

{

	T->m_pLeft = singleRotateWithLeft(T->m_pLeft);

	return singleRotateWithRight(T);

}

//双旋转，先右后左

AvlTree doubleRotateWithRight(AvlTree T)    //先右后左

{

	T->m_pRight = singleRotateWithRight(T->m_pRight);

	return singleRotateWithLeft(T);

}

AvlTree AvlTreeInsert(AvlTree T, DataType x)

{

	if (T == NULL)    //如果树为空

	{

		T = (AvlNode *)malloc(sizeof(struct AvlNode));

		if (T)

		{

			T->data = x;

			T->m_pLeft = NULL;

			T->m_pRight = NULL;

			T->height = 0;

		}

		else

		{

			cout << "空间不够" << endl;

			exit(0);

		}

	}

	else if (x < T->data)        //如果插入到T结点的左子树上

	{

		T->m_pLeft = AvlTreeInsert(T->m_pLeft, x);    //先插入，后旋转

		if (Height(T->m_pLeft) - Height(T->m_pRight) == 2) ///回溯两次到这里

		{

			if (x < T->m_pLeft->data)        //左左情况，只需要右旋转

			{

				T = singleRotateWithRight(T);

			}

			else                            //左右情况，双旋转,先左

			{

				T = doubleRotateWithLeft(T);

			}

		}

	}

	else if (x > T->data)

	{

		T->m_pRight = AvlTreeInsert(T->m_pRight, x);

		if (Height(T->m_pRight) - Height(T->m_pLeft) == 2)//回溯两次到这里

		{

			if (x > T->m_pRight->data)        //右右情况，进行左旋

			{

				T = singleRotateWithLeft(T);

			}

			else                            //左右情况，双旋转,先右

			{

				T = doubleRotateWithRight(T);

			}

		}

	}

	//else如果这个数已经存在，那么不进行插入

	T->height = Max(Height(T->m_pLeft), Height(T->m_pRight)) + 1;

	return T;

}

//递归实现中序遍历

void inOrderVisitUseRecur(const AvlTree pCurrent)

{

	if (pCurrent)

	{

		inOrderVisitUseRecur(pCurrent->m_pLeft);

		cout << pCurrent->data << " ";

		if (pCurrent->m_pLeft)

			cout << " leftChild: " << pCurrent->m_pLeft->data;

		else

			cout << " leftChild: " << "NULL";

		if (pCurrent->m_pRight)

			cout << " rightChild: " << pCurrent->m_pRight->data;

		else

			cout << " rightChild: " << "NULL";

		cout << endl;

		inOrderVisitUseRecur(pCurrent->m_pRight);

	}

}
```





