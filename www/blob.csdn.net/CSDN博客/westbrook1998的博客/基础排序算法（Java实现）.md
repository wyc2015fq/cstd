# 基础排序算法（Java实现） - westbrook1998的博客 - CSDN博客





2018年04月03日 23:11:34[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39








### 冒泡排序

```java
package com.zxc.sort;

/**
 * 冒泡排序
 * @author russell
 */
public class Bubble {

    public static void sort(Comparable[] a){
        int n=a.length;
        //两个循环遍历所有元素
        for(int i=0;i<n-1;i++){
            for(int j=i+1;j<n;j++){
                //依次比较相邻的两个元素，小的放前面
                if(less(a[j],a[i])){
                    exch(a,i,j);
                }
            }
        }
    }

    private static void exch(Comparable[] a,int i, int j){
        Comparable t=a[j];
        a[j]=a[i];
        a[i]=t;
    }

    private static boolean less(Comparable a, Comparable b){
        return a.compareTo(b)<0;
    }

    private static void show(Comparable[] a){
        for(Comparable i:a){
            System.out.print(i+" ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        Integer[] integers={2,5,1,8,7,4,9,12,3};
        sort(integers);
        show(integers);
    }
}
```

### 选择排序

```java
package com.zxc.sort;

/**
 * 选择排序
 * @author russell
 */
public class Selection {

    public static void sort(Comparable[] a){
        int n=a.length;

        for(int i=0;i<n;i++){
            int min=i;
            //找出i右边最小元素
            for(int j=i+1;j<n;j++){
                if(less(a[j],a[min])){
                    min=j;
                }
            }
            //每一轮循环将i右边最小元素与i交换
            exch(a,i,min);
        }
    }

    private static void exch(Comparable[] a,int i, int j){
        Comparable t=a[j];
        a[j]=a[i];
        a[i]=t;
    }

    private static boolean less(Comparable a, Comparable b){
        return a.compareTo(b)<0;
    }

    private static void show(Comparable[] a){
        for(Comparable i:a){
            System.out.print(i+" ");
        }
        System.out.println();
    }


    public static void main(String[] args) {
        Integer[] integers={2,5,1,8,7,4,9,12,3};
        sort(integers);
        show(integers);
    }
}
```

### 插入排序

```java
package com.zxc.sort;

/**
 * 插入排序
 * @author russell
 */
public class Insertion {

    /**
     * 普通插入排序
     * @param a
     */
    public static void sort(Comparable[] a){
        int n=a.length;
        for(int i=1;i<n;i++){
            //从第二个元素开始从前一个元素比较，如果该元素小，则交换，
            //交换到前面的所有元素都小于该元素为止，至此，包括该元素的前面所有元素排序完成
            for(int j=i; j>0 && less(a[j],a[j-1]);j--){
                exch(a,j,j-1);
            }
        }
    }

    /**
     * 优化插入排序
     * @param a
     */
    public static void sort_new(Comparable[] a){
        int n=a.length;
        for(int i=1;i<n;i++){
            //先用一个临时变量保存要插入到前面的元素
            Comparable t=a[i];
            int j;
            //内循环中将较大的元素依次向右移动一位而不是总交换两个元素
            for(j=i;j>0 && less(t,a[j-1]);j--){
                a[j]=a[j-1];
            }
            //插入到正确位置
            a[j]=t;
        }
    }

    private static void exch(Comparable[] a,int i, int j){
        Comparable t=a[j];
        a[j]=a[i];
        a[i]=t;
    }

    private static boolean less(Comparable a, Comparable b){
        return a.compareTo(b)<0;
    }

    private static void show(Comparable[] a){
        for(Comparable i:a){
            System.out.print(i+" ");
        }
        System.out.println();
    }


    public static void main(String[] args) {
        Integer[] integers={2,5,1,8,7,4,9,12,3};
        //sort(integers);
        sort_new(integers);
        show(integers);
    }
}
```

### 希尔排序

```java
package com.zxc.sort;

/**
 * 希尔排序
 * @author russell
 */
public class Shell {

    public static void sort(Comparable[] a){
        int n=a.length;
        int h=1;
        //确定h
        while(h<n/3){
            h=3*h+1;
        }
        //当h=1时，就是部分有序的插入排序（比直接插入排序快）
        while(h>=1){
            for(int i=h;i<n;i++){
                for(int j=i;j>h-1 && less(a[j],a[j-h]);j-=h){
                    exch(a,j,j-h);
                }
            }
            h/=3;
        }
    }

    private static void exch(Comparable[] a,int i, int j){
        Comparable t=a[j];
        a[j]=a[i];
        a[i]=t;
    }

    private static boolean less(Comparable a, Comparable b){
        return a.compareTo(b)<0;
    }

    private static void show(Comparable[] a){
        for(Comparable i:a){
            System.out.print(i+" ");
        }
        System.out.println();
    }


    public static void main(String[] args) {
        Integer[] integers={2,5,1,8,7,4,9,12,3};
        sort(integers);
        show(integers);
    }
}
```

### 归并排序

```java
package com.zxc.sort;

/**
 * 归并排序（自顶向下）
 * @author russell
 */
public class Merge {

    /**
     * 辅助数组
     */
    private static Comparable[] aux;

    public static void sort(Comparable[] a) {
        aux = new Comparable[a.length];
        sort(a, 0, a.length - 1);
    }

    private static void sort(Comparable[] a, int lo, int hi) {
        if (hi <= lo) {
            return;
        }
        int mid = lo + (hi - lo) / 2;
        sort(a, lo, mid);
        sort(a, mid + 1, hi);
        merge(a, lo, mid, hi);
    }

    public static void merge(Comparable[] a, int lo, int mid, int hi) {
        int i = lo, j = mid + 1;
        //将排序数组复制到辅助数组中
        for (int k = lo; k <= hi; k++) {
            aux[k] = a[k];
        }
        //将辅助数组的元素重新归并到原数组
        for (int k = lo; k <= hi; k++) {
            //左半边元素已取完，只取右边
            if (i > mid) {
                a[k] = aux[j++];
            }
            //右半边元素以取完，只取左边
            else if (j > hi) {
                a[k] = aux[i++];
            }
            //右边元素小，取右边
            else if (less(aux[j], aux[i])) {
                a[k] = aux[j++];
            }
            //左边元素小，取左边
            else {
                a[k] = aux[i++];
            }
        }
    }

    private static void exch(Comparable[] a, int i, int j) {
        Comparable t = a[j];
        a[j] = a[i];
        a[i] = t;
    }

    private static boolean less(Comparable a, Comparable b) {
        return a.compareTo(b) < 0;
    }

    private static void show(Comparable[] a) {
        for (Comparable i : a) {
            System.out.print(i + " ");
        }
        System.out.println();
    }


    public static void main(String[] args) {
        Integer[] integers = {5, 4, 6};
        sort(integers);
        show(integers);
    }
}
```

```java
package com.zxc.sort;

/**
 * 归并排序（自底向上）
 * @author russell
 */
public class MergeBU {

    /**
     * 辅助数组
     */
    private static Comparable[] aux;

    public static void sort(Comparable[] a) {
        int n=a.length;
        aux = new Comparable[n];
        for(int sz=1;sz<n;sz*=2){
            for(int lo=0;lo<n-sz;lo+=2*sz){
                merge(a,lo,lo+sz-1,Math.min(lo+sz+sz-1,n-1));
            }
        }
    }


    public static void merge(Comparable[] a, int lo, int mid, int hi) {
        int i = lo, j = mid + 1;
        //将排序数组复制到辅助数组中
        for (int k = lo; k <= hi; k++) {
            aux[k] = a[k];
        }
        //将辅助数组的元素重新归并到原数组
        for (int k = lo; k <= hi; k++) {
            //左半边元素已取完，只取右边
            if (i > mid) {
                a[k] = aux[j++];
            }
            //右半边元素以取完，只取左边
            else if (j > hi) {
                a[k] = aux[i++];
            }
            //右边元素小，取右边
            else if (less(aux[j], aux[i])) {
                a[k] = aux[j++];
            }
            //左边元素小，取左边
            else {
                a[k] = aux[i++];
            }
        }
    }

    private static void exch(Comparable[] a, int i, int j) {
        Comparable t = a[j];
        a[j] = a[i];
        a[i] = t;
    }

    private static boolean less(Comparable a, Comparable b) {
        return a.compareTo(b) < 0;
    }

    private static void show(Comparable[] a) {
        for (Comparable i : a) {
            System.out.print(i + " ");
        }
        System.out.println();
    }


    public static void main(String[] args) {
        Integer[] integers = {5, 4, 6};
        sort(integers);
        show(integers);
    }
}
```

### 快速排序

```java
package com.zxc.sort;

/**
 * 快速排序
 * @author russell
 */
public class Quick {

    public static void sort(Comparable[] a){
        sort(a,0,a.length-1);
    }

    private static void sort(Comparable[] a,int lo,int hi){
        if(hi<=lo){
            return;
        }
        //获取切分元素位置，此时j左边元素都比j小，右边都比j大
        int j=partition(a,lo,hi);
        //递归分别对两边进行排序
        sort(a,lo,j-1);
        sort(a,j+1,hi);
    }

    private static int partition(Comparable[] a,int lo,int hi){
        int i=lo,j=hi+1;
        Comparable v=a[lo];
        while(true){
            //元素比v小并且i指针未到数组末尾，则继续循环
            //直到元素比v大（需要交换）,或者i指针已到数组末尾（无需交换，后面直接判断跳出内循环）
            while(less(a[++i],v)){
                if(i==hi){
                    break;
                }
            }
            //元素比v大并且j指针未到数组头部，则继续循环
            //直到元素比v小（需要交换）,或者j指针已到数组头部（无需交换，后面直接判断跳出内循环）
            while(less(v,a[--j])){
                if(j==lo){
                    break;
                }
            }
            //跳出，进行循环外的最后一次交换
            if(i>=j){
                break;
            }
            //没跳出，将指针遍历过程中不符合条件的元素交换
            exch(a,i,j);
        }
        //将切分元素位置与j指针交换，（切分元素本身没有变），最后返回该切分位置，即j指针位置
        exch(a,lo,j);
        return j;
    }

    private static void exch(Comparable[] a,int i, int j){
        Comparable t=a[j];
        a[j]=a[i];
        a[i]=t;
    }

    private static boolean less(Comparable a, Comparable b){
        return a.compareTo(b)<0;
    }

    private static void show(Comparable[] a){
        for(Comparable i:a){
            System.out.print(i+" ");
        }
        System.out.println();
    }


    public static void main(String[] args) {
        Integer[] integers={5,4,6};
        sort(integers);
        show(integers);
    }
}
```



