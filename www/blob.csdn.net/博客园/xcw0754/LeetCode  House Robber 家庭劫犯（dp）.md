# LeetCode   House Robber 家庭劫犯（dp） - xcw0754 - 博客园
# [LeetCode   House Robber 家庭劫犯（dp）](https://www.cnblogs.com/xcw0754/p/4498047.html)
题意：有一个整数序列，从中挑出一些数字，使得总和是最大，前提是，相邻的两个数字中只能挑其一。比如1 2 3 就只能挑2或者1和3。
思路：很直观的题，dp思想。降低规模，从小规模开始考虑。如果只有两个数字，那么结果很明显就能知道是其中之大者。假如已经知道了第 i 个之前的决策，那么第i+2个之前的决策也就知道了。前两个数字已经由人工得知，设为dp[0]和dp[1]，那么dp[2]=max(dp[0]+nums[2], dp[1])。状态转移方程dp[i]=max(dp[i-1], dp[i-2]+num[i] )。
这里有状态压缩的思想，只不过状态只有两个，0和1代表前一个数字是否被挑出。即dp数组的下标，1代表i-1个之前的决策结果，也代表了第i-1个已经挑出，所以第i个不能再挑出来了；但是0代表i-2个之前的决策结果，也代表了i-1个不挑出。
```
1 class Solution {
 2 public:
 3     int rob(vector<int>& nums) {
 4         if(nums.empty())    return 0;
 5         if(nums.size()==1)    return nums[0];
 6         if(nums.size()==2)    return max(nums[1],nums[0]);
 7         
 8         int dp[2];
 9         dp[0]=nums[0];    //初始化也是很重要的
10         dp[1]=max(nums[0],nums[1]);
11     
12         for(int i=2; i<nums.size(); i++)
13         {
14             int tmp=max(dp[1],dp[0]+nums[i]);
15             dp[0]=dp[1];//往前移。因为dp[0]已经没作用了
16             dp[1]=tmp;
17         }
18         return dp[1];
19     }
20 };
AC代码
```

