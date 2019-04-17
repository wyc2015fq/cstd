# leetcode-72. Edit Distance 编辑距离 - 别说话写代码的博客 - CSDN博客





2019年03月18日 14:17:45[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13标签：[72. Edit Distance																[Edit distance																[leetcode																[leetcode 72																[编辑距离](https://so.csdn.net/so/search/s.do?q=编辑距离&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode 72&t=blog)



|Given two words *word1* and *word2*, find the minimum number of operations required to convert *word1* to *word2*.You have the following 3 operations permitted on a word:- Insert a character- Delete a character- Replace a character**Example 1:****Input:** word1 = "horse", word2 = "ros"**Output:** 3**Explanation:** horse -> rorse (replace 'h' with 'r')rorse -> rose (remove 'r')rose -> ros (remove 'e')**Example 2:****Input:** word1 = "intention", word2 = "execution"**Output:** 5**Explanation:** intention -> inention (remove 't')inention -> enention (replace 'i' with 'e')enention -> exention (replace 'n' with 'x')exention -> exection (replace 'n' with 'c')exection -> execution (insert 'u')|给定两个单词 *word1* 和 *word2*，计算出将 *word1* 转换成 *word2 *所使用的最少操作数 。你可以对一个单词进行如下三种操作：- 插入一个字符- 删除一个字符- 替换一个字符**示例 1:****输入:** word1 = "horse", word2 = "ros"**输出:** 3**解释:** horse -> rorse (将 'h' 替换为 'r')rorse -> rose (删除 'r')rose -> ros (删除 'e')**示例 2:****输入:** word1 = "intention", word2 = "execution"**输出:** 5**解释:** intention -> inention (删除 't')inention -> enention (将 'i' 替换为 'e')enention -> exention (将 'n' 替换为 'x')exention -> exection (将 'n' 替换为 'c')exection -> execution (插入 'u')|
|----|----|

思路：一般求最大，最小，或者字符串相关的题目，求子数组或者子字符串相关的题目。很多都是 用dp来解。

这道题目我们定义dp[m+1][n+1]数组，dp[i][j]表示word1的前i个 字符转到word2的前j 个字符的需要变化次数。先给第一行和第一列赋值， 因为第一行和第一列 对应的字符串 可能是一个是空串。 现在只需要求出递推公式即可。

 通过观察我们可以发现，当word1[i]==word2[j]时，dp[i][j]=dp[i-1][j-1],其他情况是其位置左边，上边和左上角最小 元素+1.也就是

![](https://img-blog.csdnimg.cn/20190318141639832.png)

举个栗子：word1是“bbc"，word2是”abcd“，那么我们可以得到dp数组如下：

```
Ø a b c d
Ø 0 1 2 3 4
b 1 1 1 2 3
b 2 2 1 2 3
c 3 3 2 1 2
```

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m=word1.size(),n=word2.size();
        int dp[m+1][n+1];
        for(int i=0;i<=m;++i) dp[i][0]=i;  //dp数组初始化
        for(int i=1;i<=n;++i) dp[0][i]=i;
        for(int i=1;i<=m;++i)
        {
            for(int j=1;j<=n;++j)
            {
                if(word1[i-1]==word2[j-1])  //注意递推方程，还有表示的字符串下标
                    dp[i][j]=dp[i-1][j-1];
                else
                    dp[i][j]=min(dp[i-1][j-1],min(dp[i-1][j],dp[i][j-1]))+1;
            }
        }
        return dp[m][n];
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)](https://so.csdn.net/so/search/s.do?q=Edit distance&t=blog)](https://so.csdn.net/so/search/s.do?q=72. Edit Distance&t=blog)




