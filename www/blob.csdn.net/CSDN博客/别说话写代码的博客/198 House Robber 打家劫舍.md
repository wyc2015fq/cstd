# 198. House Robber 打家劫舍 - 别说话写代码的博客 - CSDN博客





2019年01月27日 13:41:01[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33标签：[打家劫舍																[House Robber																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=House Robber&t=blog)



|You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed, the only constraint stopping you from robbing each of them is that adjacent houses have security system connected and **it will automatically contact the police if two adjacent houses were broken into on the same night**.Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonight **without alerting the police**.**Example 1:****Input:** [1,2,3,1]**Output:** 4**Explanation:** Rob house 1 (money = 1) and then rob house 3 (money = 3).             Total amount you can rob = 1 + 3 = 4.**Example 2:****Input:** [2,7,9,3,1]**Output:** 12**Explanation:** Rob house 1 (money = 2), rob house 3 (money = 9) and rob house 5 (money = 1).             Total amount you can rob = 2 + 9 + 1 = 12.|你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，**如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警**。给定一个代表每个房屋存放金额的非负整数数组，计算你**在不触动警报装置的情况下，**能够偷窃到的最高金额。**示例 1:****输入:** [1,2,3,1]**输出:** 4**解释:** 偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。     偷窃到的最高金额 = 1 + 3 = 4 。**示例 2:****输入:** [2,7,9,3,1]**输出:** 12**解释:** 偷窃 1 号房屋 (金额 = 2), 偷窃 3 号房屋 (金额 = 9)，接着偷窃 5 号房屋 (金额 = 1)。     偷窃到的最高金额 = 2 + 9 + 1 = 12 。|
|----|----|

思路：在数组中求一个或多个 不相邻数的最大和，用dp。主要就是看dp方程是啥dp[i]=max(dp[i-1],dp[i-2]+nums[i]).

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n=nums.size();
        if(!n) return 0;
        if(n==1) return nums[0];
        vector<int> dp={nums[0],max(nums[0],nums[1])};
        for(int i=2;i<n;++i)
            dp.push_back(max(nums[i]+dp[i-2],dp[i-1]));
        return dp.back();
    }
};
```](https://so.csdn.net/so/search/s.do?q=打家劫舍&t=blog)




