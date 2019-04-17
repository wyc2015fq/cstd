# 排序算法及其Java代码实现 - 博客堂 - CSDN博客





2017年02月24日 17:17:21[最小森林](https://me.csdn.net/u012052268)阅读数：298标签：[java																[排序算法																[冒泡排序																[希尔排序																[插入排序](https://so.csdn.net/so/search/s.do?q=插入排序&t=blog)
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)





# 排序算法及其Java代码实现



- [排序算法及其Java代码实现](#排序算法及其java代码实现)- [冒泡排序 重要](#1冒泡排序-重要)
- [选择排序 重要](#2选择排序-重要)
- [插入排序](#3插入排序)
- [快速排序](#4快速排序)
- [希尔排序](#5希尔排序)

- [查找算法及其Java代码实现](#查找算法及其java代码实现)- [基本查找](#基本查找)
- [二分查找折半查找](#二分查找折半查找)





## 1.冒泡排序 （重要）

冒泡排序基本概念是：
- 依次比较相邻的两个数，将小数放在前面，大数放在后面。
- 即在第一趟：首先比较第1个和第2个数，将小数放前，大数放后。
- 然后比较第2个数和第3个数，将小数放前，大数放后，如此继续，
- 直至比较最后两个数，将小数放前，大数放后。至此第一趟结束，
- 将最大的数放到了最后。在第二趟：仍从第一对数开始比较
- （因为可能由于第2个数和第3个数的交换，使得第1个数不再小于第2个数），
- 将小数放前，大数放后，一直比较到倒数第二个数（倒数第一的位置上已经是最大的），第二趟结束，在倒数第二的位置上得到一个新的最大数
- （其实在整个数列中是第二大的数）。如此下去，重复以上过程，直至最终完成排序。 

```
public class BubbleSort {

    public static void sort(int[] data) {

        for (int i = 0; i < data.length - 1; i++) {

            for (int j = 0; j < data.length - 1 - i; j++) {

                if (data[j] > data[j + 1]) {

                    SortTest.swap(data, j, j + 1);
                }
            }
        }
    }
}
```

## 2.选择排序 （重要）

选择排序基本思路： 

 * 把第一个元素依次和后面的所有元素进行比较。 

 * 第一次结束后，就会有最小值出现在最前面。 

 * 依次类推
```
public class SelectionSort {
    public static void sort(int[] data) {
        for (int x = 0; x < data.length - 1; x++) {
            for (int y = x + 1; y < data.length; y++) {
                if (data[y] < data[x]) {
                    SortTest.swap(data, x, y);
                }
            }
        }
    }
}
```

## 3.插入排序
- 将n个元素的数列分为已有序和无序两个部分，如插入排序过程示例下所示： 　　
- {{a1}，{a2，a3，a4，…，an}} 　　
- {{a1⑴，a2⑴}，{a3⑴，a4⑴ …，an⑴}} 　
- {{a1(n-1），a2(n-1) ，…},{an(n-1)}} 　　
- 每次处理就是将无序数列的第一个元素与有序数列的元素从后往前逐个进行比较，
- 找出插入位置，将该元素插入到有序数列的合适位置中。

```
public class InsertSort {

    public static void sort(int[] data) {

        for (int i = 1; i < data.length; i++) {

            for (int j = i; (j > 0) && (data[j] < data[j - 1]); j--) {

                SortTest.swap(data, j, j - 1);
            }
        }
    }
}
```

## 4.快速排序

一趟快速排序的算法是： 　　 

 * 1）设置两个变量i、j，排序开始的时候：i=0，j=N-1； 　　 

 * 2）以第一个数组元素作为关键数据，赋值给key，即 key=A[0]； 　　 

 * 3）从j开始向前搜索，即由后开始向前搜索（j=j-1即j–）， 

 * 找到第一个小于key的值A[j]，A[i]与A[j]交换； 　　 

 * 4）从i开始向后搜索，即由前开始向后搜索（i=i+1即i++）， 找到第一个大于key的A[i]，A[i]与A[j]交换； 　　 

 * 5）重复第3、4、5步，直到 I=J； 
(3,4步是在程序中没找到时候j=j-1，i=i+1，直至找到为止。找到并交换的时候i， j指针位置不变。另外当i=j这过程一定正好是i+或j-完成的最后令循环结束。） 

```java
public class QuickSort {
    public static void sort(int[] data) {
        quickSort(data, 0, data.length - 1);
    }

    private static void quickSort(int[] data, int i, int j) {
        int pivotIndex = (i + j) / 2;
        // swap
        SortTest.swap(data, pivotIndex, j);

        int k = partition(data, i - 1, j, data[j]);
        SortTest.swap(data, k, j);
        if ((k - i) > 1)
            quickSort(data, i, k - 1);
        if ((j - k) > 1)
            quickSort(data, k + 1, j);

    }

    /**
     * @param data
     * @param i
     * @param j
     * @return
     */
    private static int partition(int[] data, int l, int r, int pivot) {
        do {
            while (data[++l] < pivot)
                ;
            while ((r != 0) && data[--r] > pivot)
                ;
            SortTest.swap(data, l, r);
        } while (l < r);
        SortTest.swap(data, l, r);
        return l;
    }
}
```

## 5.希尔排序

希尔排序：先取一个小于n的整数d1作为第一个增量， 

 * 把文件的全部记录分成（n除以d1）个组。所有距离为d1的倍数的记录放在同一个组中。 

 * 先在各组内进行直接插入排序；然后，取第二个增量d2
```java
public class ShellSort {
    public static void sort(int[] data) {
        for (int i = data.length / 2; i > 2; i /= 2) {
            for (int j = 0; j < i; j++) {
                insertSort(data, j, i);
            }
        }
        insertSort(data, 0, 1);
    }

    /**
     * @param data
     * @param j
     * @param i
     */
    private static void insertSort(int[] data, int start, int inc) {
        for (int i = start + inc; i < data.length; i += inc) {
            for (int j = i; (j >= inc) && (data[j] < data[j - inc]); j -= inc) {
                SortTest.swap(data, j, j - inc);
            }
        }
    }
}
```

# 查找算法及其Java代码实现

## 基本查找

针对数组无序的情况

```
public static int getIndex(int[] arr,int value) {
                int index = -1;

                for(int x=0; x<arr.length; x++) {
                    if(arr[x] == value) {
                        index = x;
                        break;
                    }
                }

                return index;
            }
```

## 二分查找(折半查找)

针对数组有序的情况(千万不要先排序，在查找)

```
public static int binarySearch(int[] arr,int value) {
                int min = 0;
                int max = arr.length-1;
                int mid = (min+max)/2;

                while(arr[mid] != value) {
                    if(arr[mid] > value) {
                        max = mid - 1;
                    }else if(arr[mid] < value) {
                        min = mid + 1;
                    }

                    if(min > max) {
                        return -1;
                    }

                    mid = (min+max)/2;
                }

                return mid;
            }
```](https://so.csdn.net/so/search/s.do?q=希尔排序&t=blog)](https://so.csdn.net/so/search/s.do?q=冒泡排序&t=blog)](https://so.csdn.net/so/search/s.do?q=排序算法&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




