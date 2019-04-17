# Leetcode 90 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
// 重复元素在去重的时候会出现顺序不同去不了重，这时候需要对add进行排序
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> add;
        for(int i=0;i <= nums.size();i++){
            DFS(res,add,nums,0,i);
        }
        set<vector<int>> gg(res.begin(),res.end());
        return vector<vector<int>>(gg.begin(),gg.end());
    }    
    void DFS(vector<vector<int>>& res,vector<int> add,vector<int> nums,int pos,int k){
        if(k == 0){
            sort(add.begin(),add.end());
            res.push_back(add);
        }
        else{
            for(int i=pos;i < nums.size();i++){
                add.push_back(nums[i]);
                DFS(res,add,nums,i+1,k-1);
                add.pop_back();
            }
        }
    }
};
```













