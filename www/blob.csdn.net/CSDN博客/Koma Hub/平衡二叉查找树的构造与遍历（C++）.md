# 平衡二叉查找树的构造与遍历（C++） - Koma Hub - CSDN博客
2018年04月23日 00:00:56[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：83
首先平衡二叉树是建立在二叉查找树之上的二叉树，需要明白的概念是左旋和右旋，二者代码如下：
```cpp
void rotateLL(Node *&bbst)
/* LL型平衡调整：单向右旋平衡处理 */
{
	Node *lc;
	lc = bbst->lchild;
	bbst->lchild = lc->rchild;
	lc->rchild = bbst;
	bbst = lc;
}
void rotateRR(Node *&bbst)
/* RR型平衡调整：单向左旋平衡处理 */
{
	Node *rc;
	rc = bbst->rchild;
	bbst->rchild = rc->lchild;
	rc->lchild = bbst;
	bbst = rc;
}
```
用正数数组{3,2,5,1,4}建立一个二叉查找树，并进行右旋和左旋的处理，结果如下图：
![](https://img-blog.csdn.net/20180422235325309)
下面是我写的平衡二叉查找树的c++代码，参考了《数据结构》-彭波算法8-8~8-12，期间在书中的算法中找到错误（书中算法8-12在二叉树右子树插入节点后的switch判断语句的case EH中，LH显然需要改为RH，否则不能得出正确结果），给出我的算法：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;
#define LH 1 //左子树高
#define EH 0 //左右子树等高
#define RH -1//右子树高
/* 整形二叉树节点 */
typedef struct Node {
	int data;
	int bf; /* 平衡因子 */
	struct Node *lchild, *rchild;
} Node, *BBST;
void rotateLL(Node *&bbst)
/* LL型平衡调整：单向右旋平衡处理 */
{
	Node *lc;
	lc = bbst->lchild;
	bbst->lchild = lc->rchild;
	lc->rchild = bbst;
	bbst = lc;
}
void rotateRR(Node *&bbst)
/* RR型平衡调整：单向左旋平衡处理 */
{
	Node *rc;
	rc = bbst->rchild;
	bbst->rchild = rc->lchild;
	rc->lchild = bbst;
	bbst = rc;
}
void rotateLR(Node *&bbst)
/* LR型平衡调整：先左后右双向旋转平衡处理 */
{
	Node *lc, *rd;
	lc = bbst->lchild;//lc指向bbst的左子树
	switch(lc->bf) //bbst左子树进行平衡分析
	{
		case LH:  //新节点插入在bbst的左孩子左子树，则做LL处理
			bbst->bf = lc->bf = EH;
			rotateLL(bbst);
			break;
		case RH: //新节点插入在bbst的左孩子右子树，则做LR处理
			rd = lc->rchild;
			switch(rd->bf)//修改bbst和左孩子的平衡因子
			{
				case LH: 
					bbst->bf = RH; 
					lc->bf = EH;
					break;
				case EH: 
					bbst->bf = lc->bf = EH;
					break;
				case RH: 
					bbst->bf = EH; 
					lc->bf = LH;
					break;
			}
			rd->bf = EH;
			rotateRR(bbst->lchild);//对bbst的左子树作RR处理
			rotateLL(bbst);	//对bbst作LL处理
			break;
	}
}
void rotateRL(Node *&bbst)
/* RL型平衡调整：先右后左双向旋转平衡处理 */
{
	Node *rc, *ld;
	rc = bbst->rchild; //rc指向bbst的右子树的根节点
	switch(rc->bf) //检查bbst的右子树的平衡情况，并做处理
	{
		case RH:  //新节点插入在bbst的右孩子右子树，作RR处理
			bbst->bf = rc->bf = EH;
			rotateRR(bbst);
			break;
		case LH:   //新节点插入在bbst的右孩子左子树，作RL处理
			ld = rc->lchild; //令ld指向bbst的右孩子的左子树的根节点
			switch(ld->bf)  //修改bbst和右孩子的平衡因子
			{
				case LH: 
					bbst->bf = LH; 
					rc->bf = EH;
					break;
				case EH: 
					bbst->bf = rc->bf = EH;
					break;
				case RH: 
					bbst->bf = EH; 
					rc->bf = RH;
					break;
			}
			ld->bf = EH;
			rotateLL(bbst->rchild); //对bbst的右子树作LL处理
			rotateRR(bbst);  //对bbst作RR处理
			break;
	}
}
int insertBBSTNode(int data, Node *&bbst, int &high)
/* 插入节点Node */
{
	if(bbst == NULL)
	{
		bbst = new Node;//(Node*)malloc(sizeof(Node));
		bbst->data = data;
		bbst->lchild = NULL;
		bbst->rchild = NULL;
		bbst->bf = EH;
		high = 1;
	}
	else 
	{
		if(data == (bbst->data))
		{
			cout<<data<<"already exist.\n";
			high = 0;
			return 0;
		}
		else if(data < (bbst->data))
		{
			if(!insertBBSTNode(data, bbst->lchild, high))
			{
				return 0;  //不插入
			}
			if(high) //成功插入到左子树，且左子树变高了
			{
				switch(bbst->bf) 
				{
					case LH:   //插入前左子树比右子树高
						rotateLR(bbst); //左平衡处理
						high = 0;  //处理后左右子树等高
						break;
					case EH:   //插入前左子树右子树等高
						bbst->bf = LH; //插入后左子树增高
						high = 1; //树变高
						break;
					case RH:  //插入前右子树比左子树高
						bbst->bf = EH;//插入后左右子树等高
						high = 0;//树未增高
						break;
				}
			}
		}
		else
		{
			if(!insertBBSTNode(data, bbst->rchild, high))
			{
				return 0;
			}
			if(high)
			{
				switch(bbst->bf)
				{
					case LH:
						bbst->bf = EH;
						high = 0;
						break;
					case EH:
						bbst->bf = RH;
						high = 1;
						break;
					case RH:
						rotateRL(bbst);
						high = 0;
						break;
				}
			}
		}
	}
	return 1;
}
int insertArray2BBST(Node *&bbst, int *v, int num)
/* 向BST中插入数组 */
{
	int i=0, high = 0;
	while(i<num)
	{
		//cout<<v[i]<<endl;
		insertBBSTNode(v[i], bbst, high);
		i++;
	}
	return 1;
}
void Traverse(Node *bbst, int type)  
/* 1-先序遍历,2-中序遍历，3-后序遍历 */
{  
    if(bbst == NULL)  
        return;  
    else{ 
		switch(type)
		{
			case 1:
				cout<<bbst->data<<" - "; 
				Traverse(bbst->lchild,type);  
				Traverse(bbst->rchild,type);  
				break;
			case 2:
				Traverse(bbst->lchild,type);  
				cout<<bbst->data<<" - "; 
				Traverse(bbst->rchild,type);
				break;
			case 3:
				Traverse(bbst->lchild,type); 
				Traverse(bbst->rchild,type); 
				cout<<bbst->data<<" - "; 
				break;
			default:
				cout<<"please type = 1,2,3"<<endl;
				break;
		}
    }  
} 
int main( int argc, char **argv)
{
	int *v1, *v2;
	int n1 = 5, n2 = 1;
	int array1[] = {1,2,3,4,5};
	int array2[] = {1};
	v1 = array1;
	v2 = array2;
	
	Node *bbst = NULL;
	insertArray2BBST(bbst,v1,n1);
	insertArray2BBST(bbst,v2,n2);
	Traverse(bbst,1);cout<<endl;
	Traverse(bbst,2);cout<<endl;
	Traverse(bbst,3);cout<<endl;
	//rotateLL(bbst);
	//rotateLR(bbst);
	//Traverse(bbst,1);cout<<endl;
	//rotateRR(bbst);
	//Traverse(bbst,1);cout<<endl;
	
	return 1;
}
```
改程序的运行结果为：
```
D:\test>g++ BBST.cpp
D:\test>a.exe
1already exist.
2 - 1 - 4 - 3 - 5 -前序遍历
1 - 2 - 3 - 4 - 5 -中序遍历
1 - 3 - 5 - 4 - 2 -后序遍历
```
再次修改输入的字符串进行测试：
```cpp
int n1 = 13, n2 = 1;
int array1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
int array2[] = {1};
```
结果为：
```
D:\test>g++ BBST.cpp
D:\test>a.exe
1already exist.
8 - 4 - 2 - 1 - 3 - 6 - 5 - 7 - 10 - 9 - 12 - 11 - 13 -前序遍历
1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 -中序遍历
1 - 3 - 2 - 5 - 7 - 6 - 4 - 9 - 11 - 13 - 12 - 10 - 8 -后序遍历
```
