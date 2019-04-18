# LeetCode  Maximum Subarray （最大子段和） - xcw0754 - 博客园
# [LeetCode  Maximum Subarray （最大子段和）](https://www.cnblogs.com/xcw0754/p/4921733.html)
题意：
　　给一个序列，求至少含一个元素的最大子段和？
思路：
　　跟求普通的最大子段和差不多，只不过需要注意一下顺序。由于至少需要一个元素，所以先将ans=nums[0]。接下来可以用sum求和了，如果小于0就重置为0，表示重新开始累加。注意代码中两个if的顺序不可以呼唤。
```
1 class Solution {
 2 public:
 3     int maxSubArray(vector<int>& nums) {
 4         int sum=0, ans=nums[0];
 5         for(int i=0; i<nums.size(); i++)
 6         {
 7             sum+=nums[i];
 8             if(sum>ans)    ans=sum;
 9             if(sum<0)    sum=0;
10         }
11         return ans;
12     }
13 };
AC代码
```

