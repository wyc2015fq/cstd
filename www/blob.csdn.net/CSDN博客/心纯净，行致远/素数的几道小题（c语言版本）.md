# 素数的几道小题（c语言版本） - 心纯净，行致远 - CSDN博客





2018年10月21日 09:31:56[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：59








求素数一直以来是c语言的经典题目，下面是PAT做题过程中想到的几道素数小题，整理如下：

# 1 求素数的两种方法

## 1.1 判断n是否能被1~n-1整除

```cpp
#include<stdio.h>
int main()
{
    int i, n;
    scanf("%d", &n);
    for (i = 2; i < n ; i++)
    {
        if (n%i == 0)
            break;
    }
        if (i < n) printf("This is not a prime.");
        else printf("This is a prime.");
        return 0;
}
```

## 1.2 判断n是否能被2~√n间的整数整除

```
#include<stdio.h>
#include<math.h>
int main()
{
    int n,i;
	double k;
    scanf("%d", &n);
    k = sqrt(n);
    for (i = 2; i <= k;i++)
	{
        if (n%i == 0) break;
	}
 
    if (i <=k) printf("This is not a prime.");
    else printf("This is a prime");
    return 0;
 
}
```

# 2 如何求N以内的所有素数

```cpp
//求出N以内的所有素数
#include <stdio.h> 
#include <math.h>

int main(){
	int n,i,j;	//n is the input num
	scanf("%d",&n);
	
	for(i=2;i<=n;i++){
		for(j=2;j<=sqrt(i);j++){
			if(i%j==0)
				break;	// mean n is not prime
		}
		if(j>sqrt(i))
		printf("%d	",i);
	} 
}
```

# 3 PAT1007

**1007 素数对猜想 （20 point(s)）**

让我们定义d​n​​为：d​n​​=p​n+1​​−p​n​​，其中p​i​​是第i个素数。显然有d​1​​=1，且对于n>1有d​n​​是偶数。“素数对猜想”认为“存在无穷多对相邻且差为2的素数”。

现给定任意正整数`N`(<10​5​​)，请计算不超过`N`的满足猜想的素数对的个数。

### 输入格式:

输入在一行给出正整数`N`。

### 输出格式:

在一行中输出不超过`N`的满足猜想的素数对的个数。

### 输入样例:

```
20
```

### 输出样例:

```
4
```

程序如下：

```
//PAT1007V1
 #include <stdio.h>
 #include <math.h>
 
 int main(){
 	int n,i,j,t=0,count=0;
 	int a[10001];
 	scanf("%d",&n);
 	
 	for(i=2;i<=n;i++){
		for(j=2;j<=sqrt(i);j++){
			if(i%j==0)
				break;	// mean i is not prime
		}
		if(j>sqrt(i))	//mean i is prime
		{
			a[t++]=i;
		}
	}

	for(i=0;i<t-1;i++)
		for(j=i+1;j<t;j++){
			if(a[j]-a[i]==2)
				count++;
		}
	printf("%d",count);
	
	
 }
```





