# Leetcode 26 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int res = 0;
        nums.push_back(INT_MAX);  //注意末尾的补位
        for(int i=0;i < nums.size()-1;i++){
            if(nums[i] != nums[i+1]){
                nums[res] = nums[i];
                res++;
            }    
        }
        return res;
    }
};
```













