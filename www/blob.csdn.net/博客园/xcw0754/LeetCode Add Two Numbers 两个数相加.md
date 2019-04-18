# LeetCode  Add Two Numbers 两个数相加 - xcw0754 - 博客园
# [LeetCode  Add Two Numbers 两个数相加](https://www.cnblogs.com/xcw0754/p/4115765.html)
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
11 ListNode *creatnode(int a){
12     ListNode *nod=new ListNode(a);
13     nod->next=0;
14     return nod;
15 }
16 ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
17     if(l1==0)    return l2;
18     if(l2==0)    return l1;
19     struct ListNode *p=l1,*c=0;
20     int tem=0,s=0;
21     while(l1!=0&&l2!=0){    //直到有一边先结束
22         tem=l1->val+l2->val+tem;
23         s=tem%10;            //个位
24         tem=tem/10;            //十位
25         l1->val=s;        
26         if(l1->next==0)
27             c=l1;            
28         l1=l1->next;
29         l2=l2->next;
30     }
31     if(l1!=0&&l2==0||l1==0&&l2!=0){
32         if(l1==0&&l2!=0){
33             c->next=l2;
34             l1=c->next;
35         }
36         if(tem==0)
37             return p;
38         while(l1!=0){
39             tem=l1->val+tem;
40             l1->val=tem%10;
41             tem=tem/10;
42             if(l1->next==0)
43                 c=l1;
44             l1=l1->next;    //l1可能为NULL
45         }
46         if(tem!=0)
47             c->next=creatnode(tem);
48     }
49     else if(l1==0&&l2==0&&tem!=0){
50         c->next=creatnode(tem);
51     } 
52     return p;
53 }
54 };
```
题意：两个十进制数，反向存储在单向链表里，每位数占一个节点。求和的链表。
思路：将提供的第一条链表作为返回的结果，相加之后的结果存在这条链表中。当l1比l2长时，结果存l1没问题；当l2比l1长时，将l2比l1多出的元素加在l1的后面，这样就不用创建过多的结点了。完全与链表长无关。
注意：考虑两个链表等长和非等长情况，考虑可能出现1+9999，0+0这类情况。
吐槽：先是用节省内存的做法，感觉代码太长。重写出减少代码量的，结果内存超出限制。重新改进节省内存的做法。其实如果再写一个函数用于加法，代码量会更少。 此代码比别人的长了一些~

