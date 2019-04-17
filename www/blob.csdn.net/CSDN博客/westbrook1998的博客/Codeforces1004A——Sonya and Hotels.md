# Codeforces1004A——Sonya and Hotels - westbrook1998的博客 - CSDN博客





2018年08月09日 19:08:29[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31标签：[set																[暴力](https://so.csdn.net/so/search/s.do?q=暴力&t=blog)](https://so.csdn.net/so/search/s.do?q=set&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Sonya decided that having her own hotel business is the best way of earning money because she can profit and rest wherever she wants. 

  The country where Sonya lives is an endless line. There is a city in each integer coordinate on this line. She has n hotels, where the i-th hotel is located in the city with coordinate xi. Sonya is a smart girl, so she does not open two or more hotels in the same city. 

  Sonya understands that her business needs to be expanded by opening new hotels, so she decides to build one more. She wants to make the minimum distance from this hotel to all others to be equal to d. The girl understands that there are many possible locations to construct such a hotel. Thus she wants to know the number of possible coordinates of the cities where she can build a new hotel. 

  Because Sonya is lounging in a jacuzzi in one of her hotels, she is asking you to find the number of cities where she can build a new hotel so that the minimum distance from the original n hotels to the new one is equal to d. 

  Input 

  The first line contains two integers n and d (1≤n≤100, 1≤d≤109) — the number of Sonya’s hotels and the needed minimum distance from a new hotel to all others. 

  The second line contains n different integers in strictly increasing order x1,x2,…,xn (−109≤xi≤109) — coordinates of Sonya’s hotels. 

  Output 

  Print the number of cities where Sonya can build a new hotel so that the minimum distance from this hotel to all others is equal to d. 

  Examples 

  Input 

  4 3 

  -3 2 9 16 

  Output 

  6 

  Input 

  5 2 

  4 8 11 18 19 

  Output 

  5 

  Note 

  In the first example, there are 6 possible cities where Sonya can build a hotel. These cities have coordinates −6, 5, 6, 12, 13, and 19. 

  In the second example, there are 5 possible cities where Sonya can build a hotel. These cities have coordinates 2, 6, 13, 16, and 21.
这个题是不难，但是一开始读不懂，就是给出一个数列，然后问要加一个数，使得到已知的所有数的最短距离等于所给的d 

数不多，直接每一个一次枚举+d和-d 然后判断其他数是否有比这个距离小的 两重循环也才100*100 

注意结果不能重复，所以我用了set去重
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;
long long a[105];
set<int> res;
int main(void){
    int n;
    long long d;
    scanf("%d%lld",&n,&d);
    for(int i=0;i<n;i++){
        scanf("%lld",&a[i]);
    }
    for(int i=0;i<n;i++){
        long long t=a[i]+d;
        int j;
        for(j=0;j<n;j++){
            if(abs(t-a[j])<d){
                break;
            }
        }
        if(j==n){
            //printf("%lld\n",t);
            res.insert(t);
        }
        t=a[i]-d;
        for(j=0;j<n;j++){
            if(abs(t-a[j])<d){
                break;
            }
        }
        if(j==n){
            //printf("%lld\n",t);
            res.insert(t);
        }
    }
    printf("%d\n",res.size());
    return 0;
}
```







