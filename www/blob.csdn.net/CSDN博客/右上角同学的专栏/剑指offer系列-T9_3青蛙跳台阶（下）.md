# 剑指offer系列-T9_3青蛙跳台阶（下） - 右上角同学的专栏 - CSDN博客
2016年06月30日 22:07:46[右上角徐](https://me.csdn.net/u011032983)阅读数：300
```java
/**
 * @author xhl 跳台阶 题目描述 一只青蛙一次可以跳上1级台阶，也可以跳上2级……也可以跳n级。 求该青蛙跳上一个n级的台阶总共有多少种跳法。
 *         n>=1
 *         被注释的部分是for循环实现，时间复杂度是o(n),但空间复杂度是o(n)，
 *         第二种递归实现，时间复杂度是o(2^n)，空间复杂度为o(1)
 */
public class offerT9_3 {
	/*int FrogJump(int stepnum) {
	int[] n = new int[stepnum];
	n[0] = 1;
	for (int i = 1; i < stepnum; i++)
		n[i] = 0;
	for (int i = 1; i < stepnum; i++) {
		for (int j = i - 1; j >= 0; j--)
			n[i] = n[i] + n[j];
		n[i] = n[i] + 1;
	}
	return n[stepnum - 1];
}*/
	int FrogJump(int stepnum) {
		int n=0;
		if(stepnum==1)
			n=1;
		else{
		for(int i=stepnum-1;i>=1;i--)
			n=n+FrogJump(i);
		n=n+1;
		}
		return n;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT9_3 o = new offerT9_3();
		System.out.println(o.FrogJump(4));
	}
}
```
