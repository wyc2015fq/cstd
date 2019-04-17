# LeetCode 40 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
// 既然不能重复利用，就在递归中选择下一个数，不能重复的话，就用set
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        set<vector<int>> res;
        sort(candidates.begin(),candidates.end());
        vector<int> add;
        DFS(candidates,target,0,add,res);
        return vector<vector<int>>(res.begin(),res.end());

    }
    void DFS(vector<int>& candidates, int target,int start,vector<int>& add,set<vector<int>>& res){
        if(target < 0)return;
        else if(target == 0){res.insert(add);}
        else{
            for(int i=start;i < candidates.size();i++){
                add.push_back(candidates[i]);
                DFS(candidates,target-candidates[i],i+1,add,res);
                add.pop_back();
            }
        }
    }
};
```













