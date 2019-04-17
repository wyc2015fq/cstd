# Answering Queries（LightOJ-1369） - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 19:32:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：80
个人分类：[LightOJ																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

The problem you need to solve here is pretty simple. You are give a function f(A, n), where A is an array of integers and n is the number of elements in the array. f(A, n) is defined as follows:

```cpp
long long f( int A[], int n ) { // n = size of A
    long long sum = 0;
    for( int i = 0; i < n; i++ )
        for( int j = i + 1; j < n; j++ )
            sum += A[i] - A[j];
    return sum;
}
```

Given the array A and an integer n, and some queries of the form:

1)      0 x v (0 ≤ x < n, 0 ≤ v ≤ 106), meaning that you have to change the value of A[x] to v.

2)      1, meaning that you have to find f as described above.

# Input

Input starts with an integer T (≤ 5), denoting the number of test cases.

Each case starts with a line containing two integers: n and q (1 ≤ n, q ≤ 105). The next line contains n space separated integers between 0 and 106 denoting the array A as described above.

Each of the next q lines contains one query as described above.

# Output

For each case, print the case number in a single line first. Then for each query-type "1" print one single line containing the value of f(A, n).

# **Sample Input**

**13 51 2 310 0 310 2 11**

# Sample Output

**Case 1:-404**


题意：对于如上的函数，有 t 组数据，每组给出 n 个数和 q 组操作，其中，0 x y 代表将第 x 个数转换为 y，1 代表计算函数并输出结果

思路：根据函数，对这 n 个数的 a[i] 进行预处理，考虑每个 a[i] 作出的贡献

假设有 n 个数，根据的代码求 sum 方法，可推出一个求和规律，由于下标是从 0 开始，所以先将 n 减一，然后可得求和规律：sum =n*a[0]+(n-2)*a[1]+(n-4)*a[2]+(n-6)*a[3]+(n-8)*a[4]+……+(n-2*n)*a[n-1]，即 sum 等于系数从 n-1 开始每次减去 2 再乘 a[i]，因此在进行 q 组操作的 0 x y 更新操作时，只需要将要更新的一项去掉，然后加上该项更新后的值，如果进行 1 询问操作，自己输出即可

要注意的是，每次改变数组元素时，要记得更改 sum 的值，此外，由于数据范围的问题，要注意使用 long long

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
#define E 1e-6
#define MOD 1000003
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
using namespace std;
LL a[N];
LL b[N];//系数
LL n,q;
LL sum;
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        scanf("%lld%lld",&n,&q);
        for(int i=0;i<n;i++)
            scanf("%lld",&a[i]);

        sum=0;
        int cnt=n-1;
        for(int i=0;i<n;i++){
            sum+=cnt*a[i];//求和
            b[i]=cnt;//存储每一项对应的系数
            cnt=cnt-2;
        }

        printf("Case %d:\n",Case++);
        while(q--){
            int num;
            scanf("%d",&num);
            if(num==1)
                printf("%lld\n",sum);
            if(num==0){
                int x,y;
                scanf("%d%d",&x,&y);
                sum-=b[x]*a[x];//减去x项与对应系数的乘积去
                sum+=b[x]*y;//加上更新后的值与对应系数的乘积
                a[x]=y;//更新数组a[x]
            }
        }
    }
    return 0;
}
```






