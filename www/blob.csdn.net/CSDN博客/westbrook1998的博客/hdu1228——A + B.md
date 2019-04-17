# hdu1228——A + B - westbrook1998的博客 - CSDN博客





2018年07月07日 20:21:24[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：74








> 
读入两个小于100的正整数A和B,计算A+B.  

  需要注意的是:A和B的每一位数字由对应的英文单词给出.  

  Input 

  测试输入包含若干测试用例,每个测试用例占一行,格式为”A + B =”,相邻两字符串有一个空格间隔.当A和B同时为0时输入结束,相应的结果不要输出.  

  Output 

  对每个测试用例输出1行,即A+B的值.  

  Sample Input 

  one + two = 

  three four + five six = 

  zero seven + eight nine = 

  zero + zero = 

  Sample Output 

  3 

  90 

  96
字符串读取的处理 主要是空格格式上的处理 还有数字有可能一位也有可能两位 比较烦 但是题是很简单的

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
char a[10];
char p[2];
char nums[10][10]={"zero","one","two","three","four","five","six","seven","eight","nine"};
int get_num(char s[]){
    for(int i=0;i<10;i++){
        if(strcmp(s,nums[i])==0){
            return i;
        }
    }
    return -1;
}
int main(void){
    while(1){
        memset(a,'\0',sizeof(a));
        scanf("%s",a);
        int t=get_num(a);
        int n=0;
        int m=0;
        scanf("%s",a);
        if(a[0]=='+'){
            n=t;
        }
        else{
            n=t*10+get_num(a);
            scanf("%s",p);
        }
        scanf("%s",a);
        t=get_num(a);
        scanf("%s",a);
        if(a[0]!='='){
            t=t*10+get_num(a);
            scanf("%s",p);
        }
        m=t;
        if(n==0 && m==0){
            break;
        }
        printf("%d\n",n+m);
    }
    return 0;
}
```





