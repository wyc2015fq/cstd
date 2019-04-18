# 二叉树建立及中序遍历C++语言实现 - 江南烟雨 - CSDN博客
2010年08月20日 20:43:00[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5972
二叉树建立及中序遍历C++语言实现：
#include<stdio.h>
#include<stdlib.h>
typedef struct node{//二叉树结点数据结构定义；
	char data;
	struct node *lchild,*rchild;
}BTNode,*BTree;
void createBTree(BTree &T){
	char c;
	scanf("%c",&c);
	if(c=='#'){
		T=NULL;
	}
	else{
		BTree t;
		t=(BTree)malloc(sizeof(BTNode));
		t->data=c;
		t->lchild=NULL;
		t->rchild=NULL;
		T=t;
		createBTree(T->lchild);
		createBTree(T->rchild);
	}
}
void inOrderTra(BTree T){
	if(T!=NULL){
		inOrderTra(T->lchild);
		printf("%c ",T->data);
		inOrderTra(T->rchild);
	}
}
int main(){
	BTree T;
	printf("请输入二叉树的先序遍历序列：/n");
	createBTree(T);
	printf("中序遍历二叉树序列：/n");
	inOrderTra(T);
	printf("/n");
}
运行结果：
![](http://hi.csdn.net/attachment/201008/20/0_1282308172L0HY.gif)
![](http://hi.csdn.net/attachment/201103/14/0_1300064375zxBY.gif)
