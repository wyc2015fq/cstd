# LeetCode  Remove Nth Node From End of List 删除链表的倒数第n个结点 - xcw0754 - 博客园
# [LeetCode  Remove Nth Node From End of List 删除链表的倒数第n个结点](https://www.cnblogs.com/xcw0754/p/4098303.html)
```
1 /**
 2  * Definition for singly-linked list.
 3  * struct ListNode {
 4  *     int val;
 5  *     ListNode *next;
 6  *     ListNode(int x) : val(x), next(NULL) {}
 7  * };
 8  */
 9 class Solution {
10 public:
11 ListNode *removeNthFromEnd(ListNode *head, int n) {
12     struct ListNode *q=0,*w=0,*e=0;
13     int i,j=0;
14     if(n==0)    return head;    //无需修改
15     if(n==1&&head->next==0)        return 0;    //只有一个结点，并且要删除掉它
16     q=e=head;                //e为最后一个结点的位置
17     for(i=0;i<n;i++){            //e先设置在q的后面为n个距离的地方,q->next是要删的结点
18         e=e->next;
19         j++;
20         if(e->next==0)    //针对刚好要求删掉链表的头结点
21             break;
22     }
23     while(e->next!=0){        //将e和q保持相同距离，往后移直到e指向最后一个元素，q->next就是所要删除的结点了
24         e=e->next;    
25         q=q->next;
26     }
27     if(j<n)    // 对于需要删除头结点的情况，用j判断
28         return head->next;
29     else
30         q->next=q->next->next;
31     return head;
32 }
33 };
```
题意：给一个链表，要删除从链尾数起，第n个结点，然后返回此链。
思路：分几种情况，一是要删除尾结点，即n=1；二是要删除头结点，需要特殊判断，同时又要缩小代码量（若加多几行代码就容易写多了）。
测试一直出问题，没考虑好只有两个结点的情况。当有两个结点的链表时，n=1或n=2的情况处理好了，后面一般没问题了。

