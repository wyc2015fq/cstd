# Wedding（POJ-3648） - Alex_McAvoy的博客 - CSDN博客





2018年12月14日 16:32:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：56
个人分类：[POJ																[图论——2-SAT](https://blog.csdn.net/u011815404/article/category/8348292)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Up to thirty couples will attend a wedding feast, at which they will be seated on either side of a long table. The bride and groom sit at one end, opposite each other, and the bride wears an elaborate headdress that keeps her from seeing people on the same side as her. It is considered bad luck to have a husband and wife seated on the same side of the table. Additionally, there are several pairs of people conducting adulterous relationships (both different-sex and same-sex relationships are possible), and it is bad luck for the bride to see both members of such a pair. Your job is to arrange people at the table so as to avoid any bad luck.

# **Input**

The input consists of a number of test cases, followed by a line containing 0 0. Each test case gives n, the number of couples, followed by the number of adulterous pairs, followed by the pairs, in the form "4h 2w" (husband from couple 4, wife from couple 2), or "10w 4w", or "3h 1h". Couples are numbered from 0 to n - 1 with the bride and groom being 0w and 0h.

# Output

For each case, output a single line containing a list of the people that should be seated on the same side as the bride. If there are several solutions, any one will do. If there is no solution, output a line containing "bad luck".

# Sample Input

**10 63h 7h5w 3w7h 6w8w 3w7h 3w2w 5h0 0**

# Sample Output

**1h 2h 3w 4h 5h 6h 7h 8h 9h**


题意：1 对夫妇结婚，n-1 对夫妇去参加婚礼，有一个很长的桌子，新郎新娘同时坐在桌子两边，然后 n-1 对夫妇就座，要求任何一对夫妇都不能坐在同一边，并且有 m 对人有奸情，有奸情的两个人不能同时坐在新娘对面，只能分开做或都做到新娘的那边，要求输出坐在新娘一边的人

思路：一共有 n 对夫妇，共 2n 个人，对于每个人，将他们分成两个结点，在左边标记为 0，在右边标记为 1，首先让新娘在左边，新郎在右边

对于每对夫妇，由于不能坐同一边，假设第 i 对夫妇中 a 表示妻子 b 表示丈夫，则：
- a 在左边，则 b 在右边：(a,0,b,1)
- a 在右边，则 b 在左边：(a,1,b,0)
- b 在左边，则 a 在右边：(b,0,a,1)
- b 在右边，则 a 在左边：(b,1,a,0)

然后对于每对有奸情的 a 与 b 由于他们不能同时在新娘的对面，即不能在右边，则：
- a 在右边，则 b 在左边：(a,1,b,0)
- b 在右边，则 a 在左边：(b,1,a,0)

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
    x=x*2+xVal;
    y=y*2+yVal;
    G[x^1].push_back(y);
    G[y^1].push_back(x);
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
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){

        init(n*2);

        addAndClause(0,1,0,0);//新娘在左
        addAndClause(1,0,1,1);//新郎在右

        //n-1对夫妇
        for(int i=1;i<n;i++){
            //拆点
            int a=i*2;//妻子
            int b=i*2+1;//丈夫

            //初始条件
            addAndClause(a,0,b,1);
            addAndClause(a,1,b,0);
            addAndClause(b,0,a,1);
            addAndClause(b,1,a,0);
        }

        for(int i=0;i<m;i++){
            int a,b;//有奸情的两个人
            char ch1[10],ch2[10];

            scanf("%d%s%d%s",&a,ch1,&b,ch2);

            //拆点
            if(ch1[0]=='w')
                a=a*2;//妻子
            else
                a=a*2+1;//丈夫
            if(ch2[0]=='w')
                b=b*2;//妻子
            else
                b=b*2+1;//丈夫

            //添加限制条件
            addAndClause(a,1,b,0);
            addAndClause(b,1,a,0);
        }

        if(!twoSAT(n*2))
            printf("bad luck\n");
        else{
            for(int i=2;i<2*n;i+=2){
                if(vis[i*2])
                    printf("%dw ",i/2);
                if(vis[(i+1)*2])
                    printf("%dh ",i/2);
            }
            printf("\n");
        }
    }

    return 0;
}
```






