# [LeetCode] Largest Palindrome Product 最大回文串乘积 - Grandyang - 博客园







# [[LeetCode] Largest Palindrome Product 最大回文串乘积](https://www.cnblogs.com/grandyang/p/7644725.html)








Find the largest palindrome made from the product of two n-digit numbers.

Since the result could be very large, you should return the largest palindrome mod 1337.

Example:

Input: 2

Output: 987

Explanation: 99 x 91 = 9009, 9009 % 1337 = 987



Note:

The range of n is [1,8].



这道题给我们一个数字n，问两个n位数的乘积组成的最大回文数是多少，返回的结果对1337取余。博主刚开始用暴力搜索做，遍历所有的数字组合，求乘积，再来判断是否是回文数，最终TLE了，只能换一种思路来做。论坛上的这种思路真心叼啊，博主感觉这题绝比不该Easy啊。首先我们还是要确定出n位数的范围，最大值upper，可以取到，最小值lower，不能取到。然后我们遍历这区间的所有数字，对于每个遍历到的数字，我们用当前数字当作回文数的前半段，将其翻转一下拼接到后面，此时组成一个回文数，这里用到了一个规律，当n>1时，两个n位数乘积的最大回文数一定是2n位的。下面我们就要来验证这个回文数能否由两个n位数相乘的来，我们还是遍历区间中的数，从upper开始遍历，但此时结束位置不是lower，而是当前数的平方大于回文数，因为我们遍历的是相乘得到回文数的两个数中的较大数，一旦超过这个范围，就变成较小数了，就重复计算了。比如对于回文数9009，其是由99和91组成的，其较大数的范围是[99,95]，所以当遍历到94时，另一个数至少需要是95，而这种情况在之前已经验证过了。当回文数能整除较大数时，说明是成立的，直接对1337取余返回即可，参见代码如下：



```
class Solution {
public:
    int largestPalindrome(int n) {
        int upper = pow(10, n) - 1, lower = upper / 10;
        for (int i = upper; i > lower; --i) {
            string t = to_string(i);
            long p = stol(t + string(t.rbegin(), t.rend()));
            for (long j = upper; j * j > p; --j) {
                if (p % j == 0) return p % 1337;
            }
        }
        return 9;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/74372/an-easy-9-line-java-solution](https://discuss.leetcode.com/topic/74372/an-easy-9-line-java-solution)

[https://discuss.leetcode.com/topic/74125/java-solution-using-assumed-max-palindrom](https://discuss.leetcode.com/topic/74125/java-solution-using-assumed-max-palindrom)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













