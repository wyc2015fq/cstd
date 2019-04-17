# [LintCode] Find the Missing Number 寻找丢失的数字 - Grandyang - 博客园







# [[LintCode] Find the Missing Number 寻找丢失的数字](https://www.cnblogs.com/grandyang/p/4781519.html)







Given an array contains *N* numbers of 0 .. *N*, find which number doesn't exist in the array.

**Example**

Given *N* = `3` and the array `[0, 1, 3]`, return `2`.



**Challenge**

Do it in-place with O(1) extra memory and O(n) time.



这道题是LeetCode上的原题，请参见我之前的博客[Missing Number 丢失的数字](http://www.cnblogs.com/grandyang/p/4756677.html)。那道题用了两种方法解题，但是LintCode的OJ更加严格，有一个超大的数据集，求和会超过int的范围，所以对于解法一的话需要用long来计算数组之和，其余部分都一样，记得最后把结果转成int即可，参见代码如下：



解法一：

```
class Solution {
public:
    /**    
     * @param nums: a vector of integers
     * @return: an integer
     */
    int findMissing(vector<int> &nums) {
        // write your code here
        long sum = 0, n = nums.size();
        for (auto &a : nums) {
            sum += a;
        }
        return (int)(n * (n + 1) * 0.5 - sum);
    }
};
```



用位操作Bit Manipulation和之前没有区别，参见代码如下：



解法二：

```
class Solution {
public:
    /**    
     * @param nums: a vector of integers
     * @return: an integer
     */
    int findMissing(vector<int> &nums) {
        // write your code here
        int res = 0;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); ++i) {
            res ^= nums[i] ^ (i + 1);
        }
        return res;
    }
};
```
















