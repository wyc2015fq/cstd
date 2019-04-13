
# 232. Implement Queue using Stacks - OraYang的博客 - CSDN博客

2017年08月01日 13:46:44[OraYang](https://me.csdn.net/u010665216)阅读数：135所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Implement the following operations of a queue using stacks.
push(x) -- Push element x to the back of queue.
pop() -- Removes the element from in front of queue.
peek() -- Get the front element.
empty() -- Return whether the queue is empty.
Notes:
You must useonlystandard operations of a stack -- which means onlypush
 to top,peek/pop from top,size, andis emptyoperations are valid.
Depending on your language, stack may not be supported natively. You may simulate a stack by using a list or deque (double-ended queue), as long as you use only standard operations of a stack.
You may assume that all operations are valid (for example, no pop or peek operations will be called on an empty queue).
思路：本题采用deque的数据结构，思路与以前的博客[Implement Stack using queues](http://blog.csdn.net/u010665216/article/details/76416574)一样
代码：

```python
class MyQueue {
public:
    /** Initialize your data structure here. */
    MyQueue() {
        
    }
    deque<int> opt;
    /** Push element x to the back of queue. */
    void push(int x) {
        opt.push_back(x);
        
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        int temp = opt.front();
        opt.pop_front();
        return temp;
        
    }
    
    /** Get the front element. */
    int peek() {
        return opt.front();
        
    }
    
    /** Returns whether the queue is empty. */
    bool empty() {
        return opt.empty();
        
    }
};
/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue obj = new MyQueue();
 * obj.push(x);
 * int param_2 = obj.pop();
 * int param_3 = obj.peek();
 * bool param_4 = obj.empty();
 */
```


