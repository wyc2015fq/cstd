# codeforces158B——Taxi - westbrook1998的博客 - CSDN博客





2018年05月11日 23:09:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：73








> 
After the lessons n groups of schoolchildren went outside and decided to visit Polycarpus to celebrate his birthday. We know that the i-th group consists of si friends (1 ≤ si ≤ 4), and they want to go to Polycarpus together. They decided to get there by taxi. Each car can carry at most four passengers. What minimum number of cars will the children need if all members of each group should ride in the same taxi (but one taxi can take more than one group)? 

  Input 

  The first line contains integer n (1 ≤ n ≤ 105) — the number of groups of schoolchildren. The second line contains a sequence of integers s1, s2, …, sn (1 ≤ si ≤ 4). The integers are separated by a space, si is the number of children in the i-th group. 

  Output 

  Print the single number — the minimum number of taxis necessary to drive all children to Polycarpus. 

  Examples 

  Input 

  5 

  1 2 4 3 3 

  Output 

  4 

  Input 

  8 

  2 3 4 4 2 1 3 1 

  Output 

  5
刚学了这种类型题，找个几乎一模一样的练一练，这里有一个值得注意的点是，c语言（c++）中（其他语言不清楚），非0为真，所以`if(-1)`也为真，所以…最后`while(a[1]>0)`要判断大于0而不是仅仅`while(a[0])`!!!

代码：

```
#include <cstdio>
using namespace std;
int a[5];
int main(void){
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        int t;
        scanf("%d",&t);
        a[t]++;
    }
    int ans=0;
    ans+=a[4];
    while(a[3]){
        ans++;
        a[3]--;
        if(a[1]){
            a[1]--;
        }
    }
    while(a[2]>=2){
        ans++;
        a[2]-=2;
    }
    while(a[2]){
        ans++;
        a[2]--;
        if(a[1]>=2){
            a[1]-=2;
        }
        else{
            a[1]=0;
        }
    }
    while(a[1]>0){
        ans++;
        a[1]-=4;
    }
    printf("%d\n",ans);
    return 0;
}
```





