# 算法笔记：快速排序(Merge Sort) - 彩虹糖的博客 - CSDN博客





2019年04月15日 21:35:02[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：5








一. 快排的基本思路

1. 给数组洗牌。(为什么要先让数组随机排列后续会讲)

2. 选择一个元素a[j]。如果数组是要从小到大排序，那么我们把所有小于a[j]的元素放到a[j]左侧，把所有大于a[j]的元素放到a[j]的右侧。

3. 对a[j]的左侧和右侧分别排序。

二. 具体实现

1. 如何实现上述第二项中的要求？

Repeat until i and j pointers cross.

    ・Scan i from left to right so long as (a[i] < a[lo]).

    ・Scan j from right to left so long as (a[j] > a[lo]).

    ・Exchange a[i] with a[j].

When pointers cross.

    ・Exchange a[lo] with a[j].

```java
private static int partition(Comparable[] a, int lo, int hi)
{
 int i = lo, j = hi+1;
 while (true)
 {
 while (less(a[++i], a[lo]))
 if (i == hi) break;
 while (less(a[lo], a[--j]))
 if (j == lo) break;

 if (i >= j) break;
 exch(a, i, j);
 }
 exch(a, lo, j);
 return j;
}
```

2. 完整实现代码

```java
public class Quick
{
 private static int partition(Comparable[] a, int lo, int hi)
 { /* see previous slide */ }
 public static void sort(Comparable[] a)
 {
 StdRandom.shuffle(a);
 sort(a, 0, a.length - 1);
 }
 private static void sort(Comparable[] a, int lo, int hi)
 {
 if (hi <= lo) return;
 int j = partition(a, lo, hi);
 sort(a, lo, j-1);
 sort(a, j+1, hi);
 }
}
```

三. 快速排序细节

1. 为什么要先对数组进行洗牌?

举一个worst case,

![](https://img-blog.csdnimg.cn/20190408110630825.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

在上诉算法的实现过程中，我们默认使用的是数组的第一个元素作为分组的标志，假如数组已经是排好序的，我们会发现，我们每次选中的都是第一个元素，左侧没有元素，而右侧是剩余的全部元素。

2. 平均的算法复杂度

![](https://img-blog.csdnimg.cn/20190408111211598.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

3. 快速排序是一种原地排序算法，但是不稳定。

三. 算法的提升

1. 提前排序

```java
private static void sort(Comparable[] a, int lo, int hi)
 {
 if (hi <= lo + CUTOFF - 1)
 {
 Insertion.sort(a, lo, hi);
 return;
 }
 int j = partition(a, lo, hi);
 sort(a, lo, j-1);
 sort(a, j+1, hi);
 }
```

和归并排序类似，我们也是在数组长度小到一定程度的时候直接使用插入排序，这样可以提高效率。

2. 取中位数

之前我们使用洗牌的方法确定分类标志，我们也可以通过取中位数的方式确定分类标志。

```java
private static void sort(Comparable[] a, int lo, int hi)
 {
 if (hi <= lo) return;
 int m = medianOf3(a, lo, lo + (hi - lo)/2, hi);
 swap(a, lo, m);
 int j = partition(a, lo, hi);
 sort(a, lo, j-1);
 sort(a, j+1, hi);
 }
```

四. 快速选择

我们可以在线性的时间复杂度内选择出第k大的元素吗？答案是肯定的。

借助快速排序的思路，我们可以通过选择分组标志，直到分组标志为k的时候结束。

```java
public static Comparable select(Comparable[] a, int k)
{
 StdRandom.shuffle(a);
 int lo = 0, hi = a.length - 1;
 while (hi > lo)
 {
 int j = partition(a, lo, hi);
 if (j < k) lo = j + 1;
 else if (j > k) hi = j - 1;
 else return a[k];
 }
 return a[k];
}
```

![](https://img-blog.csdnimg.cn/20190409093146320.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

五. 三路分组(3-way partitioning)

这种算法主要处理的是在很多项是重复的时候，如果在此时我们依然采用上述的算法，就可能落入n^2的算法复杂度，这个时候我们引入三路分组这个算法。

算法的具体细节可以看这篇博客 [三分法 3-way partitioning](https://blog.csdn.net/petersmart123/article/details/78419745)。简单来说就是要把数组分为三部分，即<,=,>

```java
private static void sort(Comparable[] a, int lo, int hi)
{
 if (hi <= lo) return;
 int lt = lo, gt = hi;
 Comparable v = a[lo];
 int i = lo;
 while (i <= gt)
 {
 int cmp = a[i].compareTo(v);
 if (cmp < 0) exch(a, lt++, i++);
 else if (cmp > 0) exch(a, i, gt--);
 else i++;
 }
 sort(a, lo, lt - 1);
 sort(a, gt + 1, hi);
}
```





