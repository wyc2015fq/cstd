# 用stringstream实现从数字到字符串的转化 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





代码简单，字符串到数字和数字到字符串的写法类似。

```
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

int main()
{
    double num = 0.123456;
    string s;
    stringstream sstream;

    sstream << num;
    sstream >> s;

    cout << s << endl;

    sstream.clear();

    return 0;
}
```

——











