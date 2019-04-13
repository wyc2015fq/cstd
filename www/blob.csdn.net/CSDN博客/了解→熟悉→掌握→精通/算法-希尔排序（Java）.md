
# 算法 - 希尔排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 15:43:58[Chimomo](https://me.csdn.net/chimomo)阅读数：67


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * Shell sort also called "diminishing increment sort", is improvement of
 * straight insertion sort.
 * </p>
 * <p>
 * Basic idea of shell sort: Separate the whole sequence into several sub
 * sequence, and perform straight insertion sort onto sub sequence. Till the
 * whole sequence are basically ordered, perform straight insertion sort onto
 * the whole sequence.
 * </p>
 * <p>
 * Specific procedure: Take a integer d(1) (less than n) as the first increment,
 * put all the records into the same group whose distance is multiple of d(1),
 * thereby split all the sequence into number of d(1) groups, and perform
 * straight insertion sort in each group. Take the second increment d(2) (less
 * than d(1)), repeating the above grouping and sorting procedure. On the
 * analogy of the above, till the increment d(i)=1, i.e. all the records are
 * grouped in one group and straight insertion sort performed.
 * </p>
 * <p>
 * Shell sort is not stable.
 * </p>
 */
public class ShellSort {
    /**
     * The shell sort.
     *
     * @param a The array to be sorted
     */
    public static void sort(int[] a) {
        int k;
        int i;
        int t;
        int dk;
        int j;
        int[] delta = new int[a.length / 2];
        k = a.length / 2;
        i = 0;
        // Start from k = a.length / 2, repeat k = k / 2 until k == 0,
        // set the gained k sequence as increment sequence store to delta.
        do {
            k /= 2;
            delta[i++] = k;
        } while (k > 0);
        i = 0;
        while ((dk = delta[i]) > 0) {
            for (k = delta[i]; k < a.length; ++k) {
                // Insert a[k] into the ordered increment subsequence.
                if (a[k] < a[k - dk]) {
                    // Backup the element to be inserted, vacate one element position.
                    t = a[k];
                    for (j = k - dk; j >= 0 && t < a[j]; j -= dk) {
                        // Move elements backward while searching insertion position.
                        a[j + dk] = a[j];
                    }
                    // Found the insertion position, insert the element.
                    a[j + dk] = t;
                }
            }
            // Get the next increment value.
            ++i;
            // Print each round of sorting.
            for (int x = 0; x < a.length; x++) {
                System.out.print(a[x] + " ");
            }
            System.out.println();
        }
    }
}
```


