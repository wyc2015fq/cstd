# Leetcode 58 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int lengthOfLastWord(string s) {
        int n = s.size();
        int res = 0;
        int j = 0;
        for(j=n-1;j >= 0;j--){
            if(s[j] != ' ')
                break;
        }
        for(int i=j;i >= 0;i--){
            if(s[i] == ' ') break;
            res++;
        }
        return res;
    }
};
```













