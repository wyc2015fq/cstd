
# 155. Min Stack - OraYang的博客 - CSDN博客

2017年07月26日 09:01:30[OraYang](https://me.csdn.net/u010665216)阅读数：186标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.
push(x) -- Push element x onto stack.
pop() -- Removes the element on top of the stack.
top() -- Get the top element.
getMin() -- Retrieve the minimum element in the stack.
Example:
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin();   --> Returns -3.
minStack.pop();
minStack.top();      --> Returns 0.
minStack.getMin();   --> Returns -2.思路：本题，可以用C++内部栈实现，于是剩下就是实现getMin（），在实现过程中为了防止time limit，这需要设置一个标志位，与一个存储变量，用来存储上一个最小值（避免重复运算，这个还是有很大弊端，但是leetcode的测试集并没有测出来）；于是为了解决第一种内置栈方法的不足，我们额外增加一个内部栈用来存储最小值，具体代码如下：代码1（内置栈）：

```python
class MinStack {
public:
    /** initialize your data structure here. */
    MinStack() {
        
        stack<int> stk;
        
    }
    stack<int> stk;
    int flag = 0;
    int store;
    void push(int x) {
        stk.push(x);
         flag = 1;
    }
    
    void pop() {
        stk.pop();
         flag = 1;
    }
    
    int top() {
        return stk.top();
       
    }
    
    int getMin() {
        if(flag==0)
            return store;
        stack<int> res = stk;
        int temp = res.top();
        while(!res.empty())
        {
            if(temp>res.top())
                temp = res.top();
            res.pop();
        }
        store =temp;
        flag=0;
        return temp;    
        
    }
};
/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack obj = new MinStack();
 * obj.push(x);
 * obj.pop();
 * int param_3 = obj.top();
 * int param_4 = obj.getMin();
 */
```
代码2（两个栈）：
```python
class MinStack {
private:
    stack<int> stk1;
    stack<int> stk2;
public:
    /** initialize your data structure here. */
    MinStack() {
        
        
    }
    void push(int x) {  
        stk1.push(x);  
        if(stk2.empty() || x <= stk2.top())  
            stk2.push(x);  
    }  
    void pop() {  
        int top = stk1.top();  
        stk1.pop();  
        if(top == stk2.top())  
        {  
            stk2.pop();  
        }  
    }  
  
    int top() {  
        return stk1.top();  
    }  
  
    int getMin() {  
        return stk2.top();  
    }  
};
```


