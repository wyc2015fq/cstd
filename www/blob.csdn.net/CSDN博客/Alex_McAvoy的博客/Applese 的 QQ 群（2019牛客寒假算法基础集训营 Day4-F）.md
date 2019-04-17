# Applese 的 QQ 群（2019牛客寒假算法基础集训营 Day4-F） - Alex_McAvoy的博客 - CSDN博客





2019年02月03日 15:31:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：137
个人分类：[图论——AOV网与拓扑排序																[牛客](https://blog.csdn.net/u011815404/article/category/8639591)](https://blog.csdn.net/u011815404/article/category/7813285)








> 
# 【题目描述】

Applese 有一个QQ群。在这个群中，大家互相请教问题。如 b 向 a 请教过问题，就把 a 叫做是 b 的"老板"。这样一个群中就会有很多老板。

同时规定：如果 a 是 b 的老板，b 是 c 的老板，那么 a 也是 c 的老板。

为了不破坏群里面和谐交流的氛围，Applese 定了一个群规：**不允许**出现 a 既是 b 的老板， b 又是 a 的老板。

你需要帮助 Applese 判断大家是否遵守了群规。

# 【输入描述】

第一行两个整数 n, m，表示群里的人数以及请教问题的数量。

接下来 m 行，每行两个整数 a, b，表示 a 是 b 的"老板"，即 b 向 a 请教了一个问题。

注：无论是否违反了群规，a 都会成为 b 的老板。

1≤n≤105

1≤m≤2⋅105

1≤a,b≤n

# 【输出描述】

对于每次提问，输出一行"Yes"表示大家都遵守了群规，反之输出"No"。

# 【样例】

示例1

输入

4 4

1 2

2 3

3 1

1 4

输出

Yes

Yes

No

No


思路：

根据题意可以知道，输出一定是若干个 Yes 后跟着若干个 No，而题目实质是要判断有向图是否存在环，由于题目要求在第一个出现环的位置开始输出 No，由于每次都要判环一定会 TLE，无法进行在线操作，因此可以先将所有的数据读入，通过二分来判断最后一个 Yes 的位置来确定何时开始输出 No

# 【源代码】

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=1e9+7;
using namespace std;
struct Node{
    int x;
    int num;
    Node(){}
    Node(int x,int num):x(x),num(num){}
};
vector<Node> edge[N];
int vis[N];
int n,m;
bool dfs(int x,int num){
    if(vis[x]==1)//出环
        return true;
    if(vis[x]==-1)//已访问
        return false;

    vis[x]=1;//正在被占用

    for(int i=0;i<edge[x].size();i++)
        if(edge[x][i].num<=num&&dfs(edge[x][i].x,num))
            return true;
    vis[x]=-1;//解除占用并标记访问
    return false;

}

bool judge(int num){
    memset(vis,0,sizeof(vis));
    for(int i=1;i<=n;i++)
        if(!vis[i]&&dfs(i,num))
            return false;
    return true;
}
int main(){
    cin>>n>>m;
    for(int i=1;i<=m;i++){
        int x,y;
        cin>>x>>y;
        edge[x].push_back(Node(y,i));
    }

    int pos=1;
    int left=1,right=m;
    while(left<=right){
        int mid=left+right>>1;
        if(judge(mid)){
            left=mid+1;
            pos=mid;
        }
        else
            right=mid-1;
    }

    for(int i=1;i<=pos;i++)
        cout<<"Yes"<<endl;
    for(int i=pos+1;i<=m;i++)
        cout<<"No"<<endl;
    return 0;
}
```






