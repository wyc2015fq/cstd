# poj1789——Truck History - westbrook1998的博客 - CSDN博客





2018年08月14日 13:30:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31标签：[最小生成树](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Advanced Cargo Movement, Ltd. uses trucks of different types. Some trucks are used for vegetable delivery, other for furniture, or for bricks. The company has its own code describing each type of a truck. The code is simply a string of exactly seven lowercase letters (each letter on each position has a very special meaning but that is unimportant for this task). At the beginning of company’s history, just a single truck type was used but later other types were derived from it, then from the new types another types were derived, and so on.  

  Today, ACM is rich enough to pay historians to study its history. One thing historians tried to find out is so called derivation plan – i.e. how the truck types were derived. They defined the distance of truck types as the number of positions with different letters in truck type codes. They also assumed that each truck type was derived from exactly one other truck type (except for the first truck type which was not derived from any other type). The quality of a derivation plan was then defined as  

  1/Σ(to,td)d(to,td) 

  where the sum goes over all pairs of types in the derivation plan such that t o is the original type and t d the type derived from it and d(t o,t d) is the distance of the types.  

  Since historians failed, you are to write a program to help them. Given the codes of truck types, your program should find the highest possible quality of a derivation plan.  

  Input 

  The input consists of several test cases. Each test case begins with a line containing the number of truck types, N, 2 <= N <= 2 000. Each of the following N lines of input contains one truck type code (a string of seven lowercase letters). You may assume that the codes uniquely describe the trucks, i.e., no two of these N lines are the same. The input is terminated with zero at the place of number of truck types. 

  Output 

  For each test case, your program should output the text “The highest possible quality is 1/Q.”, where 1/Q is the quality of the best derivation plan. 

  Sample Input 

  4 

  aaaaaaa 

  baaaaaa 

  abaaaaa 

  aabaaaa 

  0 

  Sample Output 

  The highest possible quality is 1/3.
这道主要就是读题了 读懂然后就是最小生成树搞一下

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=2020;
const int INF=0x3f3f3f3f;
int n;
char s[N][8];
int g[N][N];
bool vis[N];
int low[N];
int dis(int i,int j){
    int ans=0;
    for(int k=0;k<7;k++){
        if(s[i][k]!=s[j][k]){
            ans++;
        }
    }
    return ans;
}
int prim(){
    _clr(vis,false);
    int ans=0;
    int Min=INF;
    int pos=1;
    vis[pos]=true;
    for(int i=1;i<=n;i++){
        if(i!=pos){
            low[i]=g[pos][i];
        }
    }
    for(int i=1;i<n;i++){
        Min=INF;
        for(int j=1;j<=n;j++){
            if(!vis[j] && low[j]<Min){
                Min=low[j];
                pos=j;
            }
        }
        ans+=Min;
        vis[pos]=true;
        for(int j=1;j<=n;j++){
            if(!vis[j] && low[j]>g[pos][j]){
                low[j]=g[pos][j];
            }
        }
    }
    return ans;
}
int main(void){
    while(~scanf("%d",&n) && n){
        for(int i=1;i<=n;i++){
            scanf("%s",s[i]);
        }
        _clr(g,INF);
        for(int i=1;i<n;i++){
            for(int j=i+1;j<=n;j++){
                g[i][j]=g[j][i]=dis(i,j);
            }
        }
        int ans=prim();
        printf("The highest possible quality is 1/%d.\n",ans);
    }
    return 0;
}
```





