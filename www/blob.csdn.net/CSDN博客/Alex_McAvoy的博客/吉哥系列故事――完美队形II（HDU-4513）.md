# 吉哥系列故事――完美队形II（HDU-4513） - Alex_McAvoy的博客 - CSDN博客





2019年03月02日 13:27:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48








> 
# Problem Description

吉哥又想出了一个新的完美队形游戏！ 

　　假设有n个人按顺序站在他的面前，他们的身高分别是h[1], h[2] ... h[n]，吉哥希望从中挑出一些人，让这些人形成一个新的队形，新的队形若满足以下三点要求，则就是新的完美队形： 

　　1、挑出的人保持原队形的相对顺序不变，且必须都是在原队形中连续的； 

　　2、左右对称，假设有m个人形成新的队形，则第1个人和第m个人身高相同，第2个人和第m-1个人身高相同，依此类推，当然如果m是奇数，中间那个人可以任意； 

　　3、从左到中间那个人，身高需保证不下降，如果用H表示新队形的高度，则H[1] <= H[2] <= H[3] .... <= H[mid]。 

　　现在吉哥想知道：最多能选出多少人组成新的完美队形呢？

# **Input**

输入数据第一行包含一个整数T，表示总共有T组测试数据(T <= 20)； 

　　每组数据首先是一个整数n(1 <= n <= 100000)，表示原先队形的人数，接下来一行输入n个整数，表示原队形从左到右站的人的身高（50 <= h <= 250，不排除特别矮小和高大的）。

# Output

请输出能组成完美队形的最多人数，每组输出占一行。

# Sample Input

**2351 52 51451 52 52 51**

# Sample Output

**34**


思路：本质上是求一个最长回文子串，使用马拉车算法，将 char 数组改为 int 数组，然后将特殊字符 # 改为 0 即可，此外，还要满足从队首到最高点满足最长不下降子串，因此需要特判 newStr[i-p[i]]<=newStr[i-p[i]+2]

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=200000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int str[N];
int newStr[N*2];
int p[N*2];
int n;
int init(){
    newStr[0]=-1;
    newStr[1]=0;

    int j=2;
    int len=n;
    for (int i=0;i<len;i++){
        newStr[j++]=str[i];
        newStr[j++]=0;
    }

    return j;
}

int manacher(){
    int len=init();
    int res=-1;

    int id;
    int mx=0;
    for(int i=1;i<len;i++){
        int j=2*id-i;
        if(i<mx)
            p[i]=min(p[j], mx-i);
        else
            p[i]=1;

        while(newStr[i-p[i]]==newStr[i+p[i]] &&  newStr[i-p[i]]<=newStr[i-p[i]+2] )
            p[i]++;

        if(mx<i+p[i]){
            id=i;
            mx=i+p[i];
        }
        res=max(res,p[i]-1);
    }
    return res;
}

int main(){
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        for(int i=0;i<n;++i)
            scanf("%d",&str[i]);
        printf("%d\n",manacher());
    }
    return 0;
}
```





