
# ACM节约每一个字节 C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月01日 21:10:55[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1024标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[ini																](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=input&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=output&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=output&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=output&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
节约每一个字节
Description
John在做一个项目，项目对存储容量有着近乎苛刻的要求，为此John需要对一些东西进行压缩存储。John的第一个问题就是一大堆的字符串，存储它们太占地方了，为此他想了一个办法：如果字符串具有相同的后缀，那么就把这么字符串的相同后缀和在一起，这样就能节约一点空间了。比如说有两个字符串分别为“Programming”和“Something”,这样它们有相同的后缀ing,这时候就能省去三个字母了。请写一个程序，计算John这样做能够省去多少个字母？
输入：
第一行是一个整数K，表示有多少个测试用例，以后每个测试用例占n+1行。每个测试用例的第一行为一个整数n（1<=n<=20），从第二行开始依次为n个字符串,字符串由英文字母组成，大小写敏感。
输出：
每行输出一个测试用例的结果。输出总共节省了多少个字母。

Sample Input
2
2
Programming
Something
3
John
AJohn
BJehn
Sample Output
3
6
\#include<iostream>
using namespace std;
char str[20][1000];
int main()
{
int k;
cin>>k;
while(k--)
{
int n;
cin>>n;
int i,j,k1,k2;
for(i=0;i<n;i++)
{
scanf("%s",str[i]);
}
int sum=0,max=0,num=0;
for(i=0;i<n-1;i++)
{
max=0;
for(j=i+1;j<n;j++)
{
num=0;
k1=strlen(str[i])-1;
k2=strlen(str[j])-1;
while(k1>=0&&k2>=0)
{
if(str[i][k1--]==str[j][k2--]) { num++;}
else break;
}
if(max<num)
max=num;
}
sum+=max;
}
cout<<sum<<endl;
}
return 0;
}


[
](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
