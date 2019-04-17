# [LeetCode] Implement Queue using Stacks 用栈来实现队列 - Grandyang - 博客园







# [[LeetCode] Implement Queue using Stacks 用栈来实现队列](https://www.cnblogs.com/grandyang/p/4626238.html)







Implement the following operations of a queue using stacks.
- push(x) -- Push element x to the back of queue.
- pop() -- Removes the element from in front of queue.
- peek() -- Get the front element.
- empty() -- Return whether the queue is empty.

**Notes:**
- You must use *only* standard operations of a stack -- which means only `push to top`, `peek/pop from top`, `size`, and `is empty` operations are valid.
- Depending on your language, stack may not be supported natively. You may simulate a stack by using a list or deque (double-ended queue), as long as you use only standard operations of a stack.
- You may assume that all operations are valid (for example, no pop or peek operations will be called on an empty queue).



这道题让我们用栈来实现队列，之前我们做过一道相反的题目[Implement Stack using Queues 用队列来实现栈](http://www.cnblogs.com/grandyang/p/4568796.html)，是用队列来实现栈。这道题颠倒了个顺序，起始并没有太大的区别，栈和队列的核心不同点就是栈是先进后出，而队列是先进先出，那么我们要用栈的先进后出的特性来模拟出队列的先进先出。那么怎么做呢，其实很简单，只要我们在插入元素的时候每次都都从前面插入即可，比如如果一个队列是1,2,3,4，那么我们在栈中保存为4,3,2,1，那么返回栈顶元素1，也就是队列的首元素，则问题迎刃而解。所以此题的难度是push函数，我们需要一个辅助栈tmp，把s的元素也逆着顺序存入tmp中，此时加入新元素x，再把tmp中的元素存回来，这样就是我们要的顺序了，其他三个操作也就直接调用栈的操作即可，参见代码如下：



解法一：

```
class MyQueue {
public:
    /** Initialize your data structure here. */
    MyQueue() {}
    
    /** Push element x to the back of queue. */
    void push(int x) {
        stack<int> tmp;
        while (!st.empty()) {
            tmp.push(st.top()); st.pop();
        }
        st.push(x);
        while (!tmp.empty()) {
            st.push(tmp.top()); tmp.pop();
        }
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        int val = st.top(); st.pop();
        return val;
    }
    
    /** Get the front element. */
    int peek() {
        return st.top();
    }
    
    /** Returns whether the queue is empty. */
    bool empty() {
        return st.empty();
    }
    
private:
    stack<int> st;
};
```



上面那个解法虽然简单，但是效率不高，因为每次在push的时候，都要翻转两边栈，下面这个方法使用了两个栈_new和_old，其中新进栈的都先缓存在_new中，入股要pop和peek的时候，才将_new中所有元素移到_old中操作，提高了效率，代码如下：



解法二：

```
class MyQueue {
public:
    /** Initialize your data structure here. */
    MyQueue() {}
    
    /** Push element x to the back of queue. */
    void push(int x) {
        _new.push(x);
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        shiftStack();
        int val = _old.top(); _old.pop();
        return val;
    }
    
    /** Get the front element. */
    int peek() {
        shiftStack();
        return _old.top();
    }
    
    /** Returns whether the queue is empty. */
    bool empty() {
        return _old.empty() && _new.empty();
    }
    
    void shiftStack() {
        if (!_old.empty()) return;
        while (!_new.empty()) {
            _old.push(_new.top());
            _new.pop();
        }
    }
    
private:
    stack<int> _old, _new;
};
```



类似题目：

[Implement Stack using Queues](http://www.cnblogs.com/grandyang/p/4568796.html)



参考资料：

[https://leetcode.com/problems/implement-queue-using-stacks/](https://leetcode.com/problems/implement-queue-using-stacks/)

[https://leetcode.com/problems/implement-queue-using-stacks/discuss/64197/Easy-Java-solution-just-edit-push()-method](https://leetcode.com/problems/implement-queue-using-stacks/discuss/64197/Easy-Java-solution-just-edit-push()-method)

[https://leetcode.com/problems/implement-queue-using-stacks/discuss/64206/Short-O(1)-amortized-C%2B%2B-Java-Ruby](https://leetcode.com/problems/implement-queue-using-stacks/discuss/64206/Short-O(1)-amortized-C%2B%2B-Java-Ruby)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












