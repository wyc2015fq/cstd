# hdu1001——Sum Problem - westbrook1998的博客 - CSDN博客





2018年06月15日 21:31:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40








> 
Hey, welcome to HDOJ(Hangzhou Dianzi University Online Judge).  

  In this problem, your task is to calculate SUM(n) = 1 + 2 + 3 + … + n.  

  Input 

  The input will consist of a series of integers n, one integer per line.  

  Output 

  For each case, output SUM(n) in one line, followed by a blank line. You may assume the result will be in the range of 32-bit signed integer.  

  Sample Input 

  1 

  100 

  Sample Output 

  1 

  5050
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int main(void){
    int n;
    while(~scanf("%d",&n)){
        int res=0;
        for(int i=1;i<=n;i++){
            res+=i;
        }
        printf("%d\n\n",res);
    }
    return 0;
}
```





