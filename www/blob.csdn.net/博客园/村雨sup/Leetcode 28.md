# Leetcode 28 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//有个神奇的东西
class Solution {
public:
    int strStr(string haystack, string needle) {
        if(needle == "")return 0;
        int lenn = haystack.size()-needle.size(); //如果不用这个直接把haystack.size()-needle.size()用进去的话，会出错！！应该算是个bug吧
        if(lenn < 0)return -1;
        for(int i=0;i <= lenn;i++){
            bool flag = true;
            if(haystack[i] == needle[0]){
                int temp = i;
                for(int j=0;j < needle.size();j++,temp++){
                    if(haystack[temp] != needle[j]){flag = false;break;}
                }
                if(flag == true) return i;
            }
        }
        return -1;
    }
};
```













