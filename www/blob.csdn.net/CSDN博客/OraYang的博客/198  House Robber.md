
# 198. House Robber - OraYang的博客 - CSDN博客

2017年07月29日 10:58:50[OraYang](https://me.csdn.net/u010665216)阅读数：176所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed, the only constraint stopping you from robbing each of them is that adjacent houses have security system connected andit
 will automatically contact the police if two adjacent houses were broken into on the same night.
Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonightwithout alerting the police.
思路：本题有两种思路：1.动态规划，状态转移方程： num[i] = (max(nums[i]+num[i-2],num[i-1]))，2.奇偶更新
代码：
动态规划：

```python
class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.size()<=1)
            return nums.empty()?0:nums[0];
        vector<int> num = {nums[0],max(nums[0],nums[1])};
        for(int i=2;i<nums.size();i++)
        {
            num.push_back(max(nums[i]+num[i-2],num[i-1]));
        }
        return num.back();
    }
};
```
奇偶更新：

```python
class Solution {
public:
    int rob(vector<int>& nums) {
        int res0 = 0,res1 = 0;
        for(int i = 0 ;i<nums.size();i++)
        {
            if(i%2==0)
            {
                res0+=nums[i];
                res0 = max(res0,res1);
            }
            else
            {
                res1+=nums[i];
                res1 = max(res0,res1);
            }
           
        }
        return res0>res1?res0:res1;
    }
};
```


