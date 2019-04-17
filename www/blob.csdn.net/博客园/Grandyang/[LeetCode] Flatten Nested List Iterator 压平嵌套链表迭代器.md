# [LeetCode] Flatten Nested List Iterator 压平嵌套链表迭代器 - Grandyang - 博客园







# [[LeetCode] Flatten Nested List Iterator 压平嵌套链表迭代器](https://www.cnblogs.com/grandyang/p/5358793.html)







Given a nested list of integers, implement an iterator to flatten it.

Each element is either an integer, or a list -- whose elements may also be integers or other lists.

**Example 1:**

Given the list `[[1,1],2,[1,1]]`,



By calling *next* repeatedly until *hasNext* returns false, the order of elements returned by *next* should be: `[1,1,2,1,1]`.


**Example 2:**

Given the list `[1,[4,[6]]]`,



By calling *next* repeatedly until *hasNext* returns false, the order of elements returned by *next* should be: `[1,4,6]`.




这道题让我们建立压平嵌套链表的迭代器，关于嵌套链表的数据结构最早出现在[Nested List Weight Sum](http://www.cnblogs.com/grandyang/p/5340305.html)中，而那道题是用的递归的方法来解的，而迭代器一般都是用迭代的方法来解的，而递归一般都需用栈来辅助遍历，由于栈的后进先出的特性，我们在对向量遍历的时候，从后往前把对象压入栈中，那么第一个对象最后压入栈就会第一个取出来处理，我们的hasNext()函数需要遍历栈，并进行处理，如果栈顶元素是整数，直接返回true，如果不是，那么移除栈顶元素，并开始遍历这个取出的list，还是从后往前压入栈，循环停止条件是栈为空，返回false，参见代码如下：



解法一:

```
class NestedIterator {
public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        for (int i = nestedList.size() - 1; i >= 0; --i) {
            s.push(nestedList[i]);
        }
    }

    int next() {
        NestedInteger t = s.top(); s.pop();
        return t.getInteger();
    }

    bool hasNext() {
        while (!s.empty()) {
            NestedInteger t = s.top(); 
            if (t.isInteger()) return true;
            s.pop();
            for (int i = t.getList().size() - 1; i >= 0; --i) {
                s.push(t.getList()[i]);
            }
        }
        return false;
    }

private:
    stack<NestedInteger> s;
};
```



我们也可以使用deque来代替stack，实现思路和上面完全一样，参见代码如下:



解法二：

```
class NestedIterator {
public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        for (auto a : nestedList) {
            d.push_back(a);
        }
    }

    int next() {
        NestedInteger t = d.front(); d.pop_front();
        return t.getInteger();
    }

    bool hasNext() {
        while (!d.empty()) {
            NestedInteger t = d.front();
            if (t.isInteger()) return true;
            d.pop_front();
            for (int i = 0; i < t.getList().size(); ++i) {
                d.insert(d.begin() + i, t.getList()[i]);
            }
        }
        return false;
    }

private:
    deque<NestedInteger> d;
};
```



虽说迭代器是要用迭代的方法，但是我们可以强行使用递归来解，怎么个强行法呢，就是我们使用一个队列queue，在构造函数的时候就利用迭代的方法把这个嵌套链表全部压平展开，然后在调用hasNext()和next()就很简单了：



解法三：

```
class NestedIterator {
public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        make_queue(nestedList);
    }

    int next() {
        int t = q.front(); q.pop();
        return t; 
    }

    bool hasNext() {
        return !q.empty();
    }
    
private:
    queue<int> q;
    void make_queue(vector<NestedInteger> &nestedList) {
        for (auto a : nestedList) {
            if (a.isInteger()) q.push(a.getInteger());
            else make_queue(a.getList());
        }
    }
};
```



类似题目：

[Nested List Weight Sum](http://www.cnblogs.com/grandyang/p/5340305.html)

[Flatten 2D Vector](http://www.cnblogs.com/grandyang/p/5209621.html)

[Zigzag Iterator](http://www.cnblogs.com/grandyang/p/5212785.html)



参考资料：

[https://leetcode.com/discuss/95841/simple-solution-with-queue](https://leetcode.com/discuss/95841/simple-solution-with-queue)

[https://leetcode.com/discuss/95892/concise-c-without-storing-all-values-at-initialization](https://leetcode.com/discuss/95892/concise-c-without-storing-all-values-at-initialization)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












