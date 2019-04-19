# nyoj   16  矩形嵌套（基础dp） - HJ - CSDN博客
2017年03月14日 23:07:56[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：231
描述 
    有n个矩形，每个矩形可以用a,b来描述，表示长和宽。矩形X(a,b)可以嵌套在矩形Y(c,d)中当且仅当a< c,b< d或者b< c,a< d（相当于旋转X90度）。例如（1,5）可以嵌套在（6,2）内，但不能嵌套在（3,4）中。你的任务是选出尽可能多的矩形排成一行，使得除最后一个外，每一个矩形都可以嵌套在下一个矩形内。
输入 
    第一行是一个正正数N(0< N<10)，表示测试数据组数， 
    每组测试数据的第一行是一个正正数n，表示该组测试数据中含有矩形的个数(n<=1000) 
    随后的n行，每行有两个数a,b(0< a,b<100)，表示矩形的长和宽 
输出 
    每组测试数据都输出一个数，表示最多符合条件的矩形数目，每组输出占一行 
样例输入
```
1
10
1 2
2 4
5 8
6 10
7 9
3 1
5 8
12 10
9 7
2 2
```
样例输出
```
5
```
解题思路：
设d[i]表示前i个能被第i个嵌套在内的最多矩形个数，则有
```
d[i]=max(d[i],d[j]+1)
```
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
struct node{
    int a,b;
}s[10005];
int d[10005];
int cmp(node x,node y)
{
    if(x.a==y.a)
      return x.b<y.b;
    return x.a<y.a;
}
int main()
{
    int t,n;
    cin>>t;
    while(t--)
    {
        cin>>n;
        for(int i=0;i<n;i++)
        {
            cin>>s[i].a>>s[i].b;
            if(s[i].a>s[i].b)     //小的放前面
              swap(s[i].a,s[i].b);
            d[i]=1;
        } 
        sort(s,s+n,cmp);
        for(int i=0;i<n;i++)
           for(int j=0;j<i;j++)
              if(s[i].a>s[j].a&&s[i].b>s[j].b)  //避免交错比较
                d[i]=max(d[i],d[j]+1);
        int max=d[0];
        for(int i=1;i<n;i++)
          if(max<d[i])
            max=d[i];
        cout<<max<<endl;
    }
    return 0;
}
```
