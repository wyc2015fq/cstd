# 剑指offer系列-T12_1全排列 - 右上角同学的专栏 - CSDN博客
2016年07月06日 22:06:56[右上角徐](https://me.csdn.net/u011032983)阅读数：103
1.普通全排列：给定一个具有n个元素的集合（n>=1），要求输出这个集合中元素的所有可能的排列。
```java
/**
 * @author xhl 全排列 全排列就是从第一个元素起每个数分别与它后面的元素(包括他自己)交换，一直到最后一个元素递归结束，输出数组。
 *         但这只是一种情况的输出，本次递归结束后还要继续和后面的其他元素交换，所以需要先再交换还原，再下一步递归。
 */
public class offerT12_1 {
	/**
	 * @param args
	 */
	void Swap(char[] array, int a, int b) {
		char temp = array[a];
		array[a] = array[b];
		array[b] = temp;
	}//实现元素交换，注意不能用Swap(char a, char b),调用此类型的函数实现不了交换，原因在于Java是值传递，所以用数组实现
	public void AllRange(char[] array, int index) {
		if (index == array.length - 1)
			System.out.println(array);// 计算到最后一个元素，不交换，输出数组，本次递归结束
		for (int i = index; i < array.length; i++) {
			Swap(array, index, i);// 与它后面的元素(包括他自己)交换
			AllRange(array, index + 1);// index后的元素也要与自己后面的交换，用递归实现
			Swap(array, index, i);// 还原，继续for循环，看下一种情况
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT12_1 o = new offerT12_1();
		char[] test = { 'a', 'b', 'c' };
		o.AllRange(test, 0);
	}
}
```
2.打印1到最大的n位数，全排列思想实现
需要考虑3个点，1：考虑到大数问题，不用int或long整型，用字符数组表示数2：用全排列的思想递归实现所有情况3：打印时不能输出00,01这样的数
```java
/**
 * @author xhl
 * 打印1到最大的n位数
 * 题目描述：
 * 输入数字n,按顺序打印出1到最大的n位十进制数。比如输入3,则打印出1、2、3一直到最大的3位数即999
 * 需要考虑3个点，1：考虑到大数问题，不用int或long整型，用字符数组表示数2：用全排列的思想递归实现所有情况3：打印时不能输出00,01这样的数
 */
public class offerT12_2 {
	/**
	 * @param args
	 */
	public void PrintToMaxOfNDigits(int n) {
		if (n <= 0)
			System.out.println("error");
		char[] test = new char[n];
		AllRange(test, 0);
	}
	void AllRange(char[] array, int index) {
		if (index == array.length)
			Printnumber(array);// 计算到最后一个元素，不交换，输出数组，本次递归结束
		else { // 这里就必须加上else了，上次是偷懒没有加，又恰好不加也能正确执行而已
			for (int i = 0; i < 10; i++) {
				array[index] = (char) (i + '0');// 这次array[index]不是和后面的元素交换来实现改变，而是设置0-9，但同样可认为是做改变
				AllRange(array, index + 1);// index后的元素也要设置0-9，用递归实现
				// 不需要还原即可继续for循环，因为这次直接能覆盖上次的改变
			}
		}
	}
	/* 打印，从第一个非0数开始打印，如果一直是0，直到i == array.length - 1，则概数是0，直接输出0 */
	void Printnumber(char[] array) {
		for (int i = 0; i < array.length; i++) {
			if (array[i] != '0') {
				System.out.println(String.valueOf(array).substring(i));
				break;
			} else if (i == array.length - 1)
				System.out.println('0');
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT12_2 o = new offerT12_2();
		o.PrintToMaxOfNDigits(3);
	}
}
```
