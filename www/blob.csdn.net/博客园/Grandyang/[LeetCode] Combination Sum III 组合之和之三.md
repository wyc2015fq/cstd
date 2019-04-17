# [LeetCode] Combination Sum III 组合之和之三 - Grandyang - 博客园







# [[LeetCode] Combination Sum III 组合之和之三](https://www.cnblogs.com/grandyang/p/4537983.html)








Find all possible combinations of ***k*** numbers that add up to a number ***n***, given that only numbers from 1 to 9 can be used and each combination should be a unique set of numbers.

Ensure that numbers within the set are sorted in ascending order.




***Example 1:***

Input:  ***k*** = 3,  ***n*** = 7

Output: 
[[1,2,4]]


***Example 2:***

Input:  ***k*** = 3,  ***n*** = 9

Output: 
[[1,2,6], [1,3,5], [2,3,4]]


**Credits:**
Special thanks to [@mithmatt](https://leetcode.com/discuss/user/mithmatt) for adding this problem and creating all test cases.



这道题题是组合之和系列的第三道题，跟之前两道 [Combination Sum](http://www.cnblogs.com/grandyang/p/4419259.html)，[Combination Sum II](http://www.cnblogs.com/grandyang/p/4419386.html) 都不太一样，那两道的联系比较紧密，变化不大，而这道跟它们最显著的不同就是这道题的个数是固定的，为k。个人认为这道题跟那道 [Combinations](http://www.cnblogs.com/grandyang/p/4332522.html) 更相似一些，但是那道题只是排序，对k个数字之和又没有要求。所以实际上这道题是它们的综合体，两者杂糅到一起就是这道题的解法了，n是k个数字之和，如果n小于0，则直接返回，如果n正好等于0，而且此时out中数字的个数正好为k，说明此时是一个正确解，将其存入结果res中，具体实现参见代码入下：



```
class Solution {
public:
    vector<vector<int> > combinationSum3(int k, int n) {
        vector<vector<int> > res;
        vector<int> out;
        combinationSum3DFS(k, n, 1, out, res);
        return res;
    }
    void combinationSum3DFS(int k, int n, int level, vector<int> &out, vector<vector<int> > &res) {
        if (n < 0) return;
        if (n == 0 && out.size() == k) res.push_back(out);
        for (int i = level; i <= 9; ++i) {
            out.push_back(i);
            combinationSum3DFS(k, n - i, i + 1, out, res);
            out.pop_back();
        }
    }
};
```



类似题目：

[Combination Sum IV](http://www.cnblogs.com/grandyang/p/5705750.html)

[Combination Sum II](http://www.cnblogs.com/grandyang/p/4419386.html)

[Combination Sum](http://www.cnblogs.com/grandyang/p/4419259.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












