# LeetCode Implement Stack using Queues （数据结构） - xcw0754 - 博客园
# [LeetCode Implement Stack using Queues （数据结构）](https://www.cnblogs.com/xcw0754/p/4927629.html)
题意：
　　用队列来实现栈。
思路：
　　没有什么捷径，纯粹模拟。但是用一个队列就够了。
```
1 class Stack {
 2 /*
 3     // Push element x onto stack.
 4     void push(int x) {
 5         
 6     }
 7 
 8     // Removes the element on top of the stack.
 9     void pop() {
10         
11     }
12 
13     // Get the top element.
14     int top() {
15         
16     }
17 
18     // Return whether the stack is empty.
19     bool empty() {
20         
21     }
22 */
23     queue<int> que;
24 public:
25     void push(int x)
26     {
27         que.push(x);
28     }
29     void pop(void)
30     {
31         if(!que.empty())
32         {
33             int n=que.size(),pos=1;
34             while(pos<n)
35             {
36                 que.push(que.front());
37                 que.pop();    
38                 pos++;
39             }
40             que.pop();
41         }
42     }
43     int top()
44     {
45         if(!que.empty())
46         {
47             int n=que.size(), pos=1,x=0;
48             while(pos<=n)
49             {
50                 if(pos==n)    x=que.front();
51                 que.push(que.front());
52                 que.pop();
53                 pos++;
54             }
55             return x;
56         }        
57     }
58     bool empty(void)
59     {
60         if(que.empty())    return true;
61         else            return false;
62     }
63 };
AC代码
```

