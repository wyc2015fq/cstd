# Codeforces992D——Nastya and a Game - westbrook1998的博客 - CSDN博客





2018年08月30日 19:28:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42标签：[区间																[枚举](https://so.csdn.net/so/search/s.do?q=枚举&t=blog)](https://so.csdn.net/so/search/s.do?q=区间&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Nastya received one more array on her birthday, this array can be used to play a traditional Byteland game on it. However, to play the game the players should first select such a subsegment of the array that , where p is the product of all integers on the given array, s is their sum, and k is a given constant for all subsegments. 

  Nastya wonders how many subsegments of the array fit the described conditions. A subsegment of an array is several consecutive integers of the array. 

  Input 

  The first line contains two integers n and k (1 ≤ n ≤ 2·105, 1 ≤ k ≤ 105), where n is the length of the array and k is the constant described above. 

  The second line contains n integers a1, a2, …, an (1 ≤ ai ≤ 108) — the elements of the array. 

  Output 

  In the only line print the number of subsegments such that the ratio between the product and the sum on them is equal to k. 

  Examples 

  Input 

  1 1 

  1 

  Output 

  1 

  Input 

  4 2 

  6 3 8 1 

  Output 

  2 

  Note 

  In the first example the only subsegment is [1]. The sum equals 1, the product equals 1, so it suits us because . 

  There are two suitable subsegments in the second example — [6, 3] and [3, 8, 1]. Subsegment [6, 3] has sum 9 and product 18, so it suits us because . Subsegment [3, 8, 1] has sum 12 and product 24, so it suits us because .
要求有多少个区间满足积除以和等于给定的k 

思路也是枚举 但是要优化一下 枚举左端点 然后中间如果有1的话是可以特殊处理的 

考虑凑多少个1可以满足这个区间的要求 还要考虑这些1全部省略直接考虑下一个非1的数的情况 然后还要考虑溢出 然后我就什么屁都不会抄了题解
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int N=2e5+50;
const int MOD=1e9+7;
const int INF=0x3f3f3f3f;
typedef long long ll;
ll n,k;
ll a[N];
//存储下一个非1的位置
int vis[N];
ll ans;
//判断两个数相乘是否溢出long long
bool Overflow(ll x,ll y){
    if(x>__LONG_LONG_MAX__/y){
        return false;
    }
    else{
        return true;
    }
}
int main(void){
    scanf("%lld%lld",&n,&k);
    for(int i=1;i<=n;i++){
        scanf("%lld",&a[i]);
    }
    vis[n+1]=-1;
    for(int i=n;i>=1;i--){
        if(a[i]>1){
            vis[i]=i;
        }
        else{
            vis[i]=vis[i+1];
        }
    }
    ll p=1;
    ll s=0;
    for(int i=1;i<=n;i++){
        int j=i;
        p=a[i];
        s=a[i];
        if(p==s*k){
            ans++;
        }
        while(true){
            int z=vis[j+1];
            if(z==-1){
                z=n+1;
            }
            int ones=z-j-1;
            if(p%k==0 && p/k-s>=1 && p/k-s<=ones){
                ans++;
            }
            if(z==n+1){
                break;
            }
            if(Overflow(p,a[z])){
                p=p*a[z];
                j=z;
                s=s+a[z]+ones;
                if(p==s*k){
                    ans++;
                }
            }
            else{
                break;
            }
        }
    }
    printf("%lld\n",ans);
    return 0;
}
```







