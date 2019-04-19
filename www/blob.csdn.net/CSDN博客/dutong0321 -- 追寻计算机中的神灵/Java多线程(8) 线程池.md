# Java多线程(8) 线程池 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年02月25日 15:53:11[dutong0321](https://me.csdn.net/dutong0321)阅读数：65
#### 概述
春节期间本有雄心壮志，奈何初二晚上遇上感冒。今天中午本打算继续冬眠，忽然女友一条微信让我不仅反思自己，不可再浑浑噩噩，于是奋起，开始继续努力，瞬间腰不疼了，腿不麻了，倍有精神了，看来精神的确可以驾驭肉体，愿2018是一个成功之年。
#### 线程池
构建新的线程因为设计与操作系统的交互所以是有代价的，如果程序中需要创建大量的生命周期很短的线程，此时应该使用线程池。一个线程池中包含许多准备运行的空闲线程，将Runnable对象提交给线程池，就会有一个线程调用run方法，当run方法退出时，线程不会死亡，而是在池中尊卑为下一个请求提供服务。 
另外一个使用线程池的理由是减少并发线程的数目。创建大量线程会大大降低性能甚至导致虚拟机崩溃。如果有一个会创建许多线程的算法，应该使用一个线程数“固定的”线程池以限制并发线程的总数。 
执行器(Executor)类有许多静态工厂方法用来构建线程池，下表对这些方法进行汇总。
|方法|描述|
|----|----|
|newCachedThreadPool|必要时创建新线程；空闲线程将会被保留60秒|
|newFixedThreadPool|该吃包含固定数量的线程；空闲线程会一直被保留|
|newSingleThreadExecutor|只有一个线程的“池”，该线程顺序执行每一个提交的任务|
|newScheduledThreadPool|用于预定执行而构建的固定线程池，替代java.util.Timer|
|newSingleThreadScheduledExecutor|用于预定执行而构建的单线程“池”|
可以使用下面的方法来将Runnable对象或Callable对象提交给ExecutorService:
```
Future<?> submit(Runnable task)
Future<T> submit(Runnable task,T result)
Future<T> submit(Callable<T> task)
```
该池会在方便时尽早执行提交的任务，调用submit时，会得到一个Future对象，可以用来查询该任务的状态。 
第一个版本get方法在完成的时候知识简单地返回null。 
第二个版本get方法在往城市返回指定的result对象。 
当用完一个线程池的时候，调用shutdown。该方法启动该池的关闭序列。被关闭的执行器不再接收新的任务。当所有人物完成以后，线程池中的线程死亡。另外一种方法是调用shutdownNow，该池取消尚未开始的所有任务并试图中断正在运行的线程。 
一下是上一个小节在文件夹中查找共有多少个子文件包含某个关键字：
```
package com.utopia.threadPool;
import java.io.File;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadPoolExecutor;
public class ThreadPoolTest {
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        Scanner in = new Scanner(System.in);
        System.out.print("Enter base directory(e.g. /usr/local/jdk1.8_141/src):");
        String directory = in.nextLine();
        System.out.print("Enter keyword(e.g. volatile):");
        String keyword = in.nextLine();
        // 创建线程池
        ExecutorService pool = Executors.newCachedThreadPool();
        // 创建线程执行任务
        MatchCounter counter = new MatchCounter(new File(directory), keyword, pool);
        // 提交指定任务去执行
        Future<Integer> result = pool.submit(counter);
        try {
            System.out.println(result.get() + " matching files.");
        } catch (Exception e) {
            e.printStackTrace();
        }
        // 关闭服务，先完成已经提交的任务而不再接收新的任务
        pool.shutdown();
        // 返回线程池在该执行器生命周期中的最大尺寸
        int largePoolSize = ((ThreadPoolExecutor) pool).getLargestPoolSize();
        System.out.println("largest pool size=" + largePoolSize);
        in.close();
    }
}
```
```
package com.utopia.future;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Callable;
import java.util.concurrent.Future;
import java.util.concurrent.FutureTask;
public class MatchCounter implements Callable<Integer> {
    private File directory;
    private String keyword;
    private int count;
    public MatchCounter(File directory, String keyword) {
        this.directory = directory;
        this.keyword = keyword;
    }
    @Override
    public Integer call() throws Exception {
        // TODO Auto-generated method stub
        count = 0;
        try {
            File[] files = directory.listFiles();
            List<Future<Integer>> result = new ArrayList<Future<Integer>>();
            for (File file : files) {
                if (file.isDirectory()) {
                    MatchCounter matchCounter = new MatchCounter(file, keyword);
                    FutureTask<Integer> futureTask = new FutureTask<Integer>(matchCounter);
                    // 如果是文件夹，那么把该文件夹放到一个新的线程中并运行该线程，并把该FutureTask放到list容器中
                    result.add(futureTask);
                    Thread thread = new Thread(futureTask);
                    thread.start();
                } else {
                    if (search(file))
                        count++;
                }
            }
            // 从list容器中，把每个FuturteTask的返回值取出来加到总计数中
            for (Future<Integer> future : result) {
                try {
                    count += future.get();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return count;
    }
    // 从文件中查找关键字，找到返回true，没有找到返回false
    public boolean search(File file) {
        try {
            try (Scanner scanner = new Scanner(file)) {
                boolean found = false;
                while (!found && scanner.hasNextLine()) {
                    String line = scanner.nextLine();
                    if (line.contains(keyword))
                        found = false;
                }
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
}
```
运行截图： 
![程序截图](https://img-blog.csdn.net/20180225154817783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用线程池应该分为以下4步：
- 调用Executors类中静态的方法newCachedThreadPool或newFixedThreadPool。
- 调用submit提交Runnable或Callable对象。
- 如果想要取消一个任务，或如果提交Callable对象，那就要保存号返回的future对象。
- 当不再提交任何任务时，调用shutdown。
