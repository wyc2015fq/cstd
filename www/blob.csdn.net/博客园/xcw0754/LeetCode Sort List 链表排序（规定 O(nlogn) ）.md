# LeetCode Sort List 链表排序（规定 O(nlogn) ） - xcw0754 - 博客园
# [LeetCode Sort List 链表排序（规定 O(nlogn) ）](https://www.cnblogs.com/xcw0754/p/4358048.html)
Status: Accepted
Runtime: 66 ms
题意：根据给出的单链表，用O(nlogn)的时间复杂度来排序。由时间复杂度想到快排、归并这两种排序。本次用的是归并排序。递归将链表的规模不断二分到只剩下1或2个元素为止，这也是递归出口，一旦出现这两种情况就可以返回。这里有个问题，链表也能二分？可以的，只是麻烦了些，用两个指针可以实现找到中点。本次代码没有详细分析具体的复杂度，但确实是归并。
注意：要考虑空链表，单个元素的链表，以及多个元素的链表。
LeetCode的代码：
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
11     ListNode *sorted(ListNode *head) {
12         //递归出口
13         if(!head->next)    return head;
14         if(!head->next->next)    //只有两个结点时就可以排序并返回了
15         {
16             if(head->val > head->next->val )
17             {
18                 head->next->next=head;
19                 head=head->next;
20                 head->next->next=0;
21             }
22             return head;
23         }        
24         
25         //找中点。方法：p2每往后移2步，p1就移1步，当p2到达最后一个结点时，p1刚好在中间
26         ListNode *p1=head,*p2=head;
27         while(p2)
28         {
29             if(p2->next)
30             {
31                 p2=p2->next->next;    //允许p2为空，但若p2->next为空，则p2->next->next会出错
32                 p1=p1->next;
33             }
34             else    break;
35         }
36         ListNode * sec=p1->next;    //左一半的最后一个结点的next必须为空，此举是必须的
37         p1->next=0;
38         
39         //二分递归
40         ListNode * temp1=sorted(head);
41         ListNode * temp2=sorted(sec);
42         
43         //归并，且返回
44         ListNode * sorted_head=NULL,*sorted_rear=NULL,*jus=NULL;
45         if(temp1->val < temp2->val )    //初始化链表头尾
46         {
47             sorted_head=sorted_rear=temp1;
48             temp1=temp1->next;
49             jus=temp1;    //jus摆在刚减少元素的那个链表，以方便判断是否该链表已没有元素了
50         }
51         else
52         {
53             sorted_head=sorted_rear=temp2;
54             temp2=temp2->next;
55             jus=temp2;
56         }
57         while(jus)    //两个链表都非空
58         {
59             if(temp1->val < temp2->val)
60             {
61                 sorted_rear->next=temp1;
62                 temp1=temp1->next;
63                 jus=temp1;
64             }
65             else
66             {
67                 sorted_rear->next=temp2;
68                 temp2=temp2->next;
69                 jus=temp2;
70             }
71             sorted_rear=sorted_rear->next;
72         }
73         if(!temp1)    //temp1为空
74             sorted_rear->next=temp2;
75         else
76             sorted_rear->next=temp1;
77         return sorted_head;
78     }
79     
80     ListNode *sortList(ListNode *head) {
81         if(!head)    return 0;
82         if(head->next==NULL)    return head;
83         return sorted(head);
84     }
85 };
Sort list
```
可自己测试的代码：
```
1 #include <iostream>
  2 using namespace std;
  3 
  4 struct ListNode {
  5     int val;
  6     ListNode *next;
  7 };
  8 
  9 ListNode *sorted(ListNode *head) {
 10     //递归出口
 11     if(!head->next)    return head;
 12     if(!head->next->next)    //只有两个结点时就可以排序并返回了
 13     {
 14         if(head->val > head->next->val )
 15         {
 16             head->next->next=head;
 17             head=head->next;
 18             head->next->next=0;
 19         }
 20         return head;
 21     }        
 22     
 23     //找中点。方法：p2每往后移2步，p1就移1步，当p2到达最后一个结点时，p1刚好在中间
 24     ListNode *p1=head,*p2=head;
 25     while(p2)
 26     {
 27         if(p2->next)
 28         {
 29             p2=p2->next->next;    //允许p2为空，但若p2->next为空，则p2->next->next会出错
 30             p1=p1->next;
 31         }
 32         else    break;
 33     }
 34     ListNode * sec=p1->next;    //左一半的最后一个结点的next必须为空，此举是必须的
 35     p1->next=0;
 36     
 37     //二分递归
 38     ListNode * temp1=sorted(head);
 39     ListNode * temp2=sorted(sec);
 40     
 41     //归并，且返回
 42     ListNode * sorted_head=NULL,*sorted_rear=NULL,*jus=NULL;
 43     if(temp1->val < temp2->val )    //初始化链表头尾
 44     {
 45         sorted_head=sorted_rear=temp1;
 46         temp1=temp1->next;
 47         jus=temp1;    //jus摆在刚减少元素的那个链表，以方便判断是否该链表已没有元素了
 48     }
 49     else
 50     {
 51         sorted_head=sorted_rear=temp2;
 52         temp2=temp2->next;
 53         jus=temp2;
 54     }
 55     while(jus)    //两个链表都非空
 56     {
 57         if(temp1->val < temp2->val)
 58         {
 59             sorted_rear->next=temp1;
 60             temp1=temp1->next;
 61             jus=temp1;
 62         }
 63         else
 64         {
 65             sorted_rear->next=temp2;
 66             temp2=temp2->next;
 67             jus=temp2;
 68         }
 69         sorted_rear=sorted_rear->next;
 70     }
 71     if(!temp1)    //temp1为空
 72         sorted_rear->next=temp2;
 73     else
 74         sorted_rear->next=temp1;
 75     return sorted_head;
 76 }
 77 
 78 ListNode *sortList(ListNode *head) {
 79     if(!head)    return 0;
 80     if(head->next==NULL)    return head;
 81     return sorted(head);
 82 }
 83 
 84 
 85 
 86 
 87 
 88 
 89 int main()
 90 {
 91     
 92     ListNode * head=new(ListNode);
 93     head->val=11;    //链表有1个元素
 94     head->next=NULL;
 95     ListNode * p=head;
 96     ListNode * temp=NULL;
 97 
 98     //int a[11]={2,1,3,6,5,8,4,9,7,10};
 99     int a[11]={2,6};    //元素自己随意添加
100     for(int i=0;i<2;i++)    //在这里控制元素个数。
101     {
102         temp=new(ListNode);
103         temp->val=a[i];
104         temp->next=NULL;
105         p->next=temp;
106         p=p->next;
107     }
108     p=sortList(head);
109     while(p)
110     {
111         printf("%d\n",p->val);
112         p=p->next;
113     }
114 
115     return 0;
116 }
Test Code
```

