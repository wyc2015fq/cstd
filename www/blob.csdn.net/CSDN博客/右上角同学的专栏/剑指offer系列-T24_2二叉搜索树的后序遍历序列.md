# 剑指offer系列-T24_2二叉搜索树的后序遍历序列 - 右上角同学的专栏 - CSDN博客
2016年07月14日 00:35:11[右上角徐](https://me.csdn.net/u011032983)阅读数：175
       本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl
 * 二叉搜索树的后序遍历序列
 * 题目描述
 * 输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。如果是则输出Yes,
 * 否则输出No。假设输入的数组的任意两个数字都互不相同。
 * 思路:递归实现。最后元素是根节点，左边起直到出现大于根的数之前，为左子树，继续判断
 * 如果直到根节点前都是大于根的数，则可继续递归判断左右子树，否则，若在到达根节点前
 * 又出现小数，则不是遍历结果，返回false。
 * 
 */
public class offerT24_2 {
	/**
	 * @param args
	 */
	public boolean VerifySquenceOfBST(int[] sequence) {
		return VerifySquenceOfBST(sequence, 0, sequence.length - 1);
	}
	public boolean VerifySquenceOfBST(int[] sequence, int start, int end) {
		if (sequence.length == 0) {
			return false;
		}
		if (sequence.length == 1) {
			return true;
		}
		/* 此处是关键，当只有2个元素，1个元素，无元素时返回true */
		if (end - start == 1 || end - start == 0 || end - start == -1)
			return true;
		int PSentinel = end;
		int Sentinel = sequence[end];// 哨兵，即根节点
		/* 寻找左子树 */
		int i = start;
		while (i < PSentinel && sequence[i] < Sentinel) {
			i++;
		}// 若无左子树元素，则依然i=start
		/* 从左子树结束处继续寻找右子树 */
		int j = i;
		while (j < PSentinel && sequence[j] > Sentinel) {
			j++;
		}
		/* 还未到哨兵前一个，就在右子树中出现比根节点小的数，导致跳出寻找 */
		if (j < PSentinel)// 正确时跳出来应该是PSentinel
			return false;
		/* 继续判断左右子树是否是搜索树 */
		return VerifySquenceOfBST(sequence, start, i - 1)
				&& VerifySquenceOfBST(sequence, j, end - 1);
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT24_2 o = new offerT24_2();
		int[] a = { 7, 4, 6, 5 };
		System.out.println(o.VerifySquenceOfBST(a));
	}
}
```
