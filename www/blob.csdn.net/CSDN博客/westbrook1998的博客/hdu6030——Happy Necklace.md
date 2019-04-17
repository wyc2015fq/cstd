# hdu6030——Happy Necklace - westbrook1998的博客 - CSDN博客





2018年08月31日 23:56:13[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：26标签：[矩阵快速幂](https://so.csdn.net/so/search/s.do?q=矩阵快速幂&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Little Q wants to buy a necklace for his girlfriend. Necklaces are single strings composed of multiple red and blue beads.  

  Little Q desperately wants to impress his girlfriend, he knows that she will like the necklace only if for every prime length continuous subsequence in the necklace, the number of red beads is not less than the number of blue beads.  

  Now Little Q wants to buy a necklace with exactly n beads. He wants to know the number of different necklaces that can make his girlfriend happy. Please write a program to help Little Q. Since the answer may be very large, please print the answer modulo 109+7.  

  Note: The necklace is a single string, {not a circle}. 

  Input 

  The first line of the input contains an integer T(1≤T≤10000), denoting the number of test cases.  

  For each test case, there is a single line containing an integer n(2≤n≤1018), denoting the number of beads on the necklace. 

  Output 

  For each test case, print a single line containing a single integer, denoting the answer modulo 109+7. 

  Sample Input 

  2 

  2 

  3 

  Sample Output 

  3 

  4
先暴力打表找递推式 然后矩阵快速幂

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
const long long N=4;
const long long MOD=1e9+7;
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
    //         printf("%lld ",a.m[i][j]);
    //     }
    //     printf("\n");
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
    //         printf("%lld ",t.m[i][j]);
    //     }
    //     printf("\n");
    // }
    return t;
}
int dabiao(int n){
    int ans=0;
    for(int i=0;i<pow(2,n);i++){
        char s[10];
        int t=i;
        for(int k=n-1;k>=0;k--){
            s[k]=t%2+'0';
            t/=2;
        }
        //printf("%s\n",s);
        int num=0;
        //len=2
        int ji=0;
        for(;ji+2<=n;ji++){
            int red=0;
            int blue=0;
            for(int k=0;k<2;k++){
                if(s[ji+k]=='1'){
                    red++;
                }
                else{
                    blue++;
                }
            }
            //printf("%d %d\n",red,blue);
            if(red>=blue && red!=0){
                num++;
                //printf("%d\n",num);
            }
        }
        //len=3
        int jj=0;
        for(;jj+3<=n;jj++){
            int red=0;
            int blue=0;
            for(int k=0;k<3;k++){
                if(s[jj+k]=='1'){
                    red++;
                }
                else{
                    blue++;
                }
            }
            //printf("%d %d\n",red,blue);
            if(red>=blue  && red!=0){
                num++;
                //printf("%d\n",num);
            }
        }
        if(num==ji+jj){
            ans++;
        }
    }
    return ans;
}
int main(void){
    //打表找递推公式
    // for(int i=2;i<=10;i++){
    //     printf("%d:%d\n",i,dabiao(i));
    // }
    Mat a;
    memset(a.m,0,sizeof(a.m));
    a.m[0][0]=9;
    a.m[0][1]=6;
    a.m[0][2]=4;
    a.m[0][3]=3;
    Mat b;
    memset(b.m,0,sizeof(b.m));
    b.m[0][0]=b.m[0][1]=b.m[1][2]=b.m[2][0]=b.m[2][3]=1;
    int t;
    scanf("%d",&t);
    long long n;
    while(t--){
        scanf("%lld",&n);
        if(n<6){
            switch(n){
                case 2:{
                    printf("3\n");
                    break;
                }
                case 3:{
                    printf("4\n");
                    break;
                }
                case 4:{
                    printf("6\n");
                    break;
                }
                case 5:{
                    printf("9\n");
                    break;
                }
            }
        }
        else{
            long long ans=(a*(b^(n-5))).m[0][0];
            printf("%lld\n",ans%MOD);
        }
    }
    return 0;
}
```





