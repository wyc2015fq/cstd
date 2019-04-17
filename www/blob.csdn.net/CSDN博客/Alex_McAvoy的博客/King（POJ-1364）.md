# King（POJ-1364） - Alex_McAvoy的博客 - CSDN博客





2018年12月21日 14:30:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：68
个人分类：[POJ																[图论——差分约束系统](https://blog.csdn.net/u011815404/article/category/8543163)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Once, in one kingdom, there was a queen and that queen was expecting a baby. The queen prayed: ``If my child was a son and if only he was a sound king.'' After nine months her child was born, and indeed, she gave birth to a nice son. 

Unfortunately, as it used to happen in royal families, the son was a little retarded. After many years of study he was able just to add integer numbers and to compare whether the result is greater or less than a given integer number. In addition, the numbers had to be written in a sequence and he was able to sum just continuous subsequences of the sequence. 


The old king was very unhappy of his son. But he was ready to make everything to enable his son to govern the kingdom after his death. With regards to his son's skills he decided that every problem the king had to decide about had to be presented in a form of a finite sequence of integer numbers and the decision about it would be done by stating an integer constraint (i.e. an upper or lower limit) for the sum of that sequence. In this way there was at least some hope that his son would be able to make some decisions. 


After the old king died, the young king began to reign. But very soon, a lot of people became very unsatisfied with his decisions and decided to dethrone him. They tried to do it by proving that his decisions were wrong. 


Therefore some conspirators presented to the young king a set of problems that he had to decide about. The set of problems was in the form of subsequences Si = {aSi, aSi+1, ..., aSi+ni} of a sequence S = {a1, a2, ..., an}. The king thought a minute and then decided, i.e. he set for the sum aSi + aSi+1 + ... + aSi+ni of each subsequence Si an integer constraint ki (i.e. aSi + aSi+1 + ... + aSi+ni < ki or aSi + aSi+1 + ... + aSi+ni > ki resp.) and declared these constraints as his decisions. 


After a while he realized that some of his decisions were wrong. He could not revoke the declared constraints but trying to save himself he decided to fake the sequence that he was given. He ordered to his advisors to find such a sequence S that would satisfy the constraints he set. Help the advisors of the king and write a program that decides whether such a sequence exists or not. 

# **Input**

The input consists of blocks of lines. Each block except the last corresponds to one set of problems and king's decisions about them. In the first line of the block there are integers n, and m where 0 < n <= 100 is length of the sequence S and 0 < m <= 100 is the number of subsequences Si. Next m lines contain particular decisions coded in the form of quadruples si, ni, oi, ki, where oi represents operator > (coded as gt) or operator < (coded as lt) respectively. The symbols si, ni and ki have the meaning described above. The last block consists of just one line containing 0.

# Output

The output contains the lines corresponding to the blocks in the input. A line contains text successful conspiracy when such a sequence does not exist. Otherwise it contains text lamentable kingdom. There is no line in the output corresponding to the last ``null'' block of the input.

# Sample Input

**4 21 2 gt 02 2 lt 21 21 0 gt 01 0 lt 00**

# Sample Output

**lamentable kingdomsuccessful conspiracy**


题意：给出一个数字序列 S={a1,a2,…an}，它有 m 个子序列 Si={a[si], a[si+1], a[si+2], … a[si+ni]}，现在给出 m 个限制条件：

第 i 个子序列的和 < ki 或 第 i 个子序列的和 > ki

思路：令 S[i]=a1+a2+...+ai，对于每一约束条件：a[si]+a[si+1]+…+a[si+ni] < ki 可转换为 S[si+ni] – S[si-1] <= ki-1，a[si]+a[si+1]+…+a[si+ni] >ki 可转换为 S[si+ni] – S[si-1] >= ki+1，从而有一个差分约束系统。

对于 S[si+ni] – S[si-1] <= ki-1，可以得到 si-1 到 si+ni 的权值为 ki-1 的边，对于 S[si+ni] – S[si-1] >= ki+1 可以得到 si+ni 到 si-1 权值为 -ki-1 的边，由于存在负值且要判断是否有解，因此需要构造一超级源点 n+1 号，使得从 n+1 号到 0、1、...、n 号点边权为 0，然后再用 SPFA 算法判是否有负环即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
using namespace std;

struct Edge{
    int from;
    int to;
    int w;
    Edge(){}
    Edge(int from,int to,int w):from(from),to(to),w(w){}
}edge[N];
int head[N],next[N],num;
int dis[N];
bool vis[N];
int outque[N];
void init(){
    num=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int from,int to,int w){
    edge[num]=Edge(from,to,w);
    next[num]=head[from];
    head[from]=num++;
}
bool SPFA(int n){
    memset(vis,false,sizeof(vis));
    memset(outque,0,sizeof(outque));
    for(int i=0;i<n;i++)
        dis[i]=INF;
    dis[n-1]=0;

    queue<int> Q;
    Q.push(n-1);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        vis[x]=false;

        outque[x]++;
        if(outque[x]>n-1)
            return true;

        for(int i=head[x];i!=-1;i=next[i]){
            Edge &e=edge[i];
            if(dis[e.to]>dis[x]+e.w){
                dis[e.to]=dis[x]+e.w;
                if(!vis[e.to]){
                    vis[e.to]=true;
                    Q.push(e.to);
                }
            }
        }
    }
    return false;
}
int main(){
    int n,m;
    while(scanf("%d",&n)!=EOF&&(n)){
        scanf("%d",&m);
        init();
        while(m--){
            char ch[10];
            int si,ni,ki;
            scanf("%d%d%s%d",&si,&ni,ch,&ki);

            if(ch[0]=='l')
                addEdge(si-1,si+ni,ki-1);
            else if(ch[0]=='g')
                addEdge(si+ni,si-1,-ki-1);
        }
        for(int i=0;i<=n;i++)
            addEdge(n+1,i,0);

        printf("%s\n",SPFA(n+2)?"successful conspiracy":"lamentable kingdom");
    }
    return 0;
}
```






