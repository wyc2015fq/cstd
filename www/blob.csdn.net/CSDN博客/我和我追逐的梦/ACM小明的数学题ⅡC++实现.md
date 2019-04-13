
# ACM小明的数学题ⅡC++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月31日 23:47:32[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1220


小明的数学题Ⅱ
Description
小明是个小学五年级的学生，为了早点去看自己爱看的卡通，他想快点把作业做完。可是可恶的数学老师今天却布置了一道难题，小明想了很久也不知道该怎么做。你的任务就是帮小明解决掉这道数学题。
题目是这样子的，有一个正整数n（1<=n<200），计算它的阶乘n!。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例，每行有一个整数n。
输出：
每行输出一个测试用例的结果
Sample Input
2
5
20
Sample Output
120
2432902008176640000
\#include<iostream>
\#include<cmath>
using namespace std;
int main()
{
int m,n,i,j,cases;
int a[400],b,d;
scanf("%d",&cases);
while(cases--) {
memset(a,0,sizeof(a));
scanf("%d",&n);
//m=(int)log10(n)*n/6+2;
a[1]=1;
d=0;m=1;
for(i=2;i<=n;i++)
{
for(j=1;j<=m;j++)
{
b=a[j]*i+d;
a[j]=b % 1000000;
d=b/1000000;
}
if (d !=0) {a[j]=d;} m++;
}
for(i=m;i>=1;i--)
if (a[i]==0) continue;
else
{
m=i;
break;
}
printf("%d",a[m]);
for(i=m-1;i>=1;i--)
{
if (a[i]>99999) {printf("%d",a[i]);continue;}
if (a[i]>9999) {printf("0%d",a[i]);continue;}
if (a[i]>999) {printf("00%d",a[i]);continue;}
if (a[i]>99) {printf("000%d",a[i]);continue;}
if (a[i]>9) {printf("0000%d",a[i]);continue;}
printf("00000%d",a[i]);
}
printf("\n");
}
return 0;
}

