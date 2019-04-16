# poj 3273 Monthly Expense 经典二分 - 紫芝的博客 - CSDN博客





2018年07月22日 19:35:33[紫芝](https://me.csdn.net/qq_40507857)阅读数：28








# Monthly Expense
|**Time Limit:** 2000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 33725||**Accepted:** 12620|

Description

Farmer John is an astounding accounting wizard and has realized he might run out of money to run the farm. He has already calculated and recorded the exact amount of money (1 ≤ *moneyi* ≤ 10,000) that he will need to spend each day over the next *N* (1 ≤ *N* ≤ 100,000) days.

FJ wants to create a budget for a sequential set of exactly *M* (1 ≤ *M* ≤ *N*) fiscal periods called "fajomonths". Each of these fajomonths contains a set of 1 or more consecutive days. Every day is contained in exactly one fajomonth.

FJ's goal is to arrange the fajomonths so as to minimize the expenses of the fajomonth with the highest spending and thus determine his monthly spending limit.

Input

Line 1: Two space-separated integers: *N* and *M*

Lines 2..*N*+1: Line *i*+1 contains the number of dollars Farmer John spends on the *i*th day

Output

Line 1: The smallest possible monthly limit Farmer John can afford to live with.

Sample Input

7 5
100
400
300
100
500
101
400
Sample Output

500
Hint

If Farmer John schedules the months so that the first two days are a month, the third and fourth are a month, and the last three are their own months, he spends at most $500 in any month. Any other method of scheduling gives a larger minimum monthly limit.

Source

[USACO 2007 March Silver](http://poj.org/searchproblem?field=source&key=USACO+2007+March+Silver)

题意：

给出农夫在n天中每天的花费，要求把这n天分成连续的m组，

要求各组的花费之和应该尽可能地小，最后输出各组花费之和中的最大值


```cpp
#include<iostream>
using namespace std;

int n; //天数
int m; //规定的分组数

/*判断用当前的mid值能把天数n分成几组*/
/*通过比较group与m的大小，对mid值进行优化*/

bool judge_group(int mid,int money[])
{
	int sum=0;
	int group=1;    //当前mid值能把n天分成的组数(初始把全部天数作为1组)

	for(int i=1;i<=n;i++)  //从第一天开始向下遍历每天的花费
		if(sum+money[i]<=mid)  //当前i天之和<=mid时，把他们归并到一组
			sum+=money[i];
		else               //若 前i-1天之和 加上第i天的花费 大于mid
		{
			sum=money[i];  //则把前i-1天作为一组，第i天作为下一组的第一天
			group++;    //此时划分的组数+1
		}

	if(group>m)
		return false;   //若利用mid值划分的组数比规定的组数要多，则说明mid值偏小
	else
		return true;    //否则mid值偏大
}

int main(void)
{
	while(cin>>n>>m)
	{
		int* money=new int[n+1];  //每天花费的金额
		int low=0;  //下界
		int high=0; //上界

		for(int i=1;i<=n;i++)
		{
			cin>>money[i];

			high+=money[i];   //把所有天数的总花费作为上界high（相当于把n天都分作1组）
			if(low<money[i])
				low=money[i]; //把n天中花费最多的那一天的花费作为下界low（相当于把n天分为n组）
		}                     //那么要求的值必然在[low,high]范围内

		int mid=(low+high)/2;

		while(low<high)  //可能在low==high之前，分组数就已经=m，但是mid并不是最优，因此要继续二分
		{
			if(!judge_group(mid,money))
				low=mid+1;     //mid值偏小，下界前移
			else
				high=mid-1;    //mid值偏大，上界后移

			mid=(low+high)/2;
		}

		cout<<mid<<endl;  //二分搜索最后得到的mid值必然是使得分组符合要求的最优值

		delete money;
	}
	return 0;
}
```





