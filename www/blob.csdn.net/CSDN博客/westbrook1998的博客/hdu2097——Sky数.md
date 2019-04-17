# hdu2097——Sky数 - westbrook1998的博客 - CSDN博客





2018年08月18日 22:02:51[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52








> 
Sky从小喜欢奇特的东西，而且天生对数字特别敏感，一次偶然的机会，他发现了一个有趣的四位数2992，这个数，它的十进制数表示，其四位数字之和为2+9+9+2=22，它的十六进制数BB0，其四位数字之和也为22，同时它的十二进制数表示1894，其四位数字之和也为22，啊哈，真是巧啊。Sky非常喜欢这种四位数，由于他的发现，所以这里我们命名其为Sky数。但是要判断这样的数还是有点麻烦啊，那么现在请你帮忙来判断任何一个十进制的四位数，是不是Sky数吧。  

  Input 

  输入含有一些四位正整数，如果为0，则输入结束。  

  Output 

  若n为Sky数，则输出“#n is a Sky Number.”，否则输出“#n is not a Sky Number.”。每个结果占一行。注意：#n表示所读入的n值。  

  Sample Input 

  2992 

  1234 

  0 

  Sample Output 

  2992 is a Sky Number. 

  1234 is not a Sky Number.
水题休息一下 

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
bool check(int n){
    int ans=0;
    int num=n;
    while(num){
        ans+=(num%10);
        num/=10;
    }
    int res=0;
    num=n;
    while(num){
        res+=(num%12);
        num/=12;
    }
    if(ans==res){
        ans=0;
        num=n;
        while(num){
            ans+=(num%16);
            num/=16;
        }
        if(ans==res){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
int main(void){
    int n;
    while(~scanf("%d",&n) && n){
        if(check(n)){
            printf("%d is a Sky Number.\n",n);
        }
        else{
            printf("%d is not a Sky Number.\n",n);
        }
    }
    return 0;
}
```





