
# ACM非前缀编码 C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月01日 21:08:56[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1935


非前缀编码
Description
有很多方法可以实现使用2进制序列对字符进行编码，比如典型的Huffman编码，如果在对字符的2进制编码中不存在某一个字符的编码是另一个字符编码的前缀，那么就称这种编码方式为非前缀编码，Huffman编码就是一种非前缀编码。比如 A：00 B：10 C：0100 D：0101 则这种编码为非前缀编码；A:01 B:10 C:010 D:0000，则这种编码为前缀编码。
请写一个程序，判断编码是前缀编码还是非前缀编码。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例。每个测试用例为若干个字符串，字符串之间有空格隔开（最大长度不超过1000）。
输出：
每行输出一个测试用例的结果。如果是非前缀编码输出Yes，否则输出No。

Sample Input
2
01 10 0010 0000
01 10 010 0000
Sample Output
Yes
No

\#include<iostream>
\#include<stdlib.h>
\#define N 1000
using namespace std;
char str[N][N];
int len[N];
int cmp(const void *a,const void *b)
{
return strcmp((char*)a,(char*)b);
}
bool check(int i)
{
int k=len[i],j=0;
while(j<k)
if(str[i][j++]!=str[i+1][j++])
return false;
return true;
}
int main()
{
int k;
cin>>k;
while(k--)
{
scanf("%s",str[0]);
int n=1;
int flag=false;
while(getchar()!='\n')
{
scanf("%s",str[n++]);
}
qsort(str,n,sizeof(str[0]),cmp);
len[0]=strlen(str[0]);
int i=0;
for(i=1;i<n;i++)
{
len[i]=strlen(str[i]);
if(len[i-1]>len[i])
continue;
else
{
if(check(i-1))
{
flag=true;
break;
}
}
}
if(flag)
cout<<"No"<<endl;
else
cout<<"Yes"<<endl;
}
return 0;
}


