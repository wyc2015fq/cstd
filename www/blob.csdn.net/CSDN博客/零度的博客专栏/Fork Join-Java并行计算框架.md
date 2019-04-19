# Fork/Join-Java并行计算框架 - 零度的博客专栏 - CSDN博客
2017年03月27日 14:48:49[零度anngle](https://me.csdn.net/zmx729618)阅读数：349标签：[java																[分支																[合并](https://so.csdn.net/so/search/s.do?q=合并&t=blog)](https://so.csdn.net/so/search/s.do?q=分支&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/zmx729618/article/category/3108443)
                
并行计算在处处都有[大数据](http://lib.csdn.net/base/hadoop)的今天已经不是一个新鲜的词汇了，现在已经有单机多核甚至多机集群并行计算，注意，这里说的是并行，而不是并发。严格的将，并行是指系统内有多个任务同时执行，而并发是指系统内有多个任务同时存在，不同的任务按时间分片的方式切换执行，由于切换的时间很短，给人的感觉好像是在同时执行。 
[Java](http://lib.csdn.net/base/javase)在JDK7之后加入了并行计算的框架Fork/Join，可以解决我们系统中大数据计算的性能问题。Fork/Join采用的是分治法，Fork是将一个大任务拆分成若干个子任务，子任务分别去计算，而Join是获取到子任务的计算结果，然后合并，这个是递归的过程。子任务被分配到不同的核上执行时，效率最高。伪代码如下：
```
Result solve(Problem problem) {
    if (problem is small)
        directly solve problem
    else {
        split problem into independent parts
        fork new subtasks to solve each part
        join all subtasks
        compose result from subresults
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
![](http://static.blog.csdn.net/images/save_snippets.png)
Fork/Join框架的核心类是[ForkJoinPool](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/ForkJoinPool.html)，它能够接收一个[ForkJoinTask](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/ForkJoinTask.html)，并得到计算结果。ForkJoinTask有两个子类，[RecursiveTask](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/RecursiveTask.html)（有返回值）和[RecursiveAction](https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/RecursiveAction.html)（无返回结果），我们自己定义任务时，只需选择这两个类继承即可。类图如下： 
![这里写图片描述](https://img-blog.csdn.net/20150712202533055)![这里写图片描述](https://img-blog.csdn.net/20150712202117816)
下面来看一个实例：计算一个超大数组所有元素的和。代码如下：
```
import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;
/**
 * @author: shuang.gao  Date: 2015/7/14 Time: 8:16
 */
public class SumTask extends RecursiveTask<Integer> {
    private static final long serialVersionUID = -6196480027075657316L;
    private static final int THRESHOLD = 500000;
    private long[] array;
    private int low;
    private int high;
    public SumTask(long[] array, int low, int high) {
        this.array = array;
        this.low = low;
        this.high = high;
    }
    @Override
    protected Integer compute() {
        int sum = 0;
        if (high - low <= THRESHOLD) {
            // 小于阈值则直接计算
            for (int i = low; i < high; i++) {
                sum += array[i];
            }
        } else {
            // 1. 一个大任务分割成两个子任务
            int mid = (low + high) >>> 1;
            SumTask left = new SumTask(array, low, mid);
            SumTask right = new SumTask(array, mid + 1, high);
            // 2. 分别计算
            left.fork();
            right.fork();
            // 3. 合并结果
            sum = left.join() + right.join();
        }
        return sum;
    }
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        long[] array = genArray(1000000);
        System.out.println(Arrays.toString(array));
        // 1. 创建任务
        SumTask sumTask = new SumTask(array, 0, array.length - 1);
        long begin = System.currentTimeMillis();
        // 2. 创建线程池
        ForkJoinPool forkJoinPool = new ForkJoinPool();
        // 3. 提交任务到线程池
        forkJoinPool.submit(sumTask);
        // 4. 获取结果
        Integer result = sumTask.get();
        long end = System.currentTimeMillis();
        System.out.println(String.format("结果 %s 耗时 %sms", result, end - begin));
    }
    private static long[] genArray(int size) {
        long[] array = new long[size];
        for (int i = 0; i < size; i++) {
            array[i] = new Random().nextLong();
        }
        return array;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
![](http://static.blog.csdn.net/images/save_snippets.png)
我们通过调整阈值（THRESHOLD），可以发现耗时是不一样的。实际应用中，如果需要分割的任务大小是固定的，可以经过[测试](http://lib.csdn.net/base/softwaretest)，得到最佳阈值；如果大小不是固定的，就需要设计一个可伸缩的[算法](http://lib.csdn.net/base/datastructure)，来动态计算出阈值。如果子任务很多，效率并不一定会高。 
