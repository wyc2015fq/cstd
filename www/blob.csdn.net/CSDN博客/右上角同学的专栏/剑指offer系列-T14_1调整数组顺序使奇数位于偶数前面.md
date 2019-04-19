# 剑指offer系列-T14_1调整数组顺序使奇数位于偶数前面 - 右上角同学的专栏 - CSDN博客
2016年07月08日 11:09:07[右上角徐](https://me.csdn.net/u011032983)阅读数：191
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl 题目：调整数组顺序使奇数位于偶数前面
 * 题目描述：输入一个整数数组，实现一个函数来调整该数组中数字的顺序，
 * 使得所有的奇数位于数组的前半部分，所有的偶数位于位于数组的后半部分，
 * 并保证奇数和奇数，偶数和偶数之间的相对位置不变。
 * 思路：我能说我直接想到了快速排序吗，和快速排序思想一样，做法类似
 */
public class offerT14 {
	/**
	 * @param args
	 */
	void ReorderOddEven(int[] array) {
		int i = 0;
		int j = array.length - 1;
		if (array.length != 0) {
			while (i < j) {
				while ((array[j] & 0x1) == 0 && i < j)
					// 每次移动前都判断i < j
					j--;
				while ((array[i] & 0x1) != 0 && i < j)
					// 每次移动前都判断i < j
					i++;
				int temp = 0;
				if (i < j) {// 交换前也要判断i < j，并确定j,i是因为分别找到偶，奇才停下的
					temp = array[i];
					array[i] = array[j];
					array[j] = temp;
				}
			}
			for (int k : array) {// 增强型for循环
				System.out.print(k);
			}
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT14 o = new offerT14();
		int[] array = { 1, 2, 3, 4, 5, 6 };
		o.ReorderOddEven(array);
	}
}
```
后面看到要求保证奇数和奇数，偶数和偶数之间的相对位置不变。其实就是考察排序算法的稳定性，网上大多示例程序是冒泡法实现，其他稳定的排序方法还有基数排序，插入排序，归并排序，笔者选取插入排序，用递归和循环两种方式实现。代码如下：
```java
/* 保证奇数和奇数，偶数和偶数之间的相对位置不变。插入法递归实现 */
/*public class offerT14 {
	void Reorder(int[] array, int k) {
		if (k == array.length - 1)
			Print(array);
		else {
			int i = k - 1;
			int p = array[k];
			while ((isEven(p) == false && isEven(array[i]) == true) && i >= 0) {
				array[i + 1] = array[i];
				i--;
			}
			array[i + 1] = p;
			Reorder(array, k + 1);
		}
}
*/
/* 保证奇数和奇数，偶数和偶数之间的相对位置不变。插入法循环实现 */
/*public class offerT14 {
	void Reorder(int[] array) {
		for (int k = 1; k < array.length; k++) {
			int i = k - 1;
			int p = array[k];
			while ((isEven(p) == false && isEven(array[i]) == true) && i >= 0) {
				array[i + 1] = array[i];
				i--;
			}
			array[i + 1] = p;
		}
		Print(array);
	}
	boolean isEven(int a) {
		return (a & 0x1) == 0;
	}
	void Print(int[] array) {
		for (int k : array) {// 增强型for循环
			System.out.print(k);
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT14 o = new offerT14();
		int[] array = { 1, 2, 3, 4, 5, 6 };
		o.Reorder(array);
	}
}
*/
```
