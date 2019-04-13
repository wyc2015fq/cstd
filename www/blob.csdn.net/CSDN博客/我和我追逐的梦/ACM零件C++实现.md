
# ACM零件C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月31日 22:25:03[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1138标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[ini																](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[图形																](https://so.csdn.net/so/search/s.do?q=图形&t=blog)[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=output&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=图形&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=ini&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
零件
Description
有一种工业零件，分成左右两半，它们的形状由’X’和空格表示的二维图形表示，比如：左半的零件形状如下：
XXXXX
XXX
XXXX
XXX
右半的零件的形状如下
XXX
XXXX
XXXX
XXXXX
所有的左半边的零件的左边第一列都是X，所有右半的零件的右边第一列都是X。
把这样的左右两个零件紧密地拼在一起，则可能存在空洞。零件本身也可能有空洞，但不会存在断裂的零件，每个空洞为一个空格，要求你写一个程序求出空洞的大小（空格的数目）。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每一个测试用例占2n+1行。每个测试用例的第一行为一个整数n，（1<=n<=20）,从第2行到n+1行为左半零件的二维图形，从第n+2行到2n+1行为右半零件。左半零件左对齐，最长一行不超过25列，
右半零件右对齐，最长一行不超过25列
输出：每行输出一个测试用例的结果。
Sample Input
2
4
XXXXX
XXX
XXXX
XXX
XXX
XXXX
XXXX
XXXXX
2
XXXXX
X
XXXXX
XXX
Sample Output
1
6
\#include<iostream>
using namespace std;
char a[21][25],b[21][25];
int len[42];
int main()
{
int k,n;
cin>>k;
while(k--)
{
cin>>n;
getchar();
int i,j,sum1=0,sum2=0;
for(i=0;i<n;i++)   gets(a[i]);
for(i=0;i<n;i++)   gets(b[i]);
for(i=0;i<n;i++)
{
sum1=0;sum2=0;
for(j=0;j<strlen(a[i]);j++)
{
if(a[i][j]=='X')
sum1++;
}
for(j=0;j<strlen(b[i]);j++)
{
if(b[i][j]=='X')
sum2++;
}
len[i]=sum1;
len[i+n]=sum2;
}
int max=0;
for(i=0;i<n;i++)
{
if(len[i]+len[i+n]>max)
max=len[i]+len[i+n];
}
int sum=0;
sum=n*max;
for(i=0;i<n;i++)
{
sum=sum-len[i]-len[i+n];
}
cout<<sum<<endl;
}
return 0;
}


[
](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
