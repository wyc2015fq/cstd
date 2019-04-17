# [LeetCode] Maximum Gap 求最大间距 - Grandyang - 博客园







# [[LeetCode] Maximum Gap 求最大间距](https://www.cnblogs.com/grandyang/p/4234970.html)







Given an unsorted array, find the maximum difference between the successive elements in its sorted form.

Return 0 if the array contains less than 2 elements.

Example 1:
Input: [3,6,9,1]
Output: 3
Explanation: The sorted form of the array is [1,3,6,9], either
             (3,6) or (6,9) has the maximum difference 3.
Example 2:
Input: [10]
Output: 0
Explanation: The array contains less than 2 elements, therefore return 0.
Note:
- You may assume all elements in the array are non-negative integers and fit in the 32-bit signed integer range.
- Try to solve it in linear time/space.



遇到这类问题肯定先想到的是要给数组排序，但是题目要求是要线性的时间和空间，那么只能用桶排序或者基排序。这里我用桶排序Bucket Sort来做，首先找出数组的最大值和最小值，然后要确定每个桶的容量，即为(最大值 - 最小值) / 个数 + 1，在确定桶的个数，即为(最大值 - 最小值) / 桶的容量 + 1，然后需要在每个桶中找出局部最大值和最小值，而最大间距的两个数不会在同一个桶中，而是一个桶的最小值和另一个桶的最大值之间的间距。代码如下：



```
class Solution {
public:
    int maximumGap(vector<int> &numss) {
        if (numss.empty()) return 0;
        int mx = INT_MIN, mn = INT_MAX, n = numss.size();
        for (int d : numss) {
            mx = max(mx, d);
            mn = min(mn, d);
        }
        int size = (mx - mn) / n + 1;
        int bucket_nums = (mx - mn) / size + 1;
        vector<int> bucket_min(bucket_nums, INT_MAX);
        vector<int> bucket_max(bucket_nums, INT_MIN);
        set<int> s;
        for (int d : numss) {
            int idx = (d - mn) / size;
            bucket_min[idx] = min(bucket_min[idx], d);
            bucket_max[idx] = max(bucket_max[idx], d);
            s.insert(idx);
        }
        int pre = 0, res = 0;
        for (int i = 1; i < n; ++i) {
            if (!s.count(i)) continue;
            res = max(res, bucket_min[i] - bucket_max[pre]);
            pre = i;
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/maximum-gap](https://leetcode.com/problems/maximum-gap)

[http://blog.csdn.net/u011345136/article/details/41963051](http://blog.csdn.net/u011345136/article/details/41963051)

[https://leetcode.com/problems/maximum-gap/discuss/50642/radix-sort-solution-in-java-with-explanation](https://leetcode.com/problems/maximum-gap/discuss/50642/radix-sort-solution-in-java-with-explanation)

[https://leetcode.com/problems/maximum-gap/discuss/50643/bucket-sort-java-solution-with-explanation-on-time-and-space](https://leetcode.com/problems/maximum-gap/discuss/50643/bucket-sort-java-solution-with-explanation-on-time-and-space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












