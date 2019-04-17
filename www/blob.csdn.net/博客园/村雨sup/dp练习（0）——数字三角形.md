# dp练习（0）——数字三角形 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





## 3298: 数字三角形
时间限制: 1 Sec  内存限制: 128 MB
提交: 114  解决: 29
[[提交](http://202.4.155.100/JudgeOnline/submitpage.php?id=3298)][[状态](http://202.4.155.100/JudgeOnline/problemstatus.php?id=3298)][[讨论版](http://202.4.155.100/JudgeOnline/bbs.php?pid=3298)]
## 题目描述

如图示出了一个数字三角形。 请编一个程序计算从顶至底的某处的一条路
径，使该路径所经过的数字的总和最大。
●每一步可沿左斜线向下或右斜线向下走；
●1＜三角形行数≤100；

●三角形中的数字为整数0，1，…99；

![](http://lx.lanqiao.cn/RequireFile.do?fid=9RdAy6Be)


## 输入

文件中首先读到的是三角形的行数。

接下来描述整个三角形

## 输出

最大总和（整数）

## 样例输入
5
7
3 8
8 1 0
2 7 4 4
4 5 2 6 5
## 样例输出
30
## 提示




```
#include<bits/stdc++.h>
using namespace std;
#define ll long long

int main()
{
    int n;
    while(cin >> n)
    {
        int a[200][200];
        memset(a,0, sizeof(a));
        for(int i=0;i < n;i++)
        {
            for(int j=0;j <= i;j++)
                cin >> a[i][j];
        }
        int b[200][200];
        memset(b,0, sizeof(b));
        b[0][0] = a[0][0];
        for(int i=0;i < n-1;i++)
        {
            for(int j=0;j <= i;j++)
            {
                b[i+1][j] = max(b[i][j]+a[i+1][j],b[i+1][j]);
                b[i+1][j+1] = max(b[i][j]+a[i+1][j+1],b[i+1][j+1]);
            }
        }
//        for(int i=0;i < n;i++)
//        {
//            for(int j=0;j <= i;j++)
//                cout << b[i][j] <<" ";
//            cout << endl;
//        }

        int maxnum = -10000;
        for(int j=0;j < n;j++)
        {
            if(maxnum < b[n-1][j])
                maxnum = b[n-1][j];
        }
        cout << maxnum << endl;
    }
    return 0;
}
```

厉害！无敌！牛逼的我！

```
#include<bits/stdc++.h>
using namespace std;
#define ll long long
int a[200][200];
int n;
 
int maxsum = -10000;
void dfs(int x,int y,int sum)
{
    sum += a[x][y];
    if(sum > maxsum)
        maxsum = sum;
    if(x == n)
        return;
    dfs(x+1,y,sum);
    dfs(x+1,y+1,sum);
}
int main()
{
    while(cin >> n)
    {
        maxsum = -10000;
        memset(a,0, sizeof(a));
        for(int i=0;i < n;i++)
        {
            for(int j=0;j <= i;j++)
            {
                cin >> a[i][j];
            }
        }
        dfs(0,0,0);
        cout << maxsum << endl;
    }
    return 0;
}
/**************************************************************
    Problem: 3298
    User: 2016016294
    Language: C++
    Result: 时间超限
****************************************************************/
```

一开始用dfs时间超限了。。

![](https://images2017.cnblogs.com/blog/1196023/201712/1196023-20171214125233842-750810331.png)

嘿嘿












