# 【归并排序】 Merge_sort 实现源码 （自写） - CD's Coding - CSDN博客





2014年09月29日 16:50:02[糖果天王](https://me.csdn.net/okcd00)阅读数：618








学校算法老师给布置了个归并排序的实现作为作业……

实在闲着无事……请允许吾辈卖个萌做个简单的命令行界面程序，以下为源码。

过程中会有简单的解释语句的输出，所以过程看的还算清楚，用以对(int)1e9以下的正整数的归并排序实现源代码。




废话不多说，贴代码:



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;
typedef long long ll;

//******************************//
//*Author: okcd00 at 2014/09/24*//
//*StuNo.: 20125209			   *//
//*Name	: 陈点 物联网一班	   *//
//* 个人作品，请勿随意盗版哦 ~ *// 
//******************************//

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

ll arr[1024],al[512],ar[512];
string pause;

bool cmp(const int a, const int b){return a > b;}

void welcome()
{
	cout<<"┏══════════════┓"<<endl;
	cout<<"┃Author: okcd00              ┃"<<endl;
	cout<<"┃StuNo.: 20125209            ┃"<<endl;
	cout<<"┃Time  : 2014/09/24          ┃"<<endl;
	cout<<"┃From  : 陈点 物联网一班     ┃"<<endl;
	cout<<"┣──────────────┨"<<endl;
	cout<<"┃ ★个人作品，请勿随意盗版★ ┃"<<endl;
	cout<<"┗══════════════┛"<<endl;
}

void menu()
{
	system("cls");
	cout<<"┏══════════════┓"<<endl 
	    <<"┃Author:  okcd00             ┃"<<endl 
	    <<"┃StuNo.:  20125209           ┃"<<endl 
	    <<"┃Time  :  2014/09/24         ┃"<<endl 
	    <<"┃From  :  陈点 物联网一班    ┃"<<endl 
	    <<"┣──────────────┨"<<endl 
	    <<"┃ 请输入指令，以回车键结束   ┃"<<endl 
	    <<"┃★(s)start: 开始 MergeSort★┃"<<endl 
	    <<"┃★(q)quit : 结束程序并退出★┃"<<endl 
	    <<"┃★%others%: 请重新输入    ★┃"<<endl 
	    <<"┃★个人作品★★请勿随意盗版★┃"<<endl 
	    <<"┗══════════════┛"<<endl;
}

void end()
{
	system("cls");
	cout<<"┏══════════════┓"<<endl
	    <<"┃Author:  okcd00             ┃"<<endl
	    <<"┃StuNo.:  20125209           ┃"<<endl
	    <<"┃Time  :  2014/09/24         ┃"<<endl
	    <<"┃From  :  陈点 物联网一班    ┃"<<endl
	    <<"┣──────────────┨"<<endl
	    <<"┃ ★  程序结束，即将退出  ★ ┃"<<endl
	    <<"┗══════════════┛"<<endl;
}

int mergetime=1;
void get_merge(int l,int r)
{
	if(l+1>=r) return;
	int pos1=0,pos2=0;
	for(int i=l;i<(l+r)/2;i++) al[i-l]=arr[i];
	for(int i=(l+r)/2;i<r;i++) ar[i-(l+r)/2]=arr[i];
	for(int i=l;i<r;i++)
	{
		if(pos2==r-(l+r)/2) arr[i]=al[pos1],pos1++;
		else if(pos1==(l+r)/2-l) arr[i]=ar[pos2],pos2++;
		else if(al[pos1]<=ar[pos2]) arr[i]=al[pos1],pos1++;
		else arr[i]=ar[pos2],pos2++;
	}
	cout<<"[第"<<mergetime++<<"次] getMerge: "<<"lef="<<l<<", rig="<<r<<endl;
	for(int i=l;i<r;i++)	cout<<arr[i]<<"\t";
	cout<<endl<<endl;
}

void do_merge(int l,int r)
{
	int lef=l,rig=r;
	if(lef+1>=rig) return ;
	else 
	{
		do_merge(lef,(lef+rig)/2);
		do_merge((lef+rig)/2,rig);
		get_merge(lef,rig);
	}
}

void merge_sort()
{
	cout<<"—— 请输入你想要排序的数字  ——" <<endl; 
	cout<<"——（以空格分隔，以-1结束） ——" <<endl;
	cout<<"我的数组为(数字个数不超过1K)："; 
	int pos=0;	ll now=0;
	while(1)
	{
		scanf("%I64d",&now);
		if(now!=-1)	arr[pos++]=now;
		else break;
	}
	system("cls");
	cout<<"★以下为我的数组★"<<endl;
	for(int i=0;i<pos;i++)
	{
		if(i!=0 && i%5==0) printf("┃\n");
		printf("┃ %010I64d ",arr[i]);
		//if(arr[i]<(int)1e5) cout<<"\t";
	}
	cout<<endl<<endl<<"——接下来开始进行归并排序——"<<endl<<endl;
	system("pause"); 
	do_merge(0,pos);
	cout<<"——归并排序完毕，输出结果——"<<endl;
	for(int i=0;i<pos;i++)
	{
		if(i!=0 && i%5==0) printf("┃\n");
		printf("┃ %010I64d ",arr[i]);
		//if(arr[i]<(int)1e5) cout<<"\t";
	}
	cout<<endl;
	system("pause"); 
}

int main()
{
	string s;
	welcome();
	system("pause"); 
	while(1)
	{
		menu();	
		cin>>s;		if(s=="quit"|| s=="q"|| s=="Q") break;
		else if(s=="s" || s=="S" ||s=="start") {merge_sort();break;}
	}
	end();
	system("pause"); 
	return 0;
}
```







