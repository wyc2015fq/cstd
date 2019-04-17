# STL中的map - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





map 容器 提供 1 对 1 的关系

定义方式：

```
map<string,int>mp;
```

写在前面的是关键字。

数据插入：

1.使用 insert 插入 pair 数据

```
mp.insert(pair<string,int>("sb",1));
```

2.用value_type进行插入

```
mp.insert(map<string,int>::value_type("sb",1));
```

3.直接插入

```
mp["sb"] = 1;
```



注意的是1,2两种插入方法，必须插入关键字不同的mp对，而方法三可以直接对关键字对应的信息覆盖。

常用函数：

```
mp.size();
sort(mp.begin,mp.end); //用迭代器排序
mp.clear();  //清空
mp.empty(); // 是否为空
```

查找函数：

```
pair mappair = mp.equal_range("sb");
        if(mappair.first == mappair.second)
            cout << "no find" << endl;
```

mappair.first 等同于lower_bound , second 等同于upper_bound。

数据删除函数：

```
int n = mp.erase("sb");  //删除成功返回 1 否则返回 0
```



讲的再多不如例题理解的透彻——hdu6015

题目大意是找出逃课值最大的方案

```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int inf = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int MAXN=510;

struct node
{
    char s[15];
    int val;

};

bool cmp(const node &a, const node &b)
{
    return a.val > b.val;
}


int main()
{
    int t;
    cin >> t;
    map<string,int>mp;
    while(t--)
    {
        int n;
        cin >> n;
        mp.clear();     //此处每次都一定要清空
        node NODE[n];
        for(int i=0;i < n;i++)
            cin >> NODE[i].s >> NODE[i].val;

        sort(NODE,NODE+n,cmp);

        ll sum = 0;
        for(int i=0;i < n;i++)
        {
            if(mp[NODE[i].s] == 2) continue;
            else
            {
                mp[NODE[i].s]++;    // 直接使用，不需要特殊的定义，map[关键字]对应的是逃课的次数
                sum += NODE[i].val;
            }
        }
        cout << sum << endl;
    }
    return 0;
}
```

/*











