# LeetCode Minimum Size Subarray Sum （最短子序列和） - xcw0754 - 博客园
# [LeetCode Minimum Size Subarray Sum （最短子序列和）](https://www.cnblogs.com/xcw0754/p/4654474.html)
题意：给一个序列，找出其中一个连续子序列，其和大于s但是所含元素最少。返回其长度。0代表整个序列之和均小于s。
思路：O(n)的方法容易想。就是扫一遍，当子序列和大于s时就一直删减子序列前面的一个元素，直到小于s就停下，继续累加后面的。
```
1 class Solution {
 2 public:
 3     int minSubArrayLen(int s, vector<int>& nums) {
 4         int ans=0x7fffffff, cnt=0, sum=0;
 5         for(int i=0; i<nums.size(); i++)
 6         {
 7             cnt++;//记录个数
 8             sum+=nums[i];
 9             while(sum>=s)
10             {
11                 ans=min(ans,cnt);//答案
12                 sum-=nums[i-cnt+1];//去掉子序列最前面的元素
13                 cnt--;
14             }
15         }
16         if(ans<0x7fffffff)    return ans;
17         else return 0;
18     }
19 };
AC代码
```

