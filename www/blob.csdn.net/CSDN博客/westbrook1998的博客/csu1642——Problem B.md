# csu1642——Problem B - westbrook1998的博客 - CSDN博客





2018年08月04日 16:08:57[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40标签：[前缀和](https://so.csdn.net/so/search/s.do?q=前缀和&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
已知两个正整数a和b，求在a与b之间（包含a和b）的所有整数的十进制表示中1出现的次数。 

  Input 

  多组数据(不超过100000组)，每组数据2个整数a，b.(1≤a,b≤1000000). 

  Output 

  每组数据的答案占一行。 

  Sample Input 

  1 10 

  10 100 

  2 1 

  Sample Output 

  2 

  20 

  1
原来这就是前缀和 

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=1000005;
int pre[MAXN];
int cal(int n){
    int ans=0;
    while(n>0){
        if(n%10==1){
            ans++;
        }
        n/=10;
    }
    return ans;
}
void init(){
    for(int i=1;i<=1000000;i++){
        pre[i]=pre[i-1]+cal(i);
    }
}
int main(void){
    init();
    int a,b;
    while(~scanf("%d%d",&a,&b)){
        if(a>b){
            int t=a;
            a=b;
            b=t;
        }
        printf("%d\n",pre[b]-pre[a-1]);
    }
    return 0;
}
```





