# poj2406——Power Strings - westbrook1998的博客 - CSDN博客





2018年08月21日 11:29:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：32标签：[KMP																[字符串匹配](https://so.csdn.net/so/search/s.do?q=字符串匹配&t=blog)](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Given two strings a and b we define a*b to be their concatenation. For example, if a = “abc” and b = “def” then a*b = “abcdef”. If we think of concatenation as multiplication, exponentiation by a non-negative integer is defined in the normal way: a^0 = “” (the empty string) and a^(n+1) = a*(a^n). 

  Input 

  Each test case is a line of input representing s, a string of printable characters. The length of s will be at least 1 and will not exceed 1 million characters. A line containing a period follows the last test case. 

  Output 

  For each s you should print the largest n such that s = a^n for some string a. 

  Sample Input 

  abcd 

  aaaa 

  ababab 

  . 

  Sample Output 

  1 

  4 

  3 

  Hint 

  This problem has huge input, use scanf instead of cin to avoid time limit exceed.
求一个串中有多少个循环节 就可以用乘法连在一起

代码:

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=1000010;
char s[N];
int Next[N];
void kmp_pre(char* s){
    _clr(Next,0);
    int n=strlen(s);
    int i=0;
    int j=-1;
    Next[0]=-1;
    while(i<n){
        if(j==-1 || s[i]==s[j]){
            Next[++i]=++j;
        }
        else{
            j=Next[j];
        }
    }
}
int main(void){
    while(~scanf("%s",s)){
        if(strcmp(".",s)==0){
            break;
        }
        kmp_pre(s);
        int n=strlen(s);
        int cnt=1;
        if(n%(n-Next[n])==0){
            cnt=n/(n-Next[n]);
        }
        printf("%d\n",cnt);
    }
    return 0;
}
```






