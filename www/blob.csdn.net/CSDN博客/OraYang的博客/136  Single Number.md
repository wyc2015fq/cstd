
# 136. Single Number - OraYang的博客 - CSDN博客

2017年07月25日 16:41:36[OraYang](https://me.csdn.net/u010665216)阅读数：126标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array of integers, every element appearstwiceexcept for one. Find that single one.
Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
思路：这道题目，算法时间复杂度要O(n)，而且不能有二外的存储空间。研读题目会发现本题数据有个特点就是数组中有且只有一个数字没有重复项，那么根据[异或(XOR)](https://en.wikipedia.org/wiki/Exclusive_or)的衍生的规律：**同一变量与另一变量和其异或值异或等于另一个数**，则可以很轻松地实现相关代码。代码：

```python
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int result = 0;
        for(int i = 0;i<nums.size();i++)
            result^=nums[i];
        return result;
    }
};
```


