# C++使用迭代和递归两种方式实现链表逆序算法 - fanyun的博客 - CSDN博客
2018年10月30日 07:20:55[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：38标签：[链表逆序](https://so.csdn.net/so/search/s.do?q=链表逆序&t=blog)
个人分类：[C++																[算法](https://blog.csdn.net/fanyun_01/article/category/7113106)](https://blog.csdn.net/fanyun_01/article/category/6102444)
1.链表逆序的两种算法
      C++实现一个链表逆序算法
2.链表逆序算法实现原理
如A->B->C->D->E，一般会有以下两种思路，如下
思路1：
先取出链表的最后一个E，然后将E作为新链表的头，
现在状态为
原始链表：A->B->C->D
新链表：E
再取出原来链表的最后一个D，然后将D插入到新链表的最后位置，
现在状态为
原始链表：A->B->C
新链表：E->D
依次类推，最后形成E->D->C->B->A
可以看出来，这个算法的复杂度为O(n2)。
思路二：
取出原始链表的第一个节点A，然后将该节点作为新链表的头节点。
现在状态为
原始链表：B->C->D->E
新链表：A
然后同上，变为了下面的状态
原始链表：C->D->E
新链表： B->A
原始链表：D->E
新链表： C->B->A
原始链表：E
新链表： D->C->B->A
原始链表：
新链表： E->D->C->B->A
这个只需要对原始链表遍历一次，就完成了这个工作，所以这个算法的复杂度为O(n)。
        通过对上面状态的变化分析，只要我们知道原始链表和新链表的头节点，我们就可以从原始链表取出第一个节点，然后将节点插入到新链表的第一个位置，由于两个链表的头结点现在都已经变化，所以我们不能丢失新头节点的地址。所以，我们要设置两个变量分别记录两个链表的头结点。下面程序中的old_head和 new_head分别表示原始链表的头节点和新链表的头节点。
```cpp
// ListReverse.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdio.h>
#include <malloc.h>
typedef struct Node {
	int data;
	struct Node* next;
}Node;
#define LIST_LEN     10     //链表长度
Node * List = NULL;          //链表
//迭代方式
void reverse_ite(Node * list) {
	Node * old_head = NULL;         //原来链表的头
	Node * new_head = NULL;      //新链表的头
	Node * cur = list;      //获得原来链表的头
							//每次将原来链表的头取出，并插入到新链表中，并且是新链表的头
	while (cur != NULL) {
		old_head = cur->next;      //将原来链表的头取出，并将第二个节点作为头节点
		cur->next = new_head;   //将取出的头设为新链表的头
		new_head = cur;      //新链表的头就是目前新链表的头
		cur = old_head;        //接着处理
	}
	List = new_head;
}
//递归方式
void reverse_recursive(Node * old_head, Node * new_head) {
	if (old_head == NULL) {
		List = new_head;
		return;
	}
	Node * tmp = old_head->next;        //将原来链表的头取出，并将第二个节点作为原来链表的头节点用于下一层递归
	old_head->next = new_head; //将取出的头设为新链表的头
	reverse_recursive(tmp, old_head);         //接着处理
}
//生成链表
void make_list() {
	List = (Node *)malloc(sizeof(Node) * LIST_LEN);
	int i = 0;
	for (i = 0; i < (LIST_LEN - 1); i++) {
		(List + i)->data = i + 1;
		(List + i)->next = List + i + 1;
	}
	(List + LIST_LEN - 1)->data = LIST_LEN;
	(List + LIST_LEN - 1)->next = NULL;
}
//打印俩表的data
void print_list() {
	Node * cur = List;
	while (cur != NULL) {
		printf("%d ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}
int main() {
	make_list();
	print_list();
	//迭代方式
	reverse_ite(List); 
	print_list();
	//递归方式
	reverse_recursive(List, NULL);
	print_list();
}
```
代码执行结果如下：
![](https://img-blog.csdnimg.cn/20181029150754292.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_27,color_FFFFFF,t_70)
