# [LeetCode] Combination Sum II 组合之和之二 - Grandyang - 博客园







# [[LeetCode] Combination Sum II 组合之和之二](https://www.cnblogs.com/grandyang/p/4419386.html)







Given a collection of candidate numbers (*C*) and a target number (*T*), find all unique combinations in *C* where the candidate numbers sums to *T*.

Each number in *C* may only be used once in the combination.

Note:
- All numbers (including target) will be positive integers.
- Elements in a combination (*a*1, *a*2, … , *a*k) must be in non-descending order. (ie, *a*1 ≤ *a*2 ≤ … ≤ *a*k).
- The solution set must not contain duplicate combinations.



For example, given candidate set `10,1,2,7,6,1,5` and target `8`, 
A solution set is: 
`[1, 7]`
`[1, 2, 5]`
`[2, 6]`
`[1, 1, 6]`



这道题跟之前那道[Combination Sum 组合之和](http://www.cnblogs.com/grandyang/p/4419259.html) 本质没有区别，只需要改动一点点即可，之前那道题给定数组中的数字可以重复使用，而这道题不能重复使用，只需要在之前的基础上修改两个地方即可，首先在递归的for循环里加上if (i > start && num[i] == num[i - 1]) continue; 这样可以防止res中出现重复项，然后就在递归调用combinationSum2DFS里面的参数换成i+1，这样就不会重复使用数组中的数字了，代码如下：



```
class Solution {
public:
    vector<vector<int> > combinationSum2(vector<int> &num, int target) {
        vector<vector<int> > res;
        vector<int> out;
        sort(num.begin(), num.end());
        combinationSum2DFS(num, target, 0, out, res);
        return res;
    }
    void combinationSum2DFS(vector<int> &num, int target, int start, vector<int> &out, vector<vector<int> > &res) {
        if (target < 0) return;
        else if (target == 0) res.push_back(out);
        else {
            for (int i = start; i < num.size(); ++i) {
                if (i > start && num[i] == num[i - 1]) continue;
                out.push_back(num[i]);
                combinationSum2DFS(num, target - num[i], i + 1, out, res);
                out.pop_back();
            }
        }
    }
};
```



类似题目：

[Combination Sum III](http://www.cnblogs.com/grandyang/p/4537983.html)

[Combination Sum](http://www.cnblogs.com/grandyang/p/4419259.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












