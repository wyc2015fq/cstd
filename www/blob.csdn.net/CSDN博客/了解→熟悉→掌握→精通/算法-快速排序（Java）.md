
# 算法 - 快速排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 14:44:53[Chimomo](https://me.csdn.net/chimomo)阅读数：115


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * Quick Sort is an improvement of Bubble Sort. Its basic thought is: Separate
 * the data to two independent parts by one round sorting, of which one part
 * data is smaller than the other part. And then perform Quick Sort separately
 * on the two parts, the whole process can be recursive, hereby the whole data
 * becomes sorted sequence.
 * </p>
 *
 * <p>
 * Assume the array is a[0]...a[N-1], first arbitrarily select one element
 * (usually set the first element as pivot) as pivot, then put all the elements
 * smaller than the pivot before the pivot, put all the elements bigger than the
 * pivot after the pivot, this process is called one trip Quick Sort. Note that
 * Quick Sort is not stable, i.e. the relative positions of the equal elements
 * may changed after Quick Sort.
 * </p>
 *
 * One trip Quick Sort algorithm is designed as following:
 * <li>
 * 1. Set two variable i and j, i=0, j=N-1.
 * </li>
 * <li>
 * 2. Set the first element as pivot, i.e. pivot=a[0].
 * </li>
 * <li>
 * 3. Search forward from j(j--), find the first element that a[j] less than
 * pivot, assign a[j] to a[i].
 * </li>
 * <li>
 * 4. Search backward from i(i++), find the first element that a[i] greater than
 * pivot, assign a[i] to a[j].
 * </li>
 * <li>
 * 5. Repeat step3 and step4 until i==j. Then, pivot=a[i], return i.
 * </li>
 *
 * The average time complexity of Quick Sort is O(nlog2n).
 *
 */
public class QuickSort {
    /**
     * The quick sort.
     *
     * @param a The array to be sorted
     * @param low The sorting start position
     * @param high The sorting end position
     */
    public static void sort(int[] a, int low, int high) {
        System.out.println("In Quick Sort:");
        if (low >= high) {
            return;
        }
        int pivot = oneTripSort(a, low, high);
        // Print each trip of sorting.
        for (int i : a) {
            System.out.print(i + " ");
        }
        System.out.println();
        // Sort the left part of pivot.
        sort(a, low, pivot - 1);
        // Sort the right part of pivot.
        sort(a, pivot + 1, high);
    }
    /**
     * One trip of Quick Sort.
     *
     * @param a The array to be sorted
     * @param low The sorting start position
     * @param high The sorting end position
     * @return The pivot position
     */
    private static int oneTripSort(int[] a, int low, int high) {
        // Set the first element as pivot.
        int pivot = a[0];
        int i = low;
        int j = high;
        while (i < j) {
            // From right to left, find the first element that less than pivot.
            while (a[j] >= pivot && i < j) {
                j--;
            }
            // Replace.
            a[i] = a[j];
            // From left to right, find the first element that greter than pivot.
            while (a[i] <= pivot && i < j) {
                i++;
            }
            // Replace.
            a[j] = a[i];
        }
        // Hereto, i must be equal to j. i (or j) is the pivot position. Return the pivot position.
        a[i] = pivot;
        return i;
    }
}
```


