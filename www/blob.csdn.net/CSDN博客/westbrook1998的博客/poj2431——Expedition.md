# poj2431——Expedition - westbrook1998的博客 - CSDN博客





2018年09月06日 21:43:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52








> 
A group of cows grabbed a truck and ventured on an expedition deep into the jungle. Being rather poor drivers, the cows unfortunately managed to run over a rock and puncture the truck’s fuel tank. The truck now leaks one unit of fuel every unit of distance it travels.  

  To repair the truck, the cows need to drive to the nearest town (no more than 1,000,000 units distant) down a long, winding road. On this road, between the town and the current location of the truck, there are N (1 <= N <= 10,000) fuel stops where the cows can stop to acquire additional fuel (1..100 units at each stop).  

  The jungle is a dangerous place for humans and is especially dangerous for cows. Therefore, the cows want to make the minimum possible number of stops for fuel on the way to the town. Fortunately, the capacity of the fuel tank on their truck is so large that there is effectively no limit to the amount of fuel it can hold. The truck is currently L units away from the town and has P units of fuel (1 <= P <= 1,000,000).  

  Determine the minimum number of stops needed to reach the town, or if the cows cannot reach the town at all.  

  Input 

  * Line 1: A single integer, N  

  * Lines 2..N+1: Each line contains two space-separated integers describing a fuel stop: The first integer is the distance from the town to the stop; the second is the amount of fuel available at that stop.  

  * Line N+2: Two space-separated integers, L and P 

  Output 

  * Line 1: A single integer giving the minimum number of fuel stops necessary to reach the town. If it is not possible to reach the town, output -1. 

  Sample Input 

  4 

  4 4 

  5 2 

  11 5 

  15 10 

  25 10 

  Sample Output 

  2 

  Hint 

  INPUT DETAILS:  

  The truck is 25 units away from the town; the truck has 10 units of fuel. Along the road, there are 4 fuel stops at distances 4, 5, 11, and 15 from the town (so these are initially at distances 21, 20, 14, and 10 from the truck). These fuel stops can supply up to 4, 2, 5, and 10 units of fuel, respectively.  

  OUTPUT DETAILS:  

  Drive 10 units, stop to acquire 10 more units of fuel, drive 4 more units, stop to acquire 5 more units of fuel, then drive to the town.
挑战书里很经典的一道题 用优先队列 

最终要的思想就是把每次路过一个加油站加或不加的决策转化为每次经过加油站就获得一次加油的机会，这样用一个优先队列来保存这些加油机会，然后每次需要加油再取出最大的一个 稳了

代码：

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;
const int N=10000005;
int l,p,n;
int a[N];
int b[N];
int main(void){
    scanf("%d",&n);
    for(int i=n-1;i>=0;i--){
        scanf("%d%d",&a[i],&b[i]);
    }
    scanf("%d%d",&l,&p);
    for(int i=0;i<n;i++){
        a[i]=l-a[i];
    }
    //把终点也看成加油站
    a[n]=l;
    b[n]=0;
    n++;
    priority_queue<int> q;
    //加油次数 当前位置 油箱油量
    int ans=0,pos=0,tank=p;
    for(int i=0;i<n;i++){
        //要走的距离
        int d=a[i]-pos;
        //需要加油
        while(tank-d<0){
            if(q.empty()){
                puts("-1");
                return 0;
            }
            tank+=q.top();
            q.pop();
            ans++;
        }
        tank-=d;
        //到下一个地点
        pos=a[i];
        //加入沿途的加油站
        q.push(b[i]);
    }
    printf("%d\n",ans);
    return 0;
}
```





