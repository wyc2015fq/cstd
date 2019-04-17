# [LeetCode] Nested List Weight Sum 嵌套链表权重和 - Grandyang - 博客园







# [[LeetCode] Nested List Weight Sum 嵌套链表权重和](https://www.cnblogs.com/grandyang/p/5340305.html)







Given a nested list of integers, return the sum of all integers in the list weighted by their depth.

Each element is either an integer, or a list -- whose elements may also be integers or other lists.

**Example 1:**

Given the list `[[1,1],2,[1,1]]`, return **10**. (four 1's at depth 2, one 2 at depth 1)

**Example 2:**

Given the list `[1,[4,[6]]]`, return **27**. (one 1 at depth 1, one 4 at depth 2, and one 6 at depth 3; 1 + 4*2 + 6*3 = 27)




这道题定义了一种嵌套链表的结构，链表可以无限往里嵌套，规定每嵌套一层，深度加1，让我们求权重之和，就是每个数字乘以其权重，再求总和。那么我们考虑，由于嵌套层数可以很大，所以我们用深度优先搜索DFS会很简单，每次遇到嵌套的，递归调用函数，一层一层往里算就可以了，我最先想的方法是遍历给的嵌套链表的数组，对于每个嵌套链表的对象，调用getSum函数，并赋深度值1，累加起来返回。在getSum函数中，首先判断其是否为整数，如果是，则返回当前深度乘以整数，如果不是，那么我们再遍历嵌套数组，对每个嵌套链表再调用递归函数，将返回值累加起来返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    int depthSum(vector<NestedInteger>& nestedList) {
        int res = 0;
        for (auto a : nestedList) {
            res += getSum(a, 1);
        }
        return res;
    }
    int getSum(NestedInteger ni, int level) {
        int res = 0;
        if (ni.isInteger()) return level * ni.getInteger();
        for (auto a : ni.getList()) {
            res += getSum(a, level + 1);
        }
        return res;
    }
};
```



但其实上面的方法可以优化，我们可以把给的那个嵌套链表的一维数组直接当做一个嵌套链表的对象，然后调用递归函数，递归函数的处理方法跟上面一样，只不过用了个三元处理使其看起来更加简洁了一些：



解法二：

```
class Solution {
public:
    int depthSum(vector<NestedInteger>& nestedList) {
        return helper(nestedList, 1);
    }
    int helper(vector<NestedInteger>& nl, int depth) {
        int res = 0;
        for (auto a : nl) {
            res += a.isInteger() ? a.getInteger() * depth : helper(a.getList(), depth + 1);
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/discuss/94956/2ms-easy-to-understand-java-solution](https://leetcode.com/discuss/94956/2ms-easy-to-understand-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














