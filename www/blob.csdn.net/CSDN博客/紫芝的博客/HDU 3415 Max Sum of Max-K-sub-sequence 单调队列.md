# HDU 3415 Max Sum of Max-K-sub-sequence  单调队列 - 紫芝的博客 - CSDN博客





2018年11月12日 15:09:49[紫芝](https://me.csdn.net/qq_40507857)阅读数：285
个人分类：[数据结构](https://blog.csdn.net/qq_40507857/article/category/7837948)









Given a circle sequence A[1],A[2],A[3]......A[n]. Circle sequence means the left neighbour of A[1] is A[n] , and the right neighbour of A[n] is A[1]. 

Now your job is to calculate the max sum of a Max-K-sub-sequence. Max-K-sub-sequence means a continuous non-empty sub-sequence which length not exceed K.

Input

The first line of the input contains an integer T(1<=T<=100) which means the number of test cases. 

Then T lines follow, each line starts with two integers N , K(1<=N<=100000 , 1<=K<=N), then N integers followed(all the integers are between -1000 and 1000).

Output

For each test case, you should output a line contains three integers, the Max Sum in the sequence, the start position of the sub-sequence, the end position of the sub-sequence. If there are more than one result, output the minimum start position, if still more than one , output the minimum length of them.

Sample Input

```
4
6 3
6 -1 2 -6 5 -5
6 4
6 -1 2 -6 5 -5
6 3
-1 2 -6 5 -5 6
6 6
-1 -1 -1 -1 -1 -1
```

Sample Output

```
7 1 3
7 1 3
7 6 2
-1 1 1
```

### 题意：

**给N个数，首尾相连形成一个环，求一个长度不超过K的连续子序列的最大和，并求出起始和终止位置**

### **分析：**

**求某段连续区间和的最大值，用单调队列来解决；**

**具体做法是：环的问题一般【1-N】，【N+1-2*N】二倍区间即可解决；先求出 SUM[1]~SUM[2*n] 的前缀和，考虑第i个数，用单调队列维护sum[i-k]~sum[i-1]的最小值，设最小值为sum[j]，那么sum[i]-sum[j]就是这个区间的最大连续和**

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define INF 0x3f3f3f3f
const int maxn=1e6+10;
int n,k;
int lcm[maxn<<1];
int que[maxn<<1],head,tail;
void init(){
    tail=head=0;
}
void push(int id){
    while(head<tail&&lcm[id]<lcm[que[tail-1]])
        tail--;
    que[tail++]=id;
}
void pop(int id){
    while(head<tail&&id-que[head]>=k)
        head++;
}
int main(){
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d%d",&n,&k);

        lcm[0]=0;
        for(int i=1;i<=n;i++){
            scanf("%d",&lcm[i]);
            lcm[i+n]=lcm[i];//解决循环序列问题
            lcm[i]+=lcm[i-1];//求前缀和
        }
        for(int i=n+1;i<=n*2;i++){
            lcm[i]+=lcm[i-1];
        }
        init();
        push(0);
        int ans=-INF;
        int s,e;
        for(int i=1;i<=n*2;i++){
            if(lcm[i]-lcm[que[head]]>ans){
                ans=lcm[i]-lcm[que[head]];
                s=que[head]+1;
                e=i;
            }
            push(i);
            pop(i);
        }
        if(e>n) e-=n;
        printf("%d %d %d\n",ans,s,e);
    }
}
```





