# Leetcode 115 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
Ø r a b b b i t
Ø 1 1 1 1 1 1 1 1
r 0 1 1 1 1 1 1 1
a 0 0 1 1 1 1 1 1
b 0 0 0 1 2 3 3 3
b 0 0 0 0 1 3 3 3
i 0 0 0 0 0 0 3 3
t 0 0 0 0 0 0 0 3
```

```
class Solution {
public:
    int numDistinct(string s, string t) {
        int lens = s.size()+1;
        int lent = t.size()+1;
        int a[lent][lens];
        for(int i=0;i < lens;i++){
            a[0][i] = 1;
        }
        for(int i=1;i < lent;i++){
            a[i][0] = 0;
        }
        //初始化
        for(int i=1;i < lent;i++){
            for(int j=1;j < lens;j++){
                a[i][j] = a[i][j-1] + (t[i-1] == s[j-1]?a[i-1][j-1]:0);
            }
        }
        return a[lent-1][lens-1];
    }
};
```

首先，若原字符串和子序列都为空时，返回1，因为空串也是空串的一个子序列。若原字符串不为空，而子序列为空，也返回1，因为空串也是任意字符串的一个子序列。而当原字符串为空，子序列不为空时，返回0，因为非空字符串不能当空字符串的子序列。理清这些，二维数组dp的边缘便可以初始化了，下面只要找出递推式，就可以更新整个dp数组了。我们通过观察上面的二维数组可以发现，当更新到dp[i][j]时，dp[i][j] >= dp[i][j - 1] 总是成立，再进一步观察发现，当 T[i - 1] == S[j - 1] 时，dp[i][j] = dp[i][j - 1] + dp[i - 1][j - 1]，若不等， dp[i][j] = dp[i][j - 1]，所以，综合以上，递推式为：

dp[i][j] = dp[i][j - 1] + (T[i - 1] == S[j - 1] ? dp[i - 1][j - 1] : 0)











