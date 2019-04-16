# HDU5178 pairs【二分法】【尺取法】 - 紫芝的博客 - CSDN博客





2018年11月11日 12:42:47[紫芝](https://me.csdn.net/qq_40507857)阅读数：252








## [pairs](https://cn.vjudge.net/problem/HDU-5178)[HDU - 5178](https://cn.vjudge.net/problem/127164/origin)

John has nn points on the X axis, and their coordinates are (x[i],0),(i=0,1,2,…,n−1) He wants to know how many pairs<a,b> that |x[b]−x[a]|≤k.(a<b)

Input

The first line contains a single integer T (about 5), indicating the number of cases. 

Each test case begins with two integers n,k(1≤n≤100000,1≤k≤10^9)

Next nn lines contain an integer x[i](−109≤x[i]≤10^9)

Output

For each case, output an integer means how many pairs<a,b>,that |x[b]−x[a]|≤k

Sample Input

```
2
5 5
-100
0
100
101
102
5 300
-100
0
100
101
102
```

Sample Output

```
3
10
```

### 题意：

有N个数组成的序列，问有多少对<a,b>满足|x[b]−x[a]|≤k.(a<b)，去掉绝对值即 x[a]-k ≤ x[b] ≤ x[a]-k (a<b)，枚举前面一个点，然后二分查找即可

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+10;
typedef long long LL;
LL x[maxn];
int n;
LL k;
int main(){
	int T;
	scanf("%d",&T);
	while(T--){
        scanf("%d%lld",&n,&k);
        for(int i=1;i<=n;i++){
            scanf("%lld",&x[i]);
        }
        sort(x+1,x+1+n);
        LL ans=0;
        for(int i=1;i<=n;i++){
            int l=lower_bound(x+i+1,x+1+n,x[i]-k)-(x+1);
            int r=upper_bound(x+1+i,x+1+n,x[i]+k)-(x+1);
            ans+=(r-l);
        }
        printf("%lld\n",ans);
	}
	return 0;
}
```

直接查找下界即可

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+10;
typedef long long LL;
LL x[maxn];
int n;
LL k;
int main(){
	int T;
	scanf("%d",&T);
	while(T--){
        scanf("%d%lld",&n,&k);
        for(int i=1;i<=n;i++){
            scanf("%lld",&x[i]);
        }
        sort(x+1,x+1+n);
        LL ans=0;
        for(int i=1;i<=n;i++){
            int r=upper_bound(x+1+i,x+1+n,x[i]+k)-(x+1);
            ans+=(r-i);
        }
        printf("%lld\n",ans);
	}
	return 0;
}
```

下标从0开始

```cpp
# include <stdio.h>
# include <algorithm>
using namespace std;
 
int a[100001];
int main()
{
    int n, k, t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&k);
        long long ans = 0;
        for(int i=0; i<n; ++i)
            scanf("%d",&a[i]);
        sort(a, a+n);
        for(int i=0; i<n-1; ++i)
        {
            int pos = upper_bound(a+i,a+n, a[i]+k)-a;
            ans += pos - i - 1;
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```

尺取法

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+10;
typedef long long LL;
LL x[maxn];
int n;
LL k;
int main(){
	int T;
	scanf("%d",&T);
	while(T--){
        scanf("%d%lld",&n,&k);
        for(int i=0;i<n;i++){
            scanf("%lld",&x[i]);
        }
        sort(x,x+n);
        LL ans=0;

       for(int l=0,r=0;l<n;++l){
            while(r+1<n&&x[r+1]-x[l]<=k)    ++r;
            ans+=(r-l);
        }
        printf("%lld\n",ans);
	}
	return 0;
}
```





