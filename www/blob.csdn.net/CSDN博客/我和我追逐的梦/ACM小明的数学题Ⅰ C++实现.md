
# ACM小明的数学题Ⅰ C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月31日 23:32:22[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1273标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[n2																](https://so.csdn.net/so/search/s.do?q=n2&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[作业																](https://so.csdn.net/so/search/s.do?q=作业&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=input&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=n2&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=n2&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=n2&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=output&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
小明的数学题Ⅰ
Description
小明是个小学五年级的学生，为了早点去看自己爱看的卡通，他想快点把作业做完。可是可恶的数学老师今天却布置了一道难题，小明想了很久也不知道该怎么做。你的任务就是帮小明解决掉这道数学题。
题目是这样子的，有一个整数a（-2^31<= a < 2^31-1），计算它的整数幂a^n，其中1<=n<=99。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例，每行有两个整数a，n。
输出：
每行输出一个测试用例的结果
Sample Input
2
3 5
-2 5
Sample Output
243
-32
\#include<iostream>
using namespace std;
__int64 a[1010],b[1010],s[30],knum,e,i1,i2,i;
void cal(int n1,int n2,__int64 s1[],__int64 s2[])
{
memset(a,0,sizeof(a));
for(i1=0;i1<n1;i1++)
{
for(i2=0;i2<n2;i2++)
{
i=i1+i2;
a[i]=a[i]+s1[i1]*s2[i2];
if (i>e) e=i;
}
for(i=0;i<=e+10;i++)  {a[i+1]+=a[i]/10;a[i]%=10; }
} e+=10;
for(i=e;i>=0;i--) if(a[i]==0) continue; else { e=i; break; }
}
void change(__int64 k)
{
i=0;
while(k>0) { s[i]=k%10; k/=10; i++; }
knum=i;
}
int main()
{
int cases,n;
__int64 k,t;
scanf("%d",&cases);
while(cases--)
{
scanf("%I64d%d",&k,&n);
if(k==0) { printf("0\n");continue; }
if(n==1) { printf("%I64d\n",k);continue; }
if(k<0 && (n%2==1) ) printf("-");
if(k<0) k=-k;
memset(s,0,sizeof(s));
memset(a,0,sizeof(a));
change(k);
for(i=0;i<knum;i++) a[i]=s[i];t=n-1;e=knum-1;
while(t>0)
{
t--;
for(i=0;i<=e;i++) b[i]=a[i];
cal(knum,e+1,s,b);
}
for(i=e;i>=0;i--) printf("%I64d",a[i]);
cout<<endl;
}
return 0;
}

[
](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
