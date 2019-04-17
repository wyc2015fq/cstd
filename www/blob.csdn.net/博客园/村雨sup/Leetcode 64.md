# Leetcode 64 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        vector<vector<int>> map(n,vector<int>(m,0));//注意二维vector的初始化写法
        map[0][0] = grid[0][0];
        for(int i=1;i < n;i++){
            map[i][0] = map[i-1][0] + grid[i][0];
        }
        for(int i=1;i < m;i++){
            map[0][i] = map[0][i-1] + grid[0][i];
        }
        for(int i=1;i < n;i++){
            for(int j=1;j < m;j++){
                map[i][j] = min(map[i-1][j],map[i][j-1]) + grid[i][j];
            }
        }
        return map[n-1][m-1];
    }
};
```













