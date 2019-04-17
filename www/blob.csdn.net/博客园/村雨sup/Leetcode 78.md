# Leetcode 78 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//和77类似的问题，可以放在一起记忆
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> add;
        res.push_back(add);
        for(int i=1;i <= nums.size();i++){
            DFS(res,add,nums,i,-1);
        }
        return res;
    }
    
    void DFS(vector<vector<int>>& res,vector<int> &add,vector<int>& nums,int k,int pos){
        if(add.size() == k){
            res.push_back(add);
        }
        else{
            for(int i=pos+1;i < nums.size();i++){
                add.push_back(nums[i]);
                DFS(res,add,nums,k,i);
                add.pop_back();
            }
        }
    }
};
```













