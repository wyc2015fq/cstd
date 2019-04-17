# Leetcode 39 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//经典回溯法
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        sort(candidates.begin(),candidates.end());
        vector<int> add;
        DFS(candidates,target,0,add,res);
        return res;
    }
    void DFS(vector<int>& candidates, int target,int start,vector<int>& add,vector<vector<int>>& res){
        if(target < 0)return;
        else if(target == 0){res.push_back(add);}
        else{
            for(int i=start;i < candidates.size();i++){
                add.push_back(candidates[i]);
                DFS(candidates,target-candidates[i],i,add,res);
                add.pop_back();
            }
        }
    }
};
```













