# LeetCode  Search Insert Position （二分查找） - xcw0754 - 博客园
# [LeetCode  Search Insert Position （二分查找）](https://www.cnblogs.com/xcw0754/p/4921625.html)
题意：
　　给一个升序的数组，如果target在里面存在了，返回其下标，若不存在，返回其插入后的下标。
思路：
　　来一个简单的二分查找就行了，注意边界。
```
1 class Solution {
 2 public:
 3     int searchInsert(vector<int>& nums,int target)
 4     {
 5         int L=0, R=nums.size();
 6         while(L<R)
 7         {
 8             int mid=R-(R-L+1)/2;
 9             if(nums[mid]>=target)    R=mid;
10             else    L=mid+1;    
11         }
12         return R;
13     }
14 };
AC代码
```

