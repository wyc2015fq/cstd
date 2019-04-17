# [LeetCode] Peeking Iterator 顶端迭代器 - Grandyang - 博客园







# [[LeetCode] Peeking Iterator 顶端迭代器](https://www.cnblogs.com/grandyang/p/4825068.html)







Given an Iterator class interface with methods: `next()` and `hasNext()`, design and implement a PeekingIterator that support the `peek()` operation -- it essentially peek() at the element that will be returned by the next call to next().

Example:
Assume that the iterator is initialized to the beginning of the list: `[1,2,3]`.

Call `next()` gets you 1, the first element in the list.
Now you call `peek()` and it returns 2, the next element. Calling `next()` after that *still* return 2. 
You call `next()` the final time and it returns 3, the last element. 
Calling `hasNext()` after that should return false.
**Hint:**
- Think of "looking ahead". You want to cache the next element.
- Is one variable sufficient? Why or why not?
- Test your design with call order of `peek()` before `next()` vs `next()` before `peek()`.
- For a clean implementation, check out [Google's guava library source code](https://github.com/google/guava/blob/703ef758b8621cfbab16814f01ddcc5324bdea33/guava-gwt/src-super/com/google/common/collect/super/com/google/common/collect/Iterators.java#L1125).

**Follow up**: How would you extend your design to be generic and work with all types, not just integer?



这道题让我们实现一个顶端迭代器，在普通的迭代器类Iterator的基础上增加了peek的功能，就是返回查看下一个值的功能，但是不移动指针，next()函数才会移动指针，那我们可以定义一个变量专门来保存下一个值，再用一个bool型变量标记是否保存了下一个值，再调用原来的一些成员函数，就可以实现这个顶端迭代器了，参见代码如下：



解法一：

```
class Iterator {
    struct Data;
    Data* data;
public:
    Iterator(const vector<int>& nums);
    Iterator(const Iterator& iter);
    virtual ~Iterator();
    // Returns the next element in the iteration.
    int next();
    // Returns true if the iteration has more elements.
    bool hasNext() const;
};

class PeekingIterator : public Iterator {
public:
    PeekingIterator(const vector<int>& nums) : Iterator(nums) {
        _flag = false;
    }

    int peek() {
        if (!_flag) _value = Iterator::next();
        _flag = true;
        return _value;
    }

    int next() {
        if (!_flag) return Iterator::next();
        _flag = false;
        return _value;
    }

    bool hasNext() const {
        return _flag || Iterator::hasNext();
    }

private:
    int _value;
    bool _flag;
};
```



这道题主要的考点就是peek函数，因为这个是默认的迭代器不具备的功能。我们其实可以使用一个小trick来实现peek功能，由于peek是要暗中观察一下下一个元素，但是迭代器并不真正移动到下一个，那么我们其实是可以创建一个副本，然后让副本移动到下一个，并返回，由于是局部变量，副本在调用结束后也会被销毁，所以并没有任何内存问题，可以说是一种相当聪明的解法了，参见代码如下：



解法二：

```
class Iterator {
    struct Data;
    Data* data;
public:
    Iterator(const vector<int>& nums);
    Iterator(const Iterator& iter);
    virtual ~Iterator();
    // Returns the next element in the iteration.
    int next();
    // Returns true if the iteration has more elements.
    bool hasNext() const;
};

class PeekingIterator : public Iterator {
public:
    PeekingIterator(const vector<int>& nums) : Iterator(nums) {}

    int peek() {
        return Iterator(*this).next();
    }

    int next() {
        return Iterator::next();
    }

    bool hasNext() const {
        return Iterator::hasNext();
    }
};
```



类似题目：

[Binary Search Tree Iterator](http://www.cnblogs.com/grandyang/p/4231455.html)

[Flatten 2D Vector](http://www.cnblogs.com/grandyang/p/5209621.html)

[Zigzag Iterator](http://www.cnblogs.com/grandyang/p/5212785.html)



参考资料：

[https://leetcode.com/problems/peeking-iterator/](https://leetcode.com/problems/peeking-iterator/)

[https://leetcode.com/problems/peeking-iterator/discuss/72650/10-line-C%2B%2B-and-14-line-Java-Implementation](https://leetcode.com/problems/peeking-iterator/discuss/72650/10-line-C%2B%2B-and-14-line-Java-Implementation)

[https://leetcode.com/problems/peeking-iterator/discuss/72554/Simple-C%2B%2B-solution-(1-line-per-method)-without-extra-member-variables](https://leetcode.com/problems/peeking-iterator/discuss/72554/Simple-C%2B%2B-solution-(1-line-per-method)-without-extra-member-variables)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












