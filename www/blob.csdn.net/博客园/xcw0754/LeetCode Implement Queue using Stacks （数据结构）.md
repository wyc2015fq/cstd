# LeetCode Implement Queue using Stacks （数据结构） - xcw0754 - 博客园
# [LeetCode Implement Queue using Stacks （数据结构）](https://www.cnblogs.com/xcw0754/p/4927606.html)
题意：
　　用栈来实现队列。
思路：
　　一个栈是不够的，至少要两个。
　　（1）插入。永远只插入到stack1中（插到栈顶）。
　　（2）弹出。如果stack2不为空，直接弹出stack2的栈顶，否则，将stack1中的所有元素转移到stack2中，栈顶自然就是队头了，再弹出。
　　（3）返回队头。与（2）一样。
　　（4）是否为空。判断两个栈是否同时为空即可。
　　只要保证stack2为空时才可以将stack1转移到stack2中，就可以保证两边并不会产生混乱而出错。
```
1 class Queue {
 2 /*
 3     // Push element x to the back of queue.
 4     void push(int x) {
 5         
 6     }
 7 
 8     // Removes the element from in front of queue.
 9     void pop(void) {
10         
11     }
12 
13     // Get the front element.
14     int peek(void) {
15         
16     }
17 
18     // Return whether the queue is empty.
19     bool empty(void) {
20         
21     }
22 */  
23     stack<int> stack1,stack2;
24 public:
25     bool change()//当stack2为空时，将stack1转到stack2中
26     {
27         while(!stack1.empty())
28         {
29             stack2.push(stack1.top());
30             stack1.pop();
31         }
32         if(stack2.empty())    return true;
33         else    return false;
34     }    
35     void push(int x)
36     {
37         stack1.push(x);
38     }
39     void pop(void)
40     {
41         if(!stack2.empty())    stack2.pop();
42         else if(!change())    stack2.pop();
43     }
44     int peek(void)
45     {
46         if(!stack2.empty())    return stack2.top();
47         else
48         {
49             if(!change())    return stack2.top();
50             return 0;
51         }
52     }
53     bool empty(void)
54     {
55         if(stack1.empty()&&stack2.empty())    return true;
56         else    return false;
57     }
58 };
AC代码
```

