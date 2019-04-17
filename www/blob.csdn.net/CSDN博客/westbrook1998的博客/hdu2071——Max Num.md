# hdu2071——Max Num - westbrook1998的博客 - CSDN博客





2018年07月23日 15:57:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：82








> 
There are some students in a class, Can you help teacher find the highest student . 

  Input 

  There are some cases. The first line contains an integer t, indicate the cases; Each case have an integer n ( 1 ≤ n ≤ 100 ) , followed n students’ height. 

  Output 

  For each case output the highest height, the height to two decimal plases; 

  Sample Input 

  2 

  3 170.00 165.00 180.00 

  4 165.00 182.00 172.00 160.00 

  Sample Output 

  180.00 

  182.00
代码：

```cpp
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
double a[105];
bool cmp(double b,double c){
    return b>c;
}
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%lf",&a[i]);
        }
        sort(a,a+n,cmp);
        printf("%.2lf\n",a[0]);
    }
    return 0;
}
```





