# 高精度乘法，string中的坑 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "bits/stdc++.h"
using namespace std;
char a[1000];
char b[1000];

int main()
{
    while(cin >> a >> b)
    {
        int lena = strlen(a);
        int lenb = strlen(b);

        for(int i=0;i < 1000;i++)
            a[i] = a[i] - '0';
        for(int i=0;i < 1000;i++)
            b[i] = b[i] - '0';

        int s[1001] = {0};
        int ans;
        for(int i = lenb-1;i>=0;i--)
        {
            int k = lenb - 1 - i;
            for(int j = lena-1;j >=0;j--)
            {
                ans = s[k] + b[i] * a[j];
                s[k] =  ans < 10 ? ans:ans%10;
                k++;
                s[k] = s[k] + ans/10;
            }
        }
//        for(int i=0;i < 1000;i++)
//            cout << s[i];

        int m = 1000;
        while(s[m] == 0)
            m--;
        while(m+1)
        {
            cout << s[m];
            m--;
        }

    }
    return 0;
}
```

当我想用上次高精度减法的string模板的时候，发现了n多个bug，首先就是string之所以可以用于高精度的减法是因为字符和数组的差是一样的，而实际的数字是数字+48（或者其他什么东西），反正string的坑就是这个。还是用char进行初始化靠谱。











