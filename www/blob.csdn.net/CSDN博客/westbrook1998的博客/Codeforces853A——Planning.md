# Codeforces853A——Planning - westbrook1998的博客 - CSDN博客





2018年09月03日 22:11:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：43标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Helen works in Metropolis airport. She is responsible for creating a departure schedule. There are n flights that must depart today, the i-th of them is planned to depart at the i-th minute of the day. 

  Metropolis airport is the main transport hub of Metropolia, so it is difficult to keep the schedule intact. This is exactly the case today: because of technical issues, no flights were able to depart during the first k minutes of the day, so now the new departure schedule must be created. 

  All n scheduled flights must now depart at different minutes between (k + 1)-th and (k + n)-th, inclusive. However, it’s not mandatory for the flights to depart in the same order they were initially scheduled to do so — their order in the new schedule can be different. There is only one restriction: no flight is allowed to depart earlier than it was supposed to depart in the initial schedule. 

  Helen knows that each minute of delay of the i-th flight costs airport ci burles. Help her find the order for flights to depart in the new schedule that minimizes the total cost for the airport. 

  Input 

  The first line contains two integers n and k (1 ≤ k ≤ n ≤ 300 000), here n is the number of flights, and k is the number of minutes in the beginning of the day that the flights did not depart. 

  The second line contains n integers c1, c2, …, cn (1 ≤ ci ≤ 107), here ci is the cost of delaying the i-th flight for one minute. 

  Output 

  The first line must contain the minimum possible total cost of delaying the flights. 

  The second line must contain n different integers t1, t2, …, tn (k + 1 ≤ ti ≤ k + n), here ti is the minute when the i-th flight must depart. If there are several optimal schedules, print any of them. 

  Example 

  Input 

  5 2 

  4 2 1 10 2 

  Output 

  20 

  3 6 7 4 5  

  Note 

  Let us consider sample test. If Helen just moves all flights 2 minutes later preserving the order, the total cost of delaying the flights would be (3 - 1)·4 + (4 - 2)·2 + (5 - 3)·1 + (6 - 4)·10 + (7 - 5)·2 = 38 burles. 

  However, the better schedule is shown in the sample answer, its cost is (3 - 1)·4 + (6 - 2)·2 + (7 - 3)·1 + (4 - 4)·10 + (5 - 5)·2 = 20 burles.
Div1的A题…果然不会做 

贪心

题意给出一个数列，每个数对应每架飞机延迟一分钟的花费，然后前k分钟无法起飞，求如何重新安排飞机的起飞顺序使得所有飞机花费最小

比如原本 4 2 1 10 2，当前2分钟无法起飞，因此前面的4和2就要移到后面来，那是要放在最后还是把后三个挤到后面，这就是一个选择的策略了，这里用到贪心的思想，从k+1分钟开始，将该分钟延误的飞机加入优先队列，然后再从队列中取出花费最大的优先起飞，这样延误的时间就尽可能小

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
using namespace std;
const int N=300050;
struct node{
    long long i,c;
    bool operator <(const node&a)const{
        return c<a.c;
    }
}p[N];
int main(void){
    int n,k;
    scanf("%d%d",&n,&k);
    for(int i=1;i<=n;i++){
        scanf("%lld",&p[i].c);
        p[i].i=i;
    }
    long long ans=0;
    priority_queue<node> q;
    for(int i=1;i<=k;i++){
        q.push(p[i]);
    }
    for(int i=k+1;i<=k+n;i++){
        if(i<=n){
            q.push(p[i]);
        }
        node t=q.top();
        q.pop();
        ans+=(i-t.i)*t.c;
        p[t.i].i=i;
    }
    printf("%lld\n",ans);
    for(int i=1;i<n;i++){
        printf("%d ",p[i].i);
    }
    printf("%d\n",p[n].i);
    return 0;
}
```





