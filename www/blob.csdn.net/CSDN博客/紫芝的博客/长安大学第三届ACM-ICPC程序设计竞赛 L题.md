# 长安大学第三届ACM-ICPC程序设计竞赛 L题 - 紫芝的博客 - CSDN博客





2018年04月26日 12:03:58[紫芝](https://me.csdn.net/qq_40507857)阅读数：45








                链接：[https://www.nowcoder.com/acm/contest/102/L](https://www.nowcoder.com/acm/contest/102/L)
来源：牛客网


## 题目描述


Many years later, *Rainbow Island *is in the mercy of big boss *qiami*. Big boss *qiami* is fond of number 9 because each side of the magic cube is made up of 9 small pieces and he changes the face value of circulating currency to 90,91,92,93,94 *Yuan*.
 One day programmer *Uucloud *went to a shop to buy Cat food worth *n Yuan*. The shopkeeper *NoMoreWords *and *Uucloud *are good friends so he will give *Uucloud *his change. *Uucloud *wants to know how many bills do they need in their trade at least.

  For example, if *Uucloud *wants to buy cat food of 8 *Yuan*, he will pay a 9 *Yuan* bill and *NoMoreWords* will give *Uucloud* 1 *Yuan* bill as change. Two paper money are used in this trade. 

![](https://uploadfiles.nowcoder.com/images/20180408/304659_1523176143997_9080EB3311100BC35B362ED45DBE1421)



## 输入描述:
The first line contains an integer number T, the number of test cases.
Next T lines contains a number n(1 ≤ n ≤ 109)。

## 输出描述:
For each test case print the number of bills they need at least.

示例1



## 输入

2
14
18



## 输出

6
2





## 解题思路：

把钱用九进制表示，从低位向高位，当前位数字位x，
如果x>=5 则选择用更大的面额交易，答案加9-x，


否则用当前面额交易，答案加x

```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int t;
	cin>>t;
	while(t--){
		int n,v[10];
		cin>>n;
		int ans=0;
		for(int i=4;i>=0;i--){
			v[i]=n/(int)pow(9,i);
			n-=(int)pow(9,i)*v[i];
		}
		for(int i=0;i<4;i++)
		{
			if(v[i]>=5){
				v[i]=9-v[i];
				v[i+1]++;
			}
			ans+=v[i];
		}
		printf("%d\n",ans+v[4]);
	}
	return 0;
}
```




