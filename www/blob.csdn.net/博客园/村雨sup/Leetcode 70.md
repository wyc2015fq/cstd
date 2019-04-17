# Leetcode 70 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int climbStairs(int n) {
        if(n == 1)return 1;
        if(n == 2)return 2;
        int x = 1;
        int y = 2;
        while(n-2){
            int t = y;
            y = x+y;
            x = t;
            n--;
        }
        return y;
    }

};
```













