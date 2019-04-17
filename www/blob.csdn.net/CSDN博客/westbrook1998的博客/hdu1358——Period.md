# hdu1358——Period - westbrook1998的博客 - CSDN博客





2018年08月21日 11:28:03[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41








> 
For each prefix of a given string S with N characters (each character has an ASCII code between 97 and 126, inclusive), we want to know whether the prefix is a periodic string. That is, for each i (2 <= i <= N) we want to know the largest K > 1 (if there is one) such that the prefix of S with length i can be written as A K , that is A concatenated K times, for some string A. Of course, we also want to know the period K.  

  Input 

  The input file consists of several test cases. Each test case consists of two lines. The first one contains N (2 <= N <= 1 000 000) – the size of the string S. The second line contains the string S. The input file ends with a line, having the number zero on it.  

  Output 

  For each test case, output “Test case #” and the consecutive test case number on a single line; then, for each prefix with length i that has a period K > 1, output the prefix size i and the period K separated by a single space; the prefix sizes must be in increasing order. Print a blank line after each test case.  

  Sample Input 

  3 

  aaa 

  12 

  aabaabaabaab 

  0 

  Sample Output 

  Test case #1 

  2 2 

  3 3 

  Test case #2 

  2 2 

  6 2 

  9 3 

  12 4
这题题意一开始没看懂 

要求的是到某个字符前面的字符串能有几个循环节组成 

其实也就是找最小循环节 然后除一下
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x))
using namespace std;
const int N=1000010;
char s[N];
int Next[N];
int n;
void kmp_pre(char* p){
    _clr(Next,0);
    int i=0;
    int j=-1;
    Next[0]=-1;
    while(i<n){
        if(j==-1 || p[i]==p[j]){
            Next[++i]=++j;
        }
        else{
            j=Next[j];
        }
    }
}
int main(void){
    int c=1;
    while(~scanf("%d",&n) && n){
        scanf("%s",s);
        kmp_pre(s);
        // for(int i=0;i<=n;i++){
        //     printf("%d ",Next[i]);
        // }
        // printf("\n");
        printf("Test case #%d\n",c++);
        for(int i=2;i<=n;i++){
            if(Next[i]!=0 && i%(i-Next[i])==0){
                printf("%d %d\n",i,i/(i-Next[i]));
            }
        }
        printf("\n");
    }
    return 0;
}
```






