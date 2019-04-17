# 238. Product of Array Except Self 除自身以外数组的乘积 - 别说话写代码的博客 - CSDN博客





2019年02月24日 20:20:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13标签：[Product of Array Except Self 																[leetcode																[除自身以外数组的乘积](https://so.csdn.net/so/search/s.do?q=除自身以外数组的乘积&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given an array `nums` of *n* integers where *n* > 1,  return an array `output` such that `output[i]` is equal to the product of all the elements of `nums` except `nums[i]`.**Example:****Input:**`[1,2,3,4]`**Output:**`[24,12,8,6]`**Note: **Please solve it **without division** and in O(*n*).**Follow up:**			Could you solve it with constant space complexity? (The output array **does not** count as extra space for the purpose of space complexity analysis.)|给定长度为 *n* 的整数数组 `nums`，其中 *n* > 1，返回输出数组 `output` ，其中 `output[i]` 等于 `nums` 中除 `nums[i]` 之外其余各元素的乘积。**示例:****输入:**`[1,2,3,4]`**输出:**`[24,12,8,6]`**说明: **请**不要使用除法，**且在 O(*n*) 时间复杂度内完成此题。**进阶：**			你可以在常数空间复杂度内完成这个题目吗？（ 出于对空间复杂度分析的目的，输出数组**不被视为**额外空间。）|
|----|----|

思路：题目说 不能用除法，而且必须O(n)。那就用两个数组，f[i]保存第i个元素之前所有元素 乘积，b[i]表示第i个 元素之后所有元素乘积，那第i个位置的除第i个位置值以外其余元素乘积为f[i]*b[i]; 比如[1,2,3,4]，则f数组为[1,1,2,6],b数组元素为[24,12,8,6]，刚好第i个 位置的值 就是f[i]*b[i];

```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        if(nums.empty()) return {};
        int n=nums.size();
        vector<int> f(n,1),b(n,1),res(n,1);
        for(int i=1;i<n;++i) f[i]=f[i-1]*nums[i-1];
        for(int i=n-1;i>0;--i) b[i-1]=b[i]*nums[i];
        for(int i=0;i<n;++i) res[i]=f[i]*b[i];
        return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Product of Array Except Self &t=blog)




