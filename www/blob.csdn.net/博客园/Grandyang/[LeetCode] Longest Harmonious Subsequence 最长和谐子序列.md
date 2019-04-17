# [LeetCode] Longest Harmonious Subsequence 最长和谐子序列 - Grandyang - 博客园







# [[LeetCode] Longest Harmonious Subsequence 最长和谐子序列](https://www.cnblogs.com/grandyang/p/6896799.html)







We define a harmonious array is an array where the difference between its maximum value and its minimum value is exactly 1.

Now, given an integer array, you need to find the length of its longest harmonious subsequence among all its possible [subsequences](https://en.wikipedia.org/wiki/Subsequence).

Example 1:
Input: [1,3,2,2,5,2,3,7]
Output: 5
Explanation: The longest harmonious subsequence is [3,2,2,2,3].



Note: The length of the input array will not exceed 20,000.



这道题给了我们一个数组，让我们找出最长的和谐子序列，关于和谐子序列就是序列中数组的最大最小差值均为1。由于这里只是让我们求长度，并不需要返回具体的子序列。所以我们可以对数组进行排序，那么实际上我们只要找出来相差为1的两个数的总共出现个数就是一个和谐子序列的长度了。明白了这一点，我们就可以建立一个数字和其出现次数之间的映射，利用map的自动排序的特性，那么我们遍历map的时候就是从小往大开始遍历，我们从第二个映射对开始遍历，每次跟其前面的映射对比较，如果二者的数字刚好差1，那么就把二个数字的出现的次数相加并更新结果res即可，参见代码如下：



解法一：

```
class Solution {
public:
    int findLHS(vector<int>& nums) {
        if (nums.empty()) return 0;
        int res = 0;
        map<int, int> m;
        for (int num : nums) ++m[num];
        for (auto it = next(m.begin()); it != m.end(); ++it) {
            auto pre = prev(it);
            if (it->first == pre->first + 1) {
                res = max(res, it->second + pre->second);
            }
        }
        return res;
    }
};
```



其实我们并不用向上面那种解法那样用next和prev来移动迭代器，我们遍历每个数字的时候，只需在map中查找该数字加1是否存在，存在就更新结果res，这样更简单一些，参见代码如下： 



解法二：

```
class Solution {
public:
    int findLHS(vector<int>& nums) {
        int res = 0;
        map<int, int> m;
        for (int num : nums) ++m[num];
        for (auto a : m) {
            if (m.count(a.first + 1)) {
                res = max(res, m[a.first] + m[a.first + 1]);
            }
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/89990/simple-java-hashmap-solution](https://discuss.leetcode.com/topic/89990/simple-java-hashmap-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












