# Codeforces1006B——Polycarp's Practice - westbrook1998的博客 - CSDN博客





2018年07月17日 23:47:55[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：115








> 
Polycarp is practicing his problem solving skill. He has a list of n problems with difficulties a1,a2,…,an, respectively. His plan is to practice for exactly k days. Each day he has to solve at least one problem from his list. Polycarp solves the problems in the order they are given in his list, he cannot skip any problem from his list. He has to solve all n problems in exactly k days. 

  Thus, each day Polycarp solves a contiguous sequence of (consecutive) problems from the start of the list. He can’t skip problems or solve them multiple times. As a result, in k days he will solve all the n problems. 

  The profit of the j-th day of Polycarp’s practice is the maximum among all the difficulties of problems Polycarp solves during the j-th day (i.e. if he solves problems with indices from l to r during a day, then the profit of the day is maxl≤i≤rai). The total profit of his practice is the sum of the profits over all k days of his practice. 

  You want to help Polycarp to get the maximum possible total profit over all valid ways to solve problems. Your task is to distribute all n problems between k days satisfying the conditions above in such a way, that the total profit is maximum. 

  For example, if n=8,k=3 and a=[5,4,2,6,5,1,9,2], one of the possible distributions with maximum total profit is: [5,4,2],[6,5],[1,9,2]. Here the total profit equals 5+6+9=20. 

  Input 

  The first line of the input contains two integers n and k (1≤k≤n≤2000) — the number of problems and the number of days, respectively. 

  The second line of the input contains n integers a1,a2,…,an (1≤ai≤2000) — difficulties of problems in Polycarp’s list, in the order they are placed in the list (i.e. in the order Polycarp will solve them). 

  Output 

  In the first line of the output print the maximum possible total profit. 

  In the second line print exactly k positive integers t1,t2,…,tk (t1+t2+⋯+tk must equal n), where tj means the number of problems Polycarp will solve during the j-th day in order to achieve the maximum possible total profit of his practice. 

  If there are many possible answers, you may print any of them. 

  Examples 

  Input 

  8 3 

  5 4 2 6 5 1 9 2 

  Output 

  20 

  3 2 3 

  Input 

  5 1 

  1 1 1 1 1 

  Output 

  1 

  5 

  Input 

  4 2 

  1 2000 2000 2 

  Output 

  4000 

  2 2 

  Note 

  The first example is described in the problem statement. 

  In the second example there is only one possible distribution. 

  In the third example the best answer is to distribute problems in the following way: [1,2000],[2000,2]. The total profit of this distribution is 2000+2000=4000.
找出n长度的数组中前k大的数然后划分成k个块（多种可能） 

用一个结构体存数字和索引 然后排序后记录前k大的数的索引位置 然后按这些位置贪心划分 

WA了两次 一次是最后贪心刚开始只考虑到第一个元素为划分点 其实并不一定 所以第一个划分点无需考虑 只要让第二个划分点减去0就是第一个划分长度 

第二次是因为前面不考虑第一个划分点 所以特判了一下 导致k=1时输出了两个划分 
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
const int MAXN=2050;
struct node{
    int v;
    int i;
}a[MAXN];
int id[MAXN];
bool cmp(node a,node b){
    return a.v>b.v;
}
int main(void){
    int n,k;
    scanf("%d%d",&n,&k);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i].v);
        a[i].i=i;
    }
    sort(a,a+n,cmp);
    int ans=0;
    for(int i=0;i<k;i++){
        id[i]=a[i].i;
        ans+=a[i].v;
    }
    id[k]=n;
    sort(id,id+k+1);
    printf("%d\n",ans);
    printf("%d ",id[1]);
    for(int i=1;i<k-1;i++){
        printf("%d ",id[i+1]-id[i]);
    }
    if(k!=1){
        printf("%d\n",id[k]-id[k-1]);
    }
}
```






