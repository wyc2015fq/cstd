# The Average（POJ-2833） - Alex_McAvoy的博客 - CSDN博客





2019年02月10日 19:18:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：77








> 
# Problem Description

In a speech contest, when a contestant finishes his speech, the judges will then grade his performance. The staff remove the highest grade and the lowest grade and compute the average of the rest as the contestant’s final grade. This is an easy problem because usually there are only several judges.

Let’s consider a generalized form of the problem above. Given n positive integers, remove the greatest n1 ones and the least n2 ones, and compute the average of the rest.

# Input

The input consists of several test cases. Each test case consists two lines. The first line contains three integers n1, n2 and n (1 ≤ n1, n2 ≤ 10, n1 + n2 < n ≤ 5,000,000) separate by a single space. The second line contains n positive integers ai (1 ≤ ai ≤ 108 for all i s.t. 1 ≤ i ≤ n) separated by a single space. The last test case is followed by three zeroes.

# Output

For each test case, output the average rounded to six digits after decimal point in a separate line.

# **Sample Input**

**1 2 51 2 3 4 54 2 102121187 902 485 531 843 582 652 926 220 1550 0 0**

# Sample Output

**3.500000562.500000**


题意：多组数据，每组首先给出 n1，n2，n 三个数，然后再给出 n 个数，要求在给出的 n 个数中，删除 n1 个最大的数，再删除 n2 个最小的数，求剩下的数的平均值

思路：构建两个优先队列，一个存放 n1 个大值，一个存放 n2 个小值，最后用总体的数之和减去两个优先队列中的值，求剩下的数平均即可

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
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int main(){
    int n1,n2,n;
    while(scanf("%d%d%d",&n1,&n2,&n)!=EOF&&(n1+n2+n)){
        double sum=0;
        priority_queue<double> small;//大者先出队
        priority_queue< double,vector<int>,greater<int> > big;//小者先出队

        for(int i=0;i<n;i++){
            int a;
            scanf("%d",&a);
            sum+=a;

            small.push(a);
            big.push(a);

            if(big.size()>n1)
                big.pop();
            if(small.size()>n2)
                small.pop();
        }

        for(int i=0;i<n1;i++){
            sum-=big.top();
            big.pop();
        }
        for(int i=0;i<n2;i++){
            sum-=small.top();
            small.pop();
        }

        printf("%.6f\n",sum/(n-n1-n2));

    }
    return 0;
}
```





