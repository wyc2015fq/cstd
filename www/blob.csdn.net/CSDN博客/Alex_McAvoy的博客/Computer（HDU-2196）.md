# Computer（HDU-2196） - Alex_McAvoy的博客 - CSDN博客





2018年10月07日 18:41:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：124








# Problem Description

A school bought the first computer some time ago(so this computer's id is 1). During the recent years the school bought N-1 new computers. Each new computer was connected to one of settled earlier. Managers of school are anxious about slow functioning of the net and want to know the maximum distance Si for which i-th computer needs to send signal (i.e. length of cable to the most distant computer). You need to provide this information. 

![](https://img-blog.csdn.net/2018100718391039?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Hint: the example input is corresponding to this graph. And from the graph, you can see that the computer 4 is farthest one from 1, so S1 = 3. Computer 4 and 5 are the farthest ones from 2, so S2 = 2. Computer 5 is the farthest one from 3, so S3 = 3. we also get S4 = 4, S5 = 4.

# **Input**

Input file contains multiple test cases.In each case there is natural number N (N<=10000) in the first line, followed by (N-1) lines with descriptions of computers. i-th line contains two natural numbers - number of computer, to which i-th computer is connected and length of cable used for connection. Total length of cable does not exceed 10^9. Numbers in lines of input are separated by a space.

# Output

Output should contain the maximal sum of guests' ratings.

# Sample Input

**51 12 13 11 1**

# Sample Output

**32344**

————————————————————————————————————————————————————

题意：给出一个树，求树上每个点可到达的最远距离

思路：搜索两次，一次记录最远距离，一次寻找答案，用 f[x][0] 表示在 x 的子树下 x 的最远距离，用 f[x][1]  表示在 x 的子树下的次远距离，用 f[x][2] 表示通过 x 的父亲能走的最远距离，对于第一次深搜，有 f[x][0]=f[y][0]+dis[i]，其中 y 是 x 的子树，对于第二次深搜，将 f[y][2] 算出来，最后答案即为 max(f[x][0],f[x][2])

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
struct Edge{
    int to;
    int next;
    int dis;
}edge[N*N];
int head[N],f[N][3];
int cnt;
void addEdge(int x,int y,int dis){
    edge[cnt].to=y;
    edge[cnt].next=head[x];
    edge[cnt].dis=dis;
    head[x]=cnt;
    cnt++;
}
void treeDP(int x,int y){   //寻找最大距离与次大距离
    for(int i=head[x];i!=-1;i=edge[i].next){
        y=edge[i].to;
        treeDP(y,x);
        int dis=edge[i].dis;
        int temp=f[y][0]+dis;
        if(temp>=f[x][0]){ //记录最大距离
            f[x][1]=f[x][0];
            f[x][0]=temp;
        }
        else if(temp>f[x][1])
            f[x][1]=temp;
    }
}
void seachRes(int x,int y){ //查找答案
    for(int i=head[x];i!=-1;i=edge[i].next){
        y=edge[i].to;
        if(f[x][0]==f[y][0]+edge[i].dis)
            f[y][2]=max(f[x][2],f[x][1])+edge[i].dis;
        else
            f[y][2]=max(f[x][2],f[x][0])+edge[i].dis;
        seachRes(y,x);
    }
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        memset(head,-1,sizeof(head));
        memset(f,0,sizeof(f));

        for(int y=2;y<=n;y++){
            int x,dis;
            scanf("%d%d",&x,&dis);
            addEdge(x,y,dis);
        }

        treeDP(1,-1);
        seachRes(1,-1);
        for(int i=1;i<=n;i++)
            printf("%d\n",max(f[i][0],f[i][2]));
    }

	return 0;
}
```





