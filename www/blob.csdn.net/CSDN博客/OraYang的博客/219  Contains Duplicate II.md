
# 219. Contains Duplicate II - OraYang的博客 - CSDN博客

2017年07月31日 09:03:54[OraYang](https://me.csdn.net/u010665216)阅读数：192标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array of integers and an integerk,
 find out whether there are two distinct indicesiandjin
 the array such thatnums[i] = nums[j]and
 theabsolutedifference
 betweeniandjis
 at mostk.
思路：
本题是从上一篇博客[217.Contains Duplicate](http://blog.csdn.net/u010665216/article/details/76409371)衍生而来，思路比较简单，还是要利用哈希表这一数据结构，其次哈希表每次映射的不再是数值本身，而是数组对应的索引
代码：

```python
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        map<int,int> mp;
        if(nums.size()<=1)
            return false;
        for(int i = 0;i<nums.size();i++)
        {
            if(mp.find(nums[i])==mp.end())
                mp[nums[i]] = i;
            else if(abs(i-mp[nums[i]])<=k)
                return true;
            else 
                mp[nums[i]] = i;
        }
        return false;   
        
    }
};
```


