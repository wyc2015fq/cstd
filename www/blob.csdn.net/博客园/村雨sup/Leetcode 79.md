# Leetcode 79 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//这是我写过最难的递归了。。。
//
class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        int m = board.size();
        int n = board[0].size();
        for(int i=0;i < m;i++){
            for(int j=0;j < n;j++)
                if(DFS(board,word,0,i,j)) return true;
        }
        return false;
    }
    // DFS搜索从x，y开始是否有word。
    bool DFS(vector<vector<char>>& board,string word,int pos,int x,int y){
        int m = board.size();
        int n = board[0].size();
        int res;
        if(pos == word.size())return true;//其实符合最后再需要一次递归调用才能被判断正确
        if(x < 0||y < 0||x >= m||y >= n||board[x][y]!=word[pos]){//边界和字符都判断了很好。
            return false;
        }
        else{
            char s = board[x][y];//递归中常见的改变以后要还原
            board[x][y] = '#';
            res = DFS(board,word,pos+1,x-1,y)||DFS(board,word,pos+1,x+1,y)||DFS(board,word,pos+1,x,y-1)||DFS(board,word,pos+1,x,y+1);//搜索四周只要有一个是true，就是true
            board[x][y] = s;
        }
        return res;
    }
};
```













