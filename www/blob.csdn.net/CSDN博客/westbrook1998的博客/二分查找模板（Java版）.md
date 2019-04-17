# 二分查找模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月25日 19:56:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：59标签：[二分查找](https://so.csdn.net/so/search/s.do?q=二分查找&t=blog)
个人分类：[Java算法模板](https://blog.csdn.net/westbrook1998/article/category/8228296)









Java里没有c++的lower_bound和upper_boud，要自己写

```java
import java.util.Arrays;
import java.util.Queue;
import java.util.Scanner;
import java.util.concurrent.LinkedBlockingDeque;

public class Main {
    /**
     * 二分查找 当v在nums中出现时返回出现的第一个位置，否则返回一个idx，在这里插入v时nums仍有序
     * @param nums 非降序数组
     * @param l
     * @param r
     * @param v
     * @return
     */
    static int lowerBound(int[] nums,int l,int r,int v){
        while(l<r){
            int m=l+((r-l)>>1);
            if(nums[m]>=v){
                r=m;
            }
            else{
                l=m+1;
            }
        }
        return l;
    }

    /**
     * 二分查找 当v在nums中出现时返回出现的最后一个位置的后一个位置，否则返回一个idx，在这里插入v时nums仍有序
     * @param nums 非降序数组
     * @param l
     * @param r
     * @param v
     * @return
     */
    static int upperBound(int[] nums,int l,int r,int v){
        while(l<r){
            int m=l+((r-l)>>1);
            System.out.println(l+" "+m+" "+r);
            if(nums[m]>v){
                r=m;
            }
            else{
                l=m+1;
            }
        }
        return l;
    }
    public static void main(String[] args) {
        int[] nums=new int[]{2,4,1,0,4,2,5};
        Arrays.sort(nums);
        for(int i:nums){
            System.out.print(i+" ");
        }
        System.out.println();
        System.out.println(lowerBound(nums,0,nums.length,4));
        System.out.println(upperBound(nums,0,nums.length,4));
    }
}
```



