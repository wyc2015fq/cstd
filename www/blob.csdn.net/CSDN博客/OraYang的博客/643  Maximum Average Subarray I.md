
# 643. Maximum Average Subarray I - OraYang的博客 - CSDN博客

2017年08月30日 10:01:32[OraYang](https://me.csdn.net/u010665216)阅读数：208所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array consisting of`n`integers, find the contiguous subarray of given length`k`that
 has the maximum average value. And you need to output the maximum average value.
Example 1:
Input:[1,12,-5,-6,50,3], k = 4Output:12.75Explanation:Maximum average is (12-5-6+50)/4 = 51/4 = 12.75
Note:
1 <=k<=n<= 30,000.
Elements of the given array will be in the range [-10,000, 10,000].
思路：本题一开始简单直接的思路是利用两层for循环，但是结果会time limit，那是每层循环都会有重复计算项，那么本题建立一个长度为k的窗口，从左至右依次滑动一个数字，每次的和就是加上最新的一个数，减去最左边的数。
代码：

```python
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        double res = -10000*k;
        double sumnum = accumulate(nums.begin(),nums.begin()+k,0);
        res = res<sumnum?sumnum:res;
        for(int i=k;i<nums.size();i++)
        {
            sumnum = sumnum+nums[i]-nums[i-k];
            res = res<sumnum?sumnum:res;
        }
        return res/k*1.0;
    }
};
```


