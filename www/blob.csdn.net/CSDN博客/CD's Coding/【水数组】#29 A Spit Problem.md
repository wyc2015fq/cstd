# 【水数组】#29 A. Spit Problem - CD's Coding - CSDN博客





2014年07月06日 16:17:14[糖果天王](https://me.csdn.net/okcd00)阅读数：733
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















In a Berland's zoo there is an enclosure with camels. It is known that camels like to spit. Bob watched these interesting animals for the whole day and registered in his notepad where each animal spitted. Now he wants to know if in the zoo there are two camels,
 which spitted at each other. Help him to solve this task.


The trajectory of a camel's spit is an arc, i.e. if the camel in position *x* spits *d* meters
 right, he can hit only the camel in position *x* + *d*, if such a camel exists.




Input


The first line contains integer *n* (1 ≤ *n* ≤ 100) —
 the amount of camels in the zoo. Each of the following *n* lines contains two integers *x**i*and *d**i* ( - 104 ≤ *x**i* ≤ 104, 1 ≤ |*d**i*| ≤ 2·104)
 — records in Bob's notepad. *x**i* is
 a position of the *i*-th camel, and *d**i* is
 a distance at which the *i*-th camel spitted. Positive values of *d**i* correspond
 to the spits right, negative values correspond to the spits left. No two camels may stand in the same position.




Output


If there are two camels, which spitted at each other, output YES. Otherwise, output NO.




Sample test(s)




input
2
0 1
1 -1




output
YES




input
3
0 1
1 1
2 -2




output
NO




input
5
2 -10
3 10
0 5
5 -5
10 1




output
YES
















数组越界类的一个问题，RUNTIME ERROR 或者ACCESS什么的一般是这种问题 

起初，写了个这个来枚举题意——然后不出所料RuntimeERROR了……



```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
#define FLT  10000
#define MAXN 20001
#define INF -99999
using namespace std;  
int Spt[MAXN];

int main()
{
	int n;
	bool flag=false;
	memset(Spt,INF,sizeof Spt);
	//memset(Who,0,sizeof Who);
	scanf("%d",&n);
	for(int ncnt=1;ncnt<=n;ncnt++)
	{
		int res=0,ran=0;
		scanf("%d%d",&res,&ran);
		Spt[res+FLT]=res+ran;
		if(Spt[res+ran+FLT]!=INF&&Spt[res+ran+FLT]==res)flag=true;
	}
	printf(flag?"YES":"NO");
	return 0;
}
```





然后我就在想：*d**i* ( - 104 ≤ *x**i* ≤ 104, 1 ≤ |*d**i*| ≤ 2·104) 

骆驼可能站在-10000~10000，但是他们的口水可以吐到-30000~30000……

然后为了试验这个题意理解清楚没就更改DEFINE的数值来检验：



```cpp
#define FLT   30000
#define MAXN  60001
```
然后就AC了你敢信么！



叹一口气……



```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
#define FLT   30000
#define MAXN  60001
#define INF -129999
using namespace std;  
int n,res,ran,Spt[MAXN];

int main()
{
	bool flag=false;
	memset(Spt,INF,sizeof Spt);
	scanf("%d",&n);
	for(int ncnt=1;ncnt<=n;ncnt++)
	{
		scanf("%d%d",&res,&ran);
		Spt[res+FLT]=res+ran;
		if(Spt[res+ran+FLT]!=INF&&Spt[res+ran+FLT]==res)flag=true;
	}
	printf(flag?"YES":"NO");
	return 0;
}
```


 想了想，这道题用俩数组应该也成，不知道有没有这种说法~姑且叫做【双数组法】好啦~



```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
//#define spt(a) a>0?SptP[a]:SptN[abs(a)] 此句不可用，会出现spt(a)为很大的数的情况 
#define MAXN 30001
#define INF	-99999
using namespace std;  
int n,res,ran,des,SptP[MAXN],SptN[MAXN];

int main()
{
	bool flag=false;
	memset(SptP,INF,sizeof SptP);
	memset(SptN,INF,sizeof SptN);
	scanf("%d",&n);
	for(int ncnt=1;ncnt<=n;ncnt++)
	{
		scanf("%d%d",&res,&ran);
		des=res+ran;
		if(res>0)	SptP[res]=des;
		else 		SptN[abs(res)]=des;
		if(des>0)
		{
			if(SptP[des]!=INF&&SptP[des]==res)flag=true;
		}
		else if(SptN[abs(des)]!=INF && SptN[abs(des)]==res)flag=true;		
	}
	printf(flag?"YES":"NO");
	return 0;
}
```







