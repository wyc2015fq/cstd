
# 268. Missing Number - OraYang的博客 - CSDN博客

2017年08月04日 09:46:09[OraYang](https://me.csdn.net/u010665216)阅读数：120标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array containingndistinct numbers taken from`0, 1, 2, ..., n`,
 find the one that is missing from the array.
For example,
Givennums=`[0, 1, 3]`return`2`.
思路：本题其实比较有趣，方法有很多，本文介绍两种主流方法，第一种"sub solution"，顾名思义做减法，一个数组存在一个缺失值，原先数组是连续数字组成，那么做减法就可以了；第二种思路是“bit solution"位操作本质上与减法操作并无不同，只要原先数组与缺失值数组逐位相与即得缺失值
代码：

```python
//sub solution
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int sum =0;
        int n = nums.size();
        for(auto& a:nums)
            sum+=a;
    return 0.5 * n * (n + 1) - sum;     
    }
};
```
代码：
```python
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int res =0;
        for(int i=0;i<nums.size();i++)
            res^=(i+1)^nums[i];
        return res;
    }
};
```




