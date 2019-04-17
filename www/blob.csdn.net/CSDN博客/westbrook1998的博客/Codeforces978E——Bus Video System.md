# Codeforces978E——Bus Video System - westbrook1998的博客 - CSDN博客





2018年08月11日 19:37:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：44标签：[思维](https://so.csdn.net/so/search/s.do?q=思维&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
The busses in Berland are equipped with a video surveillance system. The system records information about changes in the number of passengers in a bus after stops. 

  If x is the number of passengers in a bus just before the current bus stop and y is the number of passengers in the bus just after current bus stop, the system records the number y−x. So the system records show how number of passengers changed. 

  The test run was made for single bus and n bus stops. Thus, the system recorded the sequence of integers a1,a2,…,an (exactly one number for each bus stop), where ai is the record for the bus stop i. The bus stops are numbered from 1 to n in chronological order. 

  Determine the number of possible ways how many people could be in the bus before the first bus stop, if the bus has a capacity equals to w (that is, at any time in the bus there should be from 0 to w passengers inclusive). 

  Input 

  The first line contains two integers n and w (1≤n≤1000,1≤w≤109) — the number of bus stops and the capacity of the bus. 

  The second line contains a sequence a1,a2,…,an (−106≤ai≤106), where ai equals to the number, which has been recorded by the video system after the i-th bus stop. 

  Output 

  Print the number of possible ways how many people could be in the bus before the first bus stop, if the bus has a capacity equals to w. If the situation is contradictory (i.e. for any initial number of passengers there will be a contradiction), print 0. 

  Examples 

  Input 

  3 5 

  2 1 -3 

  Output 

  3 

  Input 

  2 4 

  -1 1 

  Output 

  4 

  Input 

  4 10 

  2 4 1 2 

  Output 

  2 

  Note 

  In the first example initially in the bus could be 0, 1 or 2 passengers. 

  In the second example initially in the bus could be 1, 2, 3 or 4 passengers. 

  In the third example initially in the bus could be 0 or 1 passenger.
这题也是一言难尽，思路差不多但是就是过不了 而且题解的`h=w-h` 这个没看懂，h难道不用限制一下吗

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;
int n,m;
const int MAXN=200020;
long long presum[MAXN];
long long b;
int main(void){
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++){
        scanf("%lld",&b);
        presum[i]=presum[i-1]+b;
    }
    while(m--){
        scanf("%lld",&b);
        int idx=lower_bound(presum,presum+n,b)-presum;
        printf("%d %lld\n",idx,b-presum[idx-1]);
    }
    return 0;
}
```





