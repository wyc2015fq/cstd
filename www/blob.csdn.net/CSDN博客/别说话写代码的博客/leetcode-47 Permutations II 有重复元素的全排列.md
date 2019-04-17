# leetcode-47. Permutations II 有重复元素的全排列 - 别说话写代码的博客 - CSDN博客





2019年01月08日 20:04:05[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：35
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)








|Given a collection of numbers that might contain duplicates, return all possible unique permutations.**Example:****Input:** [1,1,2]**Output:**[  [1,1,2],  [1,2,1],  [2,1,1]]|给定一个可包含重复数字的序列，返回所有不重复的全排列。**示例:****输入:** [1,1,2]**输出:**[  [1,1,2],  [1,2,1],  [2,1,1]]|
|----|----|

思路：和上一题[全排列](https://blog.csdn.net/qq_21997625/article/details/86093736)一样。 不过就是需要在所有可能后去掉重复的，使用set就可以了。

```cpp
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        set<vector<int>> res;
        permute(nums,0,res);
        return vector<vector<int>> (res.begin(),res.end());
    }
    void permute(vector<int> & nums,int start,set<vector<int>>& res)
    {
        if(start>=nums.size()) res.insert(nums);
        for(int i=start;i<nums.size();++i)
        {
            if(i!=start && nums[i]==nums[start]) continue;   //在这里取消重复
            swap(nums[i],nums[start]);
            permute(nums,start+1,res);
            swap(nums[i],nums[start]);
        }
    }
};
```





