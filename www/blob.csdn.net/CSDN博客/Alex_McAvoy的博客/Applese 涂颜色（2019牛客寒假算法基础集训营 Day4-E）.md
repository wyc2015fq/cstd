# Applese 涂颜色（2019牛客寒假算法基础集训营 Day4-E） - Alex_McAvoy的博客 - CSDN博客





2019年02月02日 15:01:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：130








> 
# 【题目描述】

精通程序设计的 Applese 叕写了一个游戏。

在这个游戏中，有一个 n 行 m 列的方阵。现在它要为这个方阵涂上黑白两种颜色。规定左右相邻两格的颜色不能相同。请你帮它统计一下有多少种涂色的方法。由于答案很大，你需要将答案对 10^9+7 取模。

# 【输入描述】

仅一行两个正整数 n, m，表示方阵的大小。

1≤n,m≤10^100000

# 【输出描述】

输出一个正整数，表示方案数对 10^9+7 取模。

# 【样例】

示例1

输入

1 1

输出

2

示例2

输入

2 2

输出

4


思路：

由于仅要求左右相邻的两格颜色必须不同，因此行与行之间去独立的，故而先考虑每一行的涂色方法再考虑整张图的涂色方法即可

对于每一行来说，设黑色为 1，白色为 0，只有 2 种涂色方法，即：101010... 或 010101....

那么对于整体来说，即有 2^n 种方法

由于题目中 n、m 的范围过大，因此需要高精度，使用 Java、Python 均可

# 【源代码】

```java
import java.math.BigInteger;
import java.util.Scanner;

public class Main {
	public static void main(String[] args) {
		Scanner input=new Scanner(System.in);
		BigInteger n=input.nextBigInteger();
		BigInteger m=input.nextBigInteger();
		BigInteger two=BigInteger.valueOf(2);
		BigInteger mod=BigInteger.valueOf(1000000007);
		BigInteger res=two.modPow(n,mod);
		System.out.println(res.toString());
	}
}
```





