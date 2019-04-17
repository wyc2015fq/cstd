# 第四次codeforces竞技结束 #FF（#255） Div 2 - CD's Coding - CSDN博客





2014年07月19日 14:18:51[糖果天王](https://me.csdn.net/okcd00)阅读数：565








这次第一次吃到了Pretest Passed但是Final WA的情况……哭瞎简直……怪我写快了没想的很多……










DZY has a hash table with *p* buckets, numbered from 0 to *p* - 1.
 He wants to insert *n* numbers, in the order they are given, into the hash table. For the *i*-th
 number *x**i*, DZY
 will put it into the bucket numbered *h*(*x**i*),
 where *h*(*x*) is the hash function. In this problem we will assume, that *h*(*x*) = *x mod p*.
 Operation *a mod b* denotes taking a remainder after division *a* by *b*.


However, each bucket can contain no more than one element. If DZY wants to insert an number into a bucket which is already filled, we say a "conflict" happens. Suppose the first conflict happens right after the *i*-th
 insertion, you should output *i*. If no conflict happens, just output -1.




Input


The first line contains two integers, *p* and *n*(2 ≤ *p*, *n* ≤ 300).
 Then *n* lines follow. The *i*-th of them contains an
 integer *x**i*(0 ≤ *x**i* ≤ 109).




Output


Output a single integer — the answer to the problem.




Sample test(s)




input
10 5
0
21
53
41
53




output
4




input
5 5
0
1
2
3
4




output
-1












这个说的是hash表，就是说问第一个哈希冲突部分的index（详情请见数据结构课本）。



### Code：



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
using namespace std;
int p,n;
int hash[301];

int geth(int x)
{
	return x%p;
}

int main()
{
	int flag=0;
	memset(hash,0,sizeof hash);
	scanf("%d%d",&p,&n);
	for(int i=1;i<=n;i++)
	{
		int tmp;
		scanf("%d",&tmp);
		if(hash[geth(tmp)]==0)hash[geth(tmp)]=1;
		else if(!flag)
						{
							flag=i;
							break;
						}
	}
	if(!flag)printf("-1");
	else printf("%d",flag);
	
	return 0;
}
```






DZY loves collecting special strings which only contain lowercase letters. For each lowercase letter *c* DZY knows its value *w**c*.
 For each special string *s* = *s*1*s*2... *s*|*s*| (|*s*| is
 the length of the string) he represents its value with a function *f*(*s*), where
![](http://espresso.codeforces.com/47c9783b69409ca6ade8e93f7d51bed11f430539.png)

Now DZY has a string *s*. He wants to insert *k* lowercase
 letters into this string in order to get the largest possible value of the resulting string. Can you help him calculate the largest possible value he could get?




Input


The first line contains a single string *s* (1 ≤ |*s*| ≤ 103).


The second line contains a single integer *k* (0 ≤ *k* ≤ 103).


The third line contains twenty-six integers from *w**a* to *w**z*.
 Each such number is non-negative and doesn't exceed 1000.




Output


Print a single integer — the largest possible value of the resulting string DZY could get.




Sample test(s)




input
abc
3
1 2 2 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1




output
41






Note


In the test sample DZY can obtain "abcbbc", *value* = 1·1 + 2·2 + 3·2 + 4·2 + 5·2 + 6·2 = 41.







这个的意思是字母表26个字母各代表着一个值，现在有一个字符串s了，问我们加上k个字母之后，![](http://espresso.codeforces.com/47c9783b69409ca6ade8e93f7d51bed11f430539.png)这个式子的最大值是多少呢？

那么如果是一个字符串的需求值最大的话，那肯定是全都是值最大的字母组成的字符串了，但是其中有几个字母是给定的，放在哪里好呢？难不成要枚举嘛？怎么可能啦……因为这个式子是越靠后的越有效，所以不管原来的字符串是什么，全都扔到最前面就行了呗~

所以就直接把原先字符串的值算出来，然后加上k个最大值乘以位权即可。

### Code：



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
using namespace std;
typedef __int64 ll;

string 	s;
int 	k,ans;
int 	w[27]; //0forMAX ,1~26


void init()
{
	
	for(int i=1;i<=26;i++)
	{
		scanf("%d",&w[i]);
		if(w[i]>=w[0])w[0]=w[i];
	}
}

void solve()
{
	int temp=0;
	temp += s.length()*2+k+1;// div2 & mulw[0]
	temp *= k;
	temp /= 2;
	temp *= w[0];
	//cout<<s.length()<<endl;
	//cout<<ans<<endl;
	//cout<<temp<<endl;
	ans+=temp;
}

int main()
{
	ans=0;
	memset(w,0,sizeof w);
	cin>>s>>k;
	init();
	for(int i=0;i<s.length();i++)
	{
		ans+= (i+1)*w[s[i]-'a'+1];
	}
	solve();
	printf("%d",ans);
	return 0;
}
```






DZY has a sequence *a*, consisting of *n* integers.


We'll call a sequence *a**i*, *a**i* + 1, ..., *a**j*(1 ≤ *i* ≤ *j* ≤ *n*) a
 subsegment of the sequence *a*. The value (*j* - *i* + 1) denotes
 the length of the subsegment.


Your task is to find the longest subsegment of *a*, such that it is possible to change at most one number (change one number to any integer you want) from
 the subsegment to make the subsegment strictly increasing.


You only need to output the length of the subsegment you find.




Input


The first line contains integer *n* (1 ≤ *n* ≤ 105).
 The next line contains *n* integers *a*1, *a*2, ..., *a**n* (1 ≤ *a**i* ≤ 109).




Output


In a single line print the answer to the problem — the maximum length of the required subsegment.




Sample test(s)




input
6
7 2 3 1 5 6




output
5






Note


You can choose subsegment *a*2, *a*3, *a*4, *a*5, *a*6 and
 change its 3rd element (that is *a*4)
 to 4.





这道题的意思呢是说有这么许多的数，问你如果只让你最多改一个数字，可以令这串数字中最长的一个满足严格递增的子串长度。

这样用dp来O(n)内给现有数据分段是必须的，然后就是考虑各种情况了。我想到的有：（事实证明想的繁了……）

1\ 当前段落为1个数字的时候

2\ 前一个段落为1个数字的时候

3\ 当前和前一个都为1个数字的时候

4\ 

5\

好多好多，反正就是很烦……

这道题我在Pretest的时候分分钟秒掉了，但是最后说我错掉了……才知道……原来还有这一手……边缘数据和其他一些可能错误的点会放在完整数据里：



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
#define update(x) ans=(x>ans)?x:ans
using namespace std;
int n,ccnt,a[100001],dp[111111];
int chain[100001]; //length of each sequence
int pos[100001];

void init()
{
	ccnt=0;
	memset(a,0,sizeof a);
	memset(dp,0,sizeof dp);
	memset(pos,0,sizeof pos);
	memset(chain,0,sizeof chain);
	for(int i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
		if(i)
		{
			if(a[i]>a[i-1])dp[i]=dp[i-1]+1;
			else 
			{
				chain[ccnt]=dp[i-1]+1;
				pos[ccnt]=i-1;
				ccnt++;
				//cout<<dp[i-1]<<":m1: \t";
				dp[i]=0;
			}
		}
	}
	chain[ccnt]=dp[n-1]+1;
	pos[ccnt]=n-1;
}

int solve()
{
	int ans=0;
	if(ccnt==0)update(chain[0]);
	else 
	{
		update(chain[0]+1);
		for(int i=1;i<=ccnt;i++)
		{
			//cout<<chain[i]<<"\t"<<pos[i]<<endl;
			if(chain[i]==1)	//I am 1
			{
				if(a[pos[i-1]]+1<a[pos[i+1]-chain[i+1]+1])	//then change chain[i]
				update(chain[i+1]+chain[i-1]+1);
				else update(chain[i-1]+1);
			}
			else if(chain[i-1]==1)// my front is 1
				{
					update(chain[i]+1);
				}
			else if(chain[i+1]==1);// my back is 1 _for i+1 judgement
			else if(a[pos[i-1]]+1<a[pos[i]-chain[i]+2]) 	//then change chain[i]'s 1st
				update(chain[i]+chain[i-1]);
			else update(chain[i]+1);
		}
   }
	return ans;
}

int main()
{
	scanf("%d",&n);
	init();
	cout<<solve();
	return 0;
}
```








Test: #17, time: 15 ms., memory: 1612 KB, exit code: 0, checker exit code: 1, verdict: WRONG_ANSWER



Input
100000
934235036 959935357 930550940 102561968 953861985 160719142 566459185 206822938 285499069 440560416 553403451 248348736 675456978 22179275 396648745 619445897 724544360 294421923 850746812 412477302 697614753 888340989 483327876 175856051 50535024 139680152 649538207 550903080 357363952 814274524 963857626 291598987 626726232 746924917 246677306 433104569 907644058 813136490 639927506 45659479 253696906 193330956 294008214 781670235 215510230 690656959 253632483 940054589 985078881 104379294 205048...


Output
31904


Answer
46213



Checker Log
wrong answer 1st numbers differ - expected: '46213', found: '31904'






看了看，全是极限数据，那么为什么这里就会是错误的呢，仔细思考，难不成存在某种可怕的溢出？

于是改成longlong，但是依然WA at 17……



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
#define update(x) ans=(x>ans)?x:ans
using namespace std;
typedef long long ll;
ll n,ccnt,a[100001],dp[111111];
ll chain[100001]; //length of each sequence
ll pos[100001];

void init()
{
	ccnt=0;
	memset(a,0LL,sizeof a);
	memset(dp,0LL,sizeof dp);
	memset(pos,0LL,sizeof pos);
	memset(chain,0LL,sizeof chain);
	for(ll i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
		if(i)
		{
			if(a[i]>a[i-1])dp[i]=dp[i-1]+1;
			else 
			{
				chain[ccnt]=dp[i-1]+1;
				pos[ccnt]=i-1;
				ccnt++;
				//cout<<dp[i-1]<<":m1: \t";
				dp[i]=0;
			}
		}
	}
	chain[ccnt]=dp[n-1]+1;
	pos[ccnt]=n-1;
}

ll solve()
{
	ll ans=0;
	if(ccnt==0)update(chain[0]);
	else 
	{
		update(chain[0]+1);
		for(int i=1;i<=ccnt;i++)
		{
			//cout<<chain[i]<<"\t"<<pos[i]<<endl;
			if(chain[i]==1)	//I am 1
			{
				if(a[pos[i-1]]+1<a[pos[i+1]-chain[i+1]+1])	//then change chain[i]
				update(chain[i+1]+chain[i-1]+1);
				else update(chain[i-1]+1);
			}
			else if(chain[i-1]==1)// my front is 1
				{
					update(chain[i]+1);
				}
			else if(chain[i+1]==1);// my back is 1 _for i+1 judgement
			else if(a[pos[i-1]]+1<a[pos[i]-chain[i]+2]) 	//then change chain[i]'s 1st
				update(chain[i]+chain[i-1]);
			else update(chain[i]+1);
		}
   }
	return ans;
}

int main()
{
	cin>>n;
	init();
	cout<<solve();
	return 0;
}
```





后来整理整理，其实只有四条需要注意的……



```cpp
if(f[i]==n)ans=max(ans,f[i]);
        else ans=max(ans,f[i]+1);
        if(a[i-f[i]+1]>a[i-f[i]-1]+1)ans=max(ans,f[i]+f[i-f[i]]);
        if(a[i-f[i]+2]>a[i-f[i]]+1)ans=max(ans,f[i]+f[i-f[i]]);
```


### Code:



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
#define inf 999999
#define maxn 100005
#define update(x) ans=(x>ans)?x:ans
using namespace std;
int n,a[maxn],f[maxn];
int main()
{
    scanf("%d",&n);
    for(int i=1;i<=n;i++)scanf("%d",&a[i]);
    f[1]=1;
    for(int i=2;i<=n;i++)
    {
        f[i]=1;if(a[i]>a[i-1])f[i]=f[i-1]+1;
    }
    int ans=-inf;
    for(int i=1;i<=n;i++)
    {
        if(f[i]==n) update(f[i]);
        else update(f[i]+1);
        if(a[i-f[i]+1]>a[i-f[i]-1]+1) update(f[i]+f[i-f[i]]);
        if(a[i-f[i]+2]>a[i-f[i]]+1) update(f[i]+f[i-f[i]]);
    }
    printf("%d\n",ans);
    return 0;
}
```







