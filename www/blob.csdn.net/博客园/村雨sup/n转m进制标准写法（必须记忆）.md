# n转m进制标准写法（必须记忆） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include <bits/stdc++.h>
using namespace std;





int main()
{
    int n,m;
    cin >> n >> m;
    char s[17] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    string str = "";
    int i;
    while(n)
    {
        i = n%m;
        n = n/m;
        str = s[i] + str;
    }
    cout << str;


    return 0;
}
```

第一：奇葩的万能头文件

第二：char中只能用单引号

第三：itoa不是在所有编译器中都能用











