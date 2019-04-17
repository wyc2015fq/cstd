# 155. Min Stack 最小栈 - 别说话写代码的博客 - CSDN博客





2019年02月24日 20:10:55[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：9标签：[最小栈																[leetcode																[Min Stack](https://so.csdn.net/so/search/s.do?q=Min Stack&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.- push(x) -- Push element x onto stack.- pop() -- Removes the element on top of the stack.- top() -- Get the top element.- getMin() -- Retrieve the minimum element in the stack.**Example:**MinStack minStack = new MinStack();minStack.push(-2);minStack.push(0);minStack.push(-3);minStack.getMin();   --> Returns -3.minStack.pop();minStack.top();      --> Returns 0.minStack.getMin();   --> Returns -2.|设计一个支持 push，pop，top 操作，并能在常数时间内检索到最小元素的栈。- push(x) -- 将元素 x 推入栈中。- pop() -- 删除栈顶的元素。- top() -- 获取栈顶元素。- getMin() -- 检索栈中的最小元素。**示例:**MinStack minStack = new MinStack();minStack.push(-2);minStack.push(0);minStack.push(-3);minStack.getMin();   --> 返回 -3.minStack.pop();minStack.top();      --> 返回 0.minStack.getMin();   --> 返回 -2.|
|----|----|

思路：当minstk为空时，minstk.push(x)；当其不为空时，minstk.push(min(minstk.top(),x))；也就是说不空的时候输入当前元素和最小栈栈顶元素的最小值。

```cpp
class MinStack {
public:
    /** initialize your data structure here. */
    MinStack() {}
    
    void push(int x) {
        stk.push(x);
        if(minstk.empty()) minstk.push(x);
        else minstk.push(min(minstk.top(),x));
    }
    
    void pop() {
        stk.pop();
        minstk.pop();
    }
    
    int top() {
        return stk.top();
    }
    
    int getMin() {
        return minstk.top();
    }
private:
    stack<int> stk,minstk;
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack obj = new MinStack();
 * obj.push(x);
 * obj.pop();
 * int param_3 = obj.top();
 * int param_4 = obj.getMin();
 */
```](https://so.csdn.net/so/search/s.do?q=最小栈&t=blog)




