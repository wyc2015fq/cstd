# hdu 1228 A+B（简单模拟） - HJ - CSDN博客
2017年03月31日 21:53:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：290
Problem Description 
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
```cpp
#include<iostream>
#include<cstring>
using namespace std;
char s[15],c[11][10]={"zero","one","two","three","four","five","six","seven","eight","nine"};
int to(char s[])             //从字符串数组中找到对应的数字 
{
    for(int i=0;i<10;i++)
      if(!strcmp(s,c[i]))
        return i;
}
int main()
{
    while(1)
    {
        int a=0,b=0;
        while(cin>>s&&strcmp(s,"+"))    //分开处理 
          a=a*10+to(s);         //如果出现二位数累加就行了 
        while(cin>>s&&strcmp(s,"="))
          b=b*10+to(s);
        if(a==0&&b==0)       //跳出循环为两个0 
          break;
        cout<<a+b<<endl;
    }   
    return 0;
}
```
