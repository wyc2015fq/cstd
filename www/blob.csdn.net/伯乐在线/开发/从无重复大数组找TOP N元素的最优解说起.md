# 从无重复大数组找TOP N元素的最优解说起 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [蓝猫163](http://www.jobbole.com/members/lanmao) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
有一类面试题，既可以考察工程师算法、也可以兼顾实践应用、甚至创新思维，这些题目便是好的题目，有区分度表现为可以有一般解，也可以有最优解。最近就发现了一个这样的好题目，拿出来晒一晒。
### 1 题目 原文：
> 
There is an array of 10000000 different int numbers. Find out its largest 100 elements. The implementation should be optimized for executing speed.
翻译：
有一个长度为1000万的int数组，各元素互不重复。如何以最快的速度找出其中最大的100个元素？
### 2 分析与解 （接下来的算法均以Java语言实现。）
首先，第一个冒出来的想法是——排序。各种排序[算法](https://java.tutorialonfree.com/)对数组进行一次sort，然后limit出max的100个即可，时间复杂度为O(nLogN)。
2.1 堆排序思路 我以堆排序来实现这个题目，这样可以使用非常少的内存空间，始终维护一个100个元素大小的最小堆，堆顶int[0]即是100个元素中最小的，插入一个新的元素的时候，将这个元素和堆顶int[0]进行交换，也就是淘汰掉堆顶，然后再维护一个最小堆，使int[0]再次存储最小的元素，循环往复，不断迭代，最终剩下的100个元素就是结果，该算法时间复杂度仍然是O(nLogN)，优点在于节省内存空间，算法时间复杂度比较理想，平均耗时400ms。
代码实现如下，


```
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
/**
  * Implementation of finding top 100 elements out of a huge int array. <br>
  * 
  * There is an array of 10000000 different int numbers. Find out its largest 100
  * elements. The implementation should be optimized for executing speed. <br>
  * 
  * Note: This is the third version of implementation, this time I make the best out
  * of the heap sort algorithm by using a minimum heap. The heap maintains the top biggest
  * numbers that guarantees the minimum number is removed every time a new number is added
  * to the heap. It saves memory usage to the limit by just using an array which size is 101
  * and a few temp elements. However, the performance is not as good as the bit map way but 
  * better than the multiple thread way. 
  * 
  * @author zhangxu04
  */
public class FindTopElements3 {
    private static final int ARRAY_LENGTH = 10000000; // big array length
    public static void main(String[] args) {
        FindTopElements3 fte = new FindTopElements3();
        // Get a array which is not in order and elements are not duplicate
        int[] array = getShuffledArray(ARRAY_LENGTH);
        // Find top 100 elements and print them by desc order in the console
        long start = System.currentTimeMillis();
        fte.findTop100(array);
        long end = System.currentTimeMillis();
        System.out.println("Costs " + (end - start) + "ms");
    }
    public void findTop100(int[] arr) {
        MinimumHeap heap = new MinimumHeap(100);
        for (Integer i : arr) {
            heap.add(i);
            if (heap.size() > 100) {
                heap.deleteTop();
            }
        }
        for (int i = 0; i < 100; i++) {
            System.out.println(heap.deleteTop());
        }
    }
    /**
      * Get shuffled int array
      * 
      * @return array not in order and elements are not duplicate
      */
    private static int[] getShuffledArray(int len) {
        System.out
                .println("Start to generate test array... this may take several seconds.");
        List<Integer> list = new ArrayList<Integer>(len);
        for (int i = 0; i < len; i++) {
            list.add(i);
        }
        Collections.shuffle(list);
        int[] ret = new int[len];
        for (int i = 0; i < len; i++) {
            ret[i] = list.get(i);
        }
        return ret;
    }
}
class MinimumHeap {
    int[] items;
    int size;
    public MinimumHeap(int size) {
        items = new int[size + 1];
        size = 0;
    }
    void shiftUp(int index) {
        int intent = items[index];
        while (index > 0) {
            int pindex = (index - 1) / 2;
            int parent = items[pindex];
            if (intent < parent) {
                items[index] = parent;
                index = pindex;
            } else {
                break;
            }
        }
        items[index] = intent;
    }
    void shiftDown(int index) {
        int intent = items[index];
        int leftIndex = 2 * index + 1;
        while (leftIndex < size) {
            int minChild = items[leftIndex];
            int minIndex = leftIndex;
            int rightIndex = leftIndex + 1;
            if (rightIndex < size) {
                int rightChild = items[rightIndex];
                if (rightChild < minChild) {
                    minChild = rightChild;
                    minIndex = rightIndex;
                }
            }
            if (minChild < intent) {
                items[index] = minChild;
                index = minIndex;
                leftIndex = index * 2 + 1;
            } else {
                break;
            }
        }
        items[index] = intent;
    }
    public void add(int item) {
        items[size++] = item;
        shiftUp(size - 1);
    }
    public int deleteTop() {
        if (size < 1) {
            return 0;
        }
        int maxItem = items[0];
        int lastItem = items[size - 1];
        size--;
        if (size < 1) {
            return lastItem;
        }
        items[0] = lastItem;
        shiftDown(0);
        return maxItem;
    }
    public boolean isEmpty() {
        return size < 1;
    }
    public int size() {
        return size;
    }
    /**
      * MinimumHeap main test
      * @param args
      */
    public static void main(String[] args) {
        MinimumHeap heap = new MinimumHeap(7);
        heap.add(2);
        heap.add(3);
        heap.add(5);
        heap.add(1);
        heap.add(4);
        heap.add(7);
        heap.add(6);
        heap.deleteTop();
        heap.deleteTop();
        while (!heap.isEmpty()) {
            System.out.println(heap.deleteTop());
        }
    }
}
```
那么挖掘下题目，两个点是我们的优化线索：
1、元素互不重复
2、最快的速度，没有提及对于系统资源以及空间的要求
2.2 多线程分而治之策略 顺着#2条线索，可以给出一个多线程的优化版本，使用分而治之的策略，将1000万大小的数组分割为1000个元素组成的若干小数组，利用JDK自带的高效排序算法void java.util.Arrays.sort(int[] a)来进行排序，多线程处理，主线程汇总结果后取出各个小数组的top 100，归并后再进行一次排序得出结果。
代码实现如下，


```
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
/**
  * Implementation of finding top 100 elements out of a huge int array. <br>
  * 
  * There is an array of 10000000 different int numbers. 
  * Find out its largest 100 elements. 
  * The implementation should be optimized for executing speed.
  * 
  * @author zhangxu04
  */
public class FindTopElements {
    private static final int ARRAY_LENGTH = 10000000; // big array length
    private static final int ELEMENT_NUM_PER_GROUP = 10000; // split big array into sub-array, this represents sub-array length
    private static final int TOP_ELEMENTS_NUM = 100; // top elements number
    private ExecutorService executorService;
    private CompletionService<int[]> completionService;
    public FindTopElements() {
        int MAX_THREAD_COUNT = 50;
        executorService = Executors.newFixedThreadPool(MAX_THREAD_COUNT);
        completionService = new ExecutorCompletionService<int[]>(executorService);
    }
    /**
      * Start from here :-)
      * @param args
      */
    public static void main(String[] args) {
        FindTopElements findTopElements = new FindTopElements();
        // Get a array which is not in order and elements are not duplicate
        int[] array = getShuffledArray(ARRAY_LENGTH);
        // Find top 100 elements and print them by desc order in the console
        long start = System.currentTimeMillis();
        findTopElements.findTop100(array);
        long end = System.currentTimeMillis();
        System.out.println("Costs " + (end - start) + "ms");
    }
    /**
      * Leveraging concurrent components of JDK, we can deal small parts of the huge array concurrently.
      * The huge array are split into several sub arrays which are submitted to a thread pool one by one.
      * By using <code>CompletionService</code>, we can take out completed result from the pool as soon as possible, 
      * which avoid the block issue when getting future result through a future task list by using 
      * <code>ExcutorService</code> and <code>Future</code> class. Moreover, the can optimize the performance of 
      * the piece of code by processing the completed results once we get them, so the overall sort invocation will 
      * not be delayed to the final moment.
      * 
      */
    private void findTop100(int[] arr) {
        System.out.println("Start to compute.");
        int groupNum = (ARRAY_LENGTH / ELEMENT_NUM_PER_GROUP);
        System.out.println("Split " + ARRAY_LENGTH + " elements into " + groupNum + " groups");
        for (int i = 0; i < groupNum; i++) {
            int[] toBeSortArray = new int[ELEMENT_NUM_PER_GROUP];
            System.arraycopy(arr, i * ELEMENT_NUM_PER_GROUP, toBeSortArray, 0, ELEMENT_NUM_PER_GROUP);
            completionService.submit(new FindTop100(toBeSortArray));
        }
        try {
            int[] overallArray = new int[TOP_ELEMENTS_NUM * groupNum];
            for (int i = 0; i < groupNum; i++) {
                System.arraycopy(completionService.take().get(), 0, overallArray, i * TOP_ELEMENTS_NUM, TOP_ELEMENTS_NUM);
            }
            Arrays.sort(overallArray);
            for (int i = 1; i <= TOP_ELEMENTS_NUM; i++) {
                System.out.println(overallArray[TOP_ELEMENTS_NUM * groupNum - i]);
            }
            System.out.println("Finish to output result.");
        } catch (Exception e) {
            e.printStackTrace();
        }
        executorService.shutdown();
    }
    /**
      * Callable of finding top 100 elements <br>
      * The steps are as below:
      * 1) Quick sort a array
      * 2) Get reverse 100 elements and put them into a new array
      * 3) return the new array
      */
    private class FindTop100 implements Callable<int[]> {
        private int[] array;
        public FindTop100(int[] array) {
            this.array = array;
        }
        @Override
        public int[] call() throws Exception {
            int len = array.length;
            Arrays.sort(array);
            int[] result = new int[TOP_ELEMENTS_NUM];
            int index = 0;
            for (int i = 1; i <= TOP_ELEMENTS_NUM; i++) {
                result[index++] = array[len - i];
            }
            return result;
        }
    }
    /**
      * Get shuffled int array
      * 
      * @return array not in order and elements are not duplicate
      */
    private static int[] getShuffledArray(int len) {
        System.out.println("Start to generate test array... this may take several seconds.");
        List<Integer> list = new ArrayList<Integer>(len);
        for (int i = 0; i < len; i++) {
            list.add(i);
        }
        Collections.shuffle(list);
        int[] ret = new int[len];
        for (int i = 0; i < len; i++) {
            ret[i] = list.get(i);
        }
        return ret;
    }
}
```
分析看来，这个解的优势在于充分利用了系统资源，使用了分而治之的思想，将时间复杂度平均分配到了每个子线程中，但是代码中大量用到了System.arraycopy进行数组拷贝，占用内存过于多，甚至需要指定JVM的内存-Xmx才可以正常运行起来，平均耗时250ms。
2.3 位图数组思路 这个思路属于比较创新的方式，考虑到优化线索#1提到的无重复元素，那么可以使用位图数组存储元素，一个int占用4个字节，32个bit，也就是说1个int可以表示32个数字的位置。 维护一个数组长度/32+1的位图数组x，遍历给定的数组，将数字安插进入这个位图数组x中，例如int[0]=62，那么


```
index=62/32=1
bit_index=62 mod 32 = 30
```
那么就置位图数组的`x[1]=x[1] | 30`，采用“位或”是为了不丢掉以前处理过的数字。
代码实现如下，


```
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
/**
  * Implementation of finding top 100 elements out of a huge int array. <br>
  * 
  * There is an array of 10000000 different int numbers. Find out its largest 100
  * elements. The implementation should be optimized for executing speed. <br>
  * 
  * Note: This is the second version of implementation, the previous one using 
  * thread pool provided by JDK concurrent toolkit is not efficient enough, the 
  * second version is an enhanced one based on bit map algorithm, which is estimated to
  * have at least a 3 times faster and consume less memory usage.
  * 
  * @author zhangxu04
  */
public class FindTopElements2 {
    private static final int ARRAY_LENGTH = 10000000; // big array length
    public static void main(String[] args) {
        FindTopElements2 fte = new FindTopElements2(ARRAY_LENGTH + 1);
        // Get a array which is not in order and elements are not duplicate
        int[] array = getShuffledArray(ARRAY_LENGTH);
        // Find top 100 elements and print them by desc order in the console
        long start = System.currentTimeMillis();
        fte.findTop100(array);
        long end = System.currentTimeMillis();
        System.out.println("Costs " + (end - start) + "ms");
    }
    private final int[] bitmap;
    private final int size;
    public FindTopElements2(final int size) {
        this.size = size;
        int len = ((size % 32) == 0) ? size / 32 : size / 32 + 1;
        this.bitmap = new int[len];
    }
    private static int index(final int number) {
        return number / 32;
    }
    private static int position(final int number) {
        return number % 32;
    }
    private void adjustBitMap(final int index, final int position) {
        int bit = bitmap[index] | (1 << position);
        bitmap[index] = bit;
    }
    public void add(int[] numArr) {
        for (int i = 0; i < numArr.length; i++) {
            add(numArr[i]);
        }
    }
    public void add(int number) {
        adjustBitMap(index(number), position(number));
    }
    public boolean getIndex(final int index) {
        if (index > size) {
            return false;
        }
        int bit = (bitmap[index(index)] >> position(index)) & 0x0001;
        return (bit == 1);
    }
    private void findTop100(int[] arr) {
        System.out.println("Start to compute.");
        add(arr);
        int[] result = new int[100];
        int index = 0;
        for (int i = bitmap.length - 1; i >= 0; i--) {
            for (int j = 31; j >= 0; j--) {
                if (((bitmap[i] >> j) & 0x0001) == 1) {
                    if (index == result.length) {
                        break;
                    }
                    result[index++] = ((i) * 32) + j ;
                }
            }
            if (index == result.length) {
                break;
            }
        }
        for (int j = 0; j < result.length; j++) {
            System.out.println(result[j]);
        }
        System.out.println("Finish to output result.");
    }
    /**
      * Get shuffled int array
      * 
      * @return array not in order and elements are not duplicate
      */
    private static int[] getShuffledArray(int len) {
        System.out.println("Start to generate test array... this may take several seconds.");
        List<Integer> list = new ArrayList<Integer>(len);
        for (int i = 0; i < len; i++) {
            list.add(i);
        }
        Collections.shuffle(list);
        int[] ret = new int[len];
        for (int i = 0; i < len; i++) {
            ret[i] = list.get(i);
        }
        return ret;
    }
}
```
这个算法的时间复杂度是O(N)，非常理想，平均耗时可以减少到50ms作用，性能比排序算法提升了10倍以上，不足在于位图数组的长度取决于给定数组的最大值，如果分布比较平均，并且最大值比较小，那么占用内存空间就可以得到有效的控制。
### 3 总结
综上给出的题目，可以看出解决一个实际问题，既可以用纯算法的思路来解决，我们甚至可以自己动手实现，例如自己写的堆排序，非常节省空间，如果用JDK自带的快速排序，那么无疑这一点不会好于我们的实现。 现今，处理大数据问题一个倾向的思路就是充分利用系统资源，充分发挥多核、大内存计算型服务器的能力，为我们提高效率，多线程是在JAVA中以及有了非常好用的API以及concurrent包下的工具类，能否有效利用这些工具提速我们的程序也很关键。同时，问题总有一些点可以让我们找到最适合的场景来解决，例如位图数组的思路，在性能上达到了最佳，同时多消耗的内存对于现代的服务器来说完全在可控范围内，因此不失为一种创新的好思路。
