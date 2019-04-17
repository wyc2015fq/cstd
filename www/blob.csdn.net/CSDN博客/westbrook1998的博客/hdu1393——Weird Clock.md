# hdu1393——Weird Clock - westbrook1998的博客 - CSDN博客





2018年05月12日 20:27:14[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：47








> 
A weird clock marked from 0 to 59 has only a minute hand. It won’t move until a special coin is thrown into its box. There are different kinds of coins as your options. However once you make your choice, you cannot use any other kind. There are infinite number of coins of each kind, each marked with a number d ( 1 <= d <= 1000 ), meaning that this coin will make the minute hand move d times clockwise the current time. For example, if the current time is 45, and d = 2. Then the minute hand will move clockwise 90 minutes and will be pointing to 15.  

  Now you are given the initial time s `( 1 <= s <= 59 )` and the coin’s type d. Write a program to find the minimum number of d-coins needed to turn the minute hand back to 0.  

  Input 

  There are several tests. Each test occupies a line containing two positive integers s and d.  

  The input is finished by a line containing 0 0.  

  Output 

  For each test print in a single line the minimum number of coins needed. If it is impossible to turn the hand back to 0, output “Impossible”.  

  Sample Input 

  30 1 

  0 0 

  Sample Output 

  1
做一下简单题，题意看清楚后应该就不难解决，主要是判断Impossible这里想不到什么太好的方法，就用了一个伪算法，如果循环了1000还没结束，就输出Impossible，结果就AC了…

代码：

```cpp
#include <cstdio>
using namespace std;
int main(void){
    int cu,d;
    while(~scanf("%d%d",&cu,&d)){
        if(cu==0 && d==0){
            break;
        }
        int ans=0;
        while(cu%60!=0){
            cu=cu*(d+1)%60;
            ans++;
            if(ans>1000){
                printf("Impossible\n");
                break;
            }
        }
        if(ans<=1000){
            printf("%d\n",ans);
        }
    }
    return 0;
}
```





