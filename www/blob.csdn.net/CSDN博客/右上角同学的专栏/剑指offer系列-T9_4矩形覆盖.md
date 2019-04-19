# 剑指offer系列-T9_4矩形覆盖 - 右上角同学的专栏 - CSDN博客
2016年07月01日 22:29:29[右上角徐](https://me.csdn.net/u011032983)阅读数：279
最近越来越意识到斐波拉契数列的普遍性，但是在辨识出是斐波拉契数列的同时也要注意，是否该问题有需要注意的细节，例如从n=多少才开始呈现斐波拉契规律。矩阵覆盖问题便是很好的例子。
```java
/**
 * @author xhl
 * 矩形覆盖
 * 题目描述
 * 我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩
 * 形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩
 * 形，总共有多少种方法？
 *
 */
public class offerT9_4 {
    public int RectCover(int target) {
		if (target == 0)
			return 0;
        if (target == 1)
			return 1;
        if (target == 2)
			return 2;
		int a0 = 1;
		int a1 = 2;
		//虽然判断出是斐波拉契数列，但要判断出从n=几时开始斐波拉契数列
		for (int i = 3; i <=target; i++) {// 只保存两个数，放置位置a0,a1
			int temp = a1;
			a1 = a0 + a1;
			a0 = temp;
		}
		return a1;
    }
}
```
