# [LeetCode] Maximum Distance in Arrays 数组中的最大距离 - Grandyang - 博客园







# [[LeetCode] Maximum Distance in Arrays 数组中的最大距离](https://www.cnblogs.com/grandyang/p/7073343.html)








Given `m` arrays, and each array is sorted in ascending order. Now you can pick up two integers from two different arrays (each array picks one) and calculate the distance. We define the distance between two integers `a` and `b` to be their absolute difference `|a-b|`. Your task is to find the maximum distance.

Example 1:
Input: 
[[1,2,3],
 [4,5],
 [1,2,3]]
Output: 4
Explanation: 
One way to reach the maximum distance 4 is to pick 1 in the first or third array and pick 5 in the second array.



Note:
- Each given array will have at least 1 number. There will be at least two non-empty arrays.
- The total number of the integers in all the `m` arrays will be in the range of [2, 10000].
- The integers in the `m` arrays will be in the range of [-10000, 10000].



这道题给我们了一些数组，每个数组都是有序的，让我们从不同的数组中各取一个数字，使得这两个数字的差的绝对值最大，让我们求这个最大值。那么我们想，既然数组都是有序的，那么差的绝对值最大的两个数字肯定是分别位于数组的首和尾，注意题目中说要从不同的数组中取数，那么即使某个数组的首尾差距很大，也不行。博主最先考虑的是用堆来做，一个最大堆，一个最小堆，最大堆存每个数组的尾元素，最小堆存每个数组的首元素，由于最大的数字和最小的数字有可能来自于同一个数组，所以我们在堆中存数字的时候还要存入当前数字所在的数组的序号，最后我们其实要分别在最大堆和最小堆中各取两个数字，如果最大的数字和最小的数字不在一个数组，那么直接返回二者的绝对差即可，如果在的话，那么要返回第二大数字和最小数字绝对差跟最大数字和第二小数字绝对差中的较大值，参见代码如下：



解法一：

```
class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        priority_queue<pair<int, int>> mx, mn;
        for (int i = 0; i < arrays.size(); ++i) {
            mn.push({-arrays[i][0], i});
            mx.push({arrays[i].back(), i});
        }
        auto a1 = mx.top(); mx.pop();
        auto b1 = mn.top(); mn.pop();
        if (a1.second != b1.second) return a1.first + b1.first;
        return max(a1.first + mn.top().first, mx.top().first + b1.first);
    }
};
```



下面这种方法还是很不错的，并没有用到堆，而是用两个变量start和end分别表示当前遍历过的数组中最小的首元素，和最大的尾元素，那么每当我们遍历到一个新的数组时，只需计算新数组尾元素和start绝对差，跟end和新数组首元素的绝对差，取二者之间的较大值来更新结果res即可，参见代码如下：



解法二：

```
class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        int res = 0, start = arrays[0][0], end = arrays[0].back();
        for (int i = 1; i < arrays.size(); ++i) {
            res = max(res, max(abs(arrays[i].back() - start), abs(end - arrays[i][0])));
            start = min(start, arrays[i][0]);
            end = max(end, arrays[i].back());
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/92858/c-o-n](https://discuss.leetcode.com/topic/92858/c-o-n)


[https://discuss.leetcode.com/topic/92859/java-solution-min-and-max](https://discuss.leetcode.com/topic/92859/java-solution-min-and-max)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












