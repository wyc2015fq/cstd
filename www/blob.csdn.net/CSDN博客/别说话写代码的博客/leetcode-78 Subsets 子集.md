# leetcode-78. Subsets 子集 - 别说话写代码的博客 - CSDN博客





2019年03月17日 15:47:25[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：6标签：[Subsets																[78.Subsets																[leetcode																[leetcode 78																[leetcode 78 子集](https://so.csdn.net/so/search/s.do?q=leetcode 78 子集&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode 78&t=blog)



|Given a set of **distinct** integers, *nums*, return all possible subsets (the power set).**Note:** The solution set must not contain duplicate subsets.**Example:****Input:** nums = [1,2,3]**Output:**[  [3],  [1],  [2],  [1,2,3],  [1,3],  [2,3],  [1,2],  []]|给定一组**不含重复元素**的整数数组 *nums*，返回该数组所有可能的子集（幂集）。**说明：**解集不能包含重复的子集。**示例:****输入:** nums = [1,2,3]**输出:**[  [3],  [1],  [2],  [1,2,3],  [1,3],  [2,3],  [1,2],  []]|
|----|----|

思路：

**第一种**：dfs。类似于排列组合那种方法

```cpp
class Solution {
public:
    void dfs(vector<int> &nums,vector<vector<int>> &res,vector<int> &out,int n)
    {
        res.push_back(out);
        for(int i=n;i<nums.size();++i)
        {
            out.push_back(nums[i]);
            dfs(nums,res,out,i+1);
            out.pop_back();
        }
    }
    vector<vector<int>> subsets(vector<int>& nums) {
        if(nums.empty()) return {};
        vector<vector<int>> res;
        vector<int> out;
        dfs(nums,res,out,0);
        return res;
    }
};
```

**第二种**：动态增长法。初始时，res中只有一个[]，接下来将1加入[]变为[1]，现在res中所有 元素为[],[1],再将2加入已有的元素序列里面为[2],[1,2]， 现在res中元素为[],[1],[2],[1,2]，再将3加入上面已有的元素序列为[3],[1,3],[2,3],[1,2,3]，那当前元素为[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]

```cpp
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res(1);
        for(int i=0;i<nums.size();++i)
        {
            int size = res.size();    //如果不加这一句res.size()会动态增长
            for(int j=0;j<size;++j)
            {
                res.push_back(res[j]);
                res.back().push_back(nums[i]);
            }
        }return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)](https://so.csdn.net/so/search/s.do?q=78.Subsets&t=blog)](https://so.csdn.net/so/search/s.do?q=Subsets&t=blog)




