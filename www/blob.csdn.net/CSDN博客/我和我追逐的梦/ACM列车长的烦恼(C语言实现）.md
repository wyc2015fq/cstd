
# ACM列车长的烦恼(C语言实现） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月30日 22:05:54[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2499


John是个小列车站的站长，每次列车在这里重新编组时他就很烦恼。因为站上只有一个人字形的编组轨道(如图)，所有的列车车厢都是从人字轨的左边依次进去，从右边出来。但有一些编组顺序John总编不出来，John怀疑有些编组顺序是不可能完成的，可John又找不出那些是顺序是可以编组出，那些不可以。请你写一个程序帮助John辨别哪些编组可以完成，哪些不能完成。输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例，每行为n+1个整数，第一个整数为n 表示有多少节车厢，后面n个整数表示需要编组成的顺序。比如说3节车厢，按照1，2，3依次入轨编组，可以在右边形成1 2 3，1 3 2，2 1 3，2 3 1，321。
输出：
每行输出一个测试用例的结果。如果可以编组输出Yes，否则输出No。
Sample Input
2
3 3 1 2
4 1 2 3 4
Sample Output
No
Yes
//c语言用堆栈实现
\#include<stdio.h>
int train[200],stack[200];
int main()
{
int k,i,mem,top,num;
int n;
scanf("%d",&k);
while(k--)
{
scanf("%d",&n);
i = 1;
while(i<=n)
{
scanf("%d",&train[i]);
i++;
}
top=0;
mem=1;
num=1;
stack[0]=-1;
while(mem<n+1)
{
stack[++top]=mem++;
while(stack[top]==train[num])
{
top--;
num++;
}
}
if(top==0)
printf("Yes\n");
else
printf("No\n");
}
return 0;
}


