# [LeetCode] Maximum Length of Pair Chain 链对的最大长度 - Grandyang - 博客园







# [[LeetCode] Maximum Length of Pair Chain 链对的最大长度](https://www.cnblogs.com/grandyang/p/7381633.html)







You are given `n` pairs of numbers. In every pair, the first number is always smaller than the second number.

Now, we define a pair `(c, d)` can follow another pair `(a, b)` if and only if `b < c`. Chain of pairs can be formed in this fashion.

Given a set of pairs, find the length longest chain which can be formed. You needn't use up all the given pairs. You can select pairs in any order.

Example 1:
Input: [[1,2], [2,3], [3,4]]
Output: 2
Explanation: The longest chain is [1,2] -> [3,4]



Note:
- The number of given pairs will be in the range [1, 1000].



这道题给了我们一些链对，规定了如果后面链对的首元素大于前链对的末元素，那么这两个链对就可以链起来，问我们最大能链多少个。那么我们想，由于规定了链对的首元素一定小于尾元素，我们需要比较的是某个链表的首元素和另一个链表的尾元素之间的关系，如果整个链对数组是无序的，那么就很麻烦，所以我们需要做的是首先对链对数组进行排序，按链对的尾元素进行排序，小的放前面。这样我们就可以利用Greedy算法进行求解了。我们可以用一个栈，先将第一个链对压入栈，然后对于后面遍历到的每一个链对，我们看其首元素是否大于栈顶链对的尾元素，如果大于的话，就将当前链对压入栈，这样最后我们返回栈中元素的个数即可，参见代码如下：



解法一：

```
class Solution {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        stack<vector<int>> st;
        sort(pairs.begin(), pairs.end(), [](vector<int>& a, vector<int>& b) {
            return a[1] < b[1];
        });
        for (auto pair : pairs) {
            if (st.empty()) st.push(pair);
            else {
                auto t = st.top();
                if (pair[0] > t[1]) st.push(pair);
            }
        }
        return st.size();
    }
};
```



我们可以对上面解法的空间进行优化，并不需要用栈来记录最长链上的每一个链对。而是用一个变量end来记录当前比较到的尾元素的值，初始化为最小值，然后遍历的时候，如果当前链对的首元素大于end，那么结果res自增1，end更新为当前链对的尾元素，参见代码如下：



解法二：

```
class Solution {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        int res = 0, end = INT_MIN;
        sort(pairs.begin(), pairs.end(), [](vector<int>& a, vector<int>& b) {
            return a[1] < b[1];
        });
        for (auto pair : pairs) {
            if (pair[0] > end) {
                ++res;
                end = pair[1];
            }
        }
        return res;
    }
};
```



这道题论坛上最火的解法是用DP来做的，但是博主认为Greedy能解的就没有必要利用到DP，而且还不省空间，有点杀鸡用牛刀的感觉。不过话说这道题链来链去，为啥会让博主想到啥啥蜈蚣啊，什么鬼。。。



类似题目：

[Increasing Subsequences](http://www.cnblogs.com/grandyang/p/6388103.html)

[Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/4938187.html)

[Non-overlapping Intervals](http://www.cnblogs.com/grandyang/p/6017505.html)



参考资料：

[https://discuss.leetcode.com/topic/96966/earliest-deadline-first-algorithm-greedy-same-as-maximum-jobs-we-can-accomplish](https://discuss.leetcode.com/topic/96966/earliest-deadline-first-algorithm-greedy-same-as-maximum-jobs-we-can-accomplish)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












