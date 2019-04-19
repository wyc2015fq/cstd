# 堆排序算法的Java实现 - 右上角同学的专栏 - CSDN博客
2016年07月25日 17:44:47[右上角徐](https://me.csdn.net/u011032983)阅读数：324
```java
/**
 * @author xhl
 * 堆排序(本代码是小顶堆的实现)
 * 参照《数据结构》（严蔚敏版），算法10.10、10.11伪代码实现。
 * 思路：1.建堆：从一个无序序列建堆的过程就是一个反复“筛选”的过程。若将此序列看成是一个完全二叉树，则最后一个非终端结点是第n/2（n/2向下取整）个
 * 元素，由此“筛选”只需从第n/2（n/2向下取整）个元素开始。
 * 		2.输出堆顶元素并重新建堆：假设输出堆顶元素之后，以堆中最后一个元素替代之，此时根结点的左、右子树均为堆，则仅需自上至下进行调整即可。我们称自
 * 堆顶至叶子的调整过程为“筛选 ”。
 */
public class HeapSort {
	/* 筛选：自上至叶子的“筛选”过程。s节点与左右节点比较，将最小值和s交换（每次比较两次），如果发生交换行为，则要对被交换的节点继续向下筛选 */
	void HeapAdjust(int[] array, int s, int m) {
		int j = s * 2 + 1;
		if (j < m) {
			int tempindex = j;
			if (j + 1 < m && array[j] > array[j + 1])
				tempindex = j + 1;// 找出左右节点最小的一个
			if (array[tempindex] < array[s]) {
				int temp2 = 0;
				temp2 = array[s];
				array[s] = array[tempindex];
				array[tempindex] = temp2;// 用最小值和s处值交换
				HeapAdjust(array, tempindex, m);
			}
		}
	}
	void heap_sort(int[] array) {
		/*
		 * 建堆：把array建成小顶堆：从array.length/2-1(即最后一个非终端节点，另外在数组中计数从0开始，
		 * 如果从1计数，最后一个非终端节点是array.length/2)。筛选，一直到根节点
		 */
		for (int i = array.length / 2 - 1; i >= 0; i--) {
			HeapAdjust(array, i, array.length);
		}
		/* 输出头结点并重新调整堆：每输出一次根节点，将最后一个元素赋给根节点（相当于删除原根节点），再
		 * 自根节点向下筛选，筛选至倒数第二个元素 */
		for (int i = 0; i < array.length; i++) {
			System.out.println(array[0]);
			array[0] = array[array.length - 1 - i];
			HeapAdjust(array, 0, array.length - 1 - i);
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		HeapSort o = new HeapSort();
		int[] array = { 0, 4, 2, 5, 1, 8, 7, 9 };
		o.heap_sort(array);
	}
}
```
