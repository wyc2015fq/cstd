# Leetcode 62 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//从理解二维dp到简化成一维dp我用了一年的时间
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(m,1);
        for(int i=1;i < n;i++){
            for(int j=1;j < m;j++){
                dp[j] = dp[j] + dp[j-1];
            }
        }
        return dp[m-1];
    }
};
```













