# POJ1703 Find them, Catch them 并查集 - 紫芝的博客 - CSDN博客





2018年05月19日 22:42:06[紫芝](https://me.csdn.net/qq_40507857)阅读数：31
个人分类：[并查集](https://blog.csdn.net/qq_40507857/article/category/7569705)











[点击打开链接](http://poj.org/problem?id=1703)



Find them, Catch them

|Time Limit: 1000MS||Memory Limit: 10000K|
|----|----|----|
|Total Submissions: 50757||Accepted: 15554|
Description

The police office in Tadu City decides to say ends to the chaos, as launch actions to root up the TWO gangs in the city, Gang Dragon and Gang Snake. However, the police first needs to identify which gang a criminal belongs to. The present question is, given two criminals; do they belong to a same clan? You must give your judgment based on incomplete information. (Since the gangsters are always acting secretly.)

Assume N (N <= 10^5) criminals are currently in Tadu City, numbered from 1 to N. And of course, at least one of them belongs to Gang Dragon, and the same for Gang Snake. You will be given M (M <= 10^5) messages in sequence, which are in the following two kinds:

1. D [a] [b]
where [a] and [b] are the numbers of two criminals, and they belong to different gangs.

2. A [a] [b]
where [a] and [b] are the numbers of two criminals. This requires you to decide whether a and b belong to a same gang.


Input

The first line of the input contains a single integer T (1 <= T <= 20), the number of test cases. Then T cases follow. Each test case begins with a line with two integers N and M, followed by M lines each containing one message as described above. 

Output

For each message "A [a] [b]" in each case, your program should give the judgment based on the information got before. The answers might be one of "In the same gang.", "In different gangs." and "Not sure yet."

Sample Input
1
5 5
A 1 2
D 1 2
A 1 2
D 2 4
A 1 4

Sample Output
Not sure yet.
In different gangs.
In the same gang.

Source

[POJ Monthly--2004.07.18](http://poj.org/searchproblem?field=source&key=POJ+Monthly--2004.07.18)




题意：



一共有n个元素，这些元素要么属于集合1，要么属于集合2，





有m个操作，A  a   b 为询问，D a   b 表示a，b属于不同的集合



解法一：







利用2*n个元素建立并查集，每个元素x，有两种表示：x和x+n



若a，b属于不同集合，则 a和b+n属于同一个集合，a+n和b属于同一个集合



对于每次询问：



若find（a)=find (b),则a和b属于同一个集合



若find(a+n)==find(b)并且find(a)==find(b+n)，则a和b属于不同集合

```cpp
#include<iostream>
#include<cstdio>
using namespace std;
const int maxn=1e5+10;
int p[maxn<<1];
int find(int x)
{
    if(x==p[x])    return x;
    return p[x]=find(p[x]);
}
void mix(int x,int y)
{
    int fx=find(x),fy=find(y);
    if(fx!=fy)
    p[fx]=fy;
}

int main()
{
    int n,m,t;
    cin>>t;
    while(t--){
        //memset(vis,0,sizeof(vis));
        cin>>n>>m;
        for(int i=1;i<=2*n;i++)
        {
            p[i]=i;
        }
        char op[2];
        int a,b;
        for(int i=0;i<m;i++)
        {
            scanf("%s%d%d",op,&a,&b);
            if(op[0]=='A')
            {
                if (find(a) == find(b))
                    printf("In the same gang.\n");
                else if (find(a)==find(b+n) && find(a+n)==find(b))
                    printf("In different gangs.\n");
                else
                    printf("Not sure yet.\n");
            }else{
                    mix(a+n,b);mix(a,b+n);
            }
        }
    }
    return 0;
}
```

解法二：

带权并查集

r数组记录每个结点与其父亲结点的关系
r[x]=0,代表x与其父亲是在同一个集合
r[x]=1,代表x与其父亲是在不在同一个集合
初始化r[x]=0,每个元素和自己在同一集合

1.find函数在寻找根节点时不断更新r数组
根据子结点与父亲结点的关系，以及父亲结点与爷爷结点的关系，
可以知道子结点与爷爷结点的关系，具体为


若a和b的关系为rab,b和c的关系为rbc，则a和C的关系为rac=(rab+rbc)%2;


|**（爷爷，父亲）**|**（父亲，儿子）**|**（爷爷，儿子）**|
|----|----|----|
|**0**|**0**|**0**|
|**0**|**1**|**1**|
|**1**|**0**|**1**|
|**1**|**1**|**0**|
2.union函数更新两棵树的关系：
定义：fx 为 x的根节点， fy 为 y 的根节点，联合时，使得fa[ fy ] = fx；
同时也要寻找 fx 和 fy 的关系，其关系为（r[ x ]  + 1 - r[ y ]) % 2;

因为确定了 x 和 y 的关系是 1 ，因此 r[ fy ] = (r[ x ] + 1 - r[ y ]) % 2；

![](https://images2015.cnblogs.com/blog/885804/201611/885804-20161108155919842-1873511475.png)

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int maxn = 100005;
int fa[maxn],r[maxn];
 
int find(int x)
{
    if (fa[x] == x) return fa[x];
    int tmp = fa[x];
    fa[x] = find(fa[x]);
    r[x] = (r[tmp] + r[x]) % 2;
    return fa[x];
}
 
void unite(int x,int y)
{
    int fx = find(x),fy = find(y);
    if (fx == fy)   return;
    fa[fy] = fx;
    r[fy] = (r[x] + 1 - r[y]) % 2;
}
 
int main()
{
    int T;
    scanf("%d",&T);
    while (T--)
    {
        int N,M,x,y;
        char opt[5];
        scanf("%d%d",&N,&M);
        for (int i = 0;i <= N;i++)   fa[i] = i,r[i] = 0;
        while (M--)
        {
            scanf("%s %d %d",opt,&x,&y);
            if (opt[0] == 'A')
            {
                if (find(x) == find(y))
                {
                    if (r[x] == r[y])   printf("In the same gang.\n");
                    else    printf("In different gangs.\n");
                }
                else    printf("Not sure yet.\n");
            }
            else    unite(x,y);
        }
    }
    return 0;
}
```





