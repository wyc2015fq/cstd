
# 算法 - 冒泡排序（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 14:40:26[Chimomo](https://me.csdn.net/chimomo)阅读数：90标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[冒泡排序](https://so.csdn.net/so/search/s.do?q=冒泡排序&t=blog)[BubbleSort](https://so.csdn.net/so/search/s.do?q=BubbleSort&t=blog)[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[](https://so.csdn.net/so/search/s.do?q=BubbleSort&t=blog)[](https://so.csdn.net/so/search/s.do?q=冒泡排序&t=blog)个人分类：[Algorithm](https://blog.csdn.net/chimomo/article/category/1826645)
[](https://so.csdn.net/so/search/s.do?q=冒泡排序&t=blog)
[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
[](https://so.csdn.net/so/search/s.do?q=算法&t=blog)

分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
```python
package chimomo.learning.java.algorithm.sort;
/**
 * @author Chimomo
 *
 * <p>
 * Bubble sort is a kind of simple sort algorithm in computer science. It
 * repeatedly visit the sequence to be sorted, compare two elements each time,
 * swap them if their order is wrong, until no elements need to be swapped. The
 * bigger element will rise to top by swapping like bubble, so it names.
 * </p>
 *
 * The bubble sort algorithm designed as following:
 * <li>
 * 1. Compare the two adjacent elements as a pair, from the first pair to last
 * pair, swap the two if the first one is bigger than the second one. Then, the
 * last element must be the biggest, remove the biggest one from unordered
 * region.
 * </li>
 * <li>
 * 2. Repeat step1 on the elements in the unordered region until no element pair
 * need to be compared.
 * </li>
 *
 * The time complexity of bubble sort is O(n^2).
 *
 * <p>
 * Stability: Bubble sort is swapping bigger element backward. Comparing is
 * comparing the adjacent two elements, swapping is also in the adjacent two
 * elements. So, if the two adjacent elements equal, you will not swap them. And
 * if the two equal elements are not adjacent, even though they are adjacent by
 * swapping the front other elements, you still will not swap them. The equal
 * elements order are not changed in sorting, so bubble sort is stable.
 * </p>
 */
public class BubbleSort {
    /**
     * The bubble sort.
     * @param a The array to be sorted
     */
    public static void sort(int[] a) {
        System.out.println("In Bubble Sort:");
        // The outer loop: Limit the count of elements in each bubble sort comparison.
        for (int i = a.length - 1; i >= 1; i--) {
            // The inner loop: Comparing the elements whose subscript from 0 to i-1.
            for (int j = 0; j <= i - 1; j++) {
                // The sorting process.
                if (a[j] > a[j + 1]) {
                    int t = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = t;
                }
            }
            System.out.println(String.format("Round {0}: ", a.length - i));
            for (int e : a) {
                System.out.print(e + " ");
            }
            System.out.println();
        }
    }
}
```


