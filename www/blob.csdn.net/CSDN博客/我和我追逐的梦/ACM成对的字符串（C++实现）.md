
# ACM成对的字符串（C++实现） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月30日 22:12:57[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1577标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=output&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=测试&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

成对的字符串
Description
有些字符串，如果满足下面的性质，则称为成对的字符串：
a. 所有的字符在字符串中出现偶数次
b. 每一对相同的字符之间不会有出现奇数次的字符
现在给你一些字符串，请判断这些字符串是否为成对的字符串。输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例。每行为一个字符串（长度不超过1000个字符）。
输出：
每行输出一个测试用例的结果。如果是，输出Yes，否则输出No。
Sample Input
2
aAbbAaaabbcc
abcdefghijklmn
Sample Output
Yes
No
\#include<iostream>
using namespace std;
char str[1000];
int cNum[100];
int main()
{
int cas;
cin>>cas;
while(cas--)
{
scanf("%s",str);
int k = strlen(str);
memset(cNum,0,sizeof(cNum));
int i=0;
int sum =0;
if(k%2)
{
cout<<"No"<<endl;
continue;
}
while(i<k)
{
if(cNum[str[i]]==0)
{
cNum[str[i]] = i+1;
}
else
{
if((i-cNum[str[i]])%2==0){sum+=2;cNum[str[i]]=0;}
}
i++;
}
if(sum==k)
cout<<"Yes"<<endl;
else
cout<<"No"<<endl;
}
return 0;
}


