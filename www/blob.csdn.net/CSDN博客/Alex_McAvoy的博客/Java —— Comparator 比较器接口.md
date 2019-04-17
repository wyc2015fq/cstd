# Java —— Comparator 比较器接口 - Alex_McAvoy的博客 - CSDN博客





2019年02月26日 15:50:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：19
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









Comparator 是 Java 中的接口，置于 java.util.Comparator 内，常用于以下两种情况：

```java
Arrays.sort(T[],Comparator<? super T> c);
Collections.sort(List<T> list,Comparator<? super T> c);
```

java.util.Comparator 接口只包括两个函数，其源码如下：

```java
package java.util;
public interface Comparator{
	int compare(T o1,T o2);
	boolean equals(Object obj);
}
```

若一个类要实现 java.util.Comparator 接口，则一定要实现 int compare(T o1, T o2) 函数，而 boolean equals(Object obj) 函数可以不实现。

int compare(T o1, T o2) 用于比较 o1、o2 的大小，如果返回值为负数，则说明 o1<o2，返回为正数，则说明 o1>o2，返回为零，则说明 o1=o2

假如 A 的值大于 B ，返回 1，此时调用 Collections.sort() 函数是升序排序，假如 A 的值大于 B，返回 -1，此时调用Collections.sort() 函数是降序排序

以对二维数组第二维进行排序为例

```java
import java.util.Arrays;
import java.util.Comparator;
 

public class MatrixSort {
    public static void main(String[] args) {
         
        //二维数组
        Integer[][] a = new Integer[][] {
            {8,7},
            {9,5},
            {6,4}
        };
     
        //排序
        Arrays.sort(a,new Comparator<Integer[]>() {
            @Override    //重写 sort 方法的标志，系统会检查方法的正确性
            public int compare(Integer[] x, Integer[] y) {
                if(x[0] < y[0]){
                    return 1;
                } else if(x[0] > y[0]){
                    return -1;
                } else {
                    return 0;
                }
            }
        });
         
        //打印
        for(Integer[] i: a){
            System.out.println(Arrays.toString(i));
        }
    }
}
```





