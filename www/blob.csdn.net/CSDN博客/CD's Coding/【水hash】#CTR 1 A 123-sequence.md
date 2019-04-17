# 【水hash】#CTR 1 A. 123-sequence - CD's Coding - CSDN博客





2014年07月30日 16:20:33[糖果天王](https://me.csdn.net/okcd00)阅读数：538

















There is a given sequence of integers *a*1, *a*2, ..., *a**n*,
 where every number is from 1 to 3 inclusively. You have to replace the minimum number of numbers in it so that all the numbers in the sequence are equal to each other.




Input


The first line contains an integer *n* (1 ≤ *n* ≤ 106).
 The second line contains a sequence of integers*a*1, *a*2, ..., *a**n* (1 ≤ *a**i* ≤ 3).




Output


Print the minimum number of replacements needed to be performed to make all the numbers in the sequence equal.




Sample test(s)




input
9
1 3 2 2 2 1 1 2 3




output
5






Note


In the example all the numbers equal to 1 and 3 should be replaced by 2.










有一个数列只有1、2、3.问最少改几个数让他们变成全都是一样的数……



那当然是找到最多的那个数字然后把另两个改成它咯






```cpp
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int num[4]={0};
	int n;	cin>>n;
	for(int i=1;i<=n;i++)	
	{
		int tmp;
		cin>>tmp;
		num[tmp]++;
	}
	if(num[1]>=num[2]&&num[1]>=num[3])cout<<num[2]+num[3]<<endl;
	else if(num[2]>=num[1]&&num[2]>=num[3])cout<<num[1]+num[3]<<endl;
	else cout<<num[1]+num[2]<<endl;
	return 0;
}
```











