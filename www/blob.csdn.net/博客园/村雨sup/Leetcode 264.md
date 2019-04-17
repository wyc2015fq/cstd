# Leetcode 264 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//这是个比较好的回溯模板
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> res;
        vector<int> add;
        DFS(res,add,k,n,1);
        return res;
    }
    
    void DFS(vector<vector<int>>& res,vector<int> add,int k,int n,int pos){
        if(n < 0) return;
        if(n == 0&&add.size() == k){
            res.push_back(add);
        }
        else{
            for(int i=pos;i <= 9;i++){
                add.push_back(i);
                DFS(res,add,k,n-i,i+1);
                add.pop_back();
            }
        }
    }
};
```













