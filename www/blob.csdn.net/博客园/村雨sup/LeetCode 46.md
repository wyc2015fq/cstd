# LeetCode 46 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
// 又是可以用回溯法做的一道题。
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<int> vis(nums.size(),0);
        vector<vector<int>> res;
        vector<int> add;
        DFS(nums,0,res,add,vis);
        return res;
    }
    void DFS(vector<int>& nums,int level,vector<vector<int>>& res,vector<int>& add,vector<int>& vis){
        if(level == nums.size()){res.push_back(add);return;}
        else{
            for(int i=0;i < nums.size();i++){   //一开始不理解为啥要加个vis，其实是因为就存在前面的也要当你第一个i取中间的时候前面的也需要算这时候，每次递归都是从0 开始，就可能遍历到第一个数，所以加个vis
                if(vis[i] == 0){
                    vis[i] = 1;
                    add.push_back(nums[i]);
                    DFS(nums,level+1,res,add,vis);
                    add.pop_back();
                    vis[i] = 0;
                }
            }
        }
    }
};
```



```
//每次start 与 i交换
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        DFS(nums,0,res);
        return res;
    }
    void DFS(vector<int>& nums,int start,vector<vector<int>>& res){
        if(start == nums.size()){res.push_back(nums);return;}
        else{
            for(int i=start;i < nums.size();i++){
                swap(nums[start],nums[i]);
                DFS(nums,start+1,res);
                swap(nums[start],nums[i]);
            }
        }
    }
};
```













