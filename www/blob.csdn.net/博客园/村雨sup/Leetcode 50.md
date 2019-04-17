# Leetcode 50 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//1开始我只是按照原来快速幂的思想，当n <0 时，n变成-n,发现当n取-INTMAX时会发生越界的问题，然后在改快速幂代码的时候逐渐了解到快速幂的本质，其实位运算对快速幂来说速度加快不了多少，还是了解原理比较好
class Solution {
public:
    double myPow(double x, int n) {
        double res = 1.0;
        int t = n;
        while(n != 0){
            if(n%2 != 0) res = res*x;
            x = x*x;
            n = n/2;
        }
        return t > 0?res:1/res;
    }
};
```













