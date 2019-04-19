# Java多线程(7) Callable与Future - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年02月07日 22:38:00[dutong0321](https://me.csdn.net/dutong0321)阅读数：44标签：[Callable																[FutureTask																[多线程																[Java																[Runnable](https://so.csdn.net/so/search/s.do?q=Runnable&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=FutureTask&t=blog)](https://so.csdn.net/so/search/s.do?q=Callable&t=blog)
个人分类：[编程世界](https://blog.csdn.net/dutong0321/article/category/6127295)
#### 概述
之前有很长时间没有写博客了，一方面生活中的琐事让人无法静心，另外一方面工作中也比较忙，最重要的是在上一章打算写一下线程安全的集合，《Java核心技术》中讲解相当浅，而且太过杂乱。在网络上面搜罗了些文章，但是却依然无法获得其要领，即使略懂一二也无法组织语言，所以就暂时告一段落，先继续研究接下来的内容，以后对java进一步了解后再进行补充。Runnable封装了一个异步运行的任务，可以把他想象成为一个没有参数和返回值的异步方法。（这句话是原书这么写的，其实，我认为是可以传递参数的，比如说通过构造函数就可以实现，可能原书作者认为通过构造函数传递参数不算？！）
#### Callable与FutureTask
Callable与Runnable相似，但是不同的是有返回值。Callable接口是一个参数化的类型，只有一个方法call，类型参数是返回值的类型。
```
public interface Callable<V> {
    V call() throws Exception;
}
```
Callable必须要与Future搭配来使用，Future用来保存异步计算的的结果。Future对象的所有者在结果计算好之后就可以获得他。
```
public interface Future<V> {
    V get() throws ...;
    V get(long timeout,TimeUnit unit) throws ...;
    void cancel(boolean mayInterrupt);
    boolean isCancelled();
    boolean isDone();
}
```
**get方法**请查看接下来的代码注释。 
**cancel方法**用来取消该计算。 
如果计算没有开始，它被取消不再开始，在运行中，如果mayInterrupt参数为true，他就会被中断。 
**isCancelled方法**如果计算被取消返回true，如果没有返回false。 
**isDone方法**如果计算还在进行返回false，如果计算完成返回false。
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
```
package com.utopia.future;
import java.io.File;
import java.util.Scanner;
import java.util.concurrent.FutureTask;
public class FutureTest {
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter base directory(e.g. /opt/jdk1.8_141/src):");
        String directory = scanner.nextLine();
        System.out.print("Enter keyword(e.g. volatile):");
        String keyword = scanner.nextLine();
        // 创建一个Callable<Integer>的类
        MatchCounter matchCounter = new MatchCounter(new File(directory), keyword);
        // 利用MatchCounter创建一个FutureTask对象，并启动一个线程
        FutureTask<Integer> futureTask = new FutureTask<Integer>(matchCounter);
        Thread thread = new Thread(futureTask);
        thread.start();
        try {
            // 获得多线程运行完成后的返回值
            // 如果多线程没有运行完成，get方法将会被阻塞，直到运行完成
            // 另外还有带有超时的方法 V get(long timeout,TimeUnit unit) throws...
            // 如果超时，抛出TimeoutException异常
            // 如果运行计算的线程被中断，以上2种get方法均抛出InterruptException
            System.out.println(futureTask.get() + " matching files.");
        } catch (Exception e) {
            e.printStackTrace();
        }
        scanner.close();
    }
}
```
该程序与之前对关键字寻找的程序很相似，不过这个程序不打印找到关键字的行，而是打印出包括该关键字的文件的数量。 
运行结果：
```
Enter base directory(e.g. /opt/jdk1.8_141/src):D:\program files\Java\jdk1.8.0_91\src
Enter keyword(e.g. volatile):volatile
7721 matching files.
```
