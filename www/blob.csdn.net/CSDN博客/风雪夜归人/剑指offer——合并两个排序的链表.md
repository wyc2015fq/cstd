# 剑指offer——合并两个排序的链表 - 风雪夜归人 - CSDN博客
2018年05月03日 21:56:18[cdjccio](https://me.csdn.net/qq_34624951)阅读数：227
Q：输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
A：选择第一个节点元素值较小的链表为基础链表，通过不断比较两个链表的元素值，将另一个链表中的节点链接到基础链表中。
```cpp
/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/
class Solution {
public:
    ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
    {
        if(pHead1 == NULL && pHead2 == NULL)
        {
            return NULL;
        }
        if(pHead1 != NULL && pHead2 == NULL)
        {
            return pHead1;
        }
        if(pHead1 == NULL && pHead2 != NULL)
        {
            return pHead2;
        }
        
        // ListNode* tmp = new ListNode(0);
        ListNode* p1 = pHead1;
        ListNode* q1 = pHead1;
        ListNode* p2 = pHead2;
        ListNode* q2 = pHead2;
        if(pHead1->val <= pHead2->val)
        {
            while(p1 && p2)
            {
                q2 = p2;
                q1 = p1;
                p2 = p2->next;
                p1 = p1->next;
                q2->next = q1->next;
                q1->next = q2;
            }
            return pHead1;
        }
        else
        {
            while(p1 && p2)
            {
                q1 = p1;
                q2 = p2;
                p1 = p1->next;
                p2 = p2->next;
                q1->next = q2->next;
                q2->next = q1;
            }
            return pHead2;
        }
    }
};
```
参考方法1：
```cpp
#include<iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
using namespace std;
typedef struct ListNode{
    int value;
    struct ListNode* next;
}ListNode;
//打印链表
void OutPut(ListNode*head){
    if(head == NULL){
        printf("NULL\n");
    }
    else{
        ListNode *p;
        p = head->next;
        while(p != NULL){
            //如果是最一个
            if(p->next == NULL){
                printf("%d\n",p->value);
            }
            else{
                printf("%d ",p->value);
            }
            p = p->next;
        }
    }
}
//创建链表
ListNode* CreateList(ListNode *head,int n){
    ListNode *newNode,*p;
    p = head;
    for(int i = 0;i < n;i++){
        newNode = (ListNode*)malloc(sizeof(ListNode));
        scanf("%d",&newNode->value);
        newNode->next = NULL;
        p->next = newNode;
        p = newNode;
    }
    return head;
}
ListNode* Merge(ListNode*head1,ListNode*head2){
    //head1,head2带有头结点
    if(head1->next == NULL && head2->next == NULL){
        return NULL;
    }
    //只有第一个字符串，无需合并，直接输出
    else if(head2->next == NULL){
        return head1;
    }
    //只有第二个字符串，无需合并，直接输出
    else if(head1->next == NULL){
        return head2;
    }
    //合并
    else{
        ListNode *p1,*p2,*p3,*head;
        head = (ListNode*)malloc(sizeof(ListNode));
        head->next = NULL;
        p1 = head1->next;
        p2 = head2->next;
        p3 = head;
        while(p1 != NULL && p2 != NULL){
            if(p1->value < p2->value){
                p3->next = p1;
                p1 = p1->next;
            }
            else{
                p3->next = p2;
                p2 = p2->next;
            }
            p3 = p3->next;
        }
        //head1没有遍历完
        while(p1 != NULL){
            p3->next = p1;
            p1 = p1->next;
            p3 = p3->next;
        }
        //head2没有遍历完
        while(p2 != NULL){
            p3->next = p2;
            p2 = p2->next;
            p3 = p3->next;
        }
        return head;
    }
}
int main() {
    int i,n,m;
    while(scanf("%d %d",&n,&m) != EOF){
        ListNode *head1,*head2;
        head1 = (ListNode*)malloc(sizeof(ListNode));
        head2 = (ListNode*)malloc(sizeof(ListNode));
        head1->next = NULL;
        head2->next = NULL;
        //创建链表
        if(n != 0){
            head1 = CreateList(head1,n);
        }
        if(m != 0){
            head2 = CreateList(head2,m);
        }
        //合并排序
        head1 = Merge(head1,head2);
        //打印链表
        if(head1 == NULL){
            printf("NULL\n");
        }
        else{
            OutPut(head1);
        }
    }//while
    return 0;
}
```
参考方法2：
![](https://img-blog.csdn.net/20180503220436942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM0NjI0OTUx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180503220458282?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM0NjI0OTUx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```cpp
#include<iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
using namespace std;
typedef struct ListNode{
    int value;
    struct ListNode* next;
}ListNode;
//打印链表
void OutPut(ListNode*head){
    if(head == NULL){
        printf("NULL\n");
    }
    else{
        ListNode *p;
        p = head;
        while(p != NULL){
            //如果是最一个
            if(p->next == NULL){
                printf("%d\n",p->value);
            }
            else{
                printf("%d ",p->value);
            }
            p = p->next;
        }
    }
}
//创建链表
ListNode* CreateList(ListNode *head,int n){
    ListNode *newNode,*p;
    p = head;
    for(int i = 0;i < n;i++){
        newNode = (ListNode*)malloc(sizeof(ListNode));
        scanf("%d",&newNode->value);
        newNode->next = NULL;
        p->next = newNode;
        p = newNode;
    }
    return head;
}
//递归
ListNode* Merge(ListNode*head1,ListNode*head2){
    if(head1 == NULL && head2 == NULL){
        return NULL;
    }
    else if(head2 == NULL){
        return head1;
    }
    else if(head1 == NULL){
        return head2;
    }
    //合并
    ListNode *head = NULL;
    if(head1->value < head2->value){
        head = head1;
        head->next = Merge(head1->next,head2);
    }
    else{
        head = head2;
        head->next = Merge(head1,head2->next);
    }
    return head;
}
int main() {
    int i,n,m;
    while(scanf("%d %d",&n,&m) != EOF){
        ListNode *head1,*head2;
        head1 = (ListNode*)malloc(sizeof(ListNode));
        head2 = (ListNode*)malloc(sizeof(ListNode));
        head1->next = NULL;
        head2->next = NULL;
        //创建链表
        if(n != 0){
            head1 = CreateList(head1,n);
        }
        if(m != 0){
            head2 = CreateList(head2,m);
        }
        //合并排序
        head1 = Merge(head1->next,head2->next);
        //打印链表
        if(head1 == NULL){
            printf("NULL\n");
        }
        else{
            OutPut(head1);
        }
    }//while
    return 0;
}
```
