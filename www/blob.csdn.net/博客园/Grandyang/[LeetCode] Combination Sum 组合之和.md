# [LeetCode] Combination Sum 组合之和 - Grandyang - 博客园







# [[LeetCode] Combination Sum 组合之和](https://www.cnblogs.com/grandyang/p/4419259.html)









Given a set of candidate numbers (`candidates`) (without duplicates) and a target number (`target`), find all unique combinations in `candidates` where the candidate numbers sums to `target`.

The same repeated number may be chosen from `candidates` unlimited number of times.

Note:
- All numbers (including `target`) will be positive integers.
- The solution set must not contain duplicate combinations.

Example 1:
Input: candidates = `[2,3,6,7], `target = `7`,
A solution set is:
[
  [7],
  [2,2,3]
]

Example 2:
Input: candidates = [2,3,5]`, `target = 8,
A solution set is:
[
  [2,2,2,2],
  [2,3,3],
  [3,5]
]





像这种结果要求返回所有符合要求解的题十有八九都是要利用到递归，而且解题的思路都大同小异，相类似的题目有 [Path Sum II](http://www.cnblogs.com/grandyang/p/4042156.html)，[Subsets II](http://www.cnblogs.com/grandyang/p/4310964.html)，[Permutations](http://www.cnblogs.com/grandyang/p/4358848.html)，[Permutations II](http://www.cnblogs.com/grandyang/p/4359825.html)，[Combinations](http://www.cnblogs.com/grandyang/p/4332522.html) 等等，如果仔细研究这些题目发现都是一个套路，都是需要另写一个递归函数，这里我们新加入三个变量，start记录当前的递归到的下标，out为一个解，res保存所有已经得到的解，每次调用新的递归函数时，此时的target要减去当前数组的的数，具体看代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        combinationSumDFS(candidates, target, 0, {}, res);
        return res;
    }
    void combinationSumDFS(vector<int>& candidates, int target, int start, vector<int> out, vector<vector<int>>& res) {
        if (target < 0) return;
        if (target == 0) {res.push_back(out); return;}
        for (int i = start; i < candidates.size(); ++i) {
            out.push_back(candidates[i]);
            combinationSumDFS(candidates, target - candidates[i], i, out, res);
            out.pop_back();
        }
    }
};
```



我们也可以不使用额外的函数，就在一个函数中完成递归，还是要先给数组排序，然后遍历，如果当前数字大于target，说明肯定无法组成target，由于排过序，之后的也无法组成target，直接break掉。如果当前数字正好等于target，那么当前单个数字就是一个解，组成一个数组然后放到结果res中。然后我们将当前位置之后的数组取出来，调用递归函数，注意此时的target要减去当前的数字，然后我们遍历递归结果返回的二维数组，将当前数字加到每一个数组最前面，然后再将每个数组加入结果res即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        sort(candidates.begin(), candidates.end());
        for (int i = 0; i < candidates.size(); ++i) {
            if (candidates[i] > target) break;
            if (candidates[i] == target) {res.push_back({candidates[i]}); break;}
            vector<int> vec = vector<int>(candidates.begin() + i, candidates.end());
            vector<vector<int>> tmp = combinationSum(vec, target - candidates[i]);
            for (auto a : tmp) {
                a.insert(a.begin(), candidates[i]);
                res.push_back(a);
            }
        }
        return res;
    }
};
```



我们也可以用迭代的解法来做，建立一个三维数组dp，这里dp[i]表示目标数为i的所有解法集合。这里的i就从1遍历到target即可，对于每个i，我们都新建一个二维数组cur，然后遍历candidates数组，如果遍历到的数字大于i，说明当前及之后的数字都无法组成i，直接break掉。否则如果相等，那么把当前数字自己组成一个数组，并且加到cur中。否则就遍历dp[i - candidates[j] - 1] 中的所有数组，如果当前数字大于数组的首元素，则跳过，因为我们的结果要求是要有序的。否则就将当前数字加入数组的开头，并且将数组放入cur之中即可，参见代码如下：



解法三：

```
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<vector<int>>> dp;
        sort(candidates.begin(), candidates.end());
        for (int i = 1; i <= target; ++i) {
            vector<vector<int>> cur;
            for (int j = 0; j < candidates.size(); ++j) {
                if (candidates[j] > i) break;
                if (candidates[j] == i) {cur.push_back({candidates[j]}); break;}
                for (auto a : dp[i - candidates[j] - 1]) {
                    if (candidates[j] > a[0]) continue;
                    a.insert(a.begin(), candidates[j]);
                    cur.push_back(a);
                }
            }
            dp.push_back(cur);
        }
        return dp[target - 1];
    }
};
```



类似题目：

[Combination Sum III](http://www.cnblogs.com/grandyang/p/4537983.html)

[Combination Sum II](http://www.cnblogs.com/grandyang/p/4419386.html)

[Combination Sum IV](http://www.cnblogs.com/grandyang/p/5705750.html)

[Combinations](http://www.cnblogs.com/grandyang/p/4332522.html)

[Factor Combinations](http://www.cnblogs.com/grandyang/p/5332722.html)

[Letter Combinations of a Phone Number](http://www.cnblogs.com/grandyang/p/4452220.html)



参考资料：

[https://leetcode.com/problems/combination-sum/](https://leetcode.com/problems/combination-sum/)

[https://leetcode.com/problems/combination-sum/discuss/16825/Recursive-java-solution](https://leetcode.com/problems/combination-sum/discuss/16825/Recursive-java-solution)

[https://leetcode.com/problems/combination-sum/discuss/16509/Iterative-Java-DP-solution](https://leetcode.com/problems/combination-sum/discuss/16509/Iterative-Java-DP-solution)

[https://leetcode.com/problems/combination-sum/discuss/16502/A-general-approach-to-backtracking-questions-in-Java-(Subsets-Permutations-Combination-Sum-Palindrome-Partitioning)](https://leetcode.com/problems/combination-sum/discuss/16502/A-general-approach-to-backtracking-questions-in-Java-(Subsets-Permutations-Combination-Sum-Palindrome-Partitioning))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












