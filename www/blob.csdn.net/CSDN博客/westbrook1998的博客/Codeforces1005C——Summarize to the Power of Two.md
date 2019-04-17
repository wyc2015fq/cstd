# Codeforces1005C——Summarize to the Power of Two - westbrook1998的博客 - CSDN博客





2018年08月08日 22:22:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：47








> 
A sequence a1,a2,…,an is called good if, for each element ai, there exists an element aj (i≠j) such that ai+aj is a power of two (that is, 2d for some non-negative integer d). 

  For example, the following sequences are good: 

  [5,3,11] (for example, for a1=5 we can choose a2=3. Note that their sum is a power of two. Similarly, such an element can be found for a2 and a3), 

  [1,1,1,1023], 

  [7,39,89,25,89], 

  []. 

  Note that, by definition, an empty sequence (with a length of 0) is good. 

  For example, the following sequences are not good: 

  [16] (for a1=16, it is impossible to find another element aj such that their sum is a power of two), 

  [4,16] (for a1=4, it is impossible to find another element aj such that their sum is a power of two), 

  [1,3,2,8,8,8] (for a3=2, it is impossible to find another element aj such that their sum is a power of two). 

  You are given a sequence a1,a2,…,an. What is the minimum number of elements you need to remove to make it good? You can delete an arbitrary set of elements. 

  Input 

  The first line contains the integer n (1≤n≤120000) — the length of the given sequence. 

  The second line contains the sequence of integers a1,a2,…,an (1≤ai≤109). 

  Output 

  Print the minimum number of elements needed to be removed from the given sequence in order to make it good. It is possible that you need to delete all n elements, make it empty, and thus get a good sequence. 

  Examples 

  Input 

  6 

  4 7 1 5 4 9 

  Output 

  1 

  Input 

  5 

  1 2 3 4 5 

  Output 

  2 

  Input 

  1 

  16 

  Output 

  1 

  Input 

  4 

  1 1 1 1023 

  Output 

  0
跟leetcode里找两个数的和那几道题差不多 要用到map 如果暴力遍历就超时了

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <set>
#include <map>
using namespace std;
int n;
const int MAXN=120050;
int a[MAXN];
int vis[MAXN];
long long p[]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648

};
map<long long,long long> mp;
int main(void){
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
        mp[a[i]]++;
    }
    sort(a,a+n);
    for(int i=0;i<n;i++){
        if(vis[i]){
            continue;
        }
        for(int j=31;j>=0;j--){
            int t=p[j]-a[i];
            if(mp[t]>0){
                //find
                if(t==a[i]){
                    if(mp[t]!=1){
                        vis[i]=1;
                    }
                }
                else{
                    vis[i]=1;
                }
            }
        }
    }
    int ans=0;
    for(int i=0;i<n;i++){
        if(vis[i]==0){
            //printf("%d\n",i);
            ans++;
        }
    }
    printf("%d\n",ans);
    return 0;
}
```





