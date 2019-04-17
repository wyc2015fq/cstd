# Priest John's Busiest Day（POJ-3683） - Alex_McAvoy的博客 - CSDN博客





2018年12月15日 22:14:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42








> 
# Problem Description

John is the only priest in his town. September 1st is the John's busiest day in a year because there is an old legend in the town that the couple who get married on that day will be forever blessed by the God of Love. This year N couples plan to get married on the blessed day. The i-th couple plan to hold their wedding from time Si to time Ti. According to the traditions in the town, there must be a special ceremony on which the couple stand before the priest and accept blessings. The i-th couple need Di minutes to finish this ceremony. Moreover, this ceremony must be either at the beginning or the ending of the wedding (i.e. it must be either from Si to Si + Di, or from Ti - Di to Ti). Could you tell John how to arrange his schedule so that he can present at every special ceremonies of the weddings.

Note that John can not be present at two weddings simultaneously.

# **Input**

The first line contains a integer N ( 1 ≤ N ≤ 1000). 

The next N lines contain the Si, Ti and Di. Si and Ti are in the format of hh:mm.

# Output

The first line of output contains "YES" or "NO" indicating whether John can be present at every special ceremony. If it is "YES", output another N lines describing the staring time and finishing time of all the ceremonies.

# Sample Input

**208:00 09:00 3008:15 09:00 20**

# Sample Output

**YES08:00 08:3008:40 09:00**


题意：n 对夫妇结婚，每次婚礼持续 d 时间，从 s 时间到 t 时间之间举行，只能选择从 s 到 s+d 或 t-d 到 t 时间两个时间段举行，现在有一个神父，问他有没有可能参加所有夫妇的婚礼，要求时间段完整且任意两对夫妇婚礼时间不重叠，若可以，则输出一个可行的方案

思路：

n 对夫妇，每对夫妇的婚礼时间只有两种选择，要么选择 [s,s+d]，要么选择 [t-d,t] ，将每个时间段设为一条线段，那么题目也就是有 n*2 条线段，其中第 i 与 i+n 只能选择一条，求是否能给出 n 条线段，使得这 n 条线段之间不存在交点

时间只是一个媒介，当某两个仪式的时间段重合，就添加限制条件进行建边

由于是一个仪式只能选择一个时间段，因此当两个仪式的时间段重合时，就添加或条件进行建边，设第 i、j 个婚礼，0 是选择前面的时间段，1 是选择后面的时间段，则：
- 当 i 的起始时间小于 j 的起始时间加上仪式持续时间 **且** j 的起始时间小于 i 的起始时间加上仪式持续时间，那么要么参加婚礼 i 的起始要么参加婚礼 j 的起始，即：or(i,0,j,0)
- 当 i 的结束时间减去仪式持续时间小于 j 的结束时间 **且** j 的结束时间减去仪式持续时间小于 i 的结束时间，那么要么参加婚礼 i 的结束要么参加婚礼 j 的结束，即：or(i,1,j,1)
- 当 i 的起始时间小于 j 的结束时间 **且** j 的结束时间减去仪式持续时间小于 i 的起始时间加上仪式持续时间，那么要么参加婚礼 i 的起始要么参加婚礼 j 的结束，即：or(i,0,j,1)
- 当 i 的结束时间减去婚礼持续时间小于 j 的起始时间加上仪式持续时间 **且** j 的起始时间小于 i 的结束时间，那么要么参加婚礼 i 的结束要么参加婚礼 j 的起始，即：or(i,1,j,0)

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
#define N 10001
#define LL long long
using namespace std;
bool vis[N*2];
int Stack[N*2],top;
vector<int> G[N*2];
void init(int n){
    memset(vis,false,sizeof(vis));
    for(int i=0;i<2*n;i++)
        G[i].clear();
}
void addOrClause(int x,int xVal,int y,int yVal){
    //由于x与y值有冲突,因此需要修改
    x=x*2+xVal;
    y=y*2+yVal;
    G[x].push_back(y^1);
    G[y].push_back(x^1);
}
void addAndClause(int x,int xval,int y,int yval) {
    x=x*2+xval;
    y=y*2+yval;
    G[x].push_back(y);
}
bool dfs(int x){
    if(vis[x^1])
        return false;

    if(vis[x])
        return true;
    vis[x]=true;

    Stack[top++]=x;

    for(int i=0;i<G[x].size();i++)
        if(!dfs(G[x][i]))
            return false;

    return true;
}
bool twoSAT(int n){
    for(int i=0;i<2*n;i+=2){
        if(!vis[i] && !vis[i+1]){
            top=0;

            if(!dfs(i)){
                while(top>0)
                    vis[Stack[--top]]=false;

                if(!dfs(i+1))
                    return false;
            }
        }
    }
    return true;
}
struct Time{
    int s,e,d;
    Time(){}
    Time(int s,int e,int d):s(s),e(e),d(d){}
}a[N];
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&(n)){
        init(n);
        for(int i=0;i<n;i++){
            int sh,sm,eh,em,d;
            scanf("%d:%d %d:%d %d",&sh,&sm,&eh,&em,&d);
            a[i]=Time(sh*60+sm,eh*60+em,d);
        }

        for(int i=0;i<n;i++){
            for(int j=i+1;j<n;j++){
                if(a[i].s<a[j].s+a[j].d && a[j].s<a[i].s+a[i].d)//参加i或j的起始
                    addOrClause(i,0,j,0);
                if(a[i].e-a[i].d<a[j].e && a[j].e-a[j].d<a[i].e)//参加i或j的结束
                    addOrClause(i,1,j,1);
                if(a[i].s<a[j].e && a[j].e-a[j].d<a[i].s+a[i].d)//参加i的起始或j的结束
                    addOrClause(i,0,j,1);
                if(a[i].e-a[i].d<a[j].s+a[j].d && a[j].s<a[i].e)//参加i的结束或j的起始
                    addOrClause(i,1,j,0);
            }
        }

        if(!twoSAT(n))
            printf("NO\n");
        else{
            printf("YES\n");
            for(int i=0;i<n;i++){
                if(vis[i*2])
                    printf("%02d:%02d %02d:%02d\n",a[i].s/60,a[i].s%60,(a[i].s+a[i].d)/60,(a[i].s+a[i].d)%60);
                else
                    printf("%02d:%02d %02d:%02d\n",(a[i].e-a[i].d)/60,(a[i].e-a[i].d)%60,a[i].e/60,a[i].e%60);
            }
        }
    }

    return 0;
}
```





