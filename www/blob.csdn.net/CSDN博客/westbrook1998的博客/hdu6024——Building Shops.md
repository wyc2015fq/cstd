# hdu6024——Building Shops - westbrook1998的博客 - CSDN博客





2018年09月01日 22:33:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：20








> 
HDU’s n classrooms are on a line ,which can be considered as a number line. Each classroom has a coordinate. Now Little Q wants to build several candy shops in these n classrooms.  

  The total cost consists of two parts. Building a candy shop at classroom i would have some cost ci. For every classroom P without any candy shop, then the distance between P and the rightmost classroom with a candy shop on P’s left side would be included in the cost too. Obviously, if there is a classroom without any candy shop, there must be a candy shop on its left side.  

  Now Little Q wants to know how to build the candy shops with the minimal cost. Please write a program to help him.  

  Input 

  The input contains several test cases, no more than 10 test cases.  

  In each test case, the first line contains an integer n(1≤n≤3000), denoting the number of the classrooms.  

  In the following n lines, each line contains two integers xi,ci(−109≤xi,ci≤109), denoting the coordinate of the i-th classroom and the cost of building a candy shop in it.  

  There are no two classrooms having same coordinate. 

  Output 

  For each test case, print a single line containing an integer, denoting the minimal cost. 

  Sample Input 

  3 

  1 2 

  2 3 

  3 4 

  4 

  1 7 

  3 1 

  5 10 

  6 1 

  Sample Output 

  5 

  11
女生赛的题目也做不出来…. 

这题题解都是用dp dp[i][0/1]表示第i个取或不取的最小花费

我本来用的贪心 没过 

注意代码里那个距离 并不是单纯指两个地点之间的距离 他是有一个累加的 原因就在注释里 因为当这个点i不建 而建的是前面的一个点j  那么j和i之间有可能还有房子没建（必须没建）  所以还要累加上这些点到j的距离

代码;

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
typedef long long ll;
using namespace std;
const int N=3005;
const int INF=0x3f3f3f3f;
ll dp[N][2];
ll dis[N][N];
struct node{
    int a,c;
}b[N];
bool cmp(node a,node b){
    return a.a<b.a;
}
int main(void){
    int n;
    while(~scanf("%d",&n)){
        for(int i=0;i<n;i++){
            scanf("%d%d",&b[i].a,&b[i].c);
        }
        sort(b,b+n,cmp);
        for(int i=0;i<n;i++){
            for(int j=i+1;j<n;j++){
                dis[j][i]=dis[j-1][i]+b[j].a-b[i].a;
            }
        }
        for(int i=0;i<n;i++){
            dp[i][0]=dp[i][1]=INF;
        }
        dp[0][1]=b[0].c;
        ll ans=INF;
        for(int i=1;i<n;i++){
            dp[i][1]=min(dp[i-1][0],dp[i-1][1])+b[i].c;
            for(int j=i-1;j>=0;j--){
                dp[i][0]=min(dp[j][1]+dis[i][j],dp[i][0]);
            }
            //printf("%d %d %d\n",i,dp[i][0],dp[i][1]);
        }
        ans=min(dp[n-1][0],dp[n-1][1]);
        printf("%lld\n",ans);
    }
    return 0;
}
// #include <cstdio>
// #include <algorithm>
// using namespace std;
// const int N=3005;
// int a[N];
// int c[N];
// int r[N];
// int n;
// bool cmp(int i,int j){
//     return a[i]<a[j];
// }
// int main(void){
//     while(~scanf("%d",&n)){
//         for(int i=0;i<n;i++){
//             r[i]=i;
//             scanf("%d%d",&a[i],&c[i]);
//         }
//         sort(r,r+n,cmp);
//         int p=a[0];
//         long long ans=c[0];
//         for(int i=1;i<n;i++){
//             if(a[r[i]]-p<c[r[i]]){
//                 ans+=(a[r[i]]-p);
//             }
//             else{
//                 p=a[r[i]];
//                 ans+=c[r[i]];
//             }
//         }
//         printf("%lld\n",ans);
//     }
//     return 0;
// }
```





