
# 217. Contains Duplicate - OraYang的博客 - CSDN博客

2017年07月31日 08:54:25[OraYang](https://me.csdn.net/u010665216)阅读数：194标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array of integers, find if the array contains any duplicates. Your function should return true if any value appears at least twice in the
 array, and it should return false if every element is distinct.
思路：
本题思路就是建立一个哈希表，扫描一次数组，一旦发现某值再次出现就返回false。
代码：

```python
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        map<int,int> mp;
        if(nums.size()<=1)
            return false;
        for(int i = 0;i<nums.size();i++)
        {
            if(mp.find(nums[i])==mp.end())
                mp[nums[i]] = nums[i];
            else 
                return true;
        }
        return false;        
    }
};
```




