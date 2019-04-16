# HDU2515 Yanghee 的算术 - 紫芝的博客 - CSDN博客





2018年09月24日 17:51:43[紫芝](https://me.csdn.net/qq_40507857)阅读数：77








Yanghee 是一个小学生。他的数学老师给全班同学布置了一道家庭作业，即根据 


一张不超过5000的n（n<50）个正整数组成的数表，两两相加得到n(n-1)/2个和，然后把它们排序。例如，如果数表含有四个数1，3，4，9，那么正确答案是4，5，7，10，12，13。Yanghee 做完作业以后和小伙伴们出去玩了一下午，回家以后发现老师给的数表不见了，可是他算出的答案还在。你能帮助Yanghee根据他的答案计算出原来的数表吗？ 

Input

输入第1行是1个正整数N,3<=n<50.然后有若干行,每行10个正整数,共计n(n-1)/2个数. 输入的数据有唯一解. 

Output

按从小到大输出n个数x[1],x[2],...,x[n],每行1个,使得任意2个数之和恰是输入的n(n-1)/2个数. 

Sample Input

```
15
3 4 5 6 7 8 9 10 11 12
13 14 15 16 5 6 7 8 9 10
11 12 13 14 15 16 17 7 8 9
10 11 12 13 14 15 16 17 18 9
10 11 12 13 14 15 16 17 18 19
11 12 13 14 15 16 17 18 19 20
13 14 15 16 17 18 19 20 21 15
16 17 18 19 20 21 22 17 18 19
20 21 22 23 19 20 21 22 23 24
21 22 23 24 25 23 24 25 26 25
26 27 27 28 29
```

Sample Output

```
1 
2 
3 
4 
5 
6 
7 
8 
9 
10 
11 
12 
13 
14 
15
```

分析：

计算出第一个数num1，然后a[0]到a[n-1]（第一行）减去第一个数就是答案了

![](https://img-blog.csdn.net/20180924174802613?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```java
import java.util.*;
import java.math.*;

public class Main {
	public static int maxn=1300,maxm=60;
	static int[] a=new int[maxn];
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int n=cin.nextInt();
		int m=n*(n-1)/2;
		for(int i=0;i<m;i++)
			a[i]=cin.nextInt();
		//int num1=(a[0]+a[1]-a[n-1])/2;
		int num1=(a[1]-a[0]);
		System.out.println(num1);
		for(int i=0;i<n-1;i++)
			System.out.println(a[i]-num1);
		cin.close();
	}
}
```





