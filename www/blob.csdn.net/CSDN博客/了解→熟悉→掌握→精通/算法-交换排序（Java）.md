
# 算法 - 交换排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 15:42:30[Chimomo](https://me.csdn.net/chimomo)阅读数：113标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[交换排序](https://so.csdn.net/so/search/s.do?q=交换排序&t=blog)[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[](https://so.csdn.net/so/search/s.do?q=交换排序&t=blog)[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)个人分类：[Algorithm](https://blog.csdn.net/chimomo/article/category/1826645)
[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * In programming design, swap sort is one of basic sorting algorithms.
 * So-called swapping, is swapping the position of two records by comparing
 * their key. The trait of swap sort is: move the record whose key is bigger
 * towards the rear of sequence and move the record whose key is smaller to the
 * front of sequence.
 * </p>
 */
public class SwapSort {
    /**
     * The swap sort.
     *
     * @param a The array to be sorted
     */
    public static void sort(int[] a) {
        for (int i = 0; i < a.length - 1; i++) {
            for (int j = i + 1; j < a.length; j++) {
                if (a[i] > a[j]) {
                    int t = a[i];
                    a[i] = a[j];
                    a[j] = t;
                }
            }
            // Print each round of sorting.
            System.out.print("Round " + (i + 1) + ": ");
            for (int k = 0; k < a.length; k++) {
                System.out.print(k + " ");
            }
            System.out.println();
        }
    }
}
```


