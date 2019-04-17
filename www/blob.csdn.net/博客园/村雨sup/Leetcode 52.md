# Leetcode 52 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//N皇后的基础上改了一点
class Solution {
public:
    int totalNQueens(int n) {
        int res = 0;
        vector<int> pos(n,-1);
        DFS(pos,0,res);
        return res;
    }
    void DFS(vector<int>& pos,int row,int& res){
        int n = pos.size();
        if(row == n){
            res++;
        }
        else{
            for(int i=0;i < n;i++){
                if(isfine(pos,row,i)){
                    pos[row] = i;
                    DFS(pos,row+1,res);
                    pos[row] = -1;
                }
            }
        }
    }
    
    bool isfine(vector<int>& pos,int row,int col){
        for(int i=0;i < row;i++){
            if(pos[i] == col || abs(row-i) == abs(col-pos[i]))
                return false;
        }
        return true;
    }

};
```













