
# 532. K-diff Pairs in an Array - OraYang的博客 - CSDN博客

2017年08月24日 10:33:55[OraYang](https://me.csdn.net/u010665216)阅读数：119标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array of integers and an integerk, you need to find the number ofuniquek-diff pairs in the array. Here ak-diffpair is defined as an
 integer pair (i, j), whereiandjare both numbers in the array and their[absolute
 difference](https://en.wikipedia.org/wiki/Absolute_difference)isk.
Example 1:
Input:[3, 1, 4, 1, 5], k = 2Output:2Explanation:There are two 2-diff pairs in the array, (1, 3) and (3, 5).
Although we have two 1s in the input, we should only return the number ofuniquepairs.
Example 2:
Input:[1, 2, 3, 4, 5], k = 1Output:4Explanation:There are four 1-diff pairs in the array, (1, 2), (2, 3), (3, 4) and (4, 5).
Example 3:
Input:[1, 3, 1, 5, 4], k = 0Output:1Explanation:There is one 0-diff pair in the array, (1, 1).
Note:
The pairs (i, j) and (j, i) count as the same pair.
The length of the array won't exceed 10,000.
All the integers in the given input belong to the range: [-1e7, 1e7].
思路：本题思路如下，分为如下几个步骤：
step1:先对数组进行排序
step2:查找当前数与k的差值是否在数组中存在（当前位置的数字除外）
step3:剔除重复项
代码：

```python
class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        sort(nums.begin(),nums.end());
        int count = 0;
        vector<pair<int,int>> res;
        if(k<0)
            return 0;
        for(int i =nums.size()-1;i>0;i--)
        {
            if(find(nums.begin(),nums.end(),nums[i]-k)!=nums.end()&&find(nums.begin(),nums.end(),nums[i]-k)!=nums.begin()+i)
            {
                res.push_back(make_pair(nums[i],nums[i]-k));
                nums.erase(nums.begin()+i);
                count++;
            }
        }
        for(int j=1;j<res.size();j++)
        {
            if(res[j].first==res[j-1].first)
                count--;
        }
        return count; 
    }
};
```


