# Leetcode 137 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for(int i=0;i < 32;i++){
            int sum = 0;
            for(int num:nums){
                sum += (num>>i)&1; //所有数字二进制第一位有1的加
            }
            res = res|((sum%3)<<i);//有1的3个 一取模没了，留下 1个的，往左移i位用”或“赋值
        }
        return res;
    }
};
```













