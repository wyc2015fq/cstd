# Perfect Election（POJ-3905） - Alex_McAvoy的博客 - CSDN博客





2018年12月11日 17:54:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：71








> 
# Problem Description

In a country (my memory fails to say which), the candidates {1, 2 ..., N} are running in the parliamentary election. An opinion poll asks the question "For any two candidates of your own choice, which election result would make you happy?". The accepted answers are shown in the table below, where the candidates i and j are not necessarily different, i.e. it may happen that i=j. There are M poll answers, some of which may be similar or identical. The problem is to decide whether there can be an election outcome (It may happen that all candidates fail to be elected, or all are elected, or only a part of them are elected. All these are acceptable election outcomes.) that conforms to all M answers. We say that such an election outcome is perfect. The result of the problem is 1 if a perfect election outcome does exist and 0 otherwise.

# **Input**

Each data set corresponds to an instance of the problem and starts with two integral numbers: 1≤N≤1000 and 1≤M≤1000000. The data set continues with M pairs ±i ±j of signed numbers, 1≤i,j≤N. Each pair encodes a poll answer as follows: 
|Accepted answers to the poll question|Encoding|
|----|----|
|I would be happy if at least one from i and j is elected.|+i +j|
|I would be happy if at least one from i and j is not elected.|-i -j|
|I would be happy if i is elected or j is not elected or both events happen.|+i -j|
|I would be happy if i is not elected or j is elected or both events happen.|-i +j|

The input data are separated by white spaces, terminate with an end of file, and are correct.

# Output

For each data set the program prints the result of the encoded election problem. The result, 1 or 0, is printed on the standard output from the beginning of a line. There must be no empty lines on output.

# Sample Input

**3 3  +1 +2  -1 +2  -1 -3 2 3  -1 +2  -1 -2  +1 -2 2 4  -1 +2  -1 -2  +1 -2  +1 +2 2 8  +1 +2  +2 +1  +1 -2  +1 -2  -2 +1  -1 +1  -2 -2  +1 -1**

# Sample Output

**1101**


题意：n 个人选举 m 条民意结果，编码方式如下：
|若 i 与 j 至少一人当选，我会高兴|+i +j|
|----|----|
|若 i 与 j 至少一人不当选，我会高兴|-i -j|
|若 i 当选或 j 不当选，我会高兴|+i -j|
|若 i 不当选 或 j 当选，我会高兴|-i +j|

现在要根据给出的 m 条民意结果，判断是否符合存在相应的选举结果，输出 1 或 0

思路：

对于 n 个参选人，每个人都有两种状态，当选或不当选，根据民意编码结果可知，+ 代表当选，- 代表不当选

设对于参选人 i、j，0 代表不当选，1 代表当选，则有：
- i 与 j 至少一人当选：or(i,1,j,1)
- i 与 j 至少一人不当选：or(i,0,j,0)
- i 当选或 j 不当选：or(i,1,j,0)
- i 不当选或 j 当选：or(i,0,j,1)

根据当选与不当选的编码方式可以看出，每条民意都是或的关系，因此直接根据 m 条民意结果进行建图即可，要注意的是，候选人是从 1 开始的，因此处理是要将编号由 1~n 改为由 0~n-1

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
void AddAndClause(int x,int xval,int y,int yval) {
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
    int n;
    int m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        init(n);
        for(int i=0;i<m;i++){
            int a,b;
            scanf("%d%d",&a,&b);
            int aVal=a<0?1:0;//a是否当选
            int bVal=b<0?1:0;//b是否当选

            //处理编号
            a=abs(a)-1;
            b=abs(b)-1;

            addOrClause(a,aVal,b,bVal);//添加限制条件
        }
        bool res=twoSAT(n);
        if(res)
            printf("1\n");
        else
            printf("0\n");
    }
    return 0;
}
```





