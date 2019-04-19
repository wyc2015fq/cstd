# 剑指offer系列-T29数组中出现次数超过一半的数字 - 右上角同学的专栏 - CSDN博客
2016年07月24日 16:16:48[右上角徐](https://me.csdn.net/u011032983)阅读数：106
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl
 * 数组中出现次数超过一半的数字
 * 题目描述：数组中有一个数字出现的次数超过数组长度的一半，请找出这个数字。例如输入一个长度为9的数组{1,2,3,2,2,2,5,4,2}。由于数字2在数组中出现了5次，超过数组长度的一半，因此输出2。如果不存在则输出0。
 * 思路：方法1 ：如果存在出现次数超过一半元素，则必定是中位数；方法2：“捆绑式排除”
 */
public class offerT29 {
	
	/*方法1，快速排序找到中位数
```
```java
<span style="white-space:pre">	时间复杂度：o(n),复杂度计算方法见博客《快速排序算法的时间复杂度分析[详解Master method]》</span>
```
```java
<span style="white-space:pre">	</span>*/
	public int getNumMethod_1(int array[]) {
		int middle = array.length >> 1;
		int start = 0;
		int end = array.length - 1;
		int pos = Partition(array, start, end);
		/* 判断快速排序的标兵是不是中位数，在中位数左边则继续快速排序右边部分；在中位数右边则继续排序左边部分 */
		while (pos != middle) {
			if (pos < middle) {
				start = pos + 1;
				pos = Partition(array, start, end);
			} else {
				end = pos - 1;
				pos = Partition(array, start, end);
			}
		}
		return array[pos];
	}
	/* 单步骤快速排序 */
	public int Partition(int array[], int start, int end) {
		int pivot = array[start];
		int i = start;
		int j = end;
		while (i < j) {
			while (i < j && array[j] >= pivot)
				j--;
			if (i < j)
				array[i] = array[j];
			while (i < j && array[i] <= pivot)
				i++;
			if (i < j)
				array[j] = array[i];
		}
		array[j] = pivot;
		return j;
	}
	/*
	 * 方法2：如果存在次数超过一半的元素，则该元素次数和一定比非该元素多。采用“捆绑式排除”，1个相同捆一个不同，2个相同捆2个不同，
	 * occurrence==0代表排除成功，继续下一组捆绑，最后resultNum标示的相同，可能是所求的超过一半的元素
```
```java
<span style="white-space:pre">	</span> * 时间复杂度o(n)
	 */
	public int getNumMethod_2(int array[]) {
		if (array.length == 0)
			return 0;
		int resultNum = array[0];
		int occurrence = 0;
		for (int i = 0; i < array.length; i++) {
			if (array[i] == resultNum)
				occurrence++;
			else {
				if (--occurrence == 0 && i + 1 < array.length)
					resultNum = array[i + 1];
			}
		}
		return resultNum;
	}
	public static void main(String[] args) {
		offerT29 o = new offerT29();
		int array[] = { 1, 1, 2, 3 };
		int count = 0;
		int count2 = 0;
		/*
		 * 方法1或2仅得到存在次数超过一半情况下，所求元素的结果；还可能本来就不存在该元素，得到的也就不满足条件，
		 * 所以要再检查一次得到的元素出现次数
		 */
		for (int num : array) {
			if (num == o.getNumMethod_1(array))
				count++;
			if (num == o.getNumMethod_2(array))
				count2++;
		}
		if (count > (array.length >> 1))
			System.out.println("Get Number Through Method_1: "
					+ o.getNumMethod_1(array));
		else
			System.out.println(0);
		if (count2 > array.length >> 1)
			System.out.println("Get Number Through Method_2: "
					+ o.getNumMethod_2(array));
		else
			System.out.println(0);
	}
}
```
