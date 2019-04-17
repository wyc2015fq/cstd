# Leetcode 7 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int reverse(int x) {
        int flag = 0;
        if(x < 0){flag = 1; x *= -1;}
        long long res = 0;  
        while (x > 0) {              //数字反转 这里写x是不行的
            res = res * 10 + x % 10;
            x = x/10;
        }
        
        if (res > INT_MAX) return 0;//防止溢出

        if(flag == 1){return -res;}
        return res;
    }
};
```

_











