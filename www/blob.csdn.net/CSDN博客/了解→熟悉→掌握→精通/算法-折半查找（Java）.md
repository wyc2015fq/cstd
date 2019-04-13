
# 算法 - 折半查找（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 14:38:28[Chimomo](https://me.csdn.net/chimomo)阅读数：91


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.search;
/**
 * @author Chimomo
 *
 * The prerequisite of binary search:
 * <li>
 * 1. The searched sequence must be sequential stored.
 * </li>
 * <li>
 * 2. The searched sequence must be sorted by keyword.
 * </li>
 */
public class BinarySearch {
    /**
     * Recursively binary search x in sorted array a with low subscript lower
     * bound and high subscript upper bound.
     *
     * Time complexity is O(log2n).
     *
     * @param a The array to be searched
     * @param x The searched target
     * @param low The subscript lower bound of array a
     * @param high The subscript upper bound of array a
     * @return The subscript of x in the array a if found, -1 otherwise.
     */
    public static int searchRecursively(int[] a, int x, int low, int high) {
        // Not found.
        if (low > high) {
            return -1;
        }
        int mid = (low + high) >> 1;
        if (x == a[mid]) {
            return mid;
        }
        return x < a[mid] ? searchRecursively(a, x, low, mid - 1) : searchRecursively(a, x, mid + 1, high);
    }
    /**
     * Circularly binary search x in sorted array a.
     *
     * Time complexity is O(log2n).
     *
     * @param a The array to be searched
     * @param x The searched target
     * @return The subscript of x in the array a if found, -1 otherwise.
     */
    public static int searchCircularly(int[] a, int x) {
        int low = 0;
        int high = a.length - 1;
        while (low <= high) {
            int mid = (low + high) >> 1;
            if (a[mid] == x) {
                return mid;
            }
            if (a[mid] < x) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return -1;
    }
}
```


