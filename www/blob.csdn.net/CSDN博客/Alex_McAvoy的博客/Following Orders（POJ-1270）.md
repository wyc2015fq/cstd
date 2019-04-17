# Following Orders（POJ-1270） - Alex_McAvoy的博客 - CSDN博客





2018年11月04日 16:19:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：37








> 
# Problem Description

Order is an important concept in mathematics and in computer science. For example, Zorn's Lemma states: ``a partially ordered set in which every chain has an upper bound contains a maximal element.'' Order is also important in reasoning about the fix-point semantics of programs. 

This problem involves neither Zorn's Lemma nor fix-point semantics, but does involve order. 

Given a list of variable constraints of the form x < y, you are to write a program that prints all orderings of the variables that are consistent with the constraints. 

For example, given the constraints x < y and x < z there are two orderings of the variables x, y, and z that are consistent with these constraints: x y z and x z y.

# **Input**

The input consists of a sequence of constraint specifications. A specification consists of two lines: a list of variables on one line followed by a list of contraints on the next line. A constraint is given by a pair of variables, where x y indicates that x < y. 

All variables are single character, lower-case letters. There will be at least two variables, and no more than 20 variables in a specification. There will be at least one constraint, and no more than 50 constraints in a specification. There will be at least one, and no more than 300 orderings consistent with the contraints in a specification. 

Input is terminated by end-of-file. 

# Output

For each constraint specification, all orderings consistent with the constraints should be printed. Orderings are printed in lexicographical (alphabetical) order, one per line. 

Output for different constraint specifications is separated by a blank line. 

# Sample Input

**a b f ga b b fv w x y zv y x v z v w v**

# Sample Output

**abfgabgfagbfgabf**

**wxzvywzxvyxwzvyxzwvyzwxvyzxwvy**


题意：第一行给出由26个单个小写字母表示的变量，第二行给出成对的变量 x，y，表示 x 要在 y 前面，要求输出所有满足第二行的变量序列且按字典序从小到大。

思路：本题本质是要用所给的字母构造一个符合要求的字母序列，即当前被选中的字母必须有效且被选的字母还没被选。

由于保证拓扑排序一定存在，因此使用 dfs 来从小到大构造拓扑排序即可。

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
#define N 1001
#define LL long long
using namespace std;
int n;//点数
bool vis[N];//标记数组
bool bucket[N];//标记变量
int path[N];//存储路径
bool G[N][N];
char str[N];
bool judge(int i,int cnt){
    for(int j=0;j<cnt;j++)
        if(G[i][path[j]])//如果在path[0,cnt-1]出现一个本应在i后面才出现的字母
            return false;
    return true;
}
void dfs(int cnt)
{
    if(cnt==n) {
        for(int i=0;i<n;i++)
            printf("%c",path[i]+'a');
        printf("\n");
    } else{
        for(int i=0;i<26;i++){
            if(bucket[i]&&!vis[i]&&judge(i,cnt)){
               vis[i]=true;//标记为已访问
               path[cnt]=i;//记录路径
               dfs(cnt+1);//向下搜索
               vis[i]=false;//回溯
            }
        }
    }
}
int main()
{
    while(gets(str))
    {
        n=0;
        memset(bucket,false,sizeof(bucket));
        memset(vis,false,sizeof(vis));
        memset(G,false,sizeof(G));

        for(int i=0;str[i];i++){
            if(str[i]!=' '){
                bucket[str[i]-'a']=true;//标记变量
                n++;//记录变量个数
            }
        }

        gets(str);
        for(int i=0;str[i];i++){
            if(str[i]!=' '){
                int x,y;
                x=str[i++]-'a';
                while(str[i]==' ')
                    i++;
                y=str[i]-'a';
                G[x][y]=1;
            }
        }
        dfs(0);
        printf("\n");
    }
    return 0;
}
```





