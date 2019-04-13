
# 算法 - 堆排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 15:18:35[Chimomo](https://me.csdn.net/chimomo)阅读数：73标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[堆排序](https://so.csdn.net/so/search/s.do?q=堆排序&t=blog)[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[](https://so.csdn.net/so/search/s.do?q=堆排序&t=blog)[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)个人分类：[Algorithm](https://blog.csdn.net/chimomo/article/category/1826645)
[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * The heap sort is a kind of selection sort, its time complexity is O(nlog2n).
 * </p>
 *
 * <p>
 * The trait of heap sort is: In the sorting process, treat the to-be-ordered
 * sequence as the sequential storage structure of a complete binary tree. Take
 * advantage of the inner relationships of parent nodes and child nodes in the
 * complete binary tree, select the biggest (or smallest) record from the
 * current unordered sequence.
 * </p>
 *
 * <p>
 * Basic thought:
 * </p>
 * <p>
 * 1. Adjust the to-be-ordered sequence to max root heap. The top element is the
 * biggest element in the max root heap.
 * </p>
 * <p>
 * 2. Swap the max root heap top element with the last element of the unordered
 * sequence, and enlist the last position of unordered sequence into the ordered
 * sequence, and then adjust the new unordered sequence to max root heap.
 * </p>
 * <p>
 * 3. Repeat the process until unordered sequence disappears.
 * </p>
 * <p>
 * Note: At the beginning, all the sequence are unordered sequence. Every
 * swapping, is swapping the top element of max root heap into ordered sequence,
 * to guarantee the ordered sequence is ordered.
 * </p>
 */
public class HeapSort {
    /**
     * The heap sort.
     *
     * @param a The array to be sorted
     */
    public static void sort(int[] a) {
        // Build max heap.
        buildMaxHeap(a);
        // Print max heap.
        for (int i : a) {
            System.out.print(i + "");
        }
        System.out.println();
        for (int i = a.length - 1; i > 0; i--) {
            // Swap the max root heap top element with the last element of the unordered sequence.
            int t = a[0];
            a[0] = a[i];
            a[i] = t;
            // Adjust the new unordered sequence to max root heap.
            maxHeaping(a, 0, i);
            // Print max root heap after each iteration.
            for (int j = 0; j < i; j++) {
                System.out.print(a[j] + " ");
            }
            System.out.println();
        }
    }
    /**
     * Build max heap from bottom to top. By the characteristic of complete
     * binary tree, the child node's subscript index starts from index =
     * a.length / 2, so adjust max heap from subscript index = (a.length / 2) -
     * 1.
     *
     * @param a The array to be sorted
     */
    private static void buildMaxHeap(int[] a) {
        for (int i = (a.length / 2) - 1; i >= 0; i--) {
            maxHeaping(a, i, a.length);
        }
    }
    /**
     * Adjust the specified node to heap.
     *
     * @param a The array to be sorted
     * @param i The specified node
     * @param heapSize The heap size, also the length of unordered sequence
     */
    private static void maxHeaping(int[] a, int i, int heapSize) {
        // Left child node
        int left = (2 * i) + 1;
        // Right child node
        int right = 2 * (i + 1);
        // Temp variable, store the large node.
        int large = i;
        // Comparing left child node.
        if (left < heapSize && a[left] > a[large]) {
            large = left;
        }
        // Comparing right child node.
        if (right < heapSize && a[right] > a[large]) {
            large = right;
        }
        // Swapping if child node is bigger than self, to make bigger element move upward.
        // And make the bigger element adjusted as heap to keep heap nature.
        if (i != large) {
            int t = a[i];
            a[i] = a[large];
            a[large] = t;
            maxHeaping(a, large, heapSize);
        }
    }
}
```


