# Java实现算法导论中求解模线性方程解（基于最大公约数欧几里得扩展算法） - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月28日 10:56:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2066
个人分类：[Algorithm																[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)](https://blog.csdn.net/fjssharpsword/article/category/6309933)

所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)








基于最大公约数欧几里得扩展算法求解算法导论中模线性方程解。具体要结合算法导论中的有关数论算法章节理解，具体代码如下：



```java
package cn.ansj;

/*
  假设方程ax=b(mod n)有解，且x0是方程的任意一个解，
  则该方程对模n恰有d个不同的解(d=gcd(a,n))，分别为：xi=x0+i*(n/d) mod n 。
 */
public class ModularLinearEquation {
	static int x,y;
	public static int extend_gcd(int a, int b){
		if (b == 0){
	        x = 1;
	        y = 0;
	        return a;
	    }
	    int re = extend_gcd(b, a % b);
	    int tmp = x;
	    x = y;
	    y = tmp - a / b * y;
	    return re;
	}
	
	public static void main(String args[]) { 
		int a=777;
		int b=66;
		int n=6;
		int d=extend_gcd(a,n);
		System.out.println("最大公约数d="+d+" And x="+x+" And y="+y);
		if(b%d!=0) System.out.println("no solution");
		else{
			int x0=x*(b/d)%n;
			System.out.println("x0="+x0);
			for(int i=1;i<d;i++)
				System.out.println("solution:"+(x0+i*(n/d))%n);
		}		  
	}
}
```


执行结果：



```
最大公约数d=3 And x=1 And y=-129
x0=4
solution:0
solution:2
```






