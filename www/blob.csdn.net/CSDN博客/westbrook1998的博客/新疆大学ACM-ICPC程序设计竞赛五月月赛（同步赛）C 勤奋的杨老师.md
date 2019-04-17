# 新疆大学ACM-ICPC程序设计竞赛五月月赛（同步赛）C	勤奋的杨老师 - westbrook1998的博客 - CSDN博客





2018年05月02日 21:46:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：101








emmm 这道题其实比赛的时候有一点思路，但是因为LIS（最大上升子序列）的不熟练导致没写出来

> 
链接：[https://www.nowcoder.com/acm/contest/116/C](https://www.nowcoder.com/acm/contest/116/C)

  来源：牛客网


> 
题目描述  

  杨老师认为他的学习能力曲线是一个拱形。勤奋的他根据时间的先后顺序罗列了一个学习清单，共有n个知识点。但是清单中的知识并不是一定要学习的，可以在不改变先后顺序的情况下有选择的进行学习，而每一个知识点都对应一个难度值。杨老师希望，后学习的知识点的难度一定不低于前一个知识点的难度`（i<j时ai<=aj）`，而可能存在一个临界点，在临界点以后，他希望后学习的知识点的难度一定不高于前一个知识点的难度`（i<j时ai>=aj）`。杨老师想尽可能多的学习知识。请问：杨老师最多可以学习多少知识？


> 
输入描述: 

  第一行：一个整数n`（0<n<500000）`接下来一行：n个整数，第i个整数ai`（0<=ai<500000）`表示第i道题目的难度。


> 
输出描述: 

  一行一个整数，表示杨老师最多可以学习多少个知识。


> 
示例1 

  输入 

  5 

  1 4 2 5 1 

  输出 

  4
思路是顺序和逆序分别求出每一个位置的最大不下降子序列长度，最后遍历每个位置的uplist和downlist和-1的最大值

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int INF=1000001;
const int maxn=500001;
int a[maxn];
int g[maxn];
int uplist[maxn];
int downlist[maxn];
int main(void){
    int n;
    while(scanf("%d",&n)!=EOF){
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        for(int i=1;i<=n;i++){
            g[i]=INF;
        }
        for(int i=0;i<n;i++){
            int k=upper_bound(g+1,g+n,a[i])-g;
            uplist[i]=k;
            g[k]=a[i];
        }
        for(int i=1;i<=n;i++){
            g[i]=INF;
        }
        for(int i=n-1;i>=0;i--){
            int k=upper_bound(g+1,g+n,a[i])-g;
            downlist[i]=k;
            g[k]=a[i];
        }
        int m=0;
        for(int i=0;i<n;i++){
            int t=uplist[i]+downlist[i]-1;
            if(t>m){
                m=t;
            }
        }
        printf("%d\n",m);
    }
    return 0;
}
```

基础的算法一定要学好，虽然起步晚，但是相信努力一定有回报的





