# LeetCode  Longest Increasing Subsequence （LIS O(nlogn)） - xcw0754 - 博客园
# [LeetCode  Longest Increasing Subsequence （LIS O(nlogn)）](https://www.cnblogs.com/xcw0754/p/4970213.html)
题意：
　　给一个数组，求严格递增的最长递增**子序列**的长度。
思路：
　　开销是一个额外的O(n)的数组。lower_bound(begin,end,val)的功能是：返回第一个大于等于val的地址。
```
1 class Solution {
 2 public:
 3     int lengthOfLIS(vector<int>& nums) {
 4         if(nums.empty())    return 0;
 5         int *p=new int[nums.size()];
 6         p[0]=nums[0];
 7         int len=0;
 8         for(int i=1; i<nums.size(); i++)    
 9         {
10             if(nums[i]>p[len])
11                 p[++len]=nums[i];
12             else
13                 *lower_bound(p,p+len+1,nums[i])=nums[i];
14         }
15         delete []p;
16         return ++len;
17     }
18 };
AC代码
```

