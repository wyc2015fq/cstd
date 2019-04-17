# Leetcode 9 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//反转之后判断就行了
class Solution {
public:
    bool isPalindrome(int x) {
        int t = x;
        if(x < 0) return false;
        int sum = 0;
        while(x > 0){
            sum = sum*10 + x%10;
            x = x/10;
        }
        if(sum == t)
            return true;
        return false;
    }
};
```













