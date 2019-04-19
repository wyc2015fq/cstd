# 排序算法：归并排序（C、Java） - Koma Hub - CSDN博客
2018年05月08日 17:25:20[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：63
个人分类：[Algorithm																[Java](https://blog.csdn.net/Rong_Toa/article/category/7156200)](https://blog.csdn.net/Rong_Toa/article/category/7221428)
归并排序
归并排序的思想是将数组分成两部分，分别进行排序，然后归并起来。归并方法将数组中两个已经排序的部分归并成一个。
![](https://img-blog.csdn.net/20180508172550886)
**自顶向下归并排序**
![](https://img-blog.csdn.net/20180508172240654)
因为每次都将问题对半分成两个子问题，而这种对半分的算法复杂度一般为 O(NlogN)，因此该归并排序方法的时间复杂度也为 O(NlogN)。
小数组的递归操作会过于频繁，可以在数组过小时切换到插入排序来提高性能。
**自底向上归并排序**
先归并那些微型数组，然后成对归并得到的子数组。
```cpp
#include<stdio.h>
#include<stdlib.h>
#define min(a,b) a<b?a:b
char tmp[] = {};
/*字符串长度*/
int length(char *in)
{
    int i=0;
    while(in[i] != '\0')
        i++;
    return i;
}
/*输出字符串*/
void showString(char *str)
{
    int N = length(str);
    int i=0;
    while(i<N)
    {
        printf("%c ",str[i]);
        i++;
    }
    printf("\n");
}
/*归并方法*/
void merge(char *str, int low, int mid, int height)
{
    int i = low, j = mid + 1;
    int k;
    for(k=low;k<=height;k++)
    {
        tmp[k] = str[k];
    }
    for(k=low;k<=height;k++)
    {
        if(i>mid)
            str[k] = tmp[j++];
        else if(j>height)
            str[k] = tmp[i++];
        else if(tmp[i] <= str[j])
            str[k] = tmp[i++];
        else str[k] = tmp[j++];
    }
    showString(str);
}
/*从顶到下的归并排序*/
void tdSort(char *str, int low, int height)
{
    if(height<=low)return;
    int mid = low + (height - low)/2;
    tdSort(str, low, mid);
    tdSort(str, mid+1, height);
    merge(str, low, mid, height);
}
/*从底到上的归并排序*/
void buSort(char *str)
{
    int N = length(str);
    int sz, lo;
    for (sz = 1; sz < N; sz += sz) 
    {
        for (lo = 0; lo < N - sz; lo += sz + sz) 
        {
            merge(str, lo, lo + sz - 1, min(lo + sz + sz - 1, N - 1));
        }
    }
}
int main(int argc, char **argv)
{
    char str[] = "bcefad";
    
    showString(str);
    tdSort(str, 0, length(str)-1);
    //buSort(str);
    showString(str);
    
    return 1;
}
```
以下Java代码转自：[https://github.com/CyC2018/Interview-Notebook/](https://github.com/CyC2018/Interview-Notebook/)
```java
/**
 * Merge Sort
 * https://github.com/CyC2018/Interview-Notebook/
 */
public class MergeSort {
    
    private int[] aux;
    /**
     * Merge归并
     */
    private void merge(int[] a, int lo, int mid, int hi) 
    {
        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++) 
        {
            aux[k] = a[k]; // copy data to tmp array
        }
        for (int k = lo; k <= hi; k++) 
        {
            if (i > mid) a[k] = aux[j++];
            else if (j > hi) a[k] = aux[i++];
            else if (aux[i]<=a[j]) a[k] = aux[i++]; // for stable
            else a[k] = aux[j++];
        }
    }
    /**
     * Top-down merge sort自顶向下并归排序
     */
    public void tdsort(int[] a) 
    {
        aux = new int[a.length];
        tdsort(a, 0, a.length - 1);
    }
    private void tdsort(int[] a, int lo, int hi) 
    {
        aux = new int[a.length];
        if (hi <= lo) return;
        int mid = lo + (hi - lo) / 2;
        tdsort(a, lo, mid);
        tdsort(a, mid + 1, hi);
        merge(a, lo, mid, hi);
    }
    /**
     * Bottom up merge sort自底向上并归排序
     */
    public void busort(int[] a) 
    {
        int N = a.length;
        aux = new int[N];
        for (int sz = 1; sz < N; sz += sz) 
        {
            for (int lo = 0; lo < N - sz; lo += sz + sz) 
            {
                merge(a, lo, lo + sz - 1, Math.min(lo + sz + sz - 1, N - 1));
            }
        }
    }
    public void show(int[] a)
    {
        int N = a.length;
        for(int i=0;i<N;i++)
        {
            System.out.print(a[i]+" ");
        }
        System.out.println();
    }
    static public void main(String[]args)
    {
        int a[] = {5,4,3,2,1};
        MergeSort ms = new MergeSort();
        ms.show(a);
        ms.tdsort(a);
        ms.show(a);
    }
}
```
