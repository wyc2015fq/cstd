# Anniversary party（HDU-1520） - Alex_McAvoy的博客 - CSDN博客





2018年10月06日 18:22:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43
个人分类：[HDU																[动态规划——树形 DP](https://blog.csdn.net/u011815404/article/category/8104361)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

There is going to be a party to celebrate the 80-th Anniversary of the Ural State University. The University has a hierarchical structure of employees. It means that the supervisor relation forms a tree rooted at the rector V. E. Tretyakov. In order to make the party funny for every one, the rector does not want both an employee and his or her immediate supervisor to be present. The personnel office has evaluated conviviality of each employee, so everyone has some number (rating) attached to him or her. Your task is to make a list of guests with the maximal possible sum of guests' conviviality ratings.

# **Input**

Employees are numbered from 1 to N. A first line of input contains a number N. 1 <= N <= 6 000. Each of the subsequent N lines contains the conviviality rating of the corresponding employee. Conviviality rating is an integer number in a range from -128 to 127. After that go T lines that describe a supervisor relation tree. Each line of the tree specification has the form: 

L K 

It means that the K-th employee is an immediate supervisor of the L-th employee. Input is ended with the line 

0 0

# Output

Output should contain the maximal sum of guests' ratings.

# Sample Input

**711111111 32 36 47 44 53 50 0**

# Sample Output

**5**

————————————————————————————————————————————————————

跟 没有上司的舞会（洛谷-P1352） 是一个题，题意及思路：[点击这里](https://blog.csdn.net/u011815404/article/details/82952012)

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
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
vector<int> son[N];
int vis[N];
int f[N][2];
void treeDP(int x){
    for(int i=0;i<son[x].size();i++){//枚举x的所有孩子
        int y=son[x][i];
        treeDP(y);//递归
        f[x][0]+=max(f[y][0],f[y][1]);//父亲没去的值为孩子去或不去的最大值
        f[x][1]+=f[y][0];//父亲去了的值为孩子不去的值
    }
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        for(int i=0;i<=n;i++)
            son[i].clear();
        memset(f,0,sizeof(f));
        memset(vis,0,sizeof(vis));

        for(int i=1;i<=n;i++)//初始化每个结点取值
            scanf("%d",&f[i][1]);

        int x,y;
        while(scanf("%d%d",&x,&y)&&x&&y){
            son[y].push_back(x);//存储关系
            vis[x]=true;//标记
        }

        for(int i=1;i<=n;i++){  //寻找根节点
            if(!vis[i]){
                treeDP(i);
                printf("%d\n",max(f[i][0],f[i][1]));//输出根节点最大值
                break;
            }
        }
    }

	return 0;
}
```






