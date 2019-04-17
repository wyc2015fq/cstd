# Leetcode 67 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
// string 转 数字很简单，-‘0’就行，数字转string 要用to_string(),是个比较好的函数，网上用的sprintf之类的都丑哭了。
class Solution {
public:
    string addBinary(string a, string b) {
        int lena = a.size();
        int lenb = b.size();
        int i = lena-1;
        int j = lenb-1;
        int jinwei = 0;
        string res = "";
        while(i >= 0||j >= 0){
            int ga = 0,gb = 0;
            if(i < 0)ga = 0;
            else{
                ga = a[i] - '0';
            }
            if(j < 0)gb = 0;
            else{
                gb = b[j] - '0';
            }

            int t = (jinwei+ga+gb)%2;
            jinwei = (jinwei+ga+gb)/2;
            res = to_string(t) + res;
            i--;j--;
        }
        if(jinwei == 1) res = "1" + res;
        return res;
    }
};
```













