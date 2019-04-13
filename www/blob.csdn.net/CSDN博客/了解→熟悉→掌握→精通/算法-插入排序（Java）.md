
# 算法 - 插入排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 14:46:59[Chimomo](https://me.csdn.net/chimomo)阅读数：58标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[插入排序](https://so.csdn.net/so/search/s.do?q=插入排序&t=blog)[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[](https://so.csdn.net/so/search/s.do?q=插入排序&t=blog)[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)个人分类：[Algorithm](https://blog.csdn.net/chimomo/article/category/1826645)
[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * Straight insertion sort is following:
 * </p>
 * <p>
 * Every time take out the first element from unordered sequence, insert it into
 * ordered sequence to make it ordered until all the elements in unordered
 * sequence inserted to ordered sequence.
 * </p>
 * <p>
 * Scan the first two elements in the first round, and insert the second element
 * into ordered sequence orderly. The second round, scan the first three
 * elements in the sequence, insert the third element into the ordered sequence
 * orderly. Repeat the process, scan n-1 round, then all the elements are
 * sorted.
 * </p>
 *
 * <p>
 * Straight insertion sort is stable, the worst time complexity is O(n^2), space
 * complexity is O(1).
 * </p>
 *
 * <p>
 * Straight insertion sort is composed of two layers of nested loops. The outer
 * loop identify and determine the element to be compared, the inner loop
 * determine the element's final position. Straight insertion sort is compare
 * the element with its preceding element, so the outer loop started from the
 * second element. Move backward the preceding element when the preceding
 * element is bigger than the to-be-compared element, continue the inner loop
 * until find the smaller preceding element and insert the to-be-compared
 * element after the smaller preceding element.
 * </p>
 *
 * <p>
 * Noted that, we must use a storage space to store the to-be-compared element,
 * since when a round of comparison completes, we need to insert the
 * to-be-compared element to the position that preceding element is smaller than
 * the to-be-compared element. Straight insertion sort is like arranging cards
 * when playing poker.
 * </p>
 *
 */
public class InsertionSort {
    /**
     * The straight insertion sort.
     *
     * @param a The array to be sorted
     */
    public static void sort(int[] a) {
        // Iterate from the second element.
        for (int i = 1; i < a.length; i++) {
            // Store to-be-comparted element.
            int t = a[i];
            int j = i - 1;
            // Find the insertion position from the current element right side.
            while (a[j] > t) {
                // Move backforward.
                a[j + 1] = a[j];
                j--;
                if (j == -1) {
                    break;
                }
            }
            // The move-forwarded elements are already moved forward, it will leave a empty position, this position is the insertion point of the to-be-compared element.
            a[j + 1] = t;
            // Print each round of insertion sort.
            for (int e : a) {
                System.out.print(e + " ");
            }
            System.out.println();
        }
    }
}
```


