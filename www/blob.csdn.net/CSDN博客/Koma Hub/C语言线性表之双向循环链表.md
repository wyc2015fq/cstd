# C语言线性表之双向循环链表 - Koma Hub - CSDN博客
2017年12月05日 19:21:42[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：138
```cpp
#include<stdio.h>
#include<stdlib.h>
/* 结构体：双向循环链表 */
typedef struct DuLNode{
    
    int data;
    struct DuLNode *prior, *next;//内部有两个指针：前、后
    
}DuLNode, *DuLinkList;
/* 创建双向链表 */
DuLinkList CreatDuLinkList(int *a)
{
    
    DuLinkList L = (DuLNode*)malloc(sizeof(DuLNode));//申请链表头空间
    L->prior = L;//prior指向自己
    L->data = a[0];//给数据体赋值
    L->next = L;//next指向自己
    
    DuLNode *p = L;//声明指针指向链表头
    
    int i = 1;
    DuLNode *node;//新建节点
    do{
        node = (DuLNode*)malloc(sizeof(DuLNode));//新建节点分配内存
        p->next = node;//表头指针的next指向新节点
        node->prior = p;//节点指针prior指向上一个节点
        node->data = a[i];//节点赋值
        L->prior = node;//表头指向表尾
        node->next = L;//表尾指向表头
        p = node;//表尾更新
        
        i++;//
    }while(a[i]!=0);
    
    return L;
}
/* 显示双向循环链表 */
void show(DuLinkList L, int flag)
{
    DuLNode *p;//声明一个指针
    p = L;//指针指向链表头
    if(flag){
        printf("show-prior: ");//如果顺序输出（next）
    }else{
        printf("show- next: ");//否则逆序输出（prior）
    }
    do{
        printf("%d - ",p->data);//输出节点数据
        if(flag){
            p = p->next;//指向下一个节点
        }else{
            p=p->prior;//指向上一个节点
        }
    }while(p != L);//如果又回到表头，则结束循环
    printf("\n");//
}
/* 在双向循环链表中插入一个节点 */
void insert(DuLinkList L, int index, int x)
{
    int i;
    for(i=0;i<index-1;i++){//先找到要插入节点的位置
        
        L = L->next;//
    }
    DuLNode *p = L->next;//指针指向下一个节点
    
    DuLNode *node = (DuLNode*)malloc(sizeof(DuLNode));//申请一个新节点内存
    node->data = x;//给节点数据域赋值
    L->next = node;//上个节点的next指向新节点
    node->prior = L;//新节点的prior指向上个节点
    
    node->next = p;//新节点next指向下一个节点
    p->prior = node;//上一个节点的prior指向新节点
}
/* 删除节点 */
void Delete(DuLinkList L, int index)
{
    int i;
    for(i=0;i<index-1;i++){//寻找到要删除的位置
        
        L = L->next;//
    }
    DuLNode *p = L->next->next;//跳过要删除的节点
    L->next = p;//重新建立连接
    p->prior = L;//重新建立连接
}
/*  */
int main(int argc, char*argv[])
{
    int a[10] = {1,2,3,4,5,6,7,8,9,0};
    
    DuLinkList L = CreatDuLinkList(a);
    
    show(L,1);
    show(L,0);
    insert(L,9,99);
    show(L,1);
    Delete(L,3);
    Delete(L,2);
    show(L,1);
    
	return 0;
}
```
运行结果：
```
-bash-4.1$ gcc -o a Demo_05_DuLinkList.c
-bash-4.1$ ./a
show-prior: 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 
show- next: 1 - 9 - 8 - 7 - 6 - 5 - 4 - 3 - 2 - 
show-prior: 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 99 - 
show-prior: 1 - 2 - 5 - 6 - 7 - 8 - 9 - 99 - 
-bash-4.1$
```
