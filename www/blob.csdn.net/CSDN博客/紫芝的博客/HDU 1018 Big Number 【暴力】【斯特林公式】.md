# HDU 1018 Big Number 【暴力】【斯特林公式】 - 紫芝的博客 - CSDN博客





2018年09月10日 18:45:10[紫芝](https://me.csdn.net/qq_40507857)阅读数：35








# Big Number

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 43485    Accepted Submission(s): 21282****

Problem Description

In many applications very large integers numbers are required. Some of these applications are using keys for secure transmission of data, encryption, etc. In this problem you are given a number, you have to determine the number of digits in the factorial of the number.

Input

Input consists of several lines of integer numbers. The first line contains an integer n, which is the number of cases to be tested, followed by n lines, one integer 1 ≤ n ≤ 107 on each line.

Output

The output contains the number of digits in the factorial of the integers appearing in the input.

Sample Input

2

10

20

Sample Output

7 19

Source

[Asia 2002, Dhaka (Bengal)](http://acm.hdu.edu.cn/search.php?field=problem&key=Asia+2002%2C+Dhaka+%28Bengal%29&source=1&searchmode=source)

Recommend

JGShining

### 题意：

**给你一个数，让你求出N！由多少位数构成，比如输出10，它的阶乖是3628800 由7位数构成，这时你要输出7**

### 分析：

### 方法1：暴力求解

10^m=a,a的位数为m+1，(int)log10(a)+1就是a的位数

**log10(n!)=log10(1*2*3*4.......*(n-1)*n)=log10(1)+log10(2)+log10(3)+log10(4)+.......+log10(n)**

**n!的位数为 floor(log(n!))+1**

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e7+7;
typedef long long LL;
//double num[maxn];//内存超限

int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        double ans=0.0;
        for(int i=1;i<=n;i++)
        ans+=log10(i);
        printf("%d\n",(int)ans+1);
    }
}
```

方法二：斯特林公式

![](https://img-blog.csdn.net/20180910201937576?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

利用Stirling公式计算n!结果的位数时，可以两边取对数(底数为e），得: 

![](https://img-blog.csdn.net/20180910202327177?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180910202515140?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e7+7;
typedef long long LL;
//double num[maxn];
#define e 2.71828
#define pi acos(-1)
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        double ans=0.0;
        ans=log10(2.0*pi*n)/2+n*(log10(n)-1/log(10));
        printf("%d\n",(int)ans+1);
    }
}
//10000000
```

方法三：java大数【超时】

```java
import java.util.*;
import java.math.*;

public class Main{
	
	public static void main(String[] args){
		Scanner cin=new Scanner(System.in);
		int T=cin.nextInt();
		while((T--)!=0) {
			int n=cin.nextInt();
			BigInteger ans=BigInteger.ONE;
			for(int i=1;i<=n;i++) {
				ans=ans.multiply(BigInteger.valueOf(i));
			}
			System.out.println(ans.toString().length());
		}
		cin.close();
	}
}
```





