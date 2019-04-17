# 【dp+动态分配二维数组+模逆元】CQU_QUALIFICATION FOR SICHUAN CONTEST 题解 - CD's Coding - CSDN博客





2014年06月03日 01:00:20[糖果天王](https://me.csdn.net/okcd00)阅读数：969











HUST的比赛地址大约是这里：http://vjudge.net/contest/view.action?cid=47447#overview


这次呢，是有个什么四川省赛的选拔，挑3个队伍去四川比赛（每个人最终得自费644元Q^Q好贵的说），现场赛好热的说……不吐槽了，写个我写出来的题目的题解好了

写出来的是A、C、D、E、G五道——







A题：[FZU 2147](http://acm.fzu.edu.cn/problem.php?pid=2147)






A - A-B Game
**Time Limit:**1000MS     **Memory Limit:**32768KB     **64bit IO Format:**%I64d
 & %I64u
Submit[Status](http://vjudge.net/contest/view.action?cid=47447#status//A/0)[Practice](http://vjudge.net/problem/viewProblem.action?id=48689)[FZU
 2147](http://acm.fzu.edu.cn/problem.php?pid=2147)





Description


Fat brother and Maze are playing a kind of special (hentai) game by two integers A and B. First Fat brother write an integer A on a white paper and then Maze start to change this integer. Every time Maze can select an integer x between 1 and A-1 then change
 A into A-(A%x). The game ends when this integer is less than or equals to B. Here is the problem, at least how many times Maze needs to perform to end this special (hentai) game.





Input


The first line of the date is an integer T, which is the number of the text cases.

Then T cases follow, each case contains two integers A and B described above.

1 <= T <=100, 2 <= B < A < 100861008610086





Output


For each case, output the case number first, and then output an integer describes the number of times Maze needs to perform. See the sample input and output for more details.





Sample Input

2
5 3
10086 110




Sample Output

Case 1: 1
Case 2: 7













这道题坑尽了万千少年……吾辈用5个WA和60分钟的绳命为一同参加选拔的孩纸们开辟了道路……少年们，这道题如果用scanf必须用%I64d啊，要不就用cin，cout…… 叹气，坑死人不偿命……

既然要求A-(A%x)最小，那就是A%x最大咯，最大是多少呢~ 偶数的话就是一半加一，奇数就是减一的一半咯~


```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
long long  a, b;

void solve(long long a, long long  b)
{
    int cnt = 0;
    while (a>b)
    {
        a -= ((a - 1) / 2);
        cnt++;
    }
    printf("%d\n", cnt);
}

int main()
{
	//freopen("in.txt","r",stdin);
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
        scanf("%I64d%I64d\n", &a, &b);
        printf("Case %d: ", _case);
        solve(a, b);
	}
	return 0;
}
```





C题：[FZU 2151](http://acm.fzu.edu.cn/problem.php?pid=2151)






C - OOXX Game
**Time Limit:**1000MS     **Memory Limit:**32768KB     **64bit IO Format:**%I64d
 & %I64u
Submit[Status](http://vjudge.net/contest/view.action?cid=47447#status//C/0)[Practice](http://vjudge.net/problem/viewProblem.action?id=48790)[FZU
 2151](http://acm.fzu.edu.cn/problem.php?pid=2151)





Description


Fat brother and Maze are playing a kind of special (hentai) game on an N*M board (N rows, M columns). At the beginning, there are N*M coins in this board with two symbol “O” or “X”. Then they take turns to choose a grid with symbol “O” and change it into
 “X”. The game ends when all the symbols in the board are “X”, and the one who cannot play in his (her) turns loses the game. Fat brother and Maze like this kind of OOXX game very much and play it day and night. They don’t even need a little rest after each
 game!

Here's the problem: Who will win the game if both use the best strategy? You can assume that Maze always goes first.





Input


The first line of the date is an integer T, which is the number of the text cases.

Then T cases follow, each case contains two integers N and M indicate the size of the board. Then goes N line, each line with M character shows the state of the board.

1 <= T <=100, 1 <= n <=100, 1 <= m <=100





Output


For each case, output the case number first, and then output the winner’s name, either Fat brother or Maze. See the sample input and output for more details.





Sample Input

3
1 4
OXXX
2 4
OOXX
OOXX
1 2
XX




Sample Output

Case 1: Maze
Case 2: Fat brother
Case 3: Fat brother











读入%c的时候回车永远是WA的预备军……




这个看似博弈，但一人只能翻一个啊！！哪来的最优情况！其实是看奇偶数的问题……






```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int cmp(const void *a, const void *b)
{
	return(*(int *)a-*(int *)b);
}

char map[101][101];

int main()
{
	//freopen("in.txt","r",stdin);
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		printf("Case %d:",_case);
		int n=0,m=0,tol=0;
		char now,huiche;
		scanf("%d%d",&n,&m);
//又被回车给害了…… 
/***************************************
		for(int i=0;i<n*m;i++)
		{
			scanf("%c",&now);
			if(now=='O')tol++;
		}
***************************************/ 
		for(int i=0;i<n;i++)
		{
			scanf("%c",&huiche); 
			for(int j=0;j<m;j++)
			{
				scanf("%c",&now);
				if(now=='O')tol++;
			}
		} 
		cout<< (tol%2==0?" Fat brother":" Maze");
		printf("\n");
	}
	return 0;
}
```







D题：[FZU 2146](http://acm.fzu.edu.cn/problem.php?pid=2146)





D - Easy Game
**Time Limit:**1000MS     **Memory Limit:**32768KB     **64bit IO Format:**%I64d
 & %I64u
Submit[Status](http://vjudge.net/contest/view.action?cid=47447#status//D/0)[Practice](http://vjudge.net/problem/viewProblem.action?id=48688)[FZU
 2146](http://acm.fzu.edu.cn/problem.php?pid=2146)





Description


Fat brother and Maze are playing a kind of special (hentai) game on a string S. Now they would like to count the length of this string. But as both Fat brother and Maze are programmers, they can recognize only two numbers 0 and 1. So instead of judging the
 length of this string, they decide to judge weather this number is even.





Input


The first line of the date is an integer T, which is the number of the text cases.

Then T cases follow, each case contains a line describe the string S in the treasure map. Not that S only contains lower case letters.

1 <= T <= 100, the length of the string is less than 10086





Output


For each case, output the case number first, and then output “Odd” if the length of S is odd, otherwise just output “Even”.





Sample Input

4
well
thisisthesimplest
problem
inthiscontest




Sample Output

Case 1: Even
Case 2: Odd
Case 3: Odd
Case 4: Odd










这道题是我拿到的FB，看到水题之后过于激动……我居然……我居然在Case %d前加了个#号啊啊啊啊啊！！！一个WA吃的我都不想玩了……



这居然也是道奇偶题…… 字符串长度奇偶…… 简称：签到题



```cpp
#include <cmath> 
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

char str[11000];

int main()
{
	//freopen("in.txt","r",stdin);
	int cases=0;
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		printf("Case %d: ",_case);
		scanf("%s",str);
		if(strlen(str)%2==0)printf("Even\n");
		else printf("Odd\n");
	}
	return 0;
}
```



E题：[CSU 1363](http://acm.csu.edu.cn/OnlineJudge/problem.php?id=1363) （其实我想问CSU是啥……毕竟E~G都中途无限JE了，也进不去的样子……）






E - Count 101
**Time Limit:**1000MS     **Memory Limit:**131072KB     **64bit IO Format:**%lld & %llu
Submit[Status](http://vjudge.net/contest/view.action?cid=47447#status//E/0)[Practice](http://vjudge.net/problem/viewProblem.action?id=49934)[CSU
 1363](http://acm.csu.edu.cn/OnlineJudge/problem.php?id=1363)





Description



You know YaoYao is fond of his chains. He has a lot of chains and each chain has n diamonds on it. There are two kinds of diamonds, labeled 0 and 1. We can write down the label of diamonds on a chain. So each chain can be
 written as a sequence consisting of 0 and 1.

We know that chains are different with each other. And their length is exactly n. And what’s more, each chain sequence doesn’t contain “101” as a substring. 

Could you tell how many chains will YaoYao have at most?







Input



There will be multiple test cases in a test data. For each test case, there is only one number n(n<10000). The end of the input is indicated by a -1, which should not be processed as a case.







Output



For each test case, only one line with a number indicating the total number of chains YaoYao can have at most of length n. The answer should be print after module 9997.







Sample Input

3
4
-1




Sample Output

7
12




Hint





We can see when the length equals to 4. We can have those chains:


0000,0001,0010,0011


0100,0110,0111,1000


1001,1100,1110,1111













这道题的意思呢，是说长度为N的不出现101序列的1/0串有多少种。



很明显的这是一道DP的题目，当我们想知道长度为i的有多少种的时候，我们需要事先知道什么呢？

1、长度为i-1的有多少种

2、长度为i-2的有多少种

为啥呢？大家想想看哦~

(1)如果知道长度为i-1的有多少种了，那是不是在这些种类后面加上一个0，都是满足条件的长度为i的情况呢~^_^ 

(2)那加上1呢？嗯，也有很多可以的呢，但是也有不可以的哦，如果加1之前倒数第二位是1，倒数第一位是0，我们就不能加1了呢，那么该怎么处理好呢~

——很简单，如果倒数第二位是1的话，那么一定会存在和它一样多数量的倒数二位为“10”的情况，原因？见（1）

说到这就很明了了，我们把每一种长度的最后一位为0还是1分开存储，比如：定义 int dp[i][2];，i是长度，dp[i][0]是长度为i的尾为0的串的种数，dp[i][1]是长度为i的尾为1的串的种数，根据上面说的，那么动态转移方程就是：



```cpp
dp[i][0]=dp[i-1][0]+dp[i-1][1];
dp[i][1]=dp[i][0]-dp[i-2][1];
```




代码如下：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

const int lim=9997;
int dp[10002][2];//0-0 1-1

void dp_init()
{
	dp[1][0]=1,dp[1][1]=1;
	dp[2][0]=2,dp[2][1]=2;
	dp[3][0]=4,dp[3][1]=3;
	dp[4][0]=7,dp[4][1]=5;
	for(int i=5;i<10001;i++)
	{
		dp[i][0]=dp[i-1][0]+dp[i-1][1];
		dp[i][1]=dp[i][0]-dp[i-2][1];
		dp[i][0]=dp[i][0]%lim;
		dp[i][1]=dp[i][1]%lim;
	}
}

void solve(int num)
{
	int ans=dp[num][0]+dp[num][1];
	cout<< (ans%lim+lim)%lim <<endl;
}

int main()
{
	memset(dp,0,sizeof(dp));
	dp_init();
	while(1)
	{
		int num=0;
		scanf("%d",&num);
		if(num<0)return 0;
		else solve(num);
	}
	return 0;
}
```



G题：






G - Good Serial Inc.
**Time Limit:**1000MS     **Memory Limit:**131072KB     **64bit IO Format:**%lld
 & %llu
Submit[Status](http://vjudge.net/contest/view.action?cid=47447#status//G/0)[Practice](http://vjudge.net/problem/viewProblem.action?id=49931)[CSU
 1360](http://acm.csu.edu.cn/OnlineJudge/problem.php?id=1360)





Description



Good Serial Inc. (GSI) produces integer sequences with length N, in which every element is an integer in range [1, M].

They call a sequence S is good if the sequence has the following property: for every substring of S with length M, i.e., S[i→i+m-1], all the elements in the substring are the same or all the elements are distinct(different
 from each other).

The company GIS is designed to produce good sequences. But how many different good sequences are there? Since the answer will be very large, just output the result after module 987654321.







Input



There are several cases. For each case, there is a line with two integers N, and M ( 1 ≤ N ≤ 1000000, 1 ≤ M ≤ 1000000 ).

The input ends up with two negative numbers, which should not be processed as a case.







Output



Print the number of different good sequences module 987654321 in a line for each case.







Sample Input

4 4
3 5
-1 -1




Sample Output

28
125












这道题漏了个回车又得了个PE……无奈了……

以及……代码如果过长请无视……看到大数取模的第一反应是除法不封闭需要乘以模逆元，求模逆元需要扩展欧几里得算法，所以直接题目都没看完就开始敲这些东西……然后剩了8分钟写核心代码……我真心读题要特训一下Q^Q

题意是说，有一个长度为N的序列，里面的数字都是在1-M之间取的整数，问满足如下条件的序列有多少个：

1、序列中任何一个长度为M的字串都相同或者都不同

2、（坑）如果序列都不到M长度，到底是怎么算呢…… （原先以为是C(n,m)  ）好吧题目里给了个125……看来是每个位置都随便放了呢，那难不成就是pow（n,m）了……

说了半天其实这个题是这么个规律：




当n>=m时，ans=m+m!    ，预处理阶乘比较省时间。

（坑死人必须得注意到的方面）当n<m时，为m^n，话说可以用快速幂。



代码如下：


```cpp
#include <map>
#include <stack>
#include <queue>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
const long long lim=987654321;
int n,m;

ll Jval[1000001];//jiecheng juzhen
void Jc_init()
{
	Jval[0]=1;
	for(int i=1;i<=1000000;i++)
	Jval[i]=(i*Jval[i-1])%lim;
}

struct _gcd
{    
        ll d;  
        ll x;  
        ll y;  
};  
 
_gcd ex_EUCLID(ll a,ll b)  //EUCLID algorithm 
{  
    _gcd aa,bb;  
    if(b==0){  
             aa.d = a;  
             aa.x = 1;  
             aa.y = 0;  
             return aa;  
            }  
    else{  
         bb = ex_EUCLID(b,a%b);   
         aa.d = bb.d;  
         aa.x = bb.y;  
         aa.y = bb.x - bb.y * (a/b);  
         }  
    return aa;  
}   

ll _Ni(ll val,ll lim)
{
	ll x;  
    _gcd aa;  
    aa = ex_EUCLID(val,lim);  //EUCLID 求模逆元 
    return aa.x;  
}

ll solve1()
{
	ll ans=m;
	return ans;
}

ll Calc()
{ 
	ll Cans=   (	Jval[m]*_Ni(Jval[m-n],lim)   ) %lim;
	return Cans; 
}

ll solve2()     //m+ m!/(m-n)！ 
{
	ll ans=0;
	ans+=m;
	ans+= Jval[m];
	return (ans%lim+lim)%lim;
}

ll solve(int m,int n)
{
	int i,j;//loop vars
	ll ans=1;
	if(m==1) return ans;
	if(n>=m)
	{
		if(m==2) for(i=1;i<=n;i++) ans=ans*m%lim;
		else  ans=Jval[m]+m; // m!+m
		return ans;
	}
	else 
	{
		for(i=1;i<=n;i++)	ans=ans*m%lim;
		return ans;
	}
}

int main()
{
	Jc_init();
		while(1)
		{
			scanf("%d%d",&n,&m);
			if(n<0&&m<0)return 0;
			printf("%I64d\n",solve(m,n)); 
		//	if(n>m)printf("%I64d\n",solve1());
		//	else printf("%I64d\n",solve2());
		}
	return 0;
}
```
我的太乱了对不起大家Q_Q


所以贴一个网上的精简版代码：


Source Code From:



# [YYOrz](http://hi.baidu.com/yyorz403) @Baidu blog






```cpp
#include <iostream>
#define MOD 987654321
using namespace std;
int main()
{
int n,m;
while(scanf("%d%d",&n,&m) && n!=-1 && m!=-1)
{
   int i;
   long long ans;
   if(n<m)
   {
    ans=1;
    for(i=1; i<=n; i++)
     ans = (ans*m)%MOD;
   }
   else
   {
    ans = 1;
    if(m==1)
     ans=1;
    else if(m==2)
    {
     for(i=1; i<=n; i++)
      ans = (ans*m)%MOD;
    }
    else
    {
     for(i=2; i<=m; i++)
      ans = (ans*i)%MOD;
     ans += m;
    }
   }
   printf("%lld\n",ans%MOD);
}
return 0;
}
```









