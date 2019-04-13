
# 225. Implement Stack using Queues - OraYang的博客 - CSDN博客

2017年07月31日 09:45:02[OraYang](https://me.csdn.net/u010665216)阅读数：222标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Implement the following operations of a stack using queues.
push(x) -- Push element x onto stack.
pop() -- Removes the element on top of the stack.
top() -- Get the top element.
empty() -- Return whether the stack is empty.
Notes:
You must useonlystandard operations of a queue -- which means onlypush
 to back,peek/pop from front,size, andis emptyoperations are valid.
Depending on your language, queue may not be supported natively. You may simulate a queue by using a list or deque (double-ended queue), as long as you use only standard operations of a queue.
You may assume that all operations are valid (for example, no pop or top operations will be called on an empty stack).
思路：本题的采用dqueue的数据结构
代码：

```python
class MyStack {
public:
    /** Initialize your data structure here. */
    MyStack() {
        
    }
    deque<int> mystack;
    /** Push element x onto stack. */
    void push(int x) {
        mystack.push_back(x) ;
        
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        int temp = mystack.back();
        mystack.pop_back();
        return temp;
        
        
    }
    
    /** Get the top element. */
    int top() {
        return mystack.back();
        
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return mystack.empty();
        
    }
};
/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack obj = new MyStack();
 * obj.push(x);
 * int param_2 = obj.pop();
 * int param_3 = obj.top();
 * bool param_4 = obj.empty();
 */
```


