# Leetcode 5 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//两种情况，和后面一样，和后面不一样
//对于找回文字串的问题，就要以每一个字符为中心，像两边扩散来寻找回文串，这个算法的时间复杂度是O(n*n)，
class Solution {
public:
    string longestPalindrome(string s) {
        if(s == "")
            return "";
        int maxlen = 1;
        int start = 0;
        int end = 0;
        for(int i=0;i < s.length()-1;i++){
            if(s[i] == s[i+1]){
                int x=i;
                int y=i+1;
                while(x >= 0 && y < s.length()){
                    if(s[x] != s[y])break;
                    x--;y++;
                }
                x++;y--;
                if(maxlen < y-x+1){start = x;end = y;maxlen = y-x+1;}
            }

            int x=i;
            int y=i;
            while(x >= 0 && y < s.length()){
                if(s[x] != s[y])break;
                x--;y++;
            }
            x++;y--;

            if(maxlen < y-x+1){start = x;end = y;maxlen = y-x+1;}

        }
        return s.substr(start,end-start+1);
    }

};
```

_











