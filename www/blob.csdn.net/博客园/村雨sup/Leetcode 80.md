# Leetcode 80 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//美滋滋，水题刷起来
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int cnt = 1;
        for(int i=1;i < nums.size();i++){
            if(nums[i] == nums[i-1]) cnt++;
            else cnt = 1;
            if(cnt > 2){
                nums.erase(nums.begin()+i);
                i--;
            }
        }
        return nums.size();
    }
};
```













