# Leetcode 55 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//很巧妙的贪心算法 reach = max(reach,nums[i] + i); 
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        int reach = 0;
        for(int i=0;i < n;i++){
            if(i > reach || reach >= n-1) break;
            reach = max(reach,nums[i] + i);
        }
        return reach >= n-1;
    }
};
```













