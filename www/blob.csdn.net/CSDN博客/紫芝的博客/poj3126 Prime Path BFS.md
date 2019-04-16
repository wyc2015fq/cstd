# poj3126 Prime Path BFS - 紫芝的博客 - CSDN博客





2018年05月30日 20:40:56[紫芝](https://me.csdn.net/qq_40507857)阅读数：45
个人分类：[BFS](https://blog.csdn.net/qq_40507857/article/category/7566192)









[点击打开链接](http://poj.org/problem?id=3126)



# Prime Path


|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 26158||**Accepted:** 14376|
Description

The ministers of the cabinet were quite upset by the message from the Chief of Security stating that they would all have to change the four-digit room numbers on their offices.
— It is a matter of security to change such things every now and then, to keep the enemy in the dark.
— But look, I have chosen my number 1033 for good reasons. I am the Prime minister, you know!
— I know, so therefore your new number 8179 is also a prime. You will just have to paste four new digits over the four old ones on your office door.
— No, it’s not that simple. Suppose that I change the first digit to an 8, then the number will read 8033 which is not a prime!
— I see, being the prime minister you cannot stand having a non-prime number on your door even for a few seconds.
— Correct! So I must invent a scheme for going from 1033 to 8179 by a path of prime numbers where only one digit is changed from one prime to the next prime.

Now, the minister of finance, who had been eavesdropping, intervened.
— No unnecessary expenditure, please! I happen to know that the price of a digit is one pound.
— Hmm, in that case I need a computer program to minimize the cost. You don't know some very cheap software gurus, do you?
— In fact, I do. You see, there is this programming contest going on... Help the prime minister to find the cheapest prime path between any two given four-digit primes! The first digit must be nonzero, of course. Here is a solution in the case above.

> 1033
1733
3733
3739
3779
8779
8179
The cost of this solution is 6 pounds. Note that the digit 1 which got pasted over in step 2 can not be reused in the last step – a new 1 must be purchased.

Input

One line with a positive number: the number of test cases (at most 100). Then for each test case, one line with two numbers separated by a blank. Both numbers are four-digit primes (without leading zeros).

Output

One line for each case, either with a number stating the minimal cost or containing the word Impossible.

Sample Input
3
1033 8179
1373 8017
1033 1033
Sample Output
6
7
0
Source

[Northwestern Europe 2006](http://poj.org/searchproblem?field=source&key=Northwestern+Europe+2006)




## 题意：

给定两个素数a b，求a变到b需要几步，并且变换时只有一个数字不同，并且是素数


## 思路：

因为是四位的素数，所以先对素数打表，然后BFS，每次进入队列的是：只有一个数字不同的四位数，并且是素数，就入队





```cpp
#include<iostream>
#include<cstring>
#include<queue>
#include<cmath>
#include<cstdio>
using namespace std;
#define MAXV 10009

bool prime[MAXV];

void init(){	//对素数打表

	int i,j;
	for(i=1000;i<=MAXV;i++){
		for(j=2;j<i;j++)
			if(i%j==0){
				prime[i]=false;
				break;
			}
		if(j==i) prime[i]=true;
	}
	/*
	memset(prime,0,sizeof(prime));
	int m=(int)sqrt(MAXV+0.5);
	for(int i=2;i<=m;i++)
    if(!prime[i])
        for(int j=i*i;j<MAXV;j+=i)
        prime[j]=1;*/
}
int bfs(int first,int last){
	bool dis[MAXV];
	queue <int>q;
	int v,i,j,temp,vtemp,count[MAXV],t[4];
	memset(dis,false,sizeof(dis));
	memset(count,0,sizeof(count));

	q.push(first);
	dis[first]=true;

	while(!q.empty()){
		v=q.front();
		q.pop();

		t[0]=v/1000;
		t[1]=v%1000/100;
		t[2]=v%100/10;
		t[3]=v%10;
//		printf("%d %d %d %d",t[0],t[1],t[2],t[3]);

		for(j=0;j<4;j++){
			temp=t[j];
			for(i=0;i<10;i++)
				if(i!=temp){
					t[j]=i;
					vtemp=t[0]*1000+t[1]*100+t[2]*10+t[3];
					if(!dis[vtemp] && prime[vtemp]){
						count[vtemp]=count[v]+1;
						dis[vtemp]=true;
						q.push(vtemp);
					}
					if(vtemp==last) return count[vtemp];
				}
			t[j]=temp;
		}
		if(v==last) return count[v];
	}
	return -1;
}

int main(){
	int n,a,b,key;
	init();
	scanf("%d",&n);
	while(n--){
		scanf("%d%d",&a,&b);
		key=bfs(a,b);
		if(key!=-1) printf("%d\n",key);
		else printf("Impossible\n");
	}
	return 0;
}
```







