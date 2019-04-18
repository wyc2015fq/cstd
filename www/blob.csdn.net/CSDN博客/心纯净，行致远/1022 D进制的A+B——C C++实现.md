# 1022 D进制的A+B——C/C++实现 - 心纯净，行致远 - CSDN博客





2018年11月23日 01:45:23[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：29标签：[C																[C  																[PAT](https://so.csdn.net/so/search/s.do?q=PAT&t=blog)
个人分类：[【PAT_BasicLevel】](https://blog.csdn.net/zhanshen112/article/category/8241634)





# 题目

> 
1022 D进制的A+B （20 分）

输入两个非负 10 进制整数 A 和 B (≤2​30​​−1)，输出 A+B 的 D (1<D≤10)进制数。

### 输入格式：

输入在一行中依次给出 3 个整数 A、B 和 D。

### 输出格式：

输出 A+B 的 D 进制数。

### 输入样例：

```
123 456 8
```

### 输出样例：

```
1103
```


# 算法

很常见的题目，今天晚上考试第一次竟然还因为一个小错误检查半天……还是不够扎实，检查错误的能力也还不够。而且想的思路还不对！！！

2018.12.06

使用字符串记录，再反过来输出，也是一样的。见代码2。关键问题是理解“除基取余法”。

# 代码

代码1

```cpp
//PAT1022V1
#include <stdio.h>
#include <math.h>

int main(){
	int a,b,c;
	int i=0,j,d,s[100];
	scanf("%d %d %d",&a,&b,&d);
//	printf("%d %d %d",a,b,d);
	c=a+b;
	while(c){
		s[i++]=(int)c%d;
		c/=d;
	}
	for(j=i-1;j>=0;j--){
		printf("%d",s[j]);
	}
	if(i==0)	printf("0");
	return 0;
}
```

2、

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
int main(){
	int a,b,s,D;	cin>>a>>b>>D;
	s=a+b;
	string str;
	while(s/D>0){
		str+=s%D+'0';
		s=s/D;	
	};
	str+=s%D+'0';
	reverse(str.begin() ,str.end() );
	cout<<str;
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=C  &t=blog)](https://so.csdn.net/so/search/s.do?q=C&t=blog)




