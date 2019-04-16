# AtCoder Beginner Contest 087 D - People on a Line 并查集（区间和问题） - 紫芝的博客 - CSDN博客





2018年10月09日 21:47:04[紫芝](https://me.csdn.net/qq_40507857)阅读数：35
个人分类：[并查集](https://blog.csdn.net/qq_40507857/article/category/7569705)









# [People on a Line](https://cn.vjudge.net/problem/AtCoder-3882)

Problem Statement

There are *N* people standing on the *x*-axis. Let the coordinate of Person *i* be *x**i*. For every *i*, *x**i* is an integer between 0 and 109 (inclusive). It is possible that more than one person is standing at the same coordinate.

You will given *M* pieces of information regarding the positions of these people. The *i*-th piece of information has the form (*L**i*,*R**i*,*D**i*). This means that Person *R**i* is to the right of Person *L**i* by *D**i* units of distance, that is, *x**R**i*−*x**L**i*=*D**i* holds.

It turns out that some of these *M* pieces of information may be incorrect. Determine if there exists a set of values (*x*1,*x*2,…,*x**N*) that is consistent with the given pieces of information.

Constraints
- 1≤*N*≤100 000
- 0≤*M*≤200 000
- 1≤*L**i*,*R**i*≤*N* (1≤*i*≤*M*)
- 0≤*D**i*≤10 000 (1≤*i*≤*M*)
- *L**i*≠*R**i* (1≤*i*≤*M*)
- If *i*≠*j*, then (*L**i*,*R**i*)≠(*L**j*,*R**j*) and (*L**i*,*R**i*)≠(*R**j*,*L**j*).
- *D**i* are integers.

Input

Input is given from Standard Input in the following format:
*N**M**L*1 *R*1 *D*1
*L*2 *R*2 *D*2
:
*L**M**R**M**D**M*
Output

If there exists a set of values (*x*1,*x*2,…,*x**N*) that is consistent with all given pieces of information, print `Yes`; if it does not exist, print `No`.

Sample Input 1

3 3
1 2 1
2 3 1
1 3 2

Sample Output 1

Yes

Some possible sets of values (*x*1,*x*2,*x*3) are (0,1,2) and (101,102,103).

Sample Input 2

3 3
1 2 1
2 3 1
1 3 5

Sample Output 2

No

If the first two pieces of information are correct, *x*3−*x*1=2 holds, which is contradictory to the last piece of information.

Sample Input 3

4 3
2 1 1
2 3 5
3 4 2

Sample Output 3

Yes

Sample Input 4

10 3
8 7 100
7 9 100
9 8 100

Sample Output 4

No

Sample Input 5

100 0

Sample Output 5

Yes
题意：给出n个数字，m个区间和，判断是否存在那样的区间数字满足m个区间和

思路：并查集区间判断

```cpp
#include <iostream>
#include<string.h>
#include<cstdio>
#include <stack>
#include <vector>
using namespace std;
#define INF 0x7ffffff
typedef long long ll;
const int maxn=1e5+10;
int pre[maxn];//存每个点的祖先
int dis[maxn];//这个点到祖先的距离
int findset(int x)
{
    if(x==pre[x])   return x;
    int root=findset(pre[x]);
    dis[x]=dis[pre[x]]+dis[x];//当前点x到根节点的距离=现在的距离+父节点到根节点的距离
    return pre[x]=root;//把每个子节点合并到他的祖先
}
bool unionset(int x,int y,int w)
{
    int fx=findset(x),fy=findset(y);
    if(fx==fy)
        return w==dis[y]-dis[x];
    pre[fy]=fx;
    dis[fy]=dis[x]+w-dis[y];
    return true;
}
int n,m;
int main()
{
   scanf("%d%d",&n,&m);
   for(int i=1;i<=n;i++)
    pre[i]=i;
   bool lose=true;
   for(int i=0;i<m;i++)
   {
       int x,y,w;
   scanf("%d%d%d",&x,&y,&w);
        if(lose)
       lose=unionset(x,y,w);
   }
   if(lose) printf("Yes\n");
   else     printf("No\n");
}
```





