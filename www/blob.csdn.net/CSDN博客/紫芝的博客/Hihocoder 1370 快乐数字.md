# Hihocoder 1370 快乐数字 - 紫芝的博客 - CSDN博客





2018年05月05日 13:27:25[紫芝](https://me.csdn.net/qq_40507857)阅读数：238









[点击打开链接](https://cn.vjudge.net/contest/226984#problem/D)


# 快来数字

判断一个正整数是否是快乐数字？

如果一个数字能够通过有限次快乐变换成为1，则是快乐数字。

快乐变换是对一个数字的每一位的平方数求和。

例如：

对于68

68 => 62+82= 100

100 => 12 + 02 + 02 = 1

因此68是快乐数字

## Input

每一行输入一个整数，代表n；如果n为-1，表示输入结束。（n <= 100000000) 

## Output

针对每组数据，输出是否是快乐数字 true/false


## Sample Input
68
-1
## Sample Output
true


```cpp
//Hihocoder 1370 快乐数字 
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e8+10;
int s[maxn];
int n;
int gm(int m)
{
	int sum=0,t=0;
	while(1){
		sum+=(m%10)*(m%10);
		m/=10;
		if(m==0){
			if(sum==1)
			return 1;
			m=sum;
			for(int i=0;i<t;i++){
				if(sum==s[i])
				return 0;
			}//for
			s[t++]=sum;
			sum=0;
		}//if
	}//while
}
int main()
{
	ios::sync_with_stdio(0);
	while(~scanf("%d",&n)&&n!=-1){
		if(gm(n))
		printf("true\n");
		else
		printf("false\n");
	}	
	return 0;
}
```




