# C语言线性表之循环单链表 - Koma Hub - CSDN博客
2017年12月05日 19:19:58[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：159
```cpp
#include<stdio.h>
#include<stdlib.h>
int typeOfLinkList;
typedef struct LNode{
    int data;
    struct LNode *next;
}LNode, *LoopLinkList;
/* 基本算法（遍历） */
void show(LNode *L, int n)
{
    int in = 0;
    printf("  Show: ");
    do{
        in ++;
        printf("%d - ", L->data);
        L = L->next;
    }while(L != NULL&&in<n);
    printf("\n");
}
/* 基本算法（查找） 返回节点*/
LNode* find1(LoopLinkList L, int x)
{
    int index;
    do{
        if(x == L->data)
            return L;
        else
            L = L->next;
    }while(L != NULL);
    return NULL;
}
/* 基本算法（查找） 返回索引号*/
int find2(LoopLinkList L, int x)
{
    int index = 0;
    do{
        if(x == L->data)
            return index;
        else
            L = L->next;
        index ++;
    }while(L != NULL);
    return 9999;
}
/* 基本算法（插入） */
void insert(LoopLinkList L, int index, int x)
{
    printf("insert: ");
    int i = 0;
    for(i=0;i<index-1&&(L) != NULL; i++, L=(L)->next)//找到要插入的位置
    {
        printf("%d - ",(L)->data);
    }
    
    LNode *node = (LNode*)malloc(sizeof(LNode));//新建节点
    node->data = x;//赋值 
    node->next = (L)->next;//新节点next替换掉next
    (L)->next = node;//next重新指向新节点
    printf("\n");
}
/* 基本算法（删除） */
void Delete(LoopLinkList L, int index)
{
    int i;
    for(i=0;i<index-1&&L != NULL;i++,L=L->next);
    L->next=L->next->next;//隔过去一个节点
}
/* 基本算法（逆序建表） 循环单链表*/
LoopLinkList CreatLinkList1(int *a)
{
    LoopLinkList L = (LNode*)malloc(sizeof(LNode));//申请一个链表头（空头）
    L->data = a[0];//给这个表头赋值
	L->next = NULL;//next指向NULL
    int i = 1;//新建节点
    LNode *node;
    LNode *tail = (LNode*)malloc(sizeof(LNode));//标定一个表尾，后面指向表头；
    tail = L;
    while(a[i] != 0){
        node = (LNode*)malloc(sizeof(LNode));//给节点分配空间
        node->data = a[i];//给节点赋值
        node->next = L;//新节点替换掉表头
        L = node;//表头重新作为表头，指向新增表头节点
        node = NULL;
        i++;
    }
    tail->next = L;//表尾指向表头
    return L;
}
/* 基本算法（顺序建表） */
LoopLinkList CreatLinkList2(int *a)
{
    LoopLinkList L = (LNode*)malloc(sizeof(LNode));//申请一个链表头（空头）
	L->next = NULL;//next指向NULL
    LNode *p = L;//让p指向这个链表头
    
    p->data = a[0];//给这个表头赋值
    int i = 1;
    LNode *node;//新建节点
    while(a[i] != 0){
        node = (LNode*)malloc(sizeof(LNode));//给节点分配空间
        node->data = a[i];//给节点赋值
        node->next = NULL;//新节点指向空
        p->next = node;//表头（尾）指向新节点
        p = node;//表头（尾）指向新的表尾
        i++;
    }
    p->next = L;//表尾指向表头，
    return L;
}
int main(int argc, char*argv[])
{
    int a[17] = {1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
    LoopLinkList L = CreatLinkList2(a);
    show(L,17);
    printf("  find: %d\n",find1(L, 6)->data);
    printf("  find: %d\n",find2(L, 13));
    show(L,17);
    insert(L, 6, 15);
    show(L,17);
    Delete(L, 7);
    show(L,18);
	return 0;
}
```
运行结果：
```cpp
-bash-4.1$ gcc -o a Demo_04_LoopLinkList.c
-bash-4.1$ ./a
  Show: 1 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14 - 15 - 1 - 
  find: 6
  find: 13
  Show: 1 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14 - 15 - 1 - 
insert: 1 - 1 - 2 - 3 - 4 - 
  Show: 1 - 1 - 2 - 3 - 4 - 5 - 15 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14 - 15 - 
  Show: 1 - 1 - 2 - 3 - 4 - 5 - 15 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14 - 15 - 1 - 1 - 
-bash-4.1$
```
