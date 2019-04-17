# leetcode-46. Permutations 全排列 - 别说话写代码的博客 - CSDN博客





2019年01月08日 19:51:53[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：31
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)










|Given a collection of **distinct** integers, return all possible permutations.**Example:****Input:** [1,2,3]**Output:**[  [1,2,3],  [1,3,2],  [2,1,3],  [2,3,1],  [3,1,2],  [3,2,1]]|给定一个**没有重复**数字的序列，返回其所有可能的全排列。**示例:****输入:** [1,2,3]**输出:**[  [1,2,3],  [1,3,2],  [2,1,3],  [2,3,1],  [3,1,2],  [3,2,1]]|
|----|----|



思路：排列组合，以及求所有可能解这一类题目都可以用dfs或者bfs。此题用dfs。具体解释可以 [参考这里](https://blog.csdn.net/qq_21997625/article/details/84674401)。

![](http://hi.csdn.net/attachment/201107/29/0_1311908383NcUG.gif)

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        dfs(nums,0,res);
        return res;
    }
    void dfs(vector<int>& nums,int start,vector<vector<int>> & res)
    {
        if(start>=nums.size()) res.push_back(nums);
        for(int i=start;i<nums.size();++i)
        {
            swap(nums[i],nums[start]);
            dfs(nums,start+1,res);
            swap(nums[i],nums[start]);
        }
    }
};
```

```python
class Solution:
    def permute(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        def dfs(nums,start,res):
            if start>=len(nums):
                res.append(nums+[])
            for i in range(start,len(nums)):
                nums[start],nums[i] = nums[i],nums[start]
                dfs(nums,start+1,res)
                nums[start],nums[i] = nums[i],nums[start]
        res=[]
        dfs(nums,0,res)
        return res
```





