# hdu2524——矩形A + B - westbrook1998的博客 - CSDN博客





2018年08月29日 20:25:15[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：24








> 
给你一个高为n ，宽为m列的网格，计算出这个网格中有多少个矩形，下图为高为2，宽为4的网格.  
![这里写图片描述](https://odzkskevi.qnssl.com/30eb0263b48b7de077ccf5a73574b88d?v=1535524142)

  Input 

  第一行输入一个t, 表示有t组数据，然后每行输入n,m,分别表示网格的高和宽 ( n < 100 , m < 100).  

  Output 

  每行输出网格中有多少个矩形. 

  Sample Input 

  2 

  1 2 

  2 4 

  Sample Output 

  3 

  30
搜到一个很厉害的题解 

n高的矩形有n+1条线 

m宽的矩形有m+1条线 

每两条横着的线和每两条竖着的线交叉就形成矩形 

因此答案为 C(m+1,2)*C(n+1,2)  Niubi
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int main(void){
    int t;
    int n,m;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        printf("%d\n",(n+1)*n*(m+1)*m/4);
    }
    return 0;
}
```






