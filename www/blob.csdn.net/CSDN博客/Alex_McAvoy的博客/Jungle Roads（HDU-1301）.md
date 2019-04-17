# Jungle Roads（HDU-1301） - Alex_McAvoy的博客 - CSDN博客





2018年05月21日 21:32:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43
个人分类：[图论——最小生成树																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813283)








> 
# Problem Description

The Head Elder of the tropical island of Lagrishan has a problem. A burst of foreign aid money was spent on extra roads between villages some years ago. But the jungle overtakes roads relentlessly, so the large road network is too expensive to maintain. The Council of Elders must choose to stop maintaining some roads. The map above on the left shows all the roads in use now and the cost in aacms per month to maintain them. Of course there needs to be some way to get between all the villages on maintained roads, even if the route is not as short as before. The Chief Elder would like to tell the Council of Elders what would be the smallest amount they could spend in aacms per month to maintain roads that would connect all the villages. The villages are labeled A through I in the maps above. The map on the right shows the roads that could be maintained most cheaply, for 216 aacms per month. Your task is to write a program that will solve such problems. 

![](https://img-blog.csdn.net/20180521213147893)

# **Input**

The input consists of one to 100 data sets, followed by a final line containing only 0. Each data set starts with a line containing only a number n, which is the number of villages, 1 < n < 27, and the villages are labeled with the first n letters of the alphabet, capitalized. Each data set is completed with n-1 lines that start with village labels in alphabetical order. There is no line for the last village. Each line for a village starts with the village label followed by a number, k, of roads from this village to villages with labels later in the alphabet. If k is greater than 0, the line continues with data for each of the k roads. The data for each road is the village label for the other end of the road followed by the monthly maintenance cost in aacms for the road. Maintenance costs will be positive integers less than 100. All data fields in the row are separated by single blanks. The road network will always allow travel between all the villages. The network will never have more than 75 roads. No village will have more than 15 roads going to other villages (before or after in the alphabet). In the sample input below, the first data set goes with the map above. 

# Output

The output is one integer per line for each data set: the minimum cost in aacms per month to maintain a road system that connect all the villages. Caution: A brute force solution that examines every possible set of roads will not finish within the one minute time limit. 

# Sample Input

**9**

**A 2 B 12 I 25B 3 C 10 H 40 I 8C 2 D 18 G 55D 1 E 44E 2 F 60 G 38F 0G 1 H 35H 1 I 353A 2 B 10 C 40B 1 C 200**

# Sample Output

**21630**


题意：有n个村庄，用字母表的前n个字母表示，接下来n-1行，每行为一个村庄字母和与其连接的村庄数，以及各村庄的字母和距离。求最小生成树

思路：最小生成树问题，套用Prim算法即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#define INF 999999999
#define N 30
#define MOD 1000000007
#define E 1e-12
using namespace std;
int g[N][N];
int w[N];
bool vis[N];
int main()
{
    int  m, n, minn, temp;
    char ch;

    while(scanf("%d",&n)!=EOF&&n)
    {
        memset(g,0x1f,sizeof(g));
        memset(w,0,sizeof(w));
        memset(vis, 1, sizeof(vis));
        for (int i=1;i<n;i++)
        {
            getchar();
            scanf("%c%d",&ch,&m);
            for (int j=1;j<=m;j++)
            {
                scanf("%c",&ch);
                scanf("%c",&ch);
                scanf("%d",&temp);
                g[i-1][ch-'A']=temp;
                g[ch-'A'][i-1]=temp;
            }
        }
        for(int i=1;i<n;i++)
            w[i]=g[i][0];

        int MST=0;
        vis[0]=0;
        for (int i = 1; i < n; i++)
        {
            minn=INF;
            for(int j=0;j<n;j++)//找出最小权值并记录位置
                if(vis[j] && w[j]<minn)
                {
                    minn=w[j];
                    temp=j;
                }
            vis[temp]=0;
            MST+=minn;
            for(int j=0;j<n;j++)
                if(vis[j] && w[j]>g[j][temp])
                    w[j] = g[j][temp];
        }
        printf("%d\n",MST);
    }
    return 0;
}
```




