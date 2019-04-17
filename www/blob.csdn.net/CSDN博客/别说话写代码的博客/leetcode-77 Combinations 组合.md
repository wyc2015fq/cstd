# leetcode-77. Combinations 组合 - 别说话写代码的博客 - CSDN博客





2019年03月11日 22:42:24[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：31标签：[组合																[Combinations																[leetcode 77](https://so.csdn.net/so/search/s.do?q=leetcode 77&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Combinations&t=blog)



|Given two integers *n* and *k*, return all possible combinations of *k* numbers out of 1 ... *n*.**Example:****Input:** n = 4, k = 2**Output:**[  [2,4],  [3,4],  [2,3],  [1,2],  [1,3],  [1,4],]|给定两个整数 *n* 和 *k*，返回 1 ... *n *中所有可能的 *k* 个数的组合。**示例:****输入:** n = 4, k = 2**输出:**[  [2,4],  [3,4],  [2,3],  [1,2],  [1,3],  [1,4],]|
|----|----|

 思路：dfs

```cpp
class Solution {
public:
    void dfs(int n,int k,int cur,vector<int> out,vector<vector<int>> & res)
    {
        if(out.size()==k)
        {
            res.push_back(out);
            return;
        }
        for(int i=cur;i<=n;++i)
        {
            out.push_back(i);
            dfs(n,k,i+1,out,res);
            out.pop_back();
        }
    }
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> res;
        dfs(n,k,1,{},res);
        return res;
        
    }
};
```](https://so.csdn.net/so/search/s.do?q=组合&t=blog)




