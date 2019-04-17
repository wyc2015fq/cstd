# hdu1757——A Simple Math Problem - westbrook1998的博客 - CSDN博客





2018年06月13日 22:38:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：28标签：[矩阵快速幂](https://so.csdn.net/so/search/s.do?q=矩阵快速幂&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Lele now is thinking about a simple function f(x).  

  If x < 10 f(x) = x.  

  If x >= 10 f(x) = a0 * f(x-1) + a1 * f(x-2) + a2 * f(x-3) + …… + a9 * f(x-10);  

  And ai(0<=i<=9) can only be 0 or 1 .  

  Now, I will give a0 ~ a9 and two positive integers k and m ,and could you help Lele to caculate f(k)%m.  

  Input 

  The problem contains mutiple test cases.Please process to the end of file.  

  In each case, there will be two lines.  

  In the first line , there are two positive integers k and m. ( k<2*10^9 , m < 10^5 )  

  In the second line , there are ten integers represent a0 ~ a9.  

  Output 

  For each case, output f(k) % m in one line. 

  Sample Input 

  10 9999 

  1 1 1 1 1 1 1 1 1 1 

  20 500 

  1 0 1 0 1 0 1 0 1 0 

  Sample Output 

  45 

  104
因为数据量很大 直接递推肯定tle了 用到了矩阵快速幂 学习一下 感觉很巧妙 还需要继续体会

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=10;
long long k,m;
struct Mat{
    int m[MAXN][MAXN];
};
Mat unit,mat;
void init(){
    memset(unit.m,0,sizeof(unit.m));
    memset(mat.m,0,sizeof(mat.m));
    for(int i=1;i<MAXN;i++){
        unit.m[i][i-1]=1;
    }
    for(int i=0;i<MAXN;i++){
        mat.m[i][i]=1;
    }
}
Mat mul(Mat a,Mat b){
    Mat t;
    for(int i=0;i<MAXN;i++){
        for(int j=0;j<MAXN;j++){
            t.m[i][j]=0;
            for(int k=0;k<MAXN;k++){
                t.m[i][j]=(t.m[i][j]+(a.m[i][k]*b.m[k][j])%m)%m;
            }
        }
    }
    return t;
}
Mat quick_pow(Mat a,Mat b,int n){
    while(n){
        if(n&1){
            b=mul(a,b);
        }
        n/=2;
        a=mul(a,a);
    }
    return b;
}
int main(void){
    while(~scanf("%lld%lld",&k,&m)){
        init();
        for(int i=0;i<MAXN;i++){
            scanf("%d",&unit.m[0][i]);
        }
        if(k<10){
            printf("%lld\n",k%m);
            continue;
        }
        else{
            Mat res=quick_pow(unit,mat,k-9);
            int ans=0;
            for(int i=0;i<MAXN;i++){
                ans+=(res.m[0][i]*(9-i))%m;
            }
            printf("%lld\n",ans%m);
        }
    }
    return 0;
}
```





