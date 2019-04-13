
# ACM远古文明的算术题 （C语言实现） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月30日 22:09:14[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2020


远古文明的算术题
Description
考古人员发现地球在一亿年以前曾经存在一个高级文明叫做Delta，而且发现这个文明的具有文字和语言。经过艰苦卓绝的工作，专家们破译了其中的一些文字和表示方法。他们使用+表示加运算，-表示减运算，*表示乘运算，/表示整数除运算，%表示取模运算，但算术式的表示和我们不同，他们把要计算的数放到前面，运算符放在计算对象的后面，比如 1 2 + 表示 1+2, 1 12 + 3 4 + * 表示(1+12)*(3+4)。考古人员希望你帮助他们编写一个程序，计算出这些计算式的值。输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例。每行为一个字符串（长度不超过200个字符），数和数，数和运算符，运算符和运算符之间分别用一个空格隔开，数都为非负整数，且小于或等于231-1。(所有计算式都符合计算规则，不会出现不可计算的计算式,且结果都为非负整数, 且小于或等于231-1)
输出：
每行输出一个测试用例的结果。使用一个字符串表示计算以后的结果。
Sample Input
2
1 2 +
1 12 + 3 4 + *
Sample Output
3
91
C语言用堆栈实现
\#include<stdio.h>
char str[1000];
int stack[1000];
int strToDigit(char str[])
{
int sum=str[0]-'0';
int i=1;
while(str[i]!='\0')
{
sum=sum*10+(str[i++]-'0');
}
return sum;
}
int main()
{
int k;
int sum ,top=0;
scanf("%d",&k);
while(k--)
{
scanf("%s",str);
sum=strToDigit(str);
stack[++top]=sum;
while(getchar()!='\n')
{
scanf("%s",str);
if(str[0]=='+')
{
stack[top-1]=stack[top-1]+stack[top];
top--;
}
else if(str[0]=='-')
{
stack[top-1]=stack[top-1]-stack[top];
top--;
}
else if(str[0]=='*')
{
stack[top-1]=stack[top-1]*stack[top];
top--;
}
else if(str[0]=='/')
{
stack[top-1]=stack[top-1]/stack[top];
top--;
}
else if(str[0]=='%')
{
stack[top-1]=stack[top-1]%stack[top];
top--;
}
else
{
sum=strToDigit(str);
stack[++top]=sum;
}
}
printf("%ld\n",stack[top]);
}
return 0;
}



