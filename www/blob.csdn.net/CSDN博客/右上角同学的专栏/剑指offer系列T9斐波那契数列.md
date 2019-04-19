# 剑指offer系列T9斐波那契数列 - 右上角同学的专栏 - CSDN博客
2016年06月30日 22:04:13[右上角徐](https://me.csdn.net/u011032983)阅读数：199
```java
/**
 * @author xhl 斐波那契数列 题目描述 大家都知道斐波那契数列， 现在要求输入一个整数n，请你输出斐波那契数列的第n项。 n>=0
 *         改程序时间复杂度o(n)
 * 
 */
public class offerT9 {
	int Fibonacci(int n) {
		if (n == 0)
			return 0;
		int a0 = 0;
		int a1 = 1;
		for (int i = 0; i < n - 1; i++) {// 只保存两个数，放置位置a0,a1
			int temp = a1;
			a1 = a0 + a1;
			a0 = temp;
		}
		return a1;// 除了要求输出n=0外，其他第n个都是输出第n个{a0,a1}组里的a1,另外
					//因为初始时已经有一组a0,a1了，所以后面只需计算n-1次
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT9 o = new offerT9();
		System.out.println(o.Fibonacci(3));
	}
}
```
