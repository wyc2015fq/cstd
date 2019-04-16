# 数据结构和算法系列 - c语言归并排序法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月22日 12:31:31[initphp](https://me.csdn.net/initphp)阅读数：816标签：[语言																[c																[merge																[n2																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[数据结构和算法系列](https://blog.csdn.net/initphp/article/category/1161961)





> 
插入排序算法采取增量式（Incremental）的策略解决问题，每次添一个元素到已排序的子序列中，逐渐将整个数组排序完毕，它的时间复杂度是O(n2)。下面介绍另一种典型的排序算法－－归并排序，它采取分而治之（Divide-and-Conquer）的策略，时间复杂度是Θ(nlgn)。归并排序的步骤如下：

- 
Divide: 把长度为n的输入序列分成两个长度为n/2的子序列。

- 
Conquer: 对这两个子序列分别采用归并排序。

- 
Combine: 将两个排序好的子序列合并成一个最终的排序序列。



在描述归并排序的步骤时又调用了归并排序本身，可见这是一个递归的过程。






```cpp
#include <stdio.h>

#define LEN 8
int a[LEN] = { 5, 2, 4, 7, 1, 3, 2, 6 };

void merge(int start, int mid, int end)
{
	int n1 = mid - start + 1;
	int n2 = end - mid;
	int left[n1], right[n2];
	int i, j, k;

	for (i = 0; i < n1; i++) /* left holds a[start..mid] */
		left[i] = a[start+i];
	for (j = 0; j < n2; j++) /* right holds a[mid+1..end] */
		right[j] = a[mid+1+j];

	i = j = 0;
	k = start;
	while (i < n1 && j < n2)
		if (left[i] < right[j])
			a[k++] = left[i++];
		else
			a[k++] = right[j++];

	while (i < n1) /* left[] is not exhausted */
		a[k++] = left[i++];
	while (j < n2) /* right[] is not exhausted */
		a[k++] = right[j++];
}

void sort(int start, int end)
{
	int mid;
	if (start < end) {
		mid = (start + end) / 2;
		printf("sort (%d-%d, %d-%d) %d %d %d %d %d %d %d %d\n", 
		       start, mid, mid+1, end, 
		       a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
		sort(start, mid);
		sort(mid+1, end);
		merge(start, mid, end);
		printf("merge (%d-%d, %d-%d) to %d %d %d %d %d %d %d %d\n", 
		       start, mid, mid+1, end, 
		       a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
	}
}

int main(void)
{
	sort(0, LEN-1);
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=n2&t=blog)](https://so.csdn.net/so/search/s.do?q=merge&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)




