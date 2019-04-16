# 长安大学第三届ACM-ICPC程序设计竞赛 E题 - 紫芝的博客 - CSDN博客





2018年04月26日 17:31:29[紫芝](https://me.csdn.net/qq_40507857)阅读数：47








                链接：[https://www.nowcoder.com/acm/contest/102/E](https://www.nowcoder.com/acm/contest/102/E)
来源：牛客网


## 题目描述


*PSacfc *is a student majors in Communication Engineering, and Wireless Mobile System is his favorite course.
 One day, when the teacher talked about Cellular Mobile Communication, he thought of a problem.
 Given *n* regular hexagons, find the minimum number of sides exposed. A side of a regular hexagon exposed means no other hexagons’ sides coincided with it. One side can be coincided with no more than one another.

  For example, the first situation has 3 hexagons with 12 sides exposed, and the second situation has 3 hexagons with 14 sides exposed. 

![](https://uploadfiles.nowcoder.com/images/20180407/304659_1523086332044_041C3777045B5CA1EE659C1452F75930)


  Your task is to find the minimum number of sides exposed with *n *regular hexagons.



## 输入描述:
The first line contains an integer number T, the number of test cases.
itℎ of each next T lines contains an integer number n(1 ≤ n ≤ 109).

## 输出描述:
For each test case print a number, minimum number of sides exposed.

示例1



## 输入

3
3
4
7



## 输出

12
14
18





```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int t;
	cin>>t;
	while(t--){
		long long n;
		cin>>n;
		long long ans=2*ceil(sqrt(12*n-3));
		printf("%lld\n",ans);
	}
	return 0;
}
```





