# HDU2069 Coin Change  组合数学 生成函数 - 紫芝的博客 - CSDN博客





2018年09月24日 21:45:18[紫芝](https://me.csdn.net/qq_40507857)阅读数：67








# Coin Change

Suppose there are 5 types of coins: 50-cent, 25-cent, 10-cent, 5-cent, and 1-cent. We want to make changes with these coins for a given amount of money. 


For example, if we have 11 cents, then we can make changes with one 10-cent coin and one 1-cent coin, or two 5-cent coins and one 1-cent coin, or one 5-cent coin and six 1-cent coins, or eleven 1-cent coins. So there are four ways of making changes for 11 cents with the above coins. Note that we count that there is one way of making change for zero cent. 


Write a program to find the total number of different ways of making changes for any amount of money in cents. Your program should be able to handle up to 100 coins. 

Input

The input file contains any number of lines, each one consisting of a number ( ≤250 ) for the amount of money in cents.

Output

For each input line, output a line containing the number of different ways of making changes with the above 5 types of coins.

Sample Input

11
26
Sample Output

4
13
题意：

有5种硬币，总数超过100个，问p元（0-350元）有多少种组合方法？

分析：

### **[生成函数](https://blog.csdn.net/qq_40507857/article/details/82824047)**

**一般生成函数每个因子都有一个起始系数n1[i]和终止系数n2[i]，这道题中每一种方案的硬币总量不超过100，所以要在原有生成函数的基础上分层处理，用a[i][j]表示组成i元、使用j个硬币的方案数**

```java
import java.util.*;
import java.math.*;

public class Main {
	static int maxn=300;// ( ≤250 ) for the amount of money
	static int maxm=110;//handle up to 100 coins
	public static int[][] a=new int[maxn][maxm];
	public static int[][] b=new int[maxn][maxm];
	//a[i][j]表示价值为i，硬币数量为j的方案数
	public static int[] n1=new int[maxn];
	public static int[] n2=new int[maxn];
	public static int[] v=new int[maxn];
	
	static long[] res=new long[maxn];
	static void init() {
		for(int i=0;i<maxn;i++) {
			for(int j=0;j<maxm;j++) {
				a[i][j]=0;
				b[i][j]=0;
			}
			
			n1[i]=0;
			n2[i]=0;
			v[i]=0;
			res[i]=0;
		}
		v[1]=1;v[2]=5;v[3]=10;v[4]=25;v[5]=50;
		
		a[0][0]=1;
		for(int i=1;i<=5;i++) {//一共5种硬币
			for(int j=0;j<=250;j++)//循环每个因子的每一项
				for(int k=0;j+k*v[i]<=250;k++)//循环a的每一项
					for(int p=0;k+p<=100;p++)//硬币总数不超过100
						b[j+k*v[i]][p+k]+=a[j][p];
			for(int k=0;k<=250;k++)
				for(int p=0;p<=100;p++) {
					a[k][p]=b[k][p];
					b[k][p]=0;
				}
		}
		for(int i=1;i<=250;i++) {
			for(int j=0;j<=100;j++)
				res[i]+=a[i][j];
		}
		res[0]=1;
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		init();
		while(cin.hasNext()) {
			int p=cin.nextInt();
			
			System.out.println(res[p]);
		}
		cin.close();
	}
}
```





