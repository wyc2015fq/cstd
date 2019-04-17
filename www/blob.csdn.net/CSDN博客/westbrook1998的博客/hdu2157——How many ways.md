# hdu2157——How many ways?? - westbrook1998的博客 - CSDN博客





2018年08月20日 12:20:26[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34标签：[矩阵快速幂](https://so.csdn.net/so/search/s.do?q=矩阵快速幂&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
春天到了, HDU校园里开满了花, 姹紫嫣红, 非常美丽. 葱头是个爱花的人, 看着校花校草竞相开放, 漫步校园, 心情也变得舒畅. 为了多看看这迷人的校园, 葱头决定, 每次上课都走不同的路线去教室, 但是由于时间问题, 每次只能经过k个地方, 比方说, 这次葱头决定经过2个地方, 那他可以先去问鼎广场看看喷泉, 再去教室, 也可以先到体育场跑几圈, 再到教室. 他非常想知道, 从A 点恰好经过k个点到达B点的方案数, 当然这个数有可能非常大, 所以你只要输出它模上1000的余数就可以了. 你能帮帮他么?? 你可决定了葱头一天能看多少校花哦 

  Input 

  输入数据有多组, 每组的第一行是2个整数 n, m(0 < n <= 20, m <= 100) 表示校园内共有n个点, 为了方便起见, 点从0到n-1编号,接着有m行, 每行有两个整数 s, t `(0<=s,t<n)` 表示从s点能到t点, 注意图是有向的.接着的一行是两个整数T,表示有T组询问(1<=T<=100),  

  接下来的T行, 每行有三个整数 A, B, k, 表示问你从A 点到 B点恰好经过k个点的方案数 (k < 20), 可以走重复边。如果不存在这样的走法, 则输出0  

  当n, m都为0的时候输入结束  

  Output 

  计算每次询问的方案数, 由于走法很多, 输出其对1000取模的结果 

  Sample Input 

  4 4 

  0 1 

  0 2 

  1 3 

  2 3 

  2 

  0 3 2 

  0 3 3 

  3 6 

  0 1 

  1 0 

  0 2 

  2 0 

  1 2 

  2 1 

  2 

  1 2 1 

  0 1 3 

  0 0 

  Sample Output 

  2 

  0 

  1 

  3
求恰好经过k条边的路径数，是矩阵快速幂的经典题目（我也是今天才直到….） 

虽然不知道为什么，但是反正把图矩阵乘k次幂，就等于i到j之间经过k条边的路径数的图矩阵

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=30;
int n,m;
int u,v;
int t;
int a,b,k;
struct Mat{
    int m[N][N];
    Mat(){
        memset(m,0,sizeof(m));
    }
};
Mat mul(Mat a,Mat b){
    Mat t;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            t.m[i][j]=0;
            for(int k=0;k<n;k++){
                t.m[i][j]+=(a.m[i][k]*b.m[k][j])%1000;
            }
        }
    }
    return t;
}
Mat quick_pow(Mat a,int k){
    Mat t;
    for(int i=0;i<n;i++){
        t.m[i][i]=1;
    }
    while(k){
        if(k&1){
            t=mul(t,a);
        }
        k/=2;
        a=mul(a,a);
    }
    return t;
}
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d%d",&n,&m) &&(n || m)){
        Mat A,B;
        while(m--){
            scanf("%d%d",&u,&v);
            A.m[u][v]=1;
            //A.m[v][u]=1;
        }
        scanf("%d",&t);
        while(t--){
            scanf("%d%d%d",&a,&b,&k);
            B=A;
            B=quick_pow(A,k);
            printf("%d\n",B.m[a][b]%1000);
        }
    }
    return 0;
}
```





