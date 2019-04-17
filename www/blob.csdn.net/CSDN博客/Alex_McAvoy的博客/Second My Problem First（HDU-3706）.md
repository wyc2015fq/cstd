# Second My Problem First（HDU-3706） - Alex_McAvoy的博客 - CSDN博客





2019年02月10日 19:19:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：81
个人分类：[HDU																[数据结构——单调栈与单调队列](https://blog.csdn.net/u011815404/article/category/8661014)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Give you three integers n, A and B. 

Then we define S i = A i mod B and T i = Min{ S k | i-A <= k <= i, k >= 1} 

Your task is to calculate the product of T i (1 <= i <= n) mod B.

# **Input**

Each line will contain three integers n(1 <= n <= 10^7),A and B(1 <= A, B <= 2^31-1). 

Process to end of file.

# Output

For each case, output the answer in a single line.

# Sample Input

**221 2 32 3 43 4 54 5 65 6 7**

# Sample Output

**23456**


题意：每一组数据给出三个整数 n、A、B，定义 Si=Ai % B，Ti=Min{ Sk | i-A<=k<=i,k>=1 }，求 T1*T2*...*Tn % B，1<=i<=n 

思路：

根据题意容易得出，Ti 实际上就是在大小为 k 的区间中最小的数字，因此可以维护一个递增的单调队列，每次将一个新值放进队列，将队尾的值与这个值比较，若大于等于则队尾出队，若队首的下标差大于 A 也出队，这样一来相当于每次都是将一个新的值放入队列，然后拿队首一个最小的作为当前的 Ti，一边更新一边记录答案即可

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
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 2000001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
struct Node{
    int pos;//位置
    LL num;//值
    Node(){}
    Node(int pos,LL num):pos(pos),num(num){}
};
int main(){

    LL n,a,b;
    while(scanf("%lld%lld%lld",&n,&a,&b)!=EOF&&(a+b+n)){
        deque<Node> Q;

        LL res=1,S=1;
        for(int i=1;i<=n;i++){
            S=S*a%b;

            //维护队尾
            while(!Q.empty()&&Q.back().num>=S)
                Q.pop_back();

            Q.push_back(Node(i,S));//入队

            //维护队首
            while(!Q.empty()&&Q.front().pos<i-a)
                Q.pop_front();

            res=res*Q.front().num%b;
        }

        printf("%lld\n",res);
    }
    return 0;
}
```






