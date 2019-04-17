# hdu2054——A == B ? - westbrook1998的博客 - CSDN博客





2018年07月08日 21:30:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45








> 
Give you two numbers A and B, if A is equal to B, you should print “YES”, or print “NO”. 

  Input 

  each test case contains two numbers A and B.  

  Output 

  for each case, if A is equal to B, you should print “YES”, or print “NO”. 

  Sample Input 

  1 2 

  2 2 

  3 3 

  4 3 

  Sample Output 

  NO 

  YES 

  YES 

  NO
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
char a[1000050];
char b[1000050];
//去掉小数点后面无用的0
void re(char s[]){
    int len=strlen(s);
    //标记是否有小数点
    int p=0;
    for(int i=0;i<len;i++){
        if(s[i]=='.'){
            p=1;
            break;
        }
    }
    //有小数点
    if(p){
        //从后面开始找0
        for(int i=len-1;i>=0;i--){
            if(s[i]=='0'){
                s[i]='\0';
            }
            else{
                break;
            }
            //注意长度要减1
            len--;
        }
        //如果小数点后全部都是无效0 最终就只剩下小数点 将小数点去掉
        if(s[len-1]=='.'){
            s[len-1]='\0';
        }
    }
}
int main(void){
    while(~scanf("%s%s",a,b)){
        re(a);
        re(b);
        if(strcmp(a,b)){
            printf("NO\n");
        }
        else{
            printf("YES\n");
        }
    }
    return 0;
}
```





