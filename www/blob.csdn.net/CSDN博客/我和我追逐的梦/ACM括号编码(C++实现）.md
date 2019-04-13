
# ACM括号编码(C++实现） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月30日 22:22:34[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1835


括号编码
Description
S = s1 s2...s2n 是一个符合格式的括号的字符串，S能按下面两种方式编码：
P编码：编码是一个整数序列P = p1 p2...pn，pi是第i个右括号之前的左括号的数目。
W编码：编码是一个整数序列W= p1 p2...pn，wi是第i个右括号的编码值，它等于这个右括号到与之匹配的左括号之间的右括号的数目（包括它自己）。
比如：
S ( ( ( ( ) ( ) ( ) ) ) )
P         4   5   6 6 6 6
W         1   1   1 4 5 6
请写一个程序将P序列转换成W序列。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每两行一个测试用例。每个测试用例第一行为一个整数n（1 <= n <= 20）,表示P序列长度，每个测试用例第二行n个非负整数，每个整数之间有一个空格分隔。
输出：
每行输出一个测试用例的结果。每行包括n个整数，每个整数之间用一个空格分隔。

Sample Input
2
6
4 5 6 6 6 6
9
4 6 6 6 6 8 9 9 9
Sample Output
1 1 1 4 5 6
1 1 2 4 5 1 1 3 9
//
\#include<iostream>
using namespace std;
int a[1000],stack[1000],count[1000];
char str[1010];
int main()
{
int k,n,i,j,s,t,top,sum;
scanf("%d",&k);
while(k--)
{
cin>>n;
s=0;a[0]=0;sum=0;
for(i=1;i<=n;i++)
{
cin>>a[i];
t=a[i]-a[i-1];
for(j=1;j<=t;j++)
{
str[++s]='(';
count[s]=sum;
}
str[++s]=')';
sum++;
count[s]=sum;
}
top=0;
for(i=1;i<s;i++)
{
if(str[i]=='(') stack[++top]=i;
else
cout<<count[i]-count[stack[top--]]<<" ";
}
cout<<count[i]-count[stack[top--]]<<endl;
}
return 0;
}


