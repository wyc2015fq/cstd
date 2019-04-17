# Codeforces1003D——Coins and Queries - westbrook1998的博客 - CSDN博客





2018年08月09日 19:03:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Polycarp has n coins, the value of the i-th coin is ai. It is guaranteed that all the values are integer powers of 2 (i.e. ai=2d for some non-negative integer number d). 

  Polycarp wants to know answers on q queries. The j-th query is described as integer number bj. The answer to the query is the minimum number of coins that is necessary to obtain the value bj using some subset of coins (Polycarp can use only coins he has). If Polycarp can’t obtain the value bj, the answer to the j-th query is -1. 

  The queries are independent (the answer on the query doesn’t affect Polycarp’s coins). 

  Input 

  The first line of the input contains two integers n and q (1≤n,q≤2⋅105) — the number of coins and the number of queries. 

  The second line of the input contains n integers a1,a2,…,an — values of coins (1≤ai≤2⋅109). It is guaranteed that all ai are integer powers of 2 (i.e. ai=2d for some non-negative integer number d). 

  The next q lines contain one integer each. The j-th line contains one integer bj — the value of the j-th query (1≤bj≤109). 

  Output 

  Print q integers ansj. The j-th integer must be equal to the answer on the j-th query. If Polycarp can’t obtain the value bj the answer to the j-th query is -1. 

  Example 

  Input 

  5 4 

  2 4 8 2 4 

  8 

  5 

  14 

  10 

  Output 

  1 

  -1 

  3 

  2
不知道为什么是贪心，反正贪心就没错了 

读取的时候记录下最大的硬币面值，同时把每个面值加到map里面，方便后面查找 

然后循环就从最大面值开始，依次除2，如果没有该面值，也就是map值为0，则跳过 
`int use=min(res[i],k/i);` 然后注意这一句，要找出手上有的硬币个数和应该拿的硬币数的最小值，比如12/4=3需要3个4元硬币，但实际res[4]=2，只有两个，则要取小的 

然后就判断什么时候凑齐和什么时候无法凑齐
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <map>
using namespace std;
map<int,int> res;
int main(void){
    int n,q,t;
    int k;
    scanf("%d%d",&n,&q);
    int Max=0;
    while(n--){
        scanf("%d",&t);
        Max=max(Max,t);
        res[t]++;
    }
    while(q--){
        int ans=0;
        scanf("%d",&k);
        for(int i=Max;i>0;i/=2){
            if(res[i]==0){
                continue;
            }
            else{
                int use=min(res[i],k/i);
                //printf("%d %d %d\n",k,i,use);
                ans+=use;
                k-=use*i;
            }

            if(k==0){
                break;
            }
        }
        if(k!=0){
            printf("-1\n");
        }
        else{
            printf("%d\n",ans);
        }
    }
    return 0;
}
```






