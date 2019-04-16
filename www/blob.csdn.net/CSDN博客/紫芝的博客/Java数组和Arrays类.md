# Java数组和Arrays类 - 紫芝的博客 - CSDN博客





2018年12月10日 17:31:48[紫芝](https://me.csdn.net/qq_40507857)阅读数：1395








> 
一维数组的两种定义方法

一维数组的三种遍历方法


```java
import java.util.Arrays;
import java.util.Scanner;

public class Main {
    static int maxn = (int) 1e5 + 10;

    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);

        //数组的两种定义方式
        int[] arr = new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};//声明的同时进行初始化
        int[] brr = new int[10];//先声明，后初始化，使用系统提供的默认值0

        //数组遍历的三种方法
        for (int i = 0; i < arr.length; i++)//方法1
             System.out.println(arr[i]);

        //方法2
        for (int x : arr)//元素类型：当前使用的数组名
            System.out.println(x);

        //方法3
        System.out.println(Arrays.toString(arr));//Arrays类中的方法
        //[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        System.out.println(Arrays.toString(brr));
        
        cin.close();
    }
}
```

> 
冒泡排序


```java
import java.util.Arrays;
import java.util.Scanner;

public class Main {
    static int maxn = (int) 1e5 + 10;

    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);

        //数组的两种定义方式
        int[] arr = new int[]{9, 6, 3, 8, 5, 2, 7, 4, 1, 0};
      	 //声明的同时进行初始化
      	 
        System.out.println(Arrays.toString(arr));
        
        for (int i = 0; i < arr.length; i++) {
        //外层循环控制排序趟数
            for (int j = i + 1; j < arr.length; j++) {
            //内层循环控制每一趟排序的次数
                if (arr[i] > arr[j]) {//从小到大排序
                    int tmp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = tmp;
                }
            }
        }
        System.out.println(Arrays.toString(arr));
        //逆序shu
        int i = 0, j = arr.length - 1;
        while (i < j) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
        System.out.println(Arrays.toString(arr));
        cin.close();
    }
}
```



