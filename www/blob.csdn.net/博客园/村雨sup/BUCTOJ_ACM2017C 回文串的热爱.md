# BUCTOJ_ACM2017C 回文串的热爱 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "iostream"
#include "algorithm"
#include "cstdio"
#include "cstring"
using namespace std;

int main()
{
    string s;
    int n;
    cin >> n;
    while(n--)
    {
        cin >> s;
        int len;
        int flag = 0;
        len = s.length();
        for(int i=0;i < len/2;i++)
        {
            if(s[i] != s[len-i-1])
                flag++;
        }
        if(flag == 0 && len%2 == 1)
            cout << "YES"<<endl;
        else if(flag == 1)
            cout << "YES"<< endl;
        else
            cout << "NO"<<endl;
    }
    return 0;
}
```

1.计算字符串的长度 s.length()











