# hdu5904——LCIS - westbrook1998的博客 - CSDN博客





2018年07月19日 23:52:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：37








> 
Alex has two sequences a1,a2,…,an and b1,b2,…,bm. He wants find a longest common subsequence that consists of consecutive values in increasing order. 

  Input 

  There are multiple test cases. The first line of input contains an integer T, indicating the number of test cases. For each test case:  

  The first line contains two integers n and m (1≤n,m≤100000) – the length of two sequences. The second line contains n integers: a1,a2,…,an (1≤ai≤106). The third line contains n integers: b1,b2,…,bm (1≤bi≤106).  

  There are at most 1000 test cases and the sum of n and m does not exceed 2×106.  

  Output 

  For each test case, output the length of longest common subsequence that consists of consecutive values in increasing order. 

  Sample Input 

  3 

  3 3 

  1 2 3 

  3 2 1 

  10 5 

  1 23 2 32 4 3 4 5 6 1 

  1 2 3 4 5 

  1 1 

  2 

  1 

  Sample Output 

  1 

  5 

  0
不想评价这道题…tle了20多次

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=1e5+10;
int a[MAXN];
int b[MAXN];
int dpa[MAXN];
int dpb[MAXN];
int main(void){
    int t,n,m;
    scanf("%d",&t);
    while(t--) {
        scanf("%d %d", &n, &m);
        memset(dpa, 0, sizeof dpa);
        memset(dpb, 0, sizeof dpb);
        for(int i=0; i<n; ++i) {
            scanf("%d", &a[i]);
            dpa[a[i]] = dpa[a[i]-1] + 1;
        }
        for(int i=0; i<m; ++i) {
            scanf("%d", &b[i]);
            dpb[b[i]] = dpb[b[i]-1] + 1;
        }
        int ans = 0;
        for(int i=1; i<=MAXN; ++i) {
            ans = max(ans, min(dpa[i], dpb[i]));
        }
        printf("%d\n", ans);
    }
    return 0;
}
```





