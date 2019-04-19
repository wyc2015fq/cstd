# 二叉查找树的构造与遍历（C++） - Koma Hub - CSDN博客
2018年01月18日 22:52:31[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：149
个人分类：[C/C++																[Algorithm](https://blog.csdn.net/Rong_Toa/article/category/7221428)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
```cpp
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
using namespace std;
typedef struct BSTNode{
    int key;
    struct BSTNode *left,*right;
}BSTNode,*BST;
int Insert(BSTNode *&bst, int data)
{
    if(bst == NULL){
        
        bst = (BSTNode*)malloc(sizeof(BSTNode));
        bst->key = data;
        bst->left = bst->right = NULL;
        return 1;
        
        
    }else if(data == bst->key){
        
        return 0;
        
    }else if(data < bst->key){
        
        Insert(bst->left,data);
        
    }else
        Insert(bst->right,data);
}
BSTNode *CreatBST(int *value, int len)
{
    BSTNode *bst = NULL;
    int i = 0;
    while(i<len)
    {
        if(Insert(bst, value[i])==1)
            i++;
    }
    return bst;
}
void prior(BSTNode *bst)
{
    if(bst == NULL)
        return;
    else{
        cout<<bst->key<<" - ";
        prior(bst->left);
        prior(bst->right);
    }
}
void mid(BSTNode *bst)
{
    if(bst == NULL)
        return;
    else{
        prior(bst->left);
        cout<<bst->key<<" - ";
        prior(bst->right);
    }
}
void after(BSTNode *bst)
{
    if(bst == NULL)
        return;
    else{
        prior(bst->left);
        prior(bst->right);
        cout<<bst->key<<" - ";
    }
}
void show(BSTNode *bst)
{
    cout<<"前序遍历:";
    prior(bst);
    cout<<endl;
    cout<<"中序遍历:";
    mid(bst);
    cout<<endl;
    cout<<"后序遍历:";
    after(bst);
    cout<<endl;
}
int main(int argc,char *argv[])
{
    int a[10] = {1,0,4,5,2,7,8,6,3,9};
    
    BSTNode *bst = CreatBST(a,10);
    show(bst);
}
```
**编译与运行：**
![](https://img-blog.csdn.net/20180118225139740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**序列{1,0,4,5,2,7,8,6,3,9}的二叉查找树如下，并用三种遍历结果验证算法准确性：**
![](https://img-blog.csdn.net/20180118224918131?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
再给另一个程序示例：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;
/** 
 *leetcode题库：4. 求两个排序数组的中位数
 *
 *  给定两个大小为 m 和 n 的有序数组 nums1 和 nums2 。
 *  请找出这两个有序数组的中位数。
 *  要求算法的时间复杂度为 O(log (m+n))。
 *
 *  示例 1: nums1 = [1, 3]   nums2 = [2]   中位数是 2.0
 *  示例 2: nums1 = [1, 2]   nums2 = [3, 4]   中位数是 (2 + 3)/2 = 2.5
 */
/* 整形二叉树节点 */
typedef struct Node {
	int data;
	struct Node *lchild, *rchild;
} Node, *BST;
int insertBSTNode(int data, Node *&bst)
/* 插入节点Node */
{
	if(bst == NULL)
	{
		bst = (Node*)malloc(sizeof(Node));
		bst->data = data;
		bst->lchild = NULL;
		bst->rchild = NULL;
		cout<<"----i"<<endl;
		return 1;
	}
	else 
	{
		if(data == (bst->data))
		{
			cout<<data<<"already exist.\n";
			return 0;
		}
		else if(data < (bst->data))
		{
			cout<<"----l"<<endl;
			insertBSTNode(data, bst->lchild);
		}
		else
		{
			cout<<"----r"<<endl;
			insertBSTNode(data, bst->rchild);
		}
	}
	return 1;
}
int insertArray2BST(Node *&bst, int *v, int num)
/* 向BST中插入数组 */
{
	int i=0;
	while(i<num)
	{
		cout<<v[i]<<endl;
		insertBSTNode(v[i],bst);
		i++;
	}
	return 1;
}
void Traverse(Node *bst, int type)  
/* 先序遍历 */
{  
    if(bst == NULL)  
        return;  
    else{ 
		switch(type)
		{
			case 1:
				cout<<bst->data<<" - "; 
				Traverse(bst->lchild,type);  
				Traverse(bst->rchild,type);  
				break;
			case 2:
				Traverse(bst->lchild,type);  
				cout<<bst->data<<" - "; 
				Traverse(bst->rchild,type);
				break;
			case 3:
				Traverse(bst->lchild,type); 
				Traverse(bst->rchild,type); 
				cout<<bst->data<<" - "; 
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
	int n1 = 6, n2 = 2;
	int array1[] = {5,3,7,1,9,4};
	int array2[] = {6,2};
	v1 = array1;
	v2 = array2;
	
	Node *bst = NULL;
	insertArray2BST(bst,v1,n1);
	insertArray2BST(bst,v2,n2);
	Traverse(bst,1);
	
	return 1;
}
```
结果：
`5 - 3 - 1 - 2 - 4 - 7 - 6 - 9 -`
