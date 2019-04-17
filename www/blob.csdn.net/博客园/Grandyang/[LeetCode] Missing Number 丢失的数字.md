# [LeetCode] Missing Number 丢失的数字 - Grandyang - 博客园







# [[LeetCode] Missing Number 丢失的数字](https://www.cnblogs.com/grandyang/p/4756677.html)







Given an array containing *n* distinct numbers taken from `0, 1, 2, ..., n`, find the one that is missing from the array.

For example,

Given *nums* = `[0, 1, 3]` return `2`.


**Note**:

Your algorithm should run in linear runtime complexity. Could you implement it using only constant extra space complexity?




这道题给我们n个数字，是0到n之间的数但是有一个数字去掉了，让我们寻找这个数字，要求线性的时间复杂度和常数级的空间复杂度。那么最直观的一个方法是用等差数列的求和公式求出0到n之间所有的数字之和，然后再遍历数组算出给定数字的累积和，然后做减法，差值就是丢失的那个数字，参见代码如下：



解法一：

```
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int sum = 0, n = nums.size();
        for (auto &a : nums) {
            sum += a;
        }
        return 0.5 * n * (n + 1) - sum;
    }
};
```



这题还有一种解法，使用位操作Bit Manipulation来解的，用到了异或操作的特性，相似的题目有[Single Number 单独的数字,](http://www.cnblogs.com/Given%20an%20array%20of%20integers,%20every%20element%20appears%20three%20times%20except%20for%20one.%20Find%20that%20single%20one.%20%20Note:%20Your%20algorithm%20should%20have%20a%20linear%20runtime%20complexity.%20Could%20you%20implement%20it%20without%20using%20extra%20memory)[Single Number II 单独的数字之二](http://www.cnblogs.com/grandyang/p/4263927.html)和[Single Number III 单独的数字之三](http://www.cnblogs.com/grandyang/p/4741122.html)。那么思路是既然0到n之间少了一个数，我们将这个少了一个数的数组合0到n之间完整的数组异或一下，那么相同的数字都变为0了，剩下的就是少了的那个数字了，参加代码如下：



解法二：

```
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int res = 0;
        for (int i = 0; i < nums.size(); ++i) {
            res ^= (i + 1) ^ nums[i];
        }
        return res;
    }
};
```



这道题还可以用二分查找法来做，我们首先要对数组排序，然后我们用二分查找法算出中间元素的下标，然后用元素值和下标值之间做对比，如果元素值大于下标值，则说明缺失的数字在左边，此时将right赋为mid，反之则将left赋为mid+1。那么看到这里，作为读者的你可能会提出，排序的时间复杂度都不止O(n)，何必要多此一举用二分查找，还不如用上面两种方法呢。对，你说的没错，但是在面试的时候，有可能人家给你的数组就是排好序的，那么此时用二分查找法肯定要优于上面两种方法，所以这种方法最好也要掌握以下~



解法三：

```
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int left = 0, right = nums.size();
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] > mid) right = mid;
            else left = mid + 1;
        }
        return right;
    }
};
```



在CareerCup中有一道类似的题，[5.7 Find Missing Integer 查找丢失的数](http://www.cnblogs.com/grandyang/p/4747429.html)，但是那道题不让我们直接访问整个int数字，而是只能访问其二进制表示数中的某一位，强行让我们使用位操作Bit Manipulation来解题，也是蛮有意思的一道题。



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












