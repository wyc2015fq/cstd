# 152. Maximum Product Subarray 乘积最大子序列 - 别说话写代码的博客 - CSDN博客





2019年02月24日 20:03:02[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：14标签：[乘积最大子序列																[Maximum Product Subarray 																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Maximum Product Subarray &t=blog)



|Given an integer array `nums`, find the contiguous subarray within an array (containing at least one number) which has the largest product.**Example 1:****Input:** [2,3,-2,4]**Output:**`6`**Explanation:** [2,3] has the largest product 6.**Example 2:****Input:** [-2,0,-1]**Output:** 0**Explanation:** The result cannot be 2, because [-2,-1] is not a subarray.|给定一个整数数组 `nums` ，找出一个序列中乘积最大的连续子序列（该序列至少包含一个数）。**示例 1:****输入:** [2,3,-2,4]**输出:**`6`**解释:** 子数组 [2,3] 有最大乘积 6。**示例 2:****输入:** [-2,0,-1]**输出:** 0**解释:** 结果不能为 2, 因为 [-2,-1] 不是子数组。|
|----|----|

思路：这个题比较复杂，如果出现0，则乘积肯定是0，还要考虑负号的情况。使用两个dp数组，一个用来保存到当前位置最大乘积数，一个用来保存到当前位置最小乘积数。其中f[i]表示子数组[0, i]范围内并且一定包含nums[i]数字的最大子数组乘积，g[i]表示子数组[0, i]范围内并且一定包含nums[i]数字的最小子数组乘积，初始化时f[0]和g[0]都初始化为nums[0]，其余都初始化为0。那么从数组的**第二个数字**开始遍历，那么此时的最大值和最小值只会在这三个数字之间产生，即f[i-1]*nums[i]，g[i-1]*nums[i]，和nums[i]。所以我们用三者中的最大值来更新f[i]，用最小值来更新g[i]，然后用f[i]和nums[i]之间的最大值来更新结果res即可

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        if(nums.empty()) return 0;
        int res=nums[0],n=nums.size();
        vector<int> f(n,0),g(n,0);
        f[0]=nums[0];
        g[0]=nums[0];
        for(int i=1;i<n;++i)
        {
            f[i]=max(max(f[i-1]*nums[i],g[i-1]*nums[i]),nums[i]);
            g[i]=min(min(f[i-1]*nums[i],g[i-1]*nums[i]),nums[i]);
            res=max(res,f[i]);
        }
        return res;
    }
};
```

参考：[http://www.cnblogs.com/grandyang/p/4028713.html](http://www.cnblogs.com/grandyang/p/4028713.html)](https://so.csdn.net/so/search/s.do?q=乘积最大子序列&t=blog)




