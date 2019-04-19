# 快速排序算法的Java实现 - 右上角同学的专栏 - CSDN博客
2016年06月29日 22:16:23[右上角徐](https://me.csdn.net/u011032983)阅读数：214
个人分类：[Java语法																[数据结构](https://blog.csdn.net/u011032983/article/category/6328409)](https://blog.csdn.net/u011032983/article/category/6297816)
```java
/**
 * 
 */
/**
 * @author xhl
 * 快速排序
 * 
 */
public class QuickSort {
	/**
	 * @param args
	 */
	int[] quicksort(int[] array, int l, int r) {
		int pivot = array[l];
		int j = r;
		int i = l;
		while (j > i) {
			while (j > i && array[j] >= pivot)
				j--;
			if (j > i)
				array[i] = array[j];
			while (j > i && array[i] <= pivot)
				i++;
			if (j > i)
				array[j] = array[i];
		}
		array[j] = pivot;
		if (j > l)
			quicksort(array, l, j - 1);
		if (j < r)
			quicksort(array, j + 1, r);
		return array;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int[] list = { 5, 5, 2, 7, 8, 2, 4, 10 };
		QuickSort o = new QuickSort();
		int[] listsort = o.quicksort(list, 0, list.length - 1);
		for (int i : listsort)
			System.out.println(i);
	}
}
```
