
# 350. Intersection of Two Arrays II - OraYang的博客 - CSDN博客

2017年08月07日 10:00:54[OraYang](https://me.csdn.net/u010665216)阅读数：127标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two arrays, write a function to compute their intersection.
Example:
Givennums1=`[1, 2, 2, 1]`,nums2=`[2,
 2]`, return`[2, 2]`.
Note:
Each element in the result should appear as many times as it shows in both arrays.
The result can be in any order.
思路：本题与前一篇博客：[Intersection of Two Arrays](http://blog.csdn.net/u010665216/article/details/76804900)有些许的不同，前一篇博客，是找到交叉线，不保留重复值，本篇博客是找到交叉项，保留重复值。本题采用find函数
代码：

```python
class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        for(int j = 0;j<nums1.size();j++)
        {
            vector<int>::iterator itr = find(nums2.begin(),nums2.end(),nums1[j]);
            if(itr!=nums2.end())
            {
                res.push_back(*itr);
                nums2.erase(itr);
                
            }
                
        }
        return res;
        
    }
};
```


