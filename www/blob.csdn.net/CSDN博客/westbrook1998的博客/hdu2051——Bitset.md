# hdu2051——Bitset - westbrook1998的博客 - CSDN博客





2018年08月06日 10:21:57[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：66








> 
Give you a number on base ten,you should output it on base two.(0 < n < 1000) 

  Input 

  For each case there is a postive number n on base ten, end of file. 

  Output 

  For each case output a number on base two. 

  Sample Input 

  1 

  2 

  3 

  Sample Output 

  1 

  10 

  11
转二进制 水 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
vector<int> r;
int main(void){
    int n;
    while(~scanf("%d",&n)){
        r.clear();
        while(n){
            r.push_back(n%2);
            n/=2;
        }
        int l=r.size();
        for(int i=l-1;i>0;i--){
            printf("%d",r[i]);
        }
        printf("%d\n",r[0]);
    }
    return 0;
}
```





