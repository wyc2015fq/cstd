# hdu  1233    还是畅通工程（Kruskal） - HJ - CSDN博客
2016年08月22日 19:17:15[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：215
Problem Description 
某省调查乡村交通状况，得到的统计表中列出了任意两村庄间的距离。省政府“畅通工程”的目标是使全省任何两个村庄间都可以实现公路交通（但不一定有直接的公路相连，只要能间接通过公路可达即可），并要求铺设的公路总长度为最小。请计算最小的公路总长度。
Input 
测试输入包含若干测试用例。每个测试用例的第1行给出村庄数目N ( < 100 )；随后的N(N-1)/2行对应村庄间的距离，每行给出一对正整数，分别是两个村庄的编号，以及此两村庄间的距离。为简单起见，村庄从1到N编号。 
当N为0时，输入结束，该用例不被处理。
Output 
对每个测试用例，在1行里输出最小的公路总长度。
Sample Input
3 
1 2 1 
1 3 2 
2 3 4 
4 
1 2 1 
1 3 4 
1 4 1 
2 3 3 
2 4 2 
3 4 5 
0
Sample Output
3 
5
Hint 
Hint
Huge input, scanf is recommended.
最小生成树入门题！！！
Kruskal算法代码如下：
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
using namespace std;
#define N 105
int father[N];
struct node    //构造结构体
{
    int u,v,w;
}edge[N*(N-1)/2];
int find(int x)       //查找联通节点
{
    return father[x]==x?x:find(father[x]);
}
int cmp(node a,node b)  //按照w从小到大排序
{
    return a.w<b.w;
}
int main()
{
    int n;
    while(~scanf("%d",&n))
    {
        if(n==0)
          break;
        for(int i=0;i<=n;i++)     //初始化
          father[i]=i;
        n=n*(n-1)/2;
        for(int i=0;i<n;i++)
          scanf("%d %d %d",&edge[i].u,&edge[i].v,&edge[i].w);
        sort(edge,edge+n,cmp);    //排序
        int ans=0;      //赋初值0
        for(int i=0;i<n;i++)
        {
            int x=find(edge[i].u);
            int y=find(edge[i].v);
            if(x!=y)
            {
                ans+=edge[i].w;      //把每次符合条件的最小值累加
                father[y]=x;     //合并
            }
        }
        printf("%d\n",ans);   //输出最小的路总长度
    }
    return 0;
}
```
