# [LeetCode] Count of Range Sum 区间和计数 - Grandyang - 博客园







# [[LeetCode] Count of Range Sum 区间和计数](https://www.cnblogs.com/grandyang/p/5162678.html)







Given an integer array `nums`, return the number of range sums that lie in `[lower, upper]` inclusive.
Range sum `S(i, j)` is defined as the sum of the elements in `nums` between indices `i` and `j` (`i` ≤ `j`), inclusive.

Note:
A naive algorithm of *O*(*n*2) is trivial. You MUST do better than that.

Example:
Given *nums* = `[-2, 5, -1]`, *lower* = `-2`, *upper* = `2`,
Return `3`.
The three ranges are : `[0, 0]`, `[2, 2]`, `[0, 2]` and their respective sums are: `-2, -1, 2`.

Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题给了我们一个数组，又给了我们一个下限和一个上限，让我们求有多少个不同的区间使得每个区间的和在给定的上下限之间。这道题的难度系数给的是Hard，的确是一道难度不小的题，题目中也说了Brute Force的方法太Naive了，那么我们只能另想方法了。To be honest，这题完全超出了我的能力范围，所以我也没挣扎了，直接上网搜大神们的解法啦。首先根据前面的那几道类似题[Range Sum Query - Mutable 区域和检索 - 可变](http://www.cnblogs.com/grandyang/p/4985506.html)，[Range Sum Query 2D - Immutable 二维区域和检索](http://www.cnblogs.com/grandyang/p/4958789.html)和[Range Sum Query - Immutable 区域和检索 - 不可变](http://www.cnblogs.com/grandyang/p/4952464.html)的解法可知类似的区间和的问题一定是要计算累积和sum的，其中sum[i] = nums[0] + nums[1] + ... + nums[i]，对于某个i来说，只有那些满足 lower <= sum[i] - sum[j] <= upper 的j能形成一个区间[j, i]满足题意，那么我们的目标就是来找到有多少个这样的j (0 =< j < i) 满足 sum[i] - upper =< sum[j] <= sum[i] - lower，我们可以用C++中由红黑树实现的multiset数据结构可以对其中数据排序，然后用upperbound和lowerbound来找临界值。lower_bound是找数组中第一个不小于给定值的数(包括等于情况)，而upper_bound是找数组中第一个大于给定值的数，那么两者相减，就是j的个数，参见代码如下：



解法一：

```
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int res = 0;
        long long sum = 0;
        multiset<long long> sums;
        sums.insert(0);
        for (int i = 0; i < nums.size(); ++i) {
            sum += nums[i];
            res += distance(sums.lower_bound(sum - upper), sums.upper_bound(sum - lower));
            sums.insert(sum);
        }
        return res;
    }        
};
```



我们再来看一种方法，这种方法的思路和前一种一样，只是没有STL的multiset和lower_bound和upper_bound函数，而是使用了Merge Sort来解，在混合的过程中，我们已经给左半边[start, mid) 和右半边 [mid, end) 排序了。当我们遍历左半边，对于每个i，我们需要在右半边找出k和j，使其满足：

j是第一个满足 sums[j] - sums[i] > upper 的下标

k是第一个满足 sums[k] - sums[i] >= lower 的下标 

那么在[lower, upper]之间的区间的个数是j - k，同时我们也需要另一个下标t，用来拷贝所有满足sums[t] < sums[i]到一个寄存器Cache中以完成混合排序的过程。(注意这里sums可能会整型溢出，我们使用长整型long long代替)。



解法二：

```
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        vector<long> sums(nums.size() + 1, 0);
        for (int i = 0; i < nums.size(); ++i) {
            sums[i + 1] = sums[i] + nums[i];
        }
        return countAndMergeSort(sums, 0, sums.size(), lower, upper);
    }
    int countAndMergeSort(vector<long> &sums, int start, int end, int lower, int upper) {
        if (end - start <= 1) return 0;
        int mid = start + (end - start) / 2;
        int cnt = countAndMergeSort(sums, start, mid, lower, upper) + countAndMergeSort(sums, mid, end, lower, upper);
        int j = mid, k = mid, t = mid;
        vector<int> cache(end - start, 0);
        for (int i = start, r = 0; i < mid; ++i, ++r) {
            while (k < end && sums[k] - sums[i] < lower) ++k;
            while (j < end && sums[j] - sums[i] <= upper) ++j;
            while (t < end && sums[t] < sums[i]) cache[r++] = sums[t++];
            cache[r] = sums[i];
            cnt += j - k;
        }
        copy(cache.begin(), cache.begin() + t - start, sums.begin() + start);
        return cnt;
    }
};
```



类似题目：

[Range Sum Query - Mutable](http://www.cnblogs.com/grandyang/p/4985506.html)

[Range Sum Query 2D - Immutable](http://www.cnblogs.com/grandyang/p/4958789.html)

[Range Sum Query - Immutable](http://www.cnblogs.com/grandyang/p/4952464.html)



参考资料：

[https://leetcode.com/discuss/79083/share-my-solution](https://leetcode.com/discuss/79083/share-my-solution)

[https://leetcode.com/discuss/79632/multiset-solution-100ms-binary-search-tree-180ms-mergesort](https://leetcode.com/discuss/79632/multiset-solution-100ms-binary-search-tree-180ms-mergesort)

[https://leetcode.com/discuss/79907/summary-divide-conquer-based-binary-indexed-based-solutions](https://leetcode.com/discuss/79907/summary-divide-conquer-based-binary-indexed-based-solutions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












