# [LeetCode] Subarray Sum Equals K 子数组和为K - Grandyang - 博客园







# [[LeetCode] Subarray Sum Equals K 子数组和为K](https://www.cnblogs.com/grandyang/p/6810361.html)







Given an array of integers and an integer k, you need to find the total number of continuous subarrays whose sum equals to k.

Example 1:
Input:nums = [1,1,1], k = 2
Output: 2



Note:
- The length of the array is in range [1, 20,000].
- The range of numbers in the array is [-1000, 1000] and the range of the integer k is [-1e7, 1e7].



这道题给了我们一个数组，让我们求和为k的连续子数组的个数，博主最开始看到这道题想着肯定要建立累加和数组啊，然后遍历累加和数组的每个数字，首先看其是否为k，是的话结果res自增1，然后再加个往前的循环，这样可以快速求出所有的子数组之和，看是否为k，参见代码如下：



解法一：

```
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int res = 0, n = nums.size();
        vector<int> sums = nums;
        for (int i = 1; i < n; ++i) {
            sums[i] = sums[i - 1] + nums[i];
        }
        for (int i = 0; i < n; ++i) {
            if (sums[i] == k) ++res;
            for (int j = i - 1; j >= 0; --j) {
                if (sums[i] - sums[j] == k) ++res;
            }
        }
        return res;
    }
};
```



上面的求累加和的方法其实并没有提高程序的执行效率，跟下面这种暴力搜索的解法并没有什么不同，博主很惊奇OJ居然这么大度，让这种解法也能通过，参见代码如下：



解法二：

```
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int res = 0, n = nums.size();
        for (int i = 0; i < n; ++i) {
            int sum = nums[i];
            if (sum == k) ++res;
            for (int j = i + 1; j < n; ++j) {
                sum += nums[j];
                if (sum == k) ++res;
            }
        }
        return res;
    }
};
```



论坛上大家比较推崇的其实是这种解法，用一个哈希表来建立连续子数组之和跟其出现次数之间的映射，初始化要加入{0,1}这对映射，这是为啥呢，因为我们的解题思路是遍历数组中的数字，用sum来记录到当前位置的累加和，我们建立哈希表的目的是为了让我们可以快速的查找sum-k是否存在，即是否有连续子数组的和为sum-k，如果存在的话，那么和为k的子数组一定也存在，这样当sum刚好为k的时候，那么数组从起始到当前位置的这段子数组的和就是k，满足题意，如果哈希表中事先没有m[0]项的话，这个符合题意的结果就无法累加到结果res中，这就是初始化的用途。上面讲解的内容顺带着也把for循环中的内容解释了，这里就不多阐述了，有疑问的童鞋请在评论区留言哈，参见代码如下：



解法三：

```
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int res = 0, sum = 0, n = nums.size();
        unordered_map<int, int> m{{0, 1}};
        for (int i = 0; i < n; ++i) {
            sum += nums[i];
            res += m[sum - k];
            ++m[sum]; 
        }
        return res;
    }
};
```



类似题目：

[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)

[Continuous Subarray Sum](http://www.cnblogs.com/grandyang/p/6504158.html)

[Subarray Product Less Than K](http://www.cnblogs.com/grandyang/p/7753959.html)

[Find Pivot Index](http://www.cnblogs.com/grandyang/p/7865693.html)



参考资料：

[https://leetcode.com/problems/subarray-sum-equals-k/](https://leetcode.com/problems/subarray-sum-equals-k/)

[https://leetcode.com/problems/subarray-sum-equals-k/discuss/102153/Basic-Java-solution](https://leetcode.com/problems/subarray-sum-equals-k/discuss/102153/Basic-Java-solution)

[https://leetcode.com/problems/subarray-sum-equals-k/discuss/134689/Three-Approaches-With-Explanation](https://leetcode.com/problems/subarray-sum-equals-k/discuss/134689/Three-Approaches-With-Explanation)

[https://leetcode.com/problems/subarray-sum-equals-k/discuss/102106/Java-Solution-PreSum-%2B-HashMap](https://leetcode.com/problems/subarray-sum-equals-k/discuss/102106/Java-Solution-PreSum-%2B-HashMap)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












