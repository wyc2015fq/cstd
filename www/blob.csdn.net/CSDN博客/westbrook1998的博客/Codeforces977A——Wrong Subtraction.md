# Codeforces977A——Wrong Subtraction - westbrook1998的博客 - CSDN博客





2018年08月15日 11:28:50[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：53标签：[暴力																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)](https://so.csdn.net/so/search/s.do?q=暴力&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Little girl Tanya is learning how to decrease a number by one, but she does it wrong with a number consisting of two or more digits. Tanya subtracts one from a number by the following algorithm: 

  If the last digit of the number is non-zero, she decreases the number by one; 

  if the last digit of the number is zero, she divides the number by 10 (i.e. removes the last digit). 

  You are given an integer number n. Tanya will subtract one from it k times. Your task is to print the result after all k subtractions. 

  It is guaranteed that the result will be positive integer number. 

  Input 

  The first line of the input contains two integer numbers n and k (2≤n≤109, 1≤k≤50) — the number from which Tanya will subtract and the number of subtractions correspondingly. 

  Output 

  Print one integer number — the result of the decreasing n by one k times. 

  It is guaranteed that the result will be positive integer number. 

  Examples 

  Input 

  512 4 

  Output 

  50 

  Input 

  1000000000 9 

  Output 

  1 

  Note 

  The first example corresponds to the following sequence: 512→511→510→51→50.
暴力 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int main(void){
    int n,k;
    scanf("%d%d",&n,&k);
    while(k--){
        if(n%10==0){
            n/=10;
        }
        else{
            n--;
        }
    }
    printf("%d\n",n);
    return 0;
}
```






