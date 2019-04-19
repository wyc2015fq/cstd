# hdu  5178   pairs（二分求解） - HJ - CSDN博客
2016年08月08日 22:47:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：557
pairs
Problem Description 
John has n points on the X axis, and their coordinates are (x[i],0),(i=0,1,2,…,n−1). He wants to know how many pairs 《a,b》 that |x[b]−x[a]|≤k.(a小于b)
Input 
The first line contains a single integer T (about 5), indicating the number of cases. 
Each test case begins with two integers n,k(1≤n≤100000,1≤k≤109). 
Next n lines contain an integer x[i](−109≤x%5bi%5d≤109), means the X coordinates.
Output 
For each case, output an integer means how many pairs《a,b》 that |x[b]−x[a]|≤k.
Sample Input 
2 
5 5 
-100 
0 
100 
101 
102 
5 300 
-100 
0 
100 
101 
102
Sample Output 
3 
10
解析：本题大意需要仔细揣摩，注意在求总数的时候需要减少循环次数，很容易超时，用二分解输入输出需要用scanf，用cin同样会超时。
思路：二分求解，取得下标值跟第一层循环下标作差加入计数变量。
代码如下：
```cpp
#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdio>
using namespace std;
long long x[100010];
int main(){
    int T; scanf("%d",&T);
    while(T--){
        int n,k;
        scanf("%d%d",&n,&k);
        for(int i=0;i<n;i++){
            scanf("%I64d",&x[i]);
        }
        sort(x,x+n);
        long long sum=0;
        for(int i=0;i<n-1;i++)
        {
            int l=i+1;
            int r=n-1;
            int mid=(l+r)>>1;
            while(1)
            {
                int t_mid=mid;
                if(x[mid]-x[i]>k)
                    r=mid-1;           //给右边下标赋值的时候中位需要先往前挪一位再赋值给右边的下标
                else if(x[mid]-x[i]<k)
                    l=mid+1;           //给左边下标同样赋值的时候中位需要先往后挪一位赋值再给左边的下标
                mid=(l+r)>>1;
                if(t_mid==mid)        //循环跳出条件
                    break;
            }
            if(x[mid]-x[i]<=k){
                sum+=(long long)mid-i;
            }
        }
        printf("%I64d\n",sum);
    }
    return 0;
}
```
