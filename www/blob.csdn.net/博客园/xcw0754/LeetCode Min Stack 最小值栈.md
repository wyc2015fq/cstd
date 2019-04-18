# LeetCode Min Stack 最小值栈 - xcw0754 - 博客园
# [LeetCode Min Stack 最小值栈](https://www.cnblogs.com/xcw0754/p/4483646.html)
题意：实现栈的四个基本功能。要求：在get最小元素值时，复杂度O(1)。
思路：链表直接实现。最快竟然还要61ms，醉了。
```
1 class MinStack {
 2 public:
 3     MinStack(){
 4         head.next=0;
 5         head.t=0;
 6         m=0x7FFFFFFF;
 7     }
 8     void push(int x) {
 9         node *p=(node *)new(node);
10         p->t=x;
11         p->next=head.next;
12         head.next=p;
13         head.t++;
14         if(x < m)    //要更新
15             m = x;
16     }
17 
18     void pop() {
19 
20         if(0==head.t)    return ;
21         else if(head.t)
22         {
23             head.t--;
24             if(head.next->t==m) //刚好等于最小值m，要更新最小值
25             {
26                 int sma=0x7FFFFFFF;
27                 node *p=head.next->next;
28                 while( p )
29                 {
30                     if(p->t<sma)
31                         sma=p->t;
32                     p=p->next;
33                 }
34                 m=sma;
35             }
36             head.next=head.next->next;
37         }
38     }
39 
40     int top() {
41         if(head.t)
42             return head.next->t;
43         return 0;
44     }
45 
46     int getMin() {
47         return m;
48     }
49 private:
50     struct node
51     {
52         int t;
53         node *next;
54     };
55     node head;
56     int m;
57 };
View Code
```

