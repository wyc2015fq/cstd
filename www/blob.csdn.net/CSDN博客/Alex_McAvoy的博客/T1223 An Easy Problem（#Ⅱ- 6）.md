# T1223	An Easy Problem（#Ⅱ- 6） - Alex_McAvoy的博客 - CSDN博客





2018年05月12日 19:04:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：715








### 【题目描述】



给定一个正整数N，求最小的、比N大的正整数M，使得M与N的二进制表示中有相同数目的1。

举个例子，假如给定的N为78，其二进制表示为1001110，包含4个1，那么最小的比N大的并且二进制表示中只包含4个1的数是83，其二进制是1010011，因此83就是答案。

### 【输入】

输入若干行，每行一个数n(1≤n≤1000000)，输入"0"结束。



### 【输出】

输出若干行对应的值。

### 【输入样例】

```
1
2
3
4
78
0
```

### 【输出样例】

```
2
4
5
8
83
```

### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#define INF 999999999
#define N 1001
using namespace std;
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF&&n)
    {
        int cnt=0;
        int temp=n;
        while(temp>0)
        {
            if(temp%2)
                cnt++;
            temp/=2;
        }
        int sum=0;
        while(1)
        {
            temp=++n;
            while(temp>0)
            {
                if(temp%2)
                    sum++;
                temp/=2;
            }
            if(cnt==sum)
            {
                cout<<n<<endl;
                break;
            }
            sum=0;
        }
    }
    return 0;
}
```





