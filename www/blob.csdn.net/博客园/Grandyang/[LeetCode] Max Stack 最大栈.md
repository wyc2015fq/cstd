# [LeetCode] Max Stack 最大栈 - Grandyang - 博客园







# [[LeetCode] Max Stack 最大栈](https://www.cnblogs.com/grandyang/p/7823424.html)








Design a max stack that supports push, pop, top, peekMax and popMax.


- push(x) -- Push element x onto stack.
- pop() -- Remove the element on top of the stack and return it.
- top() -- Get the element on the top.
- peekMax() -- Retrieve the maximum element in the stack.
- popMax() -- Retrieve the maximum element in the stack, and remove it. If you find more than one maximum elements, only remove the top-most one.



Example 1:
MaxStack stack = new MaxStack();
stack.push(5); 
stack.push(1);
stack.push(5);
stack.top(); -> 5
stack.popMax(); -> 5
stack.top(); -> 1
stack.peekMax(); -> 5
stack.pop(); -> 1
stack.top(); -> 5



Note:
- -1e7 <= x <= 1e7
- Number of operations won't exceed 10000.
- The last four operations won't be called when stack is empty.



这道题让我们实现一个最大栈，包含一般栈的功能，但是还新加了两个功能peekMax()和popMax()，随时随地可以查看和返回最大值。之前有一道很类似的题[Min Stack](http://www.cnblogs.com/grandyang/p/4091064.html)，所以我们可以借鉴那道题的解法，使用两个栈来模拟，s1为普通的栈，用来保存所有的数字，而s2为最大栈，用来保存出现的最大的数字。

在push()函数中，我们先来看s2，如果s2为空，或者s2的栈顶元素小于等于x，将x压入s2中。因为s2保存的是目前为止最大的数字，所以一旦新数字大于等于栈顶元素，说明遇到更大的数字了，压入栈。然后将数组压入s1，s1保存所有的数字，所以都得压入栈。

在pop()函数中，当s2的栈顶元素和s1的栈顶元素相同时，我们要移除s2的栈顶元素，因为一个数字不在s1中了，就不能在s2中。然后取出s1的栈顶元素，并移除s1，返回即可。

在top()函数中，直接返回s1的top()函数即可。

在peekMax()函数中，直接返回s2的top()函数即可。

在popMax()函数中，先将s2的栈顶元素保存到一个变量mx中，然后我们要在s1中删除这个元素，由于栈无法直接定位元素，所以我们用一个临时栈t，将s1的出栈元素保存到临时栈t中，当s1的栈顶元素和s2的栈顶元素相同时退出while循环，此时我们在s1中找到了s2的栈顶元素，分别将s1和s2的栈顶元素移除，然后要做的是将临时栈t中的元素加回s1中，注意此时容易犯的一个错误是，没有同时更新s2，所以我们直接调用push()函数即可，参见代码如下：



解法一：

```
class MaxStack {
public:
    /** initialize your data structure here. */
    MaxStack() {}
    
    void push(int x) {
        if (s2.empty() || s2.top() <= x) s2.push(x);
        s1.push(x);
    }
    
    int pop() {
        if (!s2.empty() && s2.top() == s1.top()) s2.pop();
        int t = s1.top(); s1.pop();
        return t;
    }
    
    int top() {
        return s1.top();
    }
    
    int peekMax() {
        return s2.top();
    }
    
    int popMax() {
        int mx = s2.top();
        stack<int> t;
        while (s1.top() != s2.top()) {
            t.push(s1.top()); s1.pop();
        }
        s1.pop(); s2.pop();
        while (!t.empty()) {
            push(t.top()); t.pop();
        }
        return mx;
    }

private:
    stack<int> s1, s2;
};
```



下面这种解法没有利用一般的stack，而是建立一种较为复杂的数据结构，首先用一个list链表来保存所有的数字，然后建立一个数字和包含所有相同的数字的位置iterator的向量容器的映射map。

在push()函数中，把新数字加到list表头，然后把数字x的位置iterator加到数字映射的向量容器的末尾。

在pop()函数中，先得到表头数字，然后把该数字对应的iterator向量容器的末尾元素删掉，如果此时向量容器为空了，将这个映射直接删除，移除表头数字，返回该数字即可。

在top()函数中，直接返回表头数字即可。

在peekMax()函数中，因为map是按key值自动排序的，直接尾映射的key值即可。

在popMax()函数中，首先保存尾映射的key值，也就是最大值到变量x中，然后在其对应的向量容器的末尾取出其在list中的iterator。然后删除该向量容器的尾元素，如果此时向量容器为空了，将这个映射直接删除。根据之前取出的iterator，在list中删除对应的数字，返回x即可，参见代码如下：



解法二：

```
class MaxStack {
public:
    /** initialize your data structure here. */
    MaxStack() {}
    
    void push(int x) {
        v.insert(v.begin(), x);
        m[x].push_back(v.begin());
    }
    
    int pop() {
        int x = *v.begin();
        m[x].pop_back();
        if (m[x].empty()) m.erase(x);
        v.erase(v.begin());
        return x;
    }
    
    int top() {
        return *v.begin();
    }
    
    int peekMax() {
        return m.rbegin()->first;
    }
    
    int popMax() {
        int x = m.rbegin()->first;
        auto it = m[x].back();
        m[x].pop_back();
        if (m[x].empty()) m.erase(x);
        v.erase(it);
        return x;
    }

private:
    list<int> v;
    map<int, vector<list<int>::iterator>> m;
};
```



类似题目：

[Min Stack](http://www.cnblogs.com/grandyang/p/4091064.html)



参考资料：

[https://discuss.leetcode.com/topic/110014/c-using-two-stack](https://discuss.leetcode.com/topic/110014/c-using-two-stack)

[https://discuss.leetcode.com/topic/110066/c-o-logn-for-write-ops-o-1-for-reads](https://discuss.leetcode.com/topic/110066/c-o-logn-for-write-ops-o-1-for-reads)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












