
# [LeetCode] Reverse Linked List I II - 链表翻转问题 - 杨秀璋的专栏 - CSDN博客

2015年09月14日 19:09:56[Eastmount](https://me.csdn.net/Eastmount)阅读数：1390


**题目概述：**
Reverse a singly linked list.
翻转一个单链表，如：1->2 输出 2->1；1->2->3 输出3->2->1。
**题目解析：**
本人真的比较笨啊！首先想到的方法就是通过判断链尾是否存在，再新建一个链表，每次移动head的链尾元素，并删除head链表中的元素，一个字“蠢”，但好歹AC且巩固了链表基础知识。你可能遇见的错误包括：
1.'ListNode' undeclared (first use in this function)
nhead=(istNode*)malloc(sizeof(ListNode));
=>
nhead=(struct ListNode*)malloc(sizeof(struct ListNode));
2.Time Limit Exceeded
在链表遍历寻找最后一个结点并插入新链表尾部中需要注意，建议的方法：
q=head; while(q) {q=q->next;}
p=(struct ListNode*)malloc(sizeof(struct ListNode));
p->val=head->val; p->next=NULL; q=p;
=>
q=head; while(q) {last=q; q=q->next;}
p=(struct ListNode*)malloc(sizeof(struct ListNode));
p->val=head->val; p->next=NULL; last->next=p;
通过借助last变量更直观，否则结果总是错误。而且此时q为next指向NULL，如果用到q->next=p就会出现RE错误，因为q都为NULL，哪来的q->next。第二个错误也可能是我个人的编程习惯吧！
第二种方法更为推荐——直接翻转，还有一种递归方法自行提高。
如下图所示，红色表示初始链表存在4个值[1, 2, 3, 4]，蓝色表示初始指针first指向第一个元素、second指向第二个元素(head->next)，third指向第三个元素；首先s->next=f断开链表并翻转指向第一个元素，同时f=s最后返回first。如果只有两个元素[1, 2]则执行"s->next=f; f=s;"后s=t=NULL返回f即可输出[2, 1]。
![](https://img-blog.csdn.net/20150914190208210)

**我的代码：**
直接翻转方法
```python
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode *first,*second,*third;
    if(head==NULL||head->next==NULL)
        return head;
    first = head;
    second = head->next;
    first->next = NULL;
    while(second!=NULL) {  //注意while(second)不能执行
        third = second->next;
        second->next = first;
        first = second;
        second = third;
    }
    return first;
}
```
"蠢"方法
```python
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
 //个人思路:判断链尾是否存在 翻转到一个新链表
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode *nhead,*q,*p,*last,*nq,*np;
    int value;
    if(head==NULL||head->next==NULL)
        return head;
    q=head;
    nhead=NULL;     //创建新表头
    while(q->next) {
        //删除最后一个链尾结点
        p=q;
        while(p->next) {
            last=p;
            p=p->next;    
        }
        value=p->val;
        last->next=NULL;
        free(p);
        //插入行结点
        nq=nhead;
        while(nq) {
            last=nq;
            nq=nq->next;
        }
        if(nhead==NULL) { //创建表头
            np=(struct ListNode*)malloc(sizeof(struct ListNode));
            np->val=value;
            nhead=np;
            nhead->next=NULL;
        }
        else {           //插入结点
            np=(struct ListNode*)malloc(sizeof(struct ListNode));
            np->val=value;
            np->next=NULL;
            last->next=np;
        }
        //q结点循环前始终指向链表头
        q=head;
    }
    //最后一个结点及头结点head
    nq=nhead;
    while(nq) {
        last=nq;       //使用nq=np总是报错WR
        nq=nq->next;
    }
    np=(struct ListNode*)malloc(sizeof(struct ListNode));
    np->val=head->val;
    np->next=NULL;
    last->next=np;    //nq->next=np会报错RE 因为nq此时为next及null,而nq->next更不知道在哪
    return nhead;
}
```

(By:Eastmount 2015-9-14 晚上7点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


