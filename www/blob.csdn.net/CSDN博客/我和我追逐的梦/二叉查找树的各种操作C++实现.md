
# 二叉查找树的各种操作C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月30日 23:02:01[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1264标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[search																](https://so.csdn.net/so/search/s.do?q=search&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[insert																](https://so.csdn.net/so/search/s.do?q=insert&t=blog)[ini																](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=insert&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=insert&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=insert&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=search&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
// 链式二叉查找树的各种操作.cpp
\#include "stdafx.h"
\#include<iostream>
using namespace std;
struct BSTree
{
int data;
BSTree *left;
BSTree *right;
};
BSTree *T=NULL;
int a[100];
//插入排序二叉树
void insert(BSTree *b,BSTree *s)
{
if(b==NULL)
{
b=s;
}
else if(s->data==b->data)
return;
else if(s->data<b->data)
insert(b->left,s);
else
insert(b->right,s);
}
//创建排序二叉树
void createBSTree(BSTree *b,int *a,int n)
{
int i;
for(i=0;i<n;i++)
{
BSTree *s = (BSTree*)malloc(sizeof(BSTree));
s->data=a[i];
s->left=NULL;
s->right=NULL;
insert(b,s);
}
}
//查询排序二叉树
BSTree *search(BSTree *b,int x)
{
if(b==NULL)
return NULL;
else
{
if(b->data==x)
return b;
else if(b->data>x)
return search(b->left,x);
else
return search(b->right,x);
}
}
//前序遍历排序二叉树
void preOrder(BSTree *b)
{
if(b==NULL)
return;
else
{
cout<<b->data<<" ";
preOrder(b->left);
preOrder(b->right);
}
}
//中序遍历排序二叉树
void inOrder(BSTree *b)
{
if(b==NULL)
return ;
else
{
inOrder(b->left);
cout<<b->data<<" ";
inOrder(b->right);
}
}
//后序遍历排序二叉树
void postOrder(BSTree *b)
{
if(b==NULL)
return ;
else
{
postOrder(b->left);
postOrder(b->right);
cout<<b->data<<" ";
}
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入案例个数："<<endl;
cin>>cases;
while(cases--)
{
int n;
cout<<"请输入元素个数："<<endl;
cin>>n;
int i;
cout<<"请输入这些元素："<<endl;
for(i=0;i<n;i++)
cin>>a[i];
cout<<"建立二叉排序树："<<endl;
createBSTree(T,a,n);
cout<<"二叉排序树建立成功！"<<endl;
cout<<"请选择您要进行的操作：1.插入(I/i)、2.查找(S/s)、3.先序输出(P/p)、4.中序输出(M/m)、5.后序输出(L/l)"<<endl;
char s;
cin>>s;
if(s=='I'||s=='i')
{
cout<<"请输入您要插入的值："<<endl;
int x;
cin>>x;
BSTree *p =(BSTree*)malloc(sizeof(BSTree));
p->data = x;
p->left = NULL;
p->right = NULL;
insert(T,p);
cout<<"插入成功！"<<endl;
}
else if(s=='S'||s=='s')
{
cout<<"请输入您要查找的值："<<endl;
int x;
cin>>x;
BSTree *p=search(T,x);
cout<<"查找的值为："<<p->data<<endl;
if(p->left != NULL)
cout<<"其左儿子的值为："<<p->left->data<<endl;
if(p->right != NULL)
cout<<"其右儿子的值为："<<p->right->data<<endl;
}
else if(s=='P'||s=='p')
{
cout<<"其前序输出为："<<endl;
preOrder(T);
}
else if(s=='M'||s=='m')
{
cout<<"其中序输出为："<<endl;
inOrder(T);
}
else
{
cout<<"其后序输出为："<<endl;
postOrder(T);
}
cout<<endl;
}
system("pause");
return 0;
}

[
](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
