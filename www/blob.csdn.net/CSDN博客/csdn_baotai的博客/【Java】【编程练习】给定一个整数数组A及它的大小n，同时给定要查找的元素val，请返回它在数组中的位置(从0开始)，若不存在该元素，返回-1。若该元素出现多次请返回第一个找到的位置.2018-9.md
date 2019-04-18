# 【Java】【编程练习】给定一个整数数组A及它的大小n，同时给定要查找的元素val，请返回它在数组中的位置(从0开始)，若不存在该元素，返回-1。若该元素出现多次请返回第一个找到的位置.2018-9 - csdn_baotai的博客 - CSDN博客

2018年09月28日 20:37:02[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：346


## 题目描述：

给定一个整数数组A及它的大小n，同时给定要查找的元素val，请返回它在数组中的位置(从0开始)，若不存在该元素，返回-1。若该元素出现多次，请返回第一个找到的位置

## 代码实现

```java
```java
import java.util.Arrays;
import java.util.Scanner;
public class Main {
    static int binarySearch(int[] A, int n,int val) {
         int lo = 0;
         int hi = n-1;
         while(hi>=lo){
            int mid = lo + (hi - lo) / 2;
            if (val < A[mid])
                hi = mid - 1;
            else if (val > A[mid]) 
                lo = mid + 1;
            else {
                // 该if用于解决相邻元素相同的情况
                if (mid > 0 && A[mid-1] == A[mid]) 
                    return mid-1;
                else 
                    return mid;
            }
        	  
    	}
        return -1;
    }
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int[] array = Arrays.stream(input.nextLine().replace("[", "").replace("]", "").split(",")).mapToInt(Integer::parseInt).toArray();
        int val = Integer.parseInt(input.nextLine());
        System.out.print(binarySearch(array,array.length, val));
    }
}
```
```

