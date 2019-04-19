# A == B ?（揭开神奇的面纱） - HJ - CSDN博客
2016年05月10日 13:07:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：790
hdu  A == B ? （2054）
Problem Description 
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
判断两个数是否相等，这是这道题的中心意思，不过题目并没有对A和B这两个数作过任何要求，是不是有问题？？？
分析一下： 
1、大小不限；（超过了long long型存数的范围，注意用字符串处理） 
2、整数小数没有规定；（考虑小数点以及小数点后面多余的0） 
3、一些奇怪的bug；（譬如：1.和1.0，0和0.0和0.等等）
思路： 
1、既然大小不限，那输入的时候就开一个字符串，不能太小，因为要考虑小数部分的长度（这点很重要）；
2、用if加循环，定义一个变量记录小数点的位置，对于小数点后面多余的0，则从字符串最后一位数开始遍历，如果为零，则变为空，直到不出现数字零为止；
因此，记录小数点的位置的原因有两点：
判断该数是否有小数位； 
从字符串最后一位数遍历，如果小数部分全部为零，并且整数部分也有零的话，就直接循环到小数点位置即可；（便于处理）
3、判断小数点存在后，再附加一个条件，只要小数点后面只有数字零或者没有任何数字，则将该数的小数点去掉；
等到上述步骤全部处理完毕后，便可以将两个字符串相与比较了
（该题是字符串问题处理的基础，考查了一些基本的问题处理方法，找全bug是整个题目的核心）
代码如下：
```cpp
#include<stdio.h>
#include<string.h>
char a[100005],b[100005];
int main()
{
    int i,j;
    while(scanf("%s%s",a,b)!=EOF)
    {
        int p,q,flag=0;
        p=strlen(a),q=strlen(b);      //记录字符串长度，便于逆序循环
        for(i=0;i<p;i++)
        {
          if(a[i]=='.')
          {
              flag=i;               //记下小数点的位置，考虑bug的各种情况
          }
        }
        if(flag)
        {
            for(j=p-1;a[j]=='0';j--)
                 a[j]='\0';            //判断小数点后面出现后继0，则清空
        }
        flag=0;
        for(i=0;i<q;i++)
        {
          if(b[i]=='.')
          {
              flag=i;
          }
        }
        if(flag)
        {
            for(j=q-1;b[j]=='0';j--)
                 b[j]='\0';
        }                                      //对字符串b的处理同上
        p=strlen(a),q=strlen(b);
        if(a[p-1]=='.')                    
          a[p-1]='\0';                   //如果以上步骤全部处理完之后小数点在最后一位，那么则表示该数的小数部分毫无意义，这样一来，便需要清空小数点的位置，便于比较
        if(b[q-1]=='.')
          b[q-1]='\0';
        if(!strcmp(a,b))
          printf("YES\n");
        else
          printf("NO\n");
    }
    return 0;
}
```
