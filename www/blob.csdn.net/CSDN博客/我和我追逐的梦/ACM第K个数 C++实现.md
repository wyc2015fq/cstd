
# ACM第K个数 C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月04日 20:04:14[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1614


第K个数
Description
给你一个整数序列和若干个问题，问题是这个序列的第i个元素到第j个元素的片断中的第k大数是什么？比如说给你的序列为（1, 5, 2, 6, 3, 7, 4），问题是（2，5，3），则从第2个元素到第5个元素为（5，2，6，3），排序以后是（2，3，5，6），则第三个数是5。
输入：
第一行为两个整数n，m（1 <= n <= 100 000, 1 <= m <= 5 000），表示序列的元素个数和问题的个数，第二行为n个正整数的序列，每个整数为一个32位整数，两个数之间有一个空格隔开。以后m行为问题，每个问题由三个整数表示i，j，k，第i个元素到第j个元素的片断中的第k大数是什么？（元素序号从1开始计数,i<=j,1<=k<=j-i+1）
输出：
每行输出对应问题的结果。
Sample Input
7 3
1 5 2 6 3 7 4
2 5 3
4 4 1
1 7 3
Sample Output
5
6
3
\#include<iostream>
using namespace std;
typedef struct nodes
{
int value;
int num;
};
nodes node[1000];
void quick_sort(int low,int high)
{
int i,j;
nodes key;
if(low<high)
{
i=low;
j=high;
key=node[low];
while(i<j)
{
while(i<j&&key.value<=node[j].value) j--;
node[i]=node[j];
while(i<j&&key.value>=node[i].value) i++;
node[j]=node[i];
}
node[i]=key;
quick_sort(low,i-1);
quick_sort(i+1,high);
}
}
int main()
{
int n;
cin>>n;
int cases;
cin>>cases;
int i=0;
for(i=0;i<n;i++)
{
cin>>node[i].value;
node[i].num=i+1;
}
quick_sort(0,n-1);
while(cases--)
{
int a,b,c,sum=0;
cin>>a>>b>>c;
for(i=0;i<n;i++)
{
if(a<=node[i].num&&node[i].num<=b)
{
sum++;
if(sum==c)
{
cout<<node[i].value<<endl;
break;
}
}
}
}
return 0;
}


