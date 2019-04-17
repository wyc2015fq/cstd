# Leetcode 125 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    bool isPalindrome(string s) {
        int i = 0;
        int j = s.size()-1;
        while(i <= j){
            if(s[i] < 48 || (s[i]>57&&s[i]<65) || (s[i]>90&&s[i]<97) || s[i] > 122)i++;
            else if(s[j] < 48 || (s[j]>57&&s[j]<65) || (s[j]>90&&s[j]<97) || s[j] > 122)j--;
            else if((s[i] + 32 - 'a') %32 != (s[j] + 32 - 'a') % 32) return false;
            else{
                i++;
                j--;
            }  
        }
        return true; 
    }
};
```

_好气啊，傻逼题写了一个多 小时，主要是判断大小写数字太麻烦了，最后借鉴了别人简单的写法。











