
# 算法 - 选择排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 14:57:16[Chimomo](https://me.csdn.net/chimomo)阅读数：58标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[选择排序](https://so.csdn.net/so/search/s.do?q=选择排序&t=blog)[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[](https://so.csdn.net/so/search/s.do?q=选择排序&t=blog)[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)个人分类：[Algorithm](https://blog.csdn.net/chimomo/article/category/1826645)
[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * The selection sort is following:
 * </p>
 * <p>
 * Every round of sorting selecting the smallest or biggest element from the
 * to-be-sorted sequence, put it at the end of already-ordered sequence, until
 * all the elements of to-be-sorted sequence are sorted. Selection sort is not
 * stable.
 * </p>
 */
public class SelectionSort {
    /**
     * The selection sort.
     *
     * @param a The array to be sorted
     */
    public static void sort(int[] a) {
        for (int i = 0; i < a.length - 1; i++) {
            // Store the sbuscript index of the smallest element.
            int min = i;
            // Find the smallest element.
            for (int j = i + 1; j < a.length; j++) {
                if (a[j] < a[min]) {
                    min = j;
                }
            }
            // Put the samllest element at already-ordered sequence.
            int t = a[min];
            a[min] = a[i];
            a[i] = t;
            // Print each round of sorting.
            System.out.println(String.format("Round %d", i + 1));
            for (int k : a) {
                System.out.print(k + " ");
            }
            System.out.println();
        }
    }
}
```


