# n阶汉诺塔 记住吧。。 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "bits/stdc++.h"
using namespace std;
int c;

void move(char a,int n,char b)
{
    printf("%2d. Move disk %d from %c to %c\n",++c,n,a,b);
}

void hanuo(int n,char a,char b,char c)
{
    if(n == 1)
        move(a,1,c);
    else
    {
        hanuo(n-1,a,c,b);
        move(a,n,c);
        hanuo(n-1,b,a,c);
    }
}

int main()
{
    int n;

    while(cin >> n)
    {
        c = 0;
        hanuo(n,'X','Y','Z');
        cout << endl;
    }
}
```

也就是a c b       b a c











