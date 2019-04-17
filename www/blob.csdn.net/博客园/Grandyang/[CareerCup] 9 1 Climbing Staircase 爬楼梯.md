# [CareerCup] 9.1 Climbing Staircase 爬楼梯 - Grandyang - 博客园







# [[CareerCup] 9.1 Climbing Staircase 爬楼梯](https://www.cnblogs.com/grandyang/p/4815094.html)







9.1 A child is running up a staircase with n steps, and can hop either 1 step, 2 steps, or 3 steps at a time. Implement a method to count how many possible ways the child can run up the stairs.



LeetCode上的原题，请参见我之前的博客[Climbing Stairs 爬梯子问题](http://www.cnblogs.com/grandyang/p/4079165.html)。



```
class Solution {
public:
    int countWays(int n) {
        vector<int> res(n + 1, 1);
        for (int i = 2; i <= n; ++i) {
            res[i] = res[i - 1] + res[i - 2];
        }
        return res.back();
    }
};
```














