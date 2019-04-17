# 【sscanf】@CQU2014 校赛_A.Algorithm 标程阅读 - CD's Coding - CSDN博客





2014年05月11日 15:04:41[糖果天王](https://me.csdn.net/okcd00)阅读数：613








**ProblemA. Algorithm**

input:algorithm.in output: algorithm.out



简单来说，所谓算法(algorithm)就是定义良好的计算过程，它取一个或一组值作为输入，并产生出一个或一组值作为输出。亦即，算法就是一系列的计算步骤，用来将输入数据转换成输出结果。

我们还可以将算法看作是一种工具，用来解决一个具有良好规格说明的计算问题。有关该问题的表述可以用通用的语言，来规定所需的输入/输出关系。与之对应的算法则描述了一个特定的计算过程，用于实现这一输入/输出关系。

(摘自《算法导论·原书第2版中文版》)

例如，需要设计算法并用程序实现：针对N条给定颜色（红色或蓝色）及长度绳子的输入，输出其能够做出满足如下条件的最长的颜色不断交替的绳环的长度。

1.要求任何时候都不能连接两条相同颜色的绳子。

2.任何两条绳子连接处会有1个单位长度的绳子因互相打结而被消耗掉，可以理解为这两条绳子各自会有0.5个单位长度会因一次连接而被消耗掉。

注意，可能存在一些1个单位长度长的绳子，它们会因为首尾的连接而被彻底消耗掉，这种消耗是被允许的（例即允许使用一条长度为1的蓝色绳子穿插在两条红色绳子之间，即使这条蓝色绳子会被连接所完全消耗掉）。



**Input**

输入第一行一个整数T(T<=50)代表输入数据组数

每组数据第一行一个整数N(1<=N<=1000)，表示绳子数量

每组数据第二行共计N个字符串，分别描述每一条绳子的长度以及颜色。每个字符串分为两个部分，第一部分为一个整数描述长度，第二部分仅为该串最末尾字符，保证该字符仅会是’B’或’R’，B表示该绳为蓝色，R表示该绳为红色。

(输入数据保证，每条绳子的绳长∈[1,100])



**Output**

每组输出一行：首先输出Case #C: (C代表数据编号，从1开始直至T)；紧跟一个空格符后输出其能够做出满足条件的最长的绳环的长度.



**SampleInput**

4

1

5B

4

6R 1B 7R 3B

7

5B 4R 3R 2R 5R 4R 3R

2

20B 20R



**SampleOutput**

Case #1: 0

Case #2: 13

Case #3: 8

Case #4: 38




Source Code：



```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<cstring>
#include<functional>
using namespace std;

#define B 2
#define L 110

#define min(x,y) (((x)<(y))?(x):(y))

int n,ans;
char cs[L];
vector<int> a[B]; //begin(),end() 来进行sort

//loop variables
int _T,i;

int main(){
	freopen("algorithm.in","r",stdin);freopen("algorithm.out","w",stdout);

	int T;scanf("%d",&T);
	for(_T=1;_T<=T;_T++){
		printf("Case #%d: ",_T);

		a[0].clear();a[1].clear();
		scanf("%d",&n);
		for(i=0;i<n;i++){
			scanf("%s",cs);
			int len;sscanf(cs,"%d",&len);
			a[(cs[strlen(cs)-1]=='B')?0:1].push_back(len); //a[0],a[1]存储同色的布条
		}
		
		//sscanf_Usage：
		//scanf("%s",tmpstring);
		//char buf[512];
		//sscanf(tmpstring,"%s",buf); //"%s","%4s","%[^]"(空格结束),
									//"%[1-9 a-z]"(到第一个不符合的为止),
									//"%*[^/]/%[^@]"(/和@之间的字符串) ,
									//"%*s%s"(第一个str不要，只要第二个)
		//printf("%s",buf);
		
		n=min(a[0].size(),a[1].size());
		sort(a[0].begin(),a[0].end(),greater<int>());//sort in vector 
		sort(a[1].begin(),a[1].end(),greater<int>());//another is less<int>
		ans=-n<<1;//产生环需要损耗的长度
		for(i=0;i<n;i++)ans+=a[0][i]+a[1][i];

		printf("%d\n",ans);
	}

	fclose(stdin);fclose(stdout);

	return 0;
}
```







