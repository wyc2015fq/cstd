# John's trip（POJ-1041） - Alex_McAvoy的博客 - CSDN博客





2018年11月03日 12:47:30[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：83








> 
# Problem Description

Little Johnny has got a new car. He decided to drive around the town to visit his friends. Johnny wanted to visit all his friends, but there was many of them. In each street he had one friend. He started thinking how to make his trip as short as possible. Very soon he realized that the best way to do it was to travel through each street of town only once. Naturally, he wanted to finish his trip at the same place he started, at his parents' house. 

The streets in Johnny's town were named by integer numbers from 1 to n, n < 1995. The junctions were independently named by integer numbers from 1 to m, m <= 44. No junction connects more than 44 streets. All junctions in the town had different numbers. Each street was connecting exactly two junctions. No two streets in the town had the same number. He immediately started to plan his round trip. If there was more than one such round trip, he would have chosen the one which, when written down as a sequence of street numbers is lexicographically the smallest. But Johnny was not able to find even one such round trip. 

Help Johnny and write a program which finds the desired shortest round trip. If the round trip does not exist the program should write a message. Assume that Johnny lives at the junction ending the street appears first in the input with smaller number. All streets in the town are two way. There exists a way from each street to another street in the town. The streets in the town are very narrow and there is no possibility to turn back the car once he is in the street 

# **Input**

Input file consists of several blocks. Each block describes one town. Each line in the block contains three integers x; y; z, where x > 0 and y > 0 are the numbers of junctions which are connected by the street number z. The end of the block is marked by the line containing x = y = 0. At the end of the input file there is an empty block, x = y = 0.

# Output

Output one line of each block contains the sequence of street numbers (single members of the sequence are separated by space) describing Johnny's round trip. If the round trip cannot be found the corresponding output block contains the message "Round trip does not exist."

# Sample Input

**1 2 12 3 23 1 61 2 52 3 33 1 40 01 2 12 3 21 3 32 4 40 00 0**

# Sample Output

**1 2 3 5 4 6 Round trip does not exist.**


题意：有一无向连通图，分给出点 x、点 y、边 z，表示由 x 点和 y 点构成了 z 边，现在要判断图中是否存在欧拉回路，若存在，则输出字典序最小的欧拉回路，即按输入顺序走过所有边的标号，如果不存在，输出 Round trip does not exist.

思路：  首先判断无向连通图的所有结点是否是偶度，如果不是则不存在欧拉回路，然后要保证字典序最小，因此寻找回路时存在数组中，最后逆序输出，且从小到大的选择与当前结点相连的可行边。

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
int n,m;
int degree[N];
bool vis[N];
int path[N];
int cnt;
struct Edge{
    int x;
    int y;
}edge[N];
void euler(int x){
    for(int i=1;i<=m;i++){
        if(!vis[i] && (edge[i].x==x || edge[i].y==x) ){
            vis[i]=true;
            euler(edge[i].x+edge[i].y-x);
            path[cnt++]=i;
        }
    }
}
int main(){
    int x,y,z;
    while(scanf("%d%d",&x,&y)!=EOF&&x){
        int start=min(x,y);//寻找始点
        cnt=1;
        n=0;
        m=0;
        memset(vis,false,sizeof(vis));
        memset(degree,0,sizeof(degree));

        scanf("%d",&z);
        edge[z].x=x;
        edge[z].y=y;
        degree[x]++;
        degree[y]++;
        m++;//记录边数
        n=max(n,max(x,y));//寻找最大点的编号，即记录点数

        while(scanf("%d%d",&x,&y)!=EOF&&x){
            scanf("%d",&z);
            edge[z].x=x;
            edge[z].y=y;
            degree[x]++;
            degree[y]++;
            m++;//记录边数
            n=max(n,max(x,y));//寻找最大点的编号，即记录点数
        }

        bool flag=false;
        for(int i=1;i<=n;i++){
            if(degree[i]%2==1){
                flag=true;
                break;
            }
        }

        if(flag)
            printf("Round trip does not exist.\n");
        else{
            euler(start);
            for(int i=m;i>=1;i--)
                printf("%d ",path[i]);
            printf("\n");
        }
    }
    return 0;
}
```





