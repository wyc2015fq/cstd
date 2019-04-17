# LeetCode 47 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        set<vector<int>> res;
        vector<int> add;
        vector<int> vis(nums.size(),0);
        BFS(nums,res,add,vis,0);
        return vector<vector<int>>(res.begin(),res.end());
        
    }
    
    void BFS(vector<int>& nums,set<vector<int>>& res,vector<int>& add,vector<int>& vis,int level){
        if(level == nums.size()){res.insert(add);return;}
        else{
            for(int i=0;i < nums.size();i++){
                if(vis[i] == 0){
                    vis[i] = 1;
                    add.push_back(nums[i]);
                    BFS(nums,res,add,vis,level+1);
                    add.pop_back();
                    vis[i] = 0;
                }
            }
        }
    }
};
```













