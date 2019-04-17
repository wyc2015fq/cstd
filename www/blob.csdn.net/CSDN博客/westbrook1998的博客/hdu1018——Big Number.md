# hdu1018——Big Number - westbrook1998的博客 - CSDN博客





2018年05月10日 14:16:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29








> 
In many applications very large integers numbers are required. Some of these applications are using keys for secure transmission of data, encryption, etc. In this problem you are given a number, you have to determine the number of digits in the factorial of the number.  

  Input 

  Input consists of several lines of integer numbers. The first line contains an integer n, which is the number of cases to be tested, followed by n lines, one integer 1 ≤ n ≤ 10 7 on each line.  

  Output 

  The output contains the number of digits in the factorial of the integers appearing in the input.  

  Sample Input 

  2 

  10 

  20 

  Sample Output 

  7 

  19
以为是难题，结果是水题… 首先是题意一开始没看懂，吃了不会英语的亏…原来求的是一个数阶乘的位数，然后百度告诉我原来求一个数的位数可以用`(int)log10(n)+1`，又吃了没文化的亏，然后就可以解决了，一次循环将每一个log10(i)加起来，最后强转int+1 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
int main(void){
    int t;
    int n;
    double ans;
    scanf("%d",&t);
    while(t--){
        ans=0;
        scanf("%d",&n);
        for(int i=2;i<=n;i++){
            ans+=log10(i);
        }
        printf("%d\n",(int)ans+1);
    }
    return 0;
}
```





