# hdu2035——人见人爱A^B - westbrook1998的博客 - CSDN博客





2018年08月05日 09:38:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：47








> 
求A^B的最后三位数表示的整数。  

  说明：A^B的含义是“A的B次方”  

  Input 

  输入数据包含多个测试实例，每个实例占一行，由两个正整数A和B组成（1<=A,B<=10000），如果A=0, B=0，则表示输入数据的结束，不做处理。 

  Output 

  对于每个测试实例，请输出A^B的最后三位表示的整数，每个输出占一行。  

  Sample Input 

  2 3 

  12 6 

  6789 10000 

  0 0 

  Sample Output 

  8 

  984 

  1
快速幂取模 

代码：

```
#include <algorithm>
using namespace std;
int a,b;
int quick_pow_mod(int a,int b){
    int res=1;
    while(b){
        if(b%2==1){
            res=(res*a)%1000;
        }
        a=(a*a)%1000;
        b/=2;
    }
    return res%1000;
}
int main(void){
    while(~scanf("%d%d",&a,&b)){
        if(a==0 && b==0){
            break;
        }
        printf("%d\n",quick_pow_mod(a,b));
    }
    return 0;
}
```





