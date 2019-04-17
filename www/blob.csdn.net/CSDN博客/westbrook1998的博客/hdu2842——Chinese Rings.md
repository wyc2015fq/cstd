# hdu2842——Chinese Rings - westbrook1998的博客 - CSDN博客





2018年08月31日 22:36:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：66标签：[矩阵快速幂](https://so.csdn.net/so/search/s.do?q=矩阵快速幂&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Dumbear likes to play the Chinese Rings (Baguenaudier). It’s a game played with nine rings on a bar. The rules of this game are very simple: At first, the nine rings are all on the bar.  

  The first ring can be taken off or taken on with one step.  

  If the first k rings are all off and the (k + 1)th ring is on, then the (k + 2)th ring can be taken off or taken on with one step. (0 ≤ k ≤ 7)  

  Now consider a game with N (N ≤ 1,000,000,000) rings on a bar, Dumbear wants to make all the rings off the bar with least steps. But Dumbear is very dumb, so he wants you to help him. 

  Input 

  Each line of the input file contains a number N indicates the number of the rings on the bar. The last line of the input file contains a number “0”. 

  Output 

  For each line, output an integer S indicates the least steps. For the integers may be very large, output S mod 200907. 

  Sample Input 

  1 

  4 

  0 

  Sample Output 

  1 

  10
今天出去玩了没做题  

做一下一道矩阵快速幂加速递推的题目 

第一次推这个矩阵 搞了很久  其实很简单嘛 

然后要开long long
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const long long N=3;
const long long MOD=200907;
struct Mat{
    long long m[N][N];
};
Mat operator * (Mat a,Mat b){
    Mat t;
    memset(t.m,0,sizeof(t.m));
    for(long long k=0;k<N;k++){
        for(long long i=0;i<N;i++){
            if(a.m[i][k]==0){
                continue;
            }
            for(long long j=0;j<N;j++){
                if(b.m[k][j]==0){
                    continue;
                }
                t.m[i][j]=(t.m[i][j]+(a.m[i][k]*b.m[k][j]))%MOD;
            }
        }
    }
    return t;
}
Mat operator ^ (Mat a,long long n){
    // for(long long i=0;i<N;i++){
    //     for(long long j=0;j<N;j++){
    //         prlong longf("%d ",a.m[i][j]);
    //     }
    //     prlong longf("\n");
    // }
    Mat t;
    for(long long i=0;i<N;i++){
        for(long long j=0;j<N;j++){
            t.m[i][j]=i==j?1:0;
        }
    }
    while(n){
        if(n%2){
            t=t*a;
        }
        n/=2;
        a=a*a;
    }
    // for(long long i=0;i<N;i++){
    //     for(long long j=0;j<N;j++){
    //         prlong longf("%d ",t.m[i][j]);
    //     }
    //     prlong longf("\n");
    // }
    return t;
}
int main(void){
    long long n;
    Mat a;
    a.m[0][0]=a.m[0][1]=a.m[2][0]=a.m[2][2]=1;
    a.m[0][2]=a.m[1][1]=a.m[1][2]=a.m[2][1]=0;
    a.m[1][0]=2;
    Mat b;
    memset(b.m,0,sizeof(b.m));
    b.m[0][0]=2;
    b.m[0][1]=1;
    b.m[0][2]=1;
    while(~scanf("%d",&n) && n){
        if(n==1){
            printf("1\n");
        }
        else if(n==2){
            printf("2\n");
        }
        else{
            long long ans=(b*(a^(n-2))).m[0][0]%MOD;
            printf("%d\n",ans);
        }
    }
    return 0;
}
```






