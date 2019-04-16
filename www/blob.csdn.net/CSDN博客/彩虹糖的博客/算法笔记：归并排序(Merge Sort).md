# 算法笔记：归并排序(Merge Sort) - 彩虹糖的博客 - CSDN博客





2019年04月09日 09:53:30[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：2








# 基本思路



1. 把数组分成两部分。

2. 递归地排序这两个数组。

3. 将数组的两部分合并。



# 目标与实现代码

**目标**：Given two sorted subarrays a[lo] to a[mid] and a[mid+1] to a[hi], replace with sorted subarray a[lo] to a[hi]

```java
private static void merge(Comparable[] a, Comparable[] aux, int lo, int mid, int hi)
{
 assert isSorted(a, lo, mid); // precondition: a[lo..mid] sorted
 assert isSorted(a, mid+1, hi); // precondition: a[mid+1..hi] sorted
 for (int k = lo; k <= hi; k++)
 aux[k] = a[k];
 int i = lo, j = mid+1;
 for (int k = lo; k <= hi; k++)
 {
 if (i > mid) a[k] = aux[j++];
 else if (j > hi) a[k] = aux[i++];
 else if (less(aux[j], aux[i])) a[k] = aux[j++];
 else a[k] = aux[i++];
 }
 assert isSorted(a, lo, hi); // postcondition: a[lo..hi] sorted
}
```

# 归并排序代码实现

```java
public class Merge
{
 private static void merge(...)
 { /* as before */ }

 private static void sort(Comparable[] a, Comparable[] aux, int lo, int hi)
 {
 if (hi <= lo) return;
 int mid = lo + (hi - lo) / 2;
 sort(a, aux, lo, mid);
 sort(a, aux, mid+1, hi);
 merge(a, aux, lo, mid, hi);
 }
 public static void sort(Comparable[] a)
 {
 aux = new Comparable[a.length];
 sort(a, aux, 0, a.length - 1);
 }
}
```

# 算法实现分析



与后面要讲到的快速排序（Quick Sort）相比，归并排序算法的代码实现可以说是十分简单和容易理解的。归并排序的过程就是一个先分再合的过程。

merge函数能成功运行的前提是参与merge的两个数组是有序的，而sort函数则似乎是什么都没有做，仅仅是把数组分成两部分再交给merge。通过这两个函数，可以成功进行排序的原因是单个元素一定是有序的，我们不断地分割再分割，最后一定会得到长度为1的数组，从这里开始，merge函数开始发挥作用，两个长度为1的数组合并为长度为2的数组，再不断合成更大的数组，最终完成整个数组的排序。

# 算法复杂度分析



**时间复杂度**

**![](https://img-blog.csdnimg.cn/20190406105716326.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)**

**空间复杂度**

通过算法的代码实现，我们很容易发现，归并排序需要一个额外的大小为N的数组去辅助我们排序。



# 归并排序提升



**1. 对于小数组直接排序**

在归并排序中，我们产生了太多长度极小的数组，事实上，在数组长度小于一定值之后（大约可以取7），我们可以直接进行排序，这样可以获得一定的提升。（）

```java
private static void sort(Comparable[] a, Comparable[] aux, int lo, int hi)
{
 if (hi <= lo + CUTOFF - 1)
 {
 Insertion.sort(a, lo, hi);
 return;
 }
 int mid = lo + (hi - lo) / 2;
 sort (a, aux, lo, mid);
 sort (a, aux, mid+1, hi);
 merge(a, aux, lo, mid, hi);
}
```

**2. 及时停止排序**

举个例子，我们想要把一个数组从小到大排序，假设我们已经完成了对数组左半部分与右半部分的排序，如果我们这个时候我们发现，左半部分的最大值，小于右半部分的最小值，那么显然，整个数组已经排好序了，这个时候，我们没有必要进行merge操作。

```java
private static void sort(Comparable[] a, Comparable[] aux, int lo, int hi)
{
 if (hi <= lo) return;
 int mid = lo + (hi - lo) / 2;
 sort (a, aux, lo, mid);
 sort (a, aux, mid+1, hi);
 if (!less(a[mid+1], a[mid])) return;
 merge(a, aux, lo, mid, hi);
}
```

**3. 避免数组的来回拷贝**

这个操作很有趣，要我写，肯定出错误。前面提到归并排序需要一个辅助数组，而且从代码中，我们可以看到我们需要把原数组中的元素不断拷贝到辅助数组中去。那么我们能不能避免这种操作呢？答案是肯定的，解决方案是不再区分原数组和辅助数组，两个数组交替使用。这个操作可谓是艺高人胆大。

```java
private static void merge(Comparable[] a, Comparable[] aux, int lo, int mid, int hi)
{
 int i = lo, j = mid+1;
 for (int k = lo; k <= hi; k++)
 {
 if (i > mid) aux[k] = a[j++];
 else if (j > hi) aux[k] = a[i++];
 else if (less(a[j], a[i])) aux[k] = a[j++];
 else aux[k] = a[i++];
 }
}
private static void sort(Comparable[] a, Comparable[] aux, int lo, int hi)
{
 if (hi <= lo) return;
 int mid = lo + (hi - lo) / 2;
 sort (aux, a, lo, mid);
 sort (aux, a, mid+1, hi);
 merge(a, aux, lo, mid, hi);
}
```

# Bottom-up mergesort

这个是归并排序的非递归实现方法，但是速度上会比递归的要慢一些。

```java
public class MergeBU
{
 private static void merge(...)
 { /* as before */ }
 public static void sort(Comparable[] a)
 {
 int N = a.length;
 Comparable[] aux = new Comparable[N];
 for (int sz = 1; sz < N; sz = sz+sz)
 for (int lo = 0; lo < N-sz; lo += sz+sz)
 merge(a, aux, lo, lo+sz-1, Math.min(lo+sz+sz-1, N-1));
 }
}
```

# 排序的稳定性

插入排序和归并排序是稳定的，而选择排序和希尔排序是不稳定的。





