# Leetcode 63 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//一维dp还是比较难写的
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid[0].size();
        int n = obstacleGrid.size();
        vector<int> dp(m,1);
        for(int j=0;j < m;j++){
            if(obstacleGrid[0][j] == 1){
                for(int i=j;i < m;i++)
                    dp[i] = 0;
                break;
            }
        }
        for(int i=1;i < n;i++){
            if(obstacleGrid[i][0] == 1){
                    dp[0] = 0;
            }
            for(int j=1;j < m;j++){
                dp[j] = dp[j] + dp[j-1];
                if(obstacleGrid[i][j] == 1){
                    dp[j] = 0;
                }
            }
        }
        
        return dp[m-1];

    }
};
```













