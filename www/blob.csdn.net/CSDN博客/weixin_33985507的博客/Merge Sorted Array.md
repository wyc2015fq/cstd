# Merge Sorted Array - weixin_33985507的博客 - CSDN博客
2016年12月02日 20:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
- 描述
Given two sorted integer arrays A and B, merge B into A as one sorted array.
Note:
You may assume that A has enough space to hold additional elements from B.
The number of elements initialized in A and B are m and n respectively.
- 
分析
传统的做法是用辅助数组C存放最终结果，从头开始逐一比较A和B的元素，然后插入到C中；
本题保证A有足够的空间，那么它并不想我们创建额外的空间，如果按照传统方法，比较，插入，那么每插入一个元素，数组A的其余元素都需要依次往后移一位，增加了时间复杂度；
既然A有足够的空间，那么可以对其预留的空间加以利用，从数组的后面开始扫描，那么完成比较，插入，后续不需要再移动任何元素了。
- 
时间复杂度O(m+n)，空间复杂度O(1)
```java
public class Solution {
    public int[] merge(int[] a, int b[]) {
        int i = a.length - 1;
        int j = b.length - 1;
        int k = a.length + b.length - 1;
        while (i >= 0 && j >= 0) {
           a[k--] = a[i] < b[j]? b[j--]:a[i--];
        }
        while (j >= 0) {
            a[k--] = b[j--];
        }
       return a;
    }
}
```
