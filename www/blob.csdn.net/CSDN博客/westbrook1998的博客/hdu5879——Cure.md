# hdu5879——Cure - westbrook1998的博客 - CSDN博客





2018年09月02日 22:13:23[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41标签：[数学](https://so.csdn.net/so/search/s.do?q=数学&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Given an integer n, we only want to know the sum of 1/k2 where k from 1 to n. 

  Input 

  There are multiple cases.  

  For each test case, there is a single line, containing a single positive integer n.  

  The input file is at most 1M.  

  Output 

  The required sum, rounded to the fifth digits after the decimal point. 

  Sample Input 

  1 

  2 

  4 

  8 

  15 

  Sample Output 

  1.00000 

  1.25000 

  1.42361 

  1.52742 

  1.58044
求级数的前n项和，只要精确到五位小数，打表发现（其实百度发现）n到1e6就会收敛接近一个值，所以就可以了 注意读入用字符串然后一位一位计算

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1e6+5;
char a[N];
double sum[N];
void init(){
    for(int i=1;i<N;i++){
        sum[i]=sum[i-1]+(double)1/i/i;
    }
    // for(int i=1;i<1000;i++){
    //     printf("%.5lf\n",sum[i]);
    // }
}
int main(void){
    init();
    while(~scanf("%s",a)){
        int l=strlen(a);
        if(l>=7){
            printf("%.5lf\n",sum[1000000]);
        }
        else{
            int n=0;
            for(int i=0;i<l;i++){
                n=n*10+a[i]-'0';
            }
            printf("%.5lf\n",sum[n]);
        }
    }
    return 0;
}
```





