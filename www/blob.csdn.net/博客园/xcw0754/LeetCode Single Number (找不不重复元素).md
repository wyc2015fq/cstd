# LeetCode Single Number (找不不重复元素) - xcw0754 - 博客园
# [LeetCode Single Number (找不不重复元素)](https://www.cnblogs.com/xcw0754/p/4657914.html)
题意：给一个序列，序列中只有1个是单个的，其他都是成对出现的。也就是序列中有奇数个元素。要求找出这个元素。
思路：成对出现用异或最好了。两个同样的数一异或就变零，剩下的，就是那个落单的。
```
1 class Solution {
 2 public:
 3     int singleNumber(vector<int>& nums) {
 4         int res=0;
 5         for(int i=0; i<nums.size(); i++)
 6             res^=nums[i];
 7         return res;
 8         
 9     }
10 };
AC代码
```

