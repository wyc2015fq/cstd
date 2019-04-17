# 【内存管理】单文件CPP清晰模拟实现4种内存管理 - CD's Coding - CSDN博客





2014年12月21日 20:36:11[糖果天王](https://me.csdn.net/okcd00)阅读数：519标签：[内存管理																[操作系统																[控制台																[linux																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[作业](https://blog.csdn.net/okcd00/article/category/1727537)





## 内存中的进程

•内存一部分供操作系统使用（驻留监控程序、内核），一部分供当前活跃的进程使用，用于存放进程需要的数据和程序等。

•内存管理最基本的操作是由处理器把程序装入内存中执行。





### •进程对应的内存空间包含5种不同的数据区：


代码段（Text）：代码段是用来存放可执行文件的操作指令，也就是说是它是可执行程序在内存中的镜像。

数据段（Data）：数据段存放可执行文件中已初始化全局变量，包括程序静态(static)分配变量和全局变量(global)。

BSS段：BSS段包含了程序中未初始化的全局变量和静态变量，在内存中 bss段全部置零。

堆（Heap）：堆是用于存放进程运行中被动态分配(malloc)的内存段。

    栈（Stack）：栈是用户存放程序临时创建的局部变量。除此以外，在函数被调用时，其参数也会被压入发起调用的进程栈中，并且待到调用结束后，函数的返回值也会被存放回栈中。






### 在linux环境下编写程序，模拟一个作业的执行过程。

•用户输入系统分配给该作业的物理块N，和该作业要访问的逻辑页号序列长度L。

•该作业要访问的逻辑页号序列可以用户输入，也可以采用某种策略生成，如随机。

•采用下面不同的页面置换算法，并给出不同算法下的页面置换情况及其对应的缺页率。

•FIFO--先进先出，置换驻留在内存中时间最长的页。

•LRU--最近最少使用，置换内存中上次使用据当前最远的页。

•OPT--最佳置换，未来访问据当前时间最长的页。

•CLOCK--时钟算法，页框关联使用位。






为了好看（我为啥每次都在这种意义不明的事情上那么麻烦……）做了类命令行的操作界面，明明就是个控制台至于么……

每个函数和用户友好表达都封装成函数，自认为写起来还是比较清晰明朗的（都划分这么细了了好不好）

纯原创，请勿随意转载使用，如需使用请告知或署名okcd00，谢谢。

### Code:


```cpp
#include <cmath> 
#include <time.h>
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <iostream>
#include <algorithm>
using namespace std;
int b[16];		//block
int mem[256];	//memory
string choice;	//choice
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
int n,l;
bool cmp(const int a, const int b)
{
	return a > b;
}

void sta()	//start
{
	cout<<"<Project4> = 20125209 IOT01 Chendian_陈点"<<endl; 
	cout<<"Please input the number of Physic Blocks (Less than 15):";	cin>>n;
	cout<<"Please input the Length of Sequence Length (Less than 255):";cin>>l;
}

void cus()	//custom
{
	cout<<"==Custom Setting=="<<endl;
	cout<<"Please Input "<<l<<" digits :(split with blank)"<<endl;
	for(int i=0;i<l;i++)
	{
		cin>>mem[i];
	}
	cout<<"\n Custom_Set Finished"<<endl;
}

void ran()	//random
{
	srand(time(NULL));
	cout<<"==Random Setting=="<<endl;
	cout<<"Now Calculating";
	cout<<endl;
	for(int i=0;i<l;i++)
	{
		mem[i]=rand()%8+1;
		cout<<".";
	}
	cout<<"\n Random_Set Finished"<<endl;
}

void inp()	//input
{
	cout<<"Do you want CUSTOM or RANDOM?(c/r):"<<endl;
	memset(mem,0,sizeof mem);
	while(1)
	{
		cin>>choice;
		if(tolower(choice[0])=='c') {cus();break;}
		if(tolower(choice[0])=='r') {ran();break;}
		//7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
		cout<<"Invalid Input , Please Try again:";
	} 
}

void dis()	//display
{
	cout<<endl	<<"@ FIFO -- 先进先出，置换驻留在内存中时间最长的页"<<endl
				<<"@ LRU  -- 最近最少使用，置换内存中上次使用据当前最远的页"<<endl
				<<"@ OPT  -- 最佳置换，未来访问据当前时间最长的页"<<endl
				<<"@ CLOCK-- 时钟算法，页框关联使用位"<<endl
				<<"@ Exit -- 退出					"<<endl
				<<"@ Another Sequence -- 更换序列	"<<endl
				<<"Please Select Replacing Algorithm(输入首字母即可):"<<endl;
} 

void fif()	//fifo
{
	int pos=0;
	int cnt=0;
	memset(b,-1,sizeof b);
	for(int i=0;i<l;i++)
	{
		int now=mem[i],f=0;
		for(int j=0;j<n;j++) if(b[j]==now) { f=1;break;}
		if(f)
		{
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[命中]\t New is %d\n",now);
		}
		else
		{
			b[pos++]=now;
			pos=pos%n;
			cnt++;
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[缺页]\t New is %d\n",now);
		} 
	}
	cout<<"缺页数:"<<cnt<<endl;
	cout<<"缺页率:"<<(double)cnt/(double)l *100.0 <<"%"<<endl;
}

void lru()	//lru
{
	int maxdis=-1;
	int chg=0,cnt=0;
	int rec[10]={0};	//record
	int dis[10]={0};	//distance
	memset(b,-1,sizeof b);
	for(int i=0;i<l;i++)
	{
		maxdis=-1;
		int now=mem[i],f=0;
		for(int j=0;j<n;j++)
		{
			if(b[j]==now) 	{f=1;break;}
			if(b[j]==-1) 	{chg=j;break;}
			dis[b[j]]=i-rec[b[j]];
			//cout<<"dis["<<j<<"]:"<<dis[b[j]]<<endl;
			if(dis[b[j]]>maxdis) 	 maxdis=dis[b[j]],chg=j;
		}
		if(f)
		{
			rec[now]=i;
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[命中]\t New is %d\n",now);
		}
		else
		{
			b[chg]=now,	cnt++;
			rec[now]=i;
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[缺页]\t New is %d\n",now);
		} 
	}
	cout<<"缺页数:"<<cnt<<endl;
	cout<<"缺页率:"<<(double)cnt/(double)l *100.0 <<"%"<<endl;
}

void opt()	//opt
{
	int maxdis=-1;
	int cnt=0;
	int dis[10]={0};	//distance
	memset(b,-1,sizeof b);
	for(int i=0;i<l;i++)
	{
		int pos=0;
		int chg=0;
		maxdis=-1;
		int now=mem[i],f=0;
		for(int j=0;j<n;j++)
		{
			if(b[j]==now) 	{f=1;break;}
			if(b[j]==-1) 	{chg=j;break;}
			dis[b[j]]=0;
			for(int k=i+1;k<l;k++)
			{
				if(mem[k]==b[j])
				{
					dis[b[j]]=k-i;
					break;
				}
			}
			//cout<<"dis["<<j<<"]:"<<dis[b[j]]<<endl;
			if(dis[b[j]]>maxdis) 	 maxdis=dis[b[j]],chg=j;
		}
		if(f)
		{
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[命中]\t New is %d\n",now);
		}
		else
		{
			b[chg]=now,	cnt++;
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[缺页]\t New is %d\n",now);
		} 
	}
	cout<<"缺页数:"<<cnt<<endl;
	cout<<"缺页率:"<<(double)cnt/(double)l *100.0 <<"%"<<endl;	
}

void clo()	//clock
{
	int cnt=0;
	int pos=0;
	int clk[10]={0};	//clock
	int dis[10]={0};	//distance
	memset(b,-1,sizeof b);
	for(int i=0;i<l;i++)
	{
		int chg=0;
		int now=mem[i],f=0;
		while(1)
		{
			if(b[pos]==now)
			{
				f=1;
				clk[pos]=1;
				break;
			}
			if(b[pos]==-1 || clk[pos]==0)
			{
				f=0;
				chg=pos;
				clk[pos]=1;
				break;
			}
			clk[pos]=0;
			pos++;
			pos=pos%n;
		}
		if(f==1)
		{
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[命中]\t New is %d\n",now);
		}
		else if(f==0)
		{
			b[chg]=now,	cnt++;
			for(int j=0;j<n;j++) if(b[j]==-1)printf("# ");else printf("%d ",b[j]);
			printf("[缺页]\t New is %d\n",now);
		} 
	}
	cout<<"缺页数:"<<cnt<<endl;
	cout<<"缺页率:"<<(double)cnt/(double)l *100.0 <<"%"<<endl;		
}

int main()
{
	sta();
	inp();
	cout<<"==Sequence Setting Finished=="<<endl; 
	cout<<"Your Sequence is:"<<endl;
	for(int i=0;i<l;i++) cout<<mem[i]<<"\t";
	while(1)
	{
		dis();
		cin>>choice;
		memset(b,0,sizeof b);
		if(tolower(choice[0])=='f') fif();
		else if(tolower(choice[0])=='l') lru();
		else if(tolower(choice[0])=='o') opt();
		else if(tolower(choice[0])=='c') clo();
		else if(tolower(choice[0])=='a') inp(); 
		else if(tolower(choice[0])=='e') break;
		else cout<<"Invalid Input , Please Try again:";
	} 
	cout<<"Thanks for Using"<<endl
		<<"\t__Author IOT Chendian 20125209"<<endl;
	return 0;
}
```

### 实际运行代码测试
![](https://img-blog.csdn.net/20141221230506645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141221230510015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141221230504968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141221230508234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141221230524632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141221230528937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141221230532291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 粗略的评析结果

FIFO算法较易实现，对具有线性顺序特征的程序比较适用，而对具有其他特征的程序则效率不高，此算法还可能出现抖动现象，当序列为周期为N的重复子序列时效率最低。

LRU算法基于程序的局部性原理，所以应该适用大多数程序，此算法实现需要维护淘汰队列。

OPT算法虽然产生的缺页数最少，然而，却需要预测程序的页面引用串，大多数情况下这是无法预知的，不可能对程序的运行过程做出精确的断言，不过此理论算法可用作衡量各种具体算法的标准。

时钟算法的话，有点像是改进了的FIFO算法，](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=控制台&t=blog)](https://so.csdn.net/so/search/s.do?q=操作系统&t=blog)](https://so.csdn.net/so/search/s.do?q=内存管理&t=blog)




