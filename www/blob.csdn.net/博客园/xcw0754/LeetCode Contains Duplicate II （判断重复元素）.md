# LeetCode Contains Duplicate II （判断重复元素） - xcw0754 - 博客园
# [LeetCode Contains Duplicate II （判断重复元素）](https://www.cnblogs.com/xcw0754/p/4619921.html)
![](https://images0.cnblogs.com/blog2015/641737/201507/032312058062575.png)
题意：如果有两个相同的元素，它们之间的距离不超过k，那么返回true，否则false。
思路：用map记录每个出现过的最近的位置，扫一边序列即可。扫到一个元素就判断它在前面什么地方出现过。本题数据有点弱。
```
1 class Solution {
 2 public:
 3     bool containsNearbyDuplicate(vector<int>& nums, int k) {
 4         if(nums.empty())   return false;
 5         unordered_map<int,int> mapp;
 6         nums.insert(nums.begin(),0);
 7         for(int i=1; i<nums.size(); i++){
 8             if(!mapp[nums[i]])    mapp[nums[i]]=i;  //第一次出现
 9             else{
10                 if(i-mapp[nums[i]]<=k )  return true;    //已经出现，且符合条件
11                 mapp[nums[i]]=i;    //不符合，更新最近一个位置
12             }
13         }
14         return false;
15     }
16 };
AC代码
```

