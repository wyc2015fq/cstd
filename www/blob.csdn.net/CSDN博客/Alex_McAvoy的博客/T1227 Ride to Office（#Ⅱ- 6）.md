# T1227	Ride to Office（#Ⅱ- 6） - Alex_McAvoy的博客 - CSDN博客





2018年05月13日 13:33:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：634








### 【题目描述】

起点与终点相隔4500米。现Charley 需要从起点骑车到终点。但是，他有个习惯，沿途需要有人陪伴，即以相同的速度， 与另外一个人一起骑。而当他遇到以更快的速度骑车的人时，他会以相应的速度跟上这个更快的人。先给定所有与Charley 同路的人各自的速度与出发时间，问Charley 以这种方式跟人，骑完4500米需要多少时间。得出的结果若是小数，则向上取整。

### 【输入】

输入若干组数据，每组数据第一行n(1≤n≤10000),n为0，表示输入结束，接着输入n行数据，每行2个数据，表示速度v和出发时间t，如果t<0，表示陪伴人提早出发了。



### 【输出】

输出对应若干行数据，每行输出1个数，表示最快到达的时间。

### 【输入样例】

```
4
20 0
25 -155
27 190
30 240
2
21 0
22 34
0
```

### 【输出样例】

```
780
771
```

### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#define INF 999999999
#define N 10001
using namespace std;
struct node{
    int v;
    int start;
    int endd;
}a[N];
int main()
{
    int n;
    int t;

    while(cin>>n&&n)
    {
        for(int i=1;i<=n;i++)
        {
            cin>>a[i].v>>a[i].start;
            t=16200/a[i].v;
            if(16200.0/a[i].v>t)
                t++;
            a[i].endd=a[i].start+t;
        }
        int min=INF;
        for(int i=1;i<=n;i++)
            if(a[i].start==0&&a[i].endd<min)
                min=a[i].endd;
        for(int i=1;i<=n;i++)
            if(a[i].start>0&&a[i].endd<min)
                min=a[i].endd;
        cout<<min<<endl;
    }
    return 0;
}
```





