
# 算法 - 归并排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 15:45:08[Chimomo](https://me.csdn.net/chimomo)阅读数：122


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * Merge sort is built on merge operation, it is a typical application of
 * "Divide and Conquer" strategy.
 * </p>
 *
 * <p>
 * The principle of merge operation:
 * <li>
 * 1. Apply for memory to store the merged sequence, its size equals to the sum
 * of the two ordered sequence.
 * </li>
 * <li>
 * 2. Set two pointers, their initial values point to the start position of two
 * ordered sequences.
 * </li>
 * <li>
 * 3. Compare the elements the two pointers pointed, put the smaller one into
 * merged sequence, and move the pointer to the next position.
 * </li>
 * <li>
 * 4. Repeat step 3 until one pointer exceeds the rear of the sequence.
 * </li>
 * <li>
 * 5. Copy the remaining elements of the other sequence to the rear of the
 * merged sequence.
 * </li>
 * </p>
 *
 * <p>
 * The principle of merge sort (suppose sequence has n elements):
 * <li>
 * 1. Perform merge operation on every two adjacent sequence elements and get
 * floor(n/2) sequences, every sequence contains two elements after sorting.
 * </li>
 * <li>
 * 2. Perform merge operation on the above sequences and get floor(n/4)
 * sequences, every sequence contains four elements.
 * </li>
 * <li>
 * 3. Repeat step 2 until all elements are sorted.
 * </li>
 * </p>
 *
 * <p>
 * The sorting speed of merge sort second only to quick sort, it's a stable
 * sorting algorithm. Generally applied on the sequence that overall unordered
 * but relatively ordered on its subsequences.
 * </p>
 */
public class MergeSort {
    /**
     * The merge sort.
     *
     * @param a The array to be sorted
     * @param length The length of to-be-merged ordered sequence
     */
    public static void sort(int[] a, int length) {
        // The length of the to-be-sorted sequence.
        int size = a.length;
        // The merge operation times of one round of merge operation.
        int mid = size / (length << 1);
        // The count of unmerged elements in the end of merged sequence after
        // merge operation.
        int c = size & ((length << 1) - 1);
        // The sort ended when there is only one ordered sequence.
        if (mid == 0) {
            return;
        }
        // Perform one round of merge sort.
        for (int i = 0; i < mid; ++i) {
            merge(a, i * 2 * length, (i * 2 + 1) * length, (length << 1) + i * 2 * length - 1);
        }
        // Merge the left elements with the last ordered sequence.
        if (c != 0) {
            merge(a, size - c - 2 * length, size - c, size - 1);
        }
        // Print each round of sorting.
        for (int i : a) {
            System.out.print(i + " ");
        }
        System.out.println();
        // Perform the next merge sort recursively.
        sort(a, 2 * length);
    }
    /**
     * The 2-way merge operation: Merge two ordered sequences to one ordered
     * sequence.
     *
     * @param a The array to be merged
     * @param s1 The start subscript of the first ordered sequence
     * @param s2 The start subscript of the second ordered sequence
     * @param e2 The end subscript of the second ordered sequence
     */
    private static void merge(int[] a, int s1, int s2, int e2) {
        // Apply for memeory whose size is the sum of the two ordered sequences,
        // This memory stores the merged sequence.
        int[] tmp = new int[e2 - s1 + 1];
        // Set two pointers, their initial positions are the starting position
        // of the two ordered sequence.
        int i = s1;
        int j = s2;
        int k = 0;
        // Repeat the following operations until one pointer exceeds the end of
        // sequence. Then comparing the elements the two pointers pointed, put
        // the smaller one into the merged sequence, and move the pointer to the
        // next position.
        while (i < s2 && j <= e2) {
            if (a[i] <= a[j]) {
                tmp[k++] = a[i++];
            } else {
                tmp[k++] = a[j++];
            }
        }
        // Copy all the left elements of the other sequence to the end of the
        // merged sequence.
        while (i < s2) {
            tmp[k++] = a[i++];
        }
        while (j <= e2) {
            tmp[k++] = a[j++];
        }
        System.arraycopy(tmp, 0, a, s1, tmp.length);
    }
}
```


