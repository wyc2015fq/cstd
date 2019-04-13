
# C++基数排序（包含计数排序） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月28日 14:58:40[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1170标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[ini																](https://so.csdn.net/so/search/s.do?q=ini&t=blog)[n2																](https://so.csdn.net/so/search/s.do?q=n2&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=存储&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=n2&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=ini&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
C++基数排序(包含计数排序)1、hanshu.h                                            //自定义头文件
\#define N1 10
\#define N2 1000
struct Node
{
int value;                                                //排序元素值
int subDigit;                                          //排序值除去地位
int digitValue;                                         //每位值，如个位值，十位值
int digitSum;                                        // 此元素的位数
};
int GetMaxDigitNum(const Node *node,const int n);                                 //头文件函数声明
int GetDigitNum(const Node node);
void CountDigitSort(Node *node1,Node *node2,int *count,int n);
2、hanshu.cpp                                                                                                // 自定义源文件，函数实现
\#include"stdafx.h"
\#include"hanShu.h"
\#include<iostream>
using namespace std;
int GetMaxDigitNum(const Node *a,const int n)                                           //得到所有输入的需要排序的元素的最大位数，如1111，四位
{
int num;
int max=0,sum=0;
int i =0;
for(i=0;i<n;i++)
{
num=a[i].value;
sum = 0;
while(num)
{
num/=10;
sum++;
}
if(max<sum)
max=sum;
}
return max;
}
int GetDigitNum(const Node node)                                                                   //得到每个输入元素的位数
{
int num;
int sum=0;
num=node.value;
if(num==0)
sum =1;
while(num)
{
num/=10;
sum++;
}
return sum;
}
void CountDigitSort(Node *node1,Node *node2,int *count,int n)   //计数排序，从个位开始，元素某位没有的就补0，此需要稳定排序
{
int i = 0;
for(i=0;i<N1;i++)
count[i]=0;
for(i=0;i<n;i++)
count[node1[i].digitValue]+=1;
for(i=1;i<N1;i++)
count[i]+=count[i-1];
//for(i=0;i<N1;i++)
//cout<<count[i]<< " ";
//cout<<endl;
for(i=n-1;i>=0;i--)
{
node2[count[node1[i].digitValue]-1] = node1[i];
count[node1[i].digitValue]--;
}
for(i=0;i<n;i++)
cout<<node2[i].value<< " ";
cout<<endl;
}
3、main()                                                                                                                         //main()函数
\#include "stdafx.h"
\#include "hanShu.h"
\#include<iostream>
using namespace std;
Node nodes[N2],sortNodes[N2];                                                                                //nodes[] 存储需要排序的元素，sortNodes[]存储排序后的元素
int count[N1];
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入排序案例的个数："<<endl;
cin>>cases;
while(cases--)
{
cout<<"请输入你需要排序的元素的个数："<<endl;
int n;
cin>>n;
int i,j;
cout<<"请输入你需要排序的元素："<<endl;
for(i=0;i<n;i++)
{
cin>>nodes[i].value;
nodes[i].subDigit = nodes[i].value;
nodes[i].digitSum = GetDigitNum(nodes[i]);
cout<< nodes[i].digitSum  << " ";
}
cout<<endl;
int digitSum;
digitSum = GetMaxDigitNum(nodes,n);
cout<<digitSum<<endl;
for(i=1;i<=digitSum;i++)
{
if(i>1)
{
for(j= 0;j<n;j++)
nodes[j]=sortNodes[j];
}
for(j=0;j<n;j++)
{
if(nodes[j].digitSum>=i)
{
nodes[j].digitValue = nodes[j].subDigit%10;
nodes[j].subDigit/=10;
}
else
nodes[j].digitValue =0 ;
}
//cout<<"K"<<endl;
//for(int k = 0;k<n;k++)
//cout<<nodes[k].digitValue<<" ";
CountDigitSort(nodes,sortNodes,count,n);                                        //分别按个位，十位，百位...排序
}
for(i=0;i<n;i++)                                                                                                //输出排序后的个元素
cout<<sortNodes[i].value<<" ";
cout<<endl;
}
system("pause");
return 0;
}

[
](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
