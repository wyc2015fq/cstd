# hdu6434——Problem I. Count - westbrook1998的博客 - CSDN博客





2018年09月22日 23:32:57[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31标签：[欧拉函数																[找规律](https://so.csdn.net/so/search/s.do?q=找规律&t=blog)](https://so.csdn.net/so/search/s.do?q=欧拉函数&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Multiple query, for each n, you need to get

n i-1

∑ ∑ [gcd(i + j, i - j) = 1]

i=1 j=1

Input

On the first line, there is a positive integer T, which describe the number of queries. Next there are T lines, each line give a positive integer n, as mentioned above.

T<=1e5, n<=2e7

Output

Your output should include T lines, for each line, output the answer for the corre- sponding n.

Sample Input

4

978

438

233

666

Sample Output

194041

38951

11065

89963
鬼知道这tm怎么能看出规律的

打表然后找出规律是1-n的偶数的欧拉函数和奇数的欧拉函数一半的和

然后还要用前缀和预处理一下再查询
代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
typedef long long ll;
const int N=2e7+5;
int dabiao[105];
int euler[N];
ll sum[N];
int main(void){
    for(int i=1;i<N;i++){
        euler[i]=i;
    }
    for(int i=2;i<N;i++){
        if(euler[i]==i){
            for(int j=i;j<N;j+=i){
                euler[j]=euler[j]/i*(i-1);
            }
        }
    }
    sum[1]=0;
    for(int i=2;i<N;i++){
        if(i%2){
            sum[i]=sum[i-1]+euler[i]/2;
        }
        else{
            sum[i]=sum[i-1]+euler[i];
        }
    }
    int t,n;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        printf("%lld\n",sum[n]);
    }
    // for(int n=1;n<100;n++){
    //     for(int i=1;i<=n;i++){
    //         for(int j=1;j<=i-1;j++){
    //             if(__gcd(i+j,i-j)==1){
    //                 dabiao[n]++;
    //             }
    //         }
    //     }
    // }
    // for(int i=1;i<20;i++){
    //     printf("%2d,",dabiao[i]);
    // }
    // printf("\n");
    // for(int i=1;i<20;i++){
    //     printf("%2d,",euler[i]);
    // }
    // printf("\n");
    return 0;
}
```







