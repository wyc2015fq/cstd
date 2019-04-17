# 【水遍历\打表】#17 A. Noldbach problem - CD's Coding - CSDN博客





2014年05月07日 17:16:03[糖果天王](https://me.csdn.net/okcd00)阅读数：577
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)













Nick is interested in prime numbers. Once he read about Goldbach problem. It states that every even integer greater than 2can
 be expressed as the sum of two primes. That got Nick's attention and he decided to invent a problem of his own and call itNoldbach problem. Since Nick is interested only in prime numbers, Noldbach
 problem states that at least *k* prime numbers from2 to *n* inclusively
 can be expressed as the sum of three integer numbers: two neighboring prime numbers and 1. For example,19 = 7 + 11 + 1,
 or 13 = 5 + 7 + 1.


Two prime numbers are called neighboring if there are no other prime numbers between them.


You are to help Nick, and find out if he is right or wrong.




Input


The first line of the input contains two integers *n* (2 ≤ *n* ≤ 1000)
 and *k* (0 ≤ *k* ≤ 1000).




Output


Output YES if at least *k* prime numbers from 2 to *n* inclusively
 can be expressed as it was described above. Otherwise outputNO.




Sample test(s)




input
27 2




output
YES



input
45 7




output
NO





Note


In the first sample the answer is YES since at least two numbers can be expressed as it was described (for example, 13 and 19). In the second sample the answer
 is NO since it is impossible to express 7 prime numbers from 2 to 45 in the desired form.





这道题的意思是说，n以内的，可以由两个相邻质数之和加一获得的质数个数够不够k个

那么，我们就： 1、打质数表 2、3到n的质数遍历，满足的cnt+1 3、比较

写到这我突然发现……我干嘛不直接算出1000以内所有的满足条件的质数表啊……

然后直接遍历Noldbach质数表，for(0~ans)第一个大于n的数，返回下标不就行了么？！！！

先把傻乎乎的方法贴出来：



```cpp
#include<algorithm>
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;

int prime[168]=
{	//1000的小数据嘛，Prime也不多~ 
	2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
	73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,
	157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,
	239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,
	331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,
	421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,
	509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,
	613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,
	709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,
	821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,
	919,929,937,941,947,953,967,971,977,983,991,997}; 
 
int primeNum[1000];
bool isPrime(int x)
{
	for(int i=2;i<sqrt(x);i++)
	if(x%i==0)return false;
	return true;
}

bool isAnswer(int x)
{
	bool flag=false;
	for(int m=0;prime[m]<=x;m++)
	if(prime[m]==x)flag=true;
	if(flag==true) 
		for(int j=0;prime[j]<=x;j++)
			if(prime[j]+prime[j+1]==x-1)return true;
	return false;
}

int main()
{
	int n,k,cnt=0;
	scanf("%d%d",&n,&k);
	for(int l=3;l<n+1;l++)
		if(isAnswer(l))cnt++;
	if (cnt>=k) printf("YES");
	else printf("No");
	return 0;
}
```


Advanced方法：



```cpp
#include<algorithm>
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;

int Advanced[]={13,19,31,37,43,53,61,79,101,113,139,163,173,199,211,223,241,
269,277,331,353,373,397,457,463,509,521,541,577,601,619,631,727,773,787,811,
829,853,883,907,919,947,967,991,65536};  //最后那个是标界，防止n>991 

int main()
{
	int n,k,cnt=0;
	scanf("%d%d",&n,&k);
	for(int i=0;Advanced[i]<=n;i++)cnt++;
	printf(cnt>=k?"Yes":"No");
	return 0;
}
```





