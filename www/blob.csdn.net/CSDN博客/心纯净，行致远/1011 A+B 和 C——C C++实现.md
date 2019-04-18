# 1011 A+B 和 C——C/C++实现 - 心纯净，行致远 - CSDN博客





2018年11月21日 16:38:25[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：48








# 题目

> 
1011 A+B 和 C （15 point(s)）

给定区间 [−2​31​​,2​31​​] 内的 3 个整数 A、B 和 C，请判断 A+B 是否大于 C。

### 输入格式：

输入第 1 行给出正整数 T (≤10)，是测试用例的个数。随后给出 T 组测试用例，每组占一行，顺序给出 A、B 和 C。整数间以空格分隔。

### 输出格式：

对每组测试用例，在一行中输出 `Case #X: true` 如果 A+B>C，否则输出 `Case #X: false`，其中 `X` 是测试用例的编号（从 1 开始）。

### 输入样例：

```
4
1 2 3
2 3 4
2147483647 0 2147483646
0 -2147483648 -2147483647
```

### 输出样例：

```
Case #1: false
Case #2: true
Case #3: true
Case #4: false
```


# 算法

没什么难的，主要是考察数据类型的大小。另外如果数据足够大，大到连double和long long int也无法存储，就按照加法的逻辑进行进位计算即可。

# 代码

C++实现

```cpp
#include <iostream>
using namespace std;

int main(){
	double a,b,c;
	int T;	cin>>T;
	for(int i=0;i<T;i++){
		cin>>a>>b>>c;
		printf("Case #%d: %s\n",i+1,a+b>c?"true":"false");
	}
	return 0;
}
```

C实现

```cpp
//PAT1011V2
#include <stdio.h>
int main(){
	int t,i=1;
	double a,b,c;
	scanf("%d",&t);
	while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF){
	//	scanf("%ld %ld %ld",&a,&b,&c);
		if((a+b)>c)
			printf("Case #%d: true\n",i++);
		else
			printf("Case #%d: false\n",i++);
	}
	return 0;
}
```





