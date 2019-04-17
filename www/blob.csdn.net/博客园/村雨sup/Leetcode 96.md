# Leetcode 96 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int numTrees(int n) {
        int dp[100];
        dp[0] = 1;
        dp[1] = 1;
        dp[2] = 2;
        for(int i=3;i <= n;i++){
            int sum = 0;
            for(int j=0;j <= (i-2)/2;j++){
                sum += dp[j]*dp[i-1-j]*2;
            }
            if(i%2 == 1){
                sum += dp[i/2]*dp[i/2];
            }
            dp[i] = sum;
        }
        return dp[n];
    }
};
```

_











