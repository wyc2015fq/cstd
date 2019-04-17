# leetcode-39. Combination Sum 组合总和 - 别说话写代码的博客 - CSDN博客





2019年01月08日 19:11:55[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：21标签：[python leetcode 																[Combination Sum 																[组合总和](https://so.csdn.net/so/search/s.do?q=组合总和&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Combination Sum &t=blog)




题目：
|Given a **set** of candidate numbers (`candidates`) **(without duplicates)** and a target number (`target`), find all unique combinations in `candidates` where the candidate numbers sums to `target`.The **same** repeated number may be chosen from `candidates` unlimited number of times.**Note:**- All numbers (including `target`) will be positive integers.- The solution set must not contain duplicate combinations.**Example 1:****Input:** candidates = `[2,3,6,7], `target = `7`,**A solution set is:**[  [7],  [2,2,3]]**Example 2:****Input:** candidates = [2,3,5]`, `target = 8,**A solution set is:**[  [2,2,2,2],  [2,3,3],  [3,5]]|给定一个**无重复元素**的数组 `candidates` 和一个目标数 `target` ，找出 `candidates` 中所有可以使数字和为 `target` 的组合。`candidates` 中的数字可以无限制重复被选取。**说明：**- 所有数字（包括 `target`）都是正整数。- 解集不能包含重复的组合。 **示例 1:****输入:** candidates = `[2,3,6,7], `target = `7`,**所求解集为:**[  [7],  [2,2,3]]**示例 2:****输入:** candidates = [2,3,5]`, `target = 8,**所求解集为:**[  [2,2,2,2],  [2,3,3],  [3,5]]|
|----|----|

思路：看到这种求所有解的情况，类似于排列组合，可以用dfs或者bfs。此题用dfs写一个递归程序就可以求所有 可能情况了。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        dfs(candidates,target,0,{},res);
        return res;
    }
    void dfs(vector<int>& candidates, int target,int start,vector<int> out,
             vector<vector<int>> &res)
    {
        if(target<0) return;
        else if(target==0) 
        {
            res.push_back(out);
            return;
        }
        for(int i=start;i<candidates.size();++i)
        {
            out.push_back(candidates[i]);
            dfs(candidates,target-candidates[i],i,out,res);
            out.pop_back();
        }
    }
};
```

```python
class Solution:
    def combinationSum(self, candidates, target):
        """
        :type candidates: List[int]
        :type target: int
        :rtype: List[List[int]]
        """
        def dfs(candidates,target,start,path,res):
            if target==0:
                return res.append(path+[])
            for i in range(start,len(candidates)):
                if target-candidates[i]>=0:
                    path.append(candidates[i])
                    dfs(candidates,target-candidates[i],i,path,res)
                    path.pop()
        
        res=[]
        dfs(candidates,target,0,[],res)
        return res
```](https://so.csdn.net/so/search/s.do?q=python leetcode &t=blog)




