# [LeetCode] Score After Flipping Matrix 翻转矩阵后的分数 - Grandyang - 博客园







# [[LeetCode] Score After Flipping Matrix 翻转矩阵后的分数](https://www.cnblogs.com/grandyang/p/10674440.html)








We have a two dimensional matrix `A` where each value is `0` or `1`.

A move consists of choosing any row or column, and toggling each value in that row or column: changing all `0`s to `1`s, and all `1`s to `0`s.

After making any number of moves, every row of this matrix is interpreted as a binary number, and the score of the matrix is the sum of these numbers.

Return the highest possible score.

Example 1:

```
Input: [[0,0,1,1],[1,0,1,0],[1,1,0,0]]
Output: 39
Explanation: Toggled to [[1,1,1,1],[1,0,0,1],[1,1,1,1]].
0b1111 + 0b1001 + 0b1111 = 15 + 9 + 15 = 39
```

Note:
- `1 <= A.length <= 20`
- `1 <= A[0].length <= 20`
- `A[i][j]` is `0` or `1`.




这道题给了我们一个只有0和1的二维数组，说是每一行代表一个数字，我们可以任意地翻转任意行和列，问我们如何能使得每一行代表的数字之和最大。在博主看来，这道题还是挺有意思的，因为你可以写的很复杂，但如果思路巧妙的话，也可以写的很简洁。当然最暴力的解法就是遍历所有的组合，对于一个 mxn 大小的矩阵，每一行都可以选择翻与不翻，同理，每一列也可以选择翻与不翻，那么总共就有 2^(m+n) 种情况，写起来比较复杂。

这道题最巧妙的解法是用贪婪算法 Greedy Algorithm 来解的，由于数字是由二进制表示的，那么最高位的权重是要大于其他位总和的，比如 1000 就要大于 0111 的，所以当最高位是0的时候，无论如何都是需要翻转当前行的，那么对于 mxn 的数组来说，每行的二进制数共有n位，最高位是1的话，就是 1<<(n-1)，那么共有m行，所以至少能将 m*(1<<(n-1)) 这么大的值收入囊中，既然最高值一定要是1，那么每一行的翻转情况就确定了，若还想增大数字之和，就只能看各列是否还能翻转了，而且是从次高位列开始看，因为最高位列必须保证都是1。由于每一行的翻转情况已经确定了，那么我们如何才能确定其他位到底是0还是1呢，这里就有个 trick，此时就要看它跟最高位是否相同了，若相同的话，不管最高位初始时是0还是1，最终都要变成1，那么当前位一定最终也会变成1，而一旦跟最高位相反，那么最后一定会是0。我们翻转当前列的条件肯定是希望翻转之后1的个数要更多一些，这样值才能增加，所以我们就要统计每列当前的1的个数，若小于0的个数，才进行翻转，然后乘以该列的值，对于第j列，其值为 1<<(n-1-j)，参见代码如下：




```
class Solution {
public:
    int matrixScore(vector<vector<int>>& A) {
        int m = A.size(), n = A[0].size(), res = (1 << (n - 1)) * m;
        for (int j = 1; j < n; ++j) {
            int cnt = 0;
            for (int i = 0; i < m; ++i) {
                cnt += (A[i][j] == A[i][0]);
            }
            res += max(cnt, m - cnt) * (1 << (n - 1 - j));
        }
        return res;
    }
};
```




参考资料：

[https://leetcode.com/problems/score-after-flipping-matrix/](https://leetcode.com/problems/score-after-flipping-matrix/)

[https://leetcode.com/problems/score-after-flipping-matrix/discuss/143722/C%2B%2BJavaPython-Easy-and-Concise](https://leetcode.com/problems/score-after-flipping-matrix/discuss/143722/C%2B%2BJavaPython-Easy-and-Concise)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












