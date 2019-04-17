# 【sprintf+数论】@CQU2014 校赛_C.Count 标程阅读 - CD's Coding - CSDN博客





2014年05月11日 16:14:38[糖果天王](https://me.csdn.net/okcd00)阅读数：612








**ProblemC. Count**

input:count.in output: count.out



计数是一个重复加(或减)1的数学行为，通常用于算出物件有多少个或放置想要之数目个物件(对第一个物件从一算起且将剩下的物件和由二开始的自然数做一对一对应)。此外，计数亦可以被(主要是被儿童)使用来学习数字名称和数字系统的知识。

由现今的考古证据可以推测人类计数的历史至少有五万年，并由此发展导致出数学符号及记数系统的发展。古代文化主要使用计数在记录如负债和资本等经济资料(即会计)。

(摘自Wikipedia)

统计整数1至N中，数字’0’至’9’各自出现了多少次。



**Input**

输入第一行一个整数T(T<=10)代表输入数据组数

每组输入数据仅一行一个整数（1<=N<=1,000,000,000）



**Output**

对于每组输入输出一组结果：

每组输出第一行为Case #C:

(C代表数据编号，从1开始直至T，:后面没有空格，直接换行)

每组输出第二行共计10个整数，分别表示数字’0’至’9’各自出现了多少次。



**SampleInput**

3

7

11

999



**SampleOutput**

Case#1:

0 1 11 1 1 1 1 0 0

Case#2:

1 4 11 1 1 1 1 1 1

Case#3:

189300 300 300 300 300 300 300 300 300



**Hint**

注意时间限制, 朴素算法会导致超时




Source Code：



```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<algorithm>
using namespace std;

#define L 21

int n,m,c,ans[L];
char rul[L];

//loop variable
int i,k,_T;

int main(){
    
    freopen("count.in","r",stdin);freopen("count.out","w",stdout);
    
    int T;scanf("%d",&T);
    for(_T=1;_T<=T;_T++){
        printf("Case #%d:\n",_T);
        
		scanf("%d",&n);
		sprintf(rul,"%d",n);		//将n读入char型的rul数组中去
		
		//Usage_sprintf :  int sprintf ( char * str, const char * format, ... );
		//char buffer [50];
		//int n, a=5, b=3;
		//n=sprintf (buffer, "%d plus %d is %d", a, b, a+b);
		//printf ("[%s] is a string %d chars long\n",buffer,n);
		
		m=strlen(rul)-1;
		c=1;
		for(i=0;i<m;i++)c*=10;	//c为11..11,共m个1

		memset(ans,0,sizeof(ans));	//初始化 0-9 的个数存储数组
		for(int k=m;k>=0;k--)
		{
			int r=n/c;								//r为当前最高位
			for(i=0;i<10;i++)ans[i]+=r*k*(c/10);	//0-9的个数均增加r*k*c/10;
			for(i=0;i<r ;i++)ans[i]+=c;				//0~r-1 的个数均增加c个。
			ans[r]+=n%c+1;							//r的个数增加<当前数去掉最高位+1>个（从000到当前abc共abc+1个）
			n%=c;									//去掉最高位，即n=n%c;
			c/=10;									//降一位继续
		}

		c=1;
		for(i=0;i<=m;i++)			//将ans[0]减去11..11，共m+1个1。
		{
			ans[0]-=c;
			c*=10;
		}

		for(i=0;i<10;i++)			//输出结果
		{
			if(i!=0)printf(" ");
			printf("%d",ans[i]);
		}
		printf("\n");
        
    }
    
    fclose(stdin);fclose(stdout);
    
    return 0;
}
```





