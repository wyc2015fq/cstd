# Get Luffy Out（POJ-2723） - Alex_McAvoy的博客 - CSDN博客





2018年12月12日 21:14:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48
个人分类：[POJ																[图论——2-SAT](https://blog.csdn.net/u011815404/article/category/8348292)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Ratish is a young man who always dreams of being a hero. One day his friend Luffy was caught by Pirate Arlong. Ratish set off at once to Arlong's island. When he got there, he found the secret place where his friend was kept, but he could not go straight in. He saw a large door in front of him and two locks in the door. Beside the large door, he found a strange rock, on which there were some odd words. The sentences were encrypted. But that was easy for Ratish, an amateur cryptographer. After decrypting all the sentences, Ratish knew the following facts: 

Behind the large door, there is a nesting prison, which consists of M floors. Each floor except the deepest one has a door leading to the next floor, and there are two locks in each of these doors. Ratish can pass through a door if he opens either of the two locks in it. There are 2N different types of locks in all. The same type of locks may appear in different doors, and a door may have two locks of the same type. There is only one key that can unlock one type of lock, so there are 2N keys for all the 2N types of locks. These 2N keys were divided into N pairs, and once one key in a pair is used, the other key will disappear and never show up again. 

Later, Ratish found N pairs of keys under the rock and a piece of paper recording exactly what kinds of locks are in the M doors. But Ratish doesn't know which floor Luffy is held, so he has to open as many doors as possible. Can you help him to choose N keys to open the maximum number of doors?

# **Input**

There are several test cases. Every test case starts with a line containing two positive integers N (1 <= N <= 210) and M (1 <= M <= 211) separated by a space, the first integer represents the number of types of keys and the second integer represents the number of doors. The 2N keys are numbered 0, 1, 2, ..., 2N - 1. Each of the following N lines contains two different integers, which are the numbers of two keys in a pair. After that, each of the following M lines contains two integers, which are the numbers of two keys corresponding to the two locks in a door. You should note that the doors are given in the same order that Ratish will meet. A test case with N = M = 0 ends the input, and should not be processed.

# Output

For each test case, output one line containing an integer, which is the maximum number of doors Ratish can open.

# Sample Input

**3 60 31 24 50 10 24 14 23 52 20 0**

# Sample Output

**4**


题意：给出 2n 把钥匙，将其分成 n 对，每对由两个不同的钥匙组成，现在按顺序出现 m 个门，每个门上有 2 个 锁，只需要打开其中一个锁就能打开门，现在要用手中的钥匙按顺序开门，对于属于同一对的钥匙，如果用了 A ，就再也不能用 B，问按顺序最多能开多少个门

思路：对于每把钥匙，其对应两个结点 用、不用，设用为 0，不用为 1

对于任一个门来说，两个锁至少要选一个，即对同一组的两把钥匙 a、b：
- a 用，则 b 不用：<a,1,b,0>
- b 用，则 a 不用：<b,1,a,0>

于是可以通过枚举来逐个判断能打开多少道门，假设另 i=3 时，添加 3 个锁生成的边，判断该 2-SAT 是否有解，若有解，则尝试 i=4，若无解，则之后的肯定无解

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

        for(int i=0;i<n;i++){
            int a,b;
            scanf("%d%d",&a,&b);//钥匙分组
            addAndClause(a,0,b,1);
            addAndClause(b,0,a,1);
        }

        int i;
        for(i=0;i<m;i++){
            memset(vis,false,sizeof(vis));

            int a,b;
            scanf("%d%d",&a,&b);//每道门可用的钥匙
            addAndClause(a,1,b,0);
            addAndClause(b,1,a,0);

            if(!twoSAT(n*2))//若当前门下无解
                break;
            
        }
        int maxx=i;//最大开门数
        for(i++;i<m;i++){
            int a,b;
            scanf("%d%d",&a,&b);//继续读完未读门
        }

        printf("%d\n",maxx);
    }

    return 0;
}
```






