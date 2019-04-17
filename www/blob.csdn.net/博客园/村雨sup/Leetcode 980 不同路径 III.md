# Leetcode 980. 不同路径 III - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 980. 不同路径 III

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-120/problems/unique-paths-iii/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-120/)







- 用户通过次数42
- 用户尝试次数43
- 通过次数46
- 提交次数60
- 题目难度Hard



在二维网格 `grid` 上，有 4 种类型的方格：
- `1` 表示起始方格。且只有一个起始方格。
- `2` 表示结束方格，且只有一个结束方格。
- `0` 表示我们可以走过的空方格。
- `-1` 表示我们无法跨越的障碍。

返回在四个方向（上、下、左、右）上行走时，从起始方格到结束方格的不同路径的数目，每一个无障碍方格都要通过一次。



示例 1：
输入：[[1,0,0,0],[0,0,0,0],[0,0,2,-1]]
输出：2
解释：我们有以下两条路径：
1. (0,0),(0,1),(0,2),(0,3),(1,3),(1,2),(1,1),(1,0),(2,0),(2,1),(2,2)
2. (0,0),(1,0),(2,0),(2,1),(1,1),(0,1),(0,2),(0,3),(1,3),(1,2),(2,2)
示例 2：
输入：[[1,0,0,0],[0,0,0,0],[0,0,0,2]]
输出：4
解释：我们有以下四条路径： 
1. (0,0),(0,1),(0,2),(0,3),(1,3),(1,2),(1,1),(1,0),(2,0),(2,1),(2,2),(2,3)
2. (0,0),(0,1),(1,1),(1,0),(2,0),(2,1),(2,2),(1,2),(0,2),(0,3),(1,3),(2,3)
3. (0,0),(1,0),(2,0),(2,1),(2,2),(1,2),(1,1),(0,1),(0,2),(0,3),(1,3),(2,3)
4. (0,0),(1,0),(2,0),(2,1),(1,1),(0,1),(0,2),(0,3),(1,3),(1,2),(2,2),(2,3)
示例 3：
输入：[[0,1],[2,0]]
输出：0
解释：
没有一条路能完全穿过每一个空的方格一次。
请注意，起始和结束方格可以位于网格中的任意位置。



提示：
- `1 <= grid.length * grid[0].length <= 20   `

```
class Solution {
public:
    int uniquePathsIII(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        int a = 0;
        int b = 0;
        vector<vector<int>> vis(n,vector<int>(m));
        int cnt = 0;
        for(int i=0;i < n;i++){
            for(int j=0;j < m;j++){
                if(grid[i][j] == 0){
                    cnt++;
                    vis[i][j] = 0;
                }
                else if(grid[i][j] == 2){
                    vis[i][j] = 0;
                }
                else if(grid[i][j] == -1){
                    vis[i][j] = 1;
                }
                else if(grid[i][j] == 1){
                    vis[i][j] = 1;
                    a = i;b = j;
                }
            }
        }
        int res = 0;
        dfs(grid,vis,a,b,res,0,cnt);
        return res;
        
    }
    void dfs(vector<vector<int>> grid,vector<vector<int>> vis,int n,int m,int& num,int cnt,int maxnum){
        if(grid[n][m] == 2){
            if(cnt-1 == maxnum) num++;
            else return;
        }
        if((n-1>=0&&n-1<grid.size())&&(m>=0&&m<grid[0].size())&&vis[n-1][m] == 0){
            vis[n-1][m] = 1;
            dfs(grid,vis,n-1,m,num,cnt+1,maxnum);
            vis[n-1][m] = 0;
        }
        if((n+1>=0&&n+1<grid.size())&&(m>=0&&m<grid[0].size())&&vis[n+1][m] == 0){
            vis[n+1][m] = 1;
            dfs(grid,vis,n+1,m,num,cnt+1,maxnum);
            vis[n+1][m] = 0;
        }
        if((n>=0&&n<grid.size())&&(m+1>=0&&m+1<grid[0].size())&&vis[n][m+1] == 0){
            vis[n][m+1] = 1;
            dfs(grid,vis,n,m+1,num,cnt+1,maxnum);
            vis[n][m+1] = 0;
        }
        if((n>=0&&n<grid.size())&&(m-1>=0&&m-1<grid[0].size())&&vis[n][m-1] == 0){
            vis[n][m-1] = 1;
            dfs(grid,vis,n,m-1,num,cnt+1,maxnum);
            vis[n][m-1] = 0;
        }
    }
};
```

牛逼哦AC了














