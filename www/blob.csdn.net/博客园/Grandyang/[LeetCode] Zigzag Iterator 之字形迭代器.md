# [LeetCode] Zigzag Iterator 之字形迭代器 - Grandyang - 博客园







# [[LeetCode] Zigzag Iterator 之字形迭代器](https://www.cnblogs.com/grandyang/p/5212785.html)







Given two 1d vectors, implement an iterator to return their elements alternately.

For example, given two 1d vectors:
v1 = [1, 2]
v2 = [3, 4, 5, 6]

By calling *next* repeatedly until *hasNext* returns `false`, the order of elements returned by *next* should be: `[1, 3, 2, 4, 5, 6]`.

**Follow up**: What if you are given `k` 1d vectors? How well can your code be extended to such cases?

**Clarification for the follow up question - Update (2015-09-18):**

The "Zigzag" order is not clearly defined and is ambiguous for `k > 2` cases. If "Zigzag" does not look right to you, replace "Zigzag" with "Cyclic". For example, given the following input:

[1,2,3]
[4,5,6,7]
[8,9]

It should return `[1,4,8,2,5,9,3,6,7]`.



这道题让我们写一个之字形迭代器，跟之前那道[Flatten 2D Vector](http://www.cnblogs.com/grandyang/p/5209621.html)有些类似，那道题是横向打印，这道题是纵向打印，虽然方向不同，但是实现思路都是大同小异。我最先想到的方法是用两个变量i和j分别记录两个向量的当前元素位置，初始化为0，然后当i<=j时，则说明需要打印v1数组的元素，反之则打印v2数组中的元素。在hasNext函数中，当i或j打印等于对应数组的长度时，我们将其赋为一个特大值，这样不影响我们打印另一个数组的值，只有当i和j都超过格子数组的长度时，返回false，参见代码如下：



解法一：

```
class ZigzagIterator {
public:
    ZigzagIterator(vector<int>& v1, vector<int>& v2) {
        v.push_back(v1);
        v.push_back(v2);
        i = j = 0;
    }
    int next() {
        return i <= j ? v[0][i++] : v[1][j++];
    }
    bool hasNext() {
        if (i >= v[0].size()) i = INT_MAX;
        if (j >= v[1].size()) j = INT_MAX;
        return i < v[0].size() || j < v[1].size();
    }
private:
    vector<vector<int>> v;
    int i, j;
};
```



下面我们来看另一种解法，这种解法直接在初始化的时候就两个数组按照之字形的顺序存入另一个一位数组中了，那么我们就按顺序打印新数组中的值即可，参见代码如下：



解法二：

```
class ZigzagIterator {
public:
    ZigzagIterator(vector<int>& v1, vector<int>& v2) {
        int n1 = v1.size(), n2 = v2.size(), n = max(n1, n2);
        for (int i = 0; i < n; ++i) {
            if (i < n1) v.push_back(v1[i]);
            if (i < n2) v.push_back(v2[i]);
        }
    }
    int next() {
        return v[i++];
    }
    bool hasNext() {
        return i < v.size();
    }
private:
    vector<int> v;
    int i = 0;
};
```



由于题目中的Follow up让我们考虑将输入换成k个数组的情况，那么上面两种解法显然就不适用了，所以我们需要一种通解。我们可以采用queue加iterator的方法，用一个queue里面保存iterator的pair，在初始化的时候，有几个数组就生成几个pair放到queue中，每个pair保存该数组的首位置和尾位置的iterator，在next()函数中，我们取出queue队首的一个pair，如果当前的iterator不等于end()，我们将其下一个位置的iterator和end存入队尾，然后返回当前位置的值。在hasNext()函数中，我们只需要看queue是否为空即可，参见代码如下：



解法三：

```
class ZigzagIterator {
public:
    ZigzagIterator(vector<int>& v1, vector<int>& v2) {
        if (!v1.empty()) q.push(make_pair(v1.begin(), v1.end()));
        if (!v2.empty()) q.push(make_pair(v2.begin(), v2.end()));
    }
    int next() {
        auto it = q.front().first, end = q.front().second;
        q.pop();
        if (it + 1 != end) q.push(make_pair(it + 1, end));
        return *it;
    }
    bool hasNext() {
        return !q.empty();
    }
private:
    queue<pair<vector<int>::iterator, vector<int>::iterator>> q;
};
```



类似题目：

[Flatten 2D Vector](http://www.cnblogs.com/grandyang/p/5209621.html)



参考资料：

[https://leetcode.com/discuss/63037/simple-java-solution-for-k-vector](https://leetcode.com/discuss/63037/simple-java-solution-for-k-vector)

[https://leetcode.com/discuss/58683/c-with-queue-compatible-with-k-vectors](https://leetcode.com/discuss/58683/c-with-queue-compatible-with-k-vectors)

[https://leetcode.com/discuss/66968/how-about-combining-the-input-vectors-into-one-the-beginning](https://leetcode.com/discuss/66968/how-about-combining-the-input-vectors-into-one-the-beginning)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












