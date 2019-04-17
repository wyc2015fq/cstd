# Leetcode 77 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//这似乎是排列组合的标准写法了已经
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> res;
        vector<int> add;
        DFS(res,k,n,add,0);
        return res;
    }
    
    void DFS(vector<vector<int>>& res,int k,int n,vector<int> &add,int pos){
        if(add.size() == k){
            res.push_back(add);
        }
        else{
            for(int i=pos+1;i <= n;i++){
                add.push_back(i);
                DFS(res,k,n,add,i);
                add.pop_back();
            }
        }
    }
};
```













