
# 628. Maximum Product of Three Numbers - OraYang的博客 - CSDN博客

2017年08月29日 11:18:59[OraYang](https://me.csdn.net/u010665216)阅读数：212标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an integer array, find three numbers whose product is maximum and output the maximum product.
Example 1:
Input:[1,2,3]Output:6
Example 2:
Input:[1,2,3,4]Output:24
Note:
The length of the given array will be in range [3,104] and all elements are in the range [-1000, 1000].
Multiplication of any three numbers in the input won't exceed the range of 32-bit signed integer.
思路：
本题先将数组排序，因为存在整数为负的情况，因此，需要对后三个数的乘积与前两个数及末尾数乘积作个比较
代码：

```python
class Solution {
public:
    int maximumProduct(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int a = nums[nums.size()-1]*nums[nums.size()-2]*nums[nums.size()-3];
        int b = nums[nums.size()-1]*nums[0]*nums[1];
        return a>b?a:b;
    }
};
```




