# Leetcode 999. 车的可用捕获量 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 999. 车的可用捕获量

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-125/problems/available-captures-for-rook/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-125/)







- 用户通过次数255
- 用户尝试次数260
- 通过次数255
- 提交次数357
- 题目难度Easy



在一个 8 x 8 的棋盘上，有一个白色车（rook）。也可能有空方块，白色的象（bishop）和黑色的卒（pawn）。它们分别以字符 “R”，“.”，“B” 和 “p” 给出。大写字符表示白棋，小写字符表示黑棋。

车按国际象棋中的规则移动：它选择四个基本方向中的一个（北，东，西和南），然后朝那个方向移动，直到它选择停止、到达棋盘的边缘或移动到同一方格来捕获该方格上颜色相反的卒。另外，车不能与其他友方（白色）象进入同一个方格。

返回车能够在一次移动中捕获到的卒的数量。


示例 1：

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/02/23/1253_example_1_improved.PNG)
输入：[[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".","R",".",".",".","p"],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]
输出：3
解释：
在本例中，车能够捕获所有的卒。

示例 2：

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/02/23/1253_example_2_improved.PNG)
输入：[[".",".",".",".",".",".",".","."],[".","p","p","p","p","p",".","."],[".","p","p","B","p","p",".","."],[".","p","B","R","B","p",".","."],[".","p","p","B","p","p",".","."],[".","p","p","p","p","p",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]
输出：0
解释：
象阻止了车捕获任何卒。

示例 3：

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/02/23/1253_example_3_improved.PNG)
输入：[[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".","p",".",".",".","."],["p","p",".","R",".","p","B","."],[".",".",".",".",".",".",".","."],[".",".",".","B",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".",".",".",".",".","."]]
输出：3
解释： 
车可以捕获位置 b5，d6 和 f5 的卒。



提示：
- `board.length == board[i].length == 8`
- `board[i][j]` 可以是 `'R'`，`'.'`，`'B'` 或 `'p'`
- 只有一个格子上存在 `board[i][j] == 'R'`

```
class Solution {
public:
    int numRookCaptures(vector<vector<char>>& board) {
        int n=0,m=0;
        for(int i=0;i < board.size();i++){
            for(int j=0;j < board[0].size();j++){
                if(board[i][j] == 'R'){n=i;m=j;}
            }
        }
        int res = 0;
        for(int i=n-1;i>=0;i--){
            if(board[i][m] == 'B')break;
            else if(board[i][m] == 'p'){res++;break;}
        }
        for(int i=n+1;i<board.size();i++){
            if(board[i][m] == 'B')break;
            else if(board[i][m] == 'p'){res++;break;}
        }
        for(int j=m-1;j>=0;j--){
            if(board[n][j] == 'B')break;
            else if(board[n][j] == 'p'){res++;break;}
        }
        for(int j=m+1;j<board[0].size();j++){
            if(board[n][j] == 'B')break;
            else if(board[n][j] == 'p'){res++;break;}
        }
        return res;   
    }
};
```

-HAOSHUIA














