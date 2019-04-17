# Leetcode 51 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//看了一次解析后，一次AC，用一个pos记录行列。
class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> res;
        vector<int> pos(n,-1);
        DFS(pos,0,res);
        return res;
    }
    

    void DFS(vector<int>& pos,int row,vector<vector<string>>& res){
        int n = pos.size();
        if(row == n){
            vector<string> out(n,string(n,'.')); //string也有这种填充写法
            for(int i=0;i < n;i++){
                out[i][pos[i]] = 'Q';
            }
            res.push_back(out);
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













