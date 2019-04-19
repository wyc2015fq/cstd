# 剑指offer系列-T9_2青蛙跳台阶（上） - 右上角同学的专栏 - CSDN博客
2016年06月30日 22:06:37[右上角徐](https://me.csdn.net/u011032983)阅读数：201
```java
/**
 * @author xhl
 * 跳台阶
 * 题目描述
 *一只青蛙一次可以跳上1级台阶，也可以跳上2级。
 *求该青蛙跳上一个n级的台阶总共有多少种跳法。
 */
public class offerT9_2 {
	/**
	 * @param args
	 * 
	 */
	int FrogJump(int n) {
		if (n == 1)
			return 1;
		int a1 = 1;
		int a2 = 2;
		for (int i = 3; i <=n ; i++) {// 只保存两个数，放置位置a0,a1
			int temp = a2;
			a2 = a2 + a1;//跳n个台阶的跳法
			a1 = temp;
		}
		return a2;// 除了要求输出n=0外，其他第n个都是输出第n个{a0,a1}组里的a1,另外
					//因为初始时已经有一组a0,a1了，所以后面只需计算n-1次
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT9_2 o = new offerT9_2();
		System.out.println(o.FrogJump(3));
	}
}
```
