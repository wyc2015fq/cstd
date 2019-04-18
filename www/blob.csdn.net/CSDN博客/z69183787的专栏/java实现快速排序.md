# JAVA实现快速排序 - z69183787的专栏 - CSDN博客
2013年04月08日 11:56:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2023
**快速排序的基本思想**：
         通过一趟排序将待排序记录分割成独立的两部分，其中一部分记录的关键字均比另一部分关键字小，则分别对这两部分继续进行排序，直到整个序列有序。
       先看一下这幅图：
![](http://hi.csdn.net/attachment/201202/23/0_1329967950X8ro.gif)
把整个序列看做一个数组，把第零个位置看做中轴，和最后一个比，如果比它小交换，比它大不做任何处理；交换了以后再和小的那端比，比它小不交换，比他大交换。这样循环往复，一趟排序完成，左边就是比中轴小的，右边就是比中轴大的，然后再用分治法，分别对这两个独立的数组进行排序。
**[html]**[view
 plain](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)[copy](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)
- public int getMiddle(Integer[] list, int low, int high) {  
-         int tmp = list[low];    //数组的第一个作为中轴  
-         while (low <high) {  
-             while (low <high && list[high] >= tmp) {  
-                 high--;  
-             }  
-             list[low] = list[high];   //比中轴小的记录移到低端  
-             while (low <high && list[low]<=tmp) {  
-                 low++;  
-             }  
-             list[high] = list[low];   //比中轴大的记录移到高端  
-         }  
-         list[low] = tmp;              //中轴记录到尾  
-         return low;                   //返回中轴的位置  
-     }  
       递归形式的分治排序算法：
**[html]**[view
 plain](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)[copy](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)
- public void _quickSort(Integer[] list, int low, int high) {  
-         if (low <high) {  
-             int middle = getMiddle(list, low, high);  //将list数组进行一分为二  
-             _quickSort(list, low, middle - 1);        //对低字表进行递归排序  
-             _quickSort(list, middle + 1, high);       //对高字表进行递归排序  
-         }  
-     }  
**[html]**[view
 plain](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)[copy](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)
- public void quick(Integer[] str) {  
-         if (str.length > 0) {    //查看数组是否为空  
-             _quickSort(str, 0, str.length - 1);  
-         }  
-     }  
   编写测试方法：
**[html]**[view
 plain](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)[copy](http://blog.csdn.net/wangkuifeng0118/article/details/7286332#)
- public class TestMain {  
- 
-     /**  
-      * @param args  
-      */  
-     public static void main(String[] args) {  
-         // TODO Auto-generated method stub  
-          Integer[] list={34,3,53,2,23,7,14,10};  
-          QuicSort qs=new QuicSort();  
-          qs.quick(list);  
-          for(int i=0;i<list.length;i++){  
-              System.out.print(list[i]+" ");  
-          }  
-          System.out.println();  
-     }  
- 
- }  
     看一下打印结果吧：
     2 3 7 10 14 23 34 53 
     这样就排序好了，快速排序是对冒泡排序的一种改进，平均时间复杂度是O(nlogn)。
