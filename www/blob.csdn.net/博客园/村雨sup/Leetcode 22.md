# Leetcode 22 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//这题感觉不如前两题回溯清楚，还要再看看
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string add;
        DFS(res,add,n,n);
        return res;
    }
    
    void DFS(vector<string>& res,string add,int x,int y){
        if(x > y) return;
        if(x == 0&&y == 0){
            res.push_back(add);
        }
        else{
            if(x > 0)DFS(res,add+"(",x-1,y);
            if(y > 0)DFS(res,add+")",x,y-1);
        }
    }
};
```













