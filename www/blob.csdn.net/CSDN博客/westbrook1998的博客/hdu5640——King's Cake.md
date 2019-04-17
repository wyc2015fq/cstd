# hdu5640——King's Cake - westbrook1998的博客 - CSDN博客





2018年07月18日 22:54:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
It is the king’s birthday before the military parade . The ministers prepared a rectangle cake of size n×m(1≤n,m≤10000) . The king plans to cut the cake himself. But he has a strange habit of cutting cakes. Each time, he will cut the rectangle cake into two pieces, one of which should be a square cake.. Since he loves squares , he will cut the biggest square cake. He will continue to do that until all the pieces are square. Now can you tell him how many pieces he can get when he finishes. 

  Input 

  The first line contains a number T(T≤1000), the number of the testcases.  

  For each testcase, the first line and the only line contains two positive numbers n,m(1≤n,m≤10000). 

  Output 

  For each testcase, print a single number as the answer. 

  Sample Input 

  2 

  2 3 

  2 5 

  Sample Output 

  3 

  4 

  hint: 

  For the first testcase you can divide the into one cake of $2\times2$ , 2 cakes of $1\times 1$
将一个长方形划分为多个正方形 

这不就是辗转相除法嘛 

代码：
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int fun(int a,int b,int n){
    if(a==b){
        return n;
    }
    else{
        return fun(max(a-b,b),min(a-b,b),n)+1;
    }
}
int main(void){
    int t;
    scanf("%d",&t);
    int n,m;
    while(t--){
        scanf("%d%d",&n,&m);
        printf("%d\n",fun(max(n,m),min(n,m),1));
    }
    return 0;
}
```






