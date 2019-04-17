# Leetcode 35 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//在数组最后插入INT_MAX是个好方法
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        nums.push_back(INT_MAX);
        for(int i=0;i < nums.size();i++){
            if(target <= nums[i])
                return i;
        }
    }
};
```













