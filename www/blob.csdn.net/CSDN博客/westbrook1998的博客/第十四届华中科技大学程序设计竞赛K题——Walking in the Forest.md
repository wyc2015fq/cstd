# 第十四届华中科技大学程序设计竞赛K题——Walking in the Forest - westbrook1998的博客 - CSDN博客





2018年04月29日 20:40:24[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：69








一个崩溃的早上，一题都做不出来，题解也到晚上看半天才看懂一部分…

题面：

> 
It’s universally acknowledged that there’re innumerable trees in the campus of HUST. 

  Now you’re going to walk through a large forest. There is a path consisting of N stones winding its way to the other side of the forest. Between every two stones there is a distance. Let di indicates the distance between the stone i and i+1.Initially you stand at the first stone, and your target is the N-th stone. You must stand in a stone all the time, and you can stride over arbitrary number of stones in one step. If you stepped from the stone i to the stone j, you stride a span of (di+di+1+…+dj-1). But there is a limitation. You’re so tired that you want to walk through the forest in no more than K steps. And to walk more comfortably, you have to minimize the distance of largest step.


题意就是说有n块石头，每块石头中间有一定的距离，一次可以跳过多个石头，但是不可以超过k步，求最大步的最小值 

早上一直没理解好题意……TAT 

其实意思就是说，如果你每一步比较小，比如一块一块石头地过去，那么步数就太多了，但是如果你直接一步跳到最后，这样又太浪费体力了，而且不符合题意，而题意就是要你找到这么一个平衡点，可以恰好走到三步，然后求其中最大的一步的距离（相对于其他两步是最大的，但相对于所有情况的最大步它是最小的一种情况）
然后这题用到二分查找 

代码：

```cpp
#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;
const int maxn=1e5+10;
long long l,r,mid;
int n,k;
int d[maxn];

//判断一次跳t能否满足题意
bool check(long long t)
{
    //跳的次数
    int tot=0;
    //累计一次跳的距离
    long long cnt=0;
    for(int i=1;i<n;i++)
    {
        //单个石头也跳不过，返回false，通过二分查找大于t的值
        if(d[i]>t){
            return false;
        }
        //两个（多个）跳不过，只能前面算跳一次，从这里重新开始跳
        if(cnt+d[i]>t){
            tot++；
            cnt=d[i];
        }
        //可以跳过，先保存（因为有可能后面还能继续跳过去）
        else{
            cnt+=d[i];
        }
    }
    //如果跳的步数大于k，说明这个步距太小了 继续二分查找大于t的
    return tot<=(k-1);
}

int main()
{
    scanf("%d%d",&n,&k); 
    for(int i=1;i<n;i++)
        scanf("%d",&d[i]),r+=d[i];
    //二分查找
    //当相差为1终止循环 输出r
    while(l<r-1)
    {
        mid=(l+r)>>1;
        //如果判断为true说明mid太大
        if(check(mid))r=mid;
        //否则太小
        else l=mid;
    }
    printf("%lld",r);
    return 0;
}
```

唉给自己加油吧QAQ




