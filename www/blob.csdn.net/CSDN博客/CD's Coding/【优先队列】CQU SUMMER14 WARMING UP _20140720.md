# 【优先队列】CQU SUMMER14 WARMING UP _20140720 - CD's Coding - CSDN博客





2014年07月20日 21:37:41[糖果天王](https://me.csdn.net/okcd00)阅读数：434标签：[HDU2519																[HDU1051																[HDU2601																[HDU1061](https://so.csdn.net/so/search/s.do?q=HDU1061&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)







# 新生晚会
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 8205    Accepted Submission(s): 2912**



Problem Description


开学了，杭电又迎来了好多新生。ACMer想为新生准备一个节目。来报名要表演节目的人很多，多达N个，但是只需要从这N个人中选M个就够了，一共有多少种选择方法？





Input


数据的第一行包括一个正整数T，接下来有T组数据，每组数据占一行。

每组数据包含两个整数N（来报名的人数,1<=N<=30），M（节目需要的人数0<=M<=30）






Output


每组数据输出一个整数，每个输出占一行





Sample Input


5
3 2
5 3
4 4
3 6
8 0






Sample Output


3
10
1
0
1







求C(n,m)，为了数值不要过大采取乘一个除一个的方法——因为n个连续自然数的乘积一定整除n的阶乘





```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
typedef __int64 ll;
/*
ll jc[31];
void j_init()
{
	memset(jc,0LL,sizeof jc);
	jc[0]=1LL;
	for(int i=1;i<31;i++)
		jc[i]=jc[i-1]*(ll)i;
}
*/

ll C(int n,int m)
{
	ll ans=1LL;
	if(n<m)			return 0;
	if(n==m||m==0)	return 1LL;
	if(m==1||n-m==1)return (ll)n;
	for(int i=1;i<=m;i++)
	{
		ans*=(ll)(n-i+1);
		ans/=(ll)i;
	}
	return ans;
}

int main()
{
	int T;
	cin>>T;
	//j_init();
	//cout<<jc[30];
	for(int i=1;i<=T;i++)
	{
		int N,M;
		cin>>N>>M;
		cout<<C(N,M)<<endl;
	}
	return 0;
}
```




# Wooden Sticks
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 11380    Accepted Submission(s): 4676**



Problem Description


There is a pile of n wooden sticks. The length and weight of each stick are known in advance. The sticks are to be processed by a woodworking machine in one by one fashion. It needs some time, called setup time, for the machine to prepare processing a stick.
 The setup times are associated with cleaning operations and changing tools and shapes in the machine. The setup times of the woodworking machine are given as follows: 


(a) The setup time for the first wooden stick is 1 minute. 

(b) Right after processing a stick of length l and weight w , the machine will need no setup time for a stick of length l' and weight w' if l<=l' and w<=w'. Otherwise, it will need 1 minute for setup. 


You are to find the minimum setup time to process a given pile of n wooden sticks. For example, if you have five sticks whose pairs of length and weight are (4,9), (5,2), (2,1), (3,5), and (1,4), then the minimum setup time should be 2 minutes since there is
 a sequence of pairs (1,4), (3,5), (4,9), (2,1), (5,2).






Input


The input consists of T test cases. The number of test cases (T) is given in the first line of the input file. Each test case consists of two lines: The first line has an integer n , 1<=n<=5000, that represents the number of wooden sticks in the test case,
 and the second line contains n 2 positive integers l1, w1, l2, w2, ..., ln, wn, each of magnitude at most 10000 , where li and wi are the length and weight of the i th wooden stick, respectively. The 2n integers are delimited by one or more spaces.






Output


The output should contain the minimum setup time in minutes, one per line.






Sample Input


3 
5 
4 9 5 2 2 1 3 5 1 4 
3 
2 2 1 1 2 2 
3 
1 3 2 2 3 1






Sample Output


2
1
3




使用了优先队列，这是个排序后贪心的问题，使用队列进行模拟即可。

每次把所有满足条件的都踢出去然后ans加一

Code：

```cpp
#include <cmath>   
#include <queue>
#include <vector>
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
const int inf=99999;
const int _sign=89757;
int main()
{
	int T;
	cin>>T;
	for(int cs=1;cs<=T;cs++)
	{
		int n;
		cin>>n;
		priority_queue <pair<int, int> > pq;
		queue < pair<int, int> > q;
		for(int ns=0;ns<n;ns++)//sort in pq
		{
			pair<int,int> tmp;
			cin>>tmp.first>>tmp.second;
			pq.push(tmp);
		}
		while(!pq.empty())//trans into q
		{
			pair<int,int> tmp=pq.top();
			q.push(tmp);
			pq.pop();
		}
		q.push(make_pair(_sign,_sign));
		int len=inf,wht=inf,ans=0;
		pair<int,int> temp;
		while(1)
		{
			temp=q.front();q.pop();
			//cout<<"#"<<temp.first<<"\t"<<temp.second<<endl;
			if(temp.first==_sign&&temp.second==_sign)
			{
				if(len==inf&&wht==inf){break;}
				len=wht=inf;
				q.push(temp);
				ans++;
			}
			else if(temp.first<=len&&temp.second<=wht)
			{
				len=temp.first;
				wht=temp.second;
			}
			else q.push(temp);
			//cout<<temp.first<<"\t"<<temp.second<<endl;
		}
		cout<<ans<<endl;
	}	
	return 0;
}
```







# Bone Collector
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 28461    Accepted Submission(s): 11609**



Problem Description


Many years ago , in Teddy’s hometown there was a man who was called “Bone Collector”. This man like to collect varies of bones , such as dog’s , cow’s , also he went to the grave …

The bone collector had a big bag with a volume of V ,and along his trip of collecting there are a lot of bones , obviously , different bone has different value and different volume, now given the each bone’s value along his trip , can you calculate out the
 maximum of the total value the bone collector can get ?
![](http://acm.hdu.edu.cn/data/images/C154-1003-1.jpg)






Input


The first line contain a integer T , the number of cases.

Followed by T cases , each case three lines , the first line contain two integer N , V, (N <= 1000 , V <= 1000 )representing the number of bones and the volume of his bag. And the second line contain N integers representing the value of each bone. The third
 line contain N integers representing the volume of each bone.





Output


One integer per line representing the maximum of the total value (this number will be less than 231).





Sample Input


1
5 10
1 2 3 4 5
5 4 3 2 1






Sample Output


14




背包问题，DP（一直弄不清状态转移方程）：


```cpp
#include<iostream>
#include<algorithm>
using namespace std;
int dp[1000][1000];
int main()
{
    int t,n,v,i,j;cin>>t;
    int va[1000],vo[1000];
    while(t--)
    {
        cin>>n>>v;
        for(i=1;i<=n;i++) cin>>va[i];
        for(i=1;i<=n;i++) cin>>vo[i];
        memset(dp,0,sizeof(dp));
         for(i=1;i<=n;i++)
            for(j=0;j<=v;j++)
            {
                if(vo[i]<=j)//ith item will into j-size bag
                    dp[i][j]=max(dp[i-1][j],dp[i-1][j-vo[i]]+va[i]);//consider about if ith in it then others beyond the limit
                else //ith item can not be into it
                    dp[i][j]=dp[i-1][j];
            }
        cout<<dp[n][v]<<endl;
    }
    return 0;
}
```









# Rightmost Digit
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 31275    Accepted Submission(s): 11928**



Problem Description


Given a positive integer N, you should output the most right digit of N^N.






Input


The input contains several test cases. The first line of the input is a single integer T which is the number of test cases. T test cases follow.

Each test case contains a single positive integer N(1<=N<=1,000,000,000).






Output


For each test case, you should output the rightmost digit of N^N.






Sample Input


2
3
4






Sample Output


7
6



*Hint*

In the first case, 3 * 3 * 3 = 27, so the rightmost digit is 7.
In the second case, 4 * 4 * 4 * 4 = 256, so the rightmost digit is 6.









问N的N次方末尾数是什么，枚举找循环节，取模即可。



```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;
//int num[10]={1,4,4,2,1,1,4,4,2,1};
int main()
{
	int T;
	cin>>T;
	int ans2[4]={6,2,4,8},ans3[4]={1,3,9,7},ans7[4]={1,7,9,3},ans8[4]={6,8,4,2};
	int ans4[2]={6,4},ans9[2]={1,9};
	for(int i=0;i<T;i++)
	{
		int N;
		cin>>N;
		int r=N%10;
		switch(r)
		{
			case 0:
				cout<<"0"<<endl;
				break;
			case 1:
				cout<<"1"<<endl;
				break;
			case 2:
				cout<<ans2[N%4]<<endl;
				break;
			case 3:
				cout<<ans3[N%4]<<endl;
				break;
			case 4:	
				cout<<ans4[N%2]<<endl;
				break;
			case 5:
				cout<<"5"<<endl;
				break;
			case 6:
				cout<<"6"<<endl;
				break;
			case 7:
				cout<<ans7[N%4]<<endl;
				break;
			case 8:
				cout<<ans8[N%4]<<endl;
				break;
			case 9:
				cout<<ans9[N%2]<<endl;
				break;
		}
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=HDU2601&t=blog)](https://so.csdn.net/so/search/s.do?q=HDU1051&t=blog)](https://so.csdn.net/so/search/s.do?q=HDU2519&t=blog)




