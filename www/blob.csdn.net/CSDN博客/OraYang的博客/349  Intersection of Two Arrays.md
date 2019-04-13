
# 349. Intersection of Two Arrays - OraYang的博客 - CSDN博客

2017年08月07日 09:39:05[OraYang](https://me.csdn.net/u010665216)阅读数：124标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two arrays, write a function to compute their intersection.
Example:
Givennums1=`[1, 2, 2, 1]`,nums2=`[2,
 2]`, return`[2]`.
Note:
Each element in the result must be unique.
The result can be in any order.
思路：
1.去除重复值
2.查找交叉项
利用C++ set标准数据结构，并巧用find查找函数
代码：

```python
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        set<int> num1;
        set<int> num2;
        for(int i = 0;i<nums1.size();i++)
            num1.insert(nums1[i]);
        for(int i = 0;i<nums2.size();i++)
            num2.insert(nums2[i]);
        for(set<int>::iterator itr = num2.begin();itr!=num2.end();itr++)
        {
            if(num1.find(*itr)!=num1.end())
                res.push_back(*itr);
        }
        return res;
    }
};
```




