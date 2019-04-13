
# 448. Find All Numbers Disappeared in an Array - OraYang的博客 - CSDN博客

2017年08月16日 14:24:22[OraYang](https://me.csdn.net/u010665216)阅读数：102标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array of integers where 1 ≤ a[i] ≤n(n= size of array), some elements appear twice and others appear once.
Find all the elements of [1,n] inclusive that do not appear in this array.
Could you do it without extra space and in O(n) runtime? You may assume the returned list does not count as extra space.
Example:
Input:[4,3,2,7,8,2,3,1]Output:[5,6]思路：找缺失项，构建map容器遍历对比即可
代码：

```python
class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        unordered_map<int,bool> mp;
        for(int j =0;j<nums.size();j++){
            mp[nums[j]] = true;
        }
        vector<int> res;
        for(int i=0;i<nums.size();i++)
        {
            if(mp.find(i+1)==mp.end())
            {
                res.push_back(i+1);
            }           
        }
        return res;
    }
};
```


