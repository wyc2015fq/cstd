# Harmonic Number （LightOJ-1234） - Alex_McAvoy的博客 - CSDN博客





2019年03月20日 12:21:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：33
个人分类：[数学——微积分相关																[LightOJ](https://blog.csdn.net/u011815404/article/category/8037630)](https://blog.csdn.net/u011815404/article/category/8776162)








> 
# Problem Description

In mathematics, the nth harmonic number is the sum of the reciprocals of the first n natural numbers:

![](https://img-blog.csdnimg.cn/20190320120310300.jpg)![](https://img-blog.csdnimg.cn/20190320120325687.jpg)

In this problem, you are given n, you have to find Hn.

# Input

Input starts with an integer T (≤ 10000), denoting the number of test cases.

Each case starts with a line containing an integer n (1 ≤ n ≤ 108).

# Output

For each case, print the case number and the nth harmonic number. Errors less than 10-8 will be ignored.

# **Sample Input**

**121234567899000000099999999100000000**

# Sample Output

**Case 1: 1Case 2: 1.5Case 3: 1.8333333333Case 4: 2.0833333333Case 5: 2.2833333333Case 6: 2.450Case 7: 2.5928571429Case 8: 2.7178571429Case 9: 2.8289682540Case 10: 18.8925358988Case 11: 18.9978964039Case 12: 18.9978964139**


题意：给出 t 组数，每组输入一个数 n，求这个数的调和级数 H(n)

思路：

当 n 很小时，直接打表求即可

当 n 很大时，需要使用欧拉给出的近似公式：![f(n)=ln(n)+\frac{1}{2*n}+C](https://private.codecogs.com/gif.latex?f%28n%29%3Dln%28n%29&plus;%5Cfrac%7B1%7D%7B2*n%7D&plus;C)，其中 C 是欧拉常数，约等于 0.5772

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
const int N=10000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
double a[N];
int main(){

    a[1]=1;
    for(int i=2;i<=10000;i++)
        a[i]=a[i-1]+1.0/i;

    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);


        if(n<=10000){
            printf("Case %d: %.10lf\n",Case++,a[n]);
        }
        else{
            double res=log(n)+1.0/(2*n)+0.57721566490153286060651209;
            printf("Case %d: %.10lf\n",Case++,res);
        }
    }
    return 0;
}
```






