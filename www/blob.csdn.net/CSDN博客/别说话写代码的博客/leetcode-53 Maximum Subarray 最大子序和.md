# leetcode-53. Maximum Subarray 最大子序和 - 别说话写代码的博客 - CSDN博客





2019年01月10日 19:49:03[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：23标签：[最大子序和																[Maximum Subarray](https://so.csdn.net/so/search/s.do?q=Maximum Subarray&t=blog)](https://so.csdn.net/so/search/s.do?q=最大子序和&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)







|Given an integer array `nums`, find the contiguous subarray (containing at least one number) which has the largest sum and return its sum.**Example:****Input:** [-2,1,-3,4,-1,2,1,-5,4],**Output:** 6**Explanation:** [4,-1,2,1] has the largest sum = 6.**Follow up:**If you have figured out the O(*n*) solution, try coding another solution using the divide and conquer approach, which is more subtle.|给定一个整数数组 `nums` ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。**示例:****输入:** [-2,1,-3,4,-1,2,1,-5,4],**输出:** 6**解释:** 连续子数组 [4,-1,2,1] 的和最大，为 6。**进阶:**如果你已经实现复杂度为 O(*n*) 的解法，尝试使用更为精妙的分治法求解。|
|----|----|

思路：O（n）遍历一遍，max初始0，res初始最小整数。 用max保存当前子序列最大和，若max小于0，就说明当前元素加上max只会更小，所以此时max=nums[i]，否则max+=nums[i]。如果mex和大于res，res=max；

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n=nums.size();
        if(n==0) return 0;
        int curmax=0,res=INT_MIN;
        for(int i=0;i<n;++i)
        {
            if(curmax<=0) curmax=nums[i];
            else curmax+=nums[i];
            if(curmax>res) res=curmax;
        }
        return res;
    }
};
```






