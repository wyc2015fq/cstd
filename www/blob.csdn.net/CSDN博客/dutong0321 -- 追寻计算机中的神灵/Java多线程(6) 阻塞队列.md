# Java多线程(6) 阻塞队列 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年01月25日 22:54:14[dutong0321](https://me.csdn.net/dutong0321)阅读数：60
#### 概述
终于进入了新的篇章，阻塞队列。在线程的同步这一节，知识点比较多，因为这些都是基础，但是在实际编程当中应该尽可能的远离底层架构，直接使用造好的轮子。对于许多线程问题，可以使用一个或者多个队列以优雅、安全的方式将其形式化：生产者线程向队列中插入元素，消费者线程则用来取出他们。（生产者和消费者是指在多线程中的生产者消费者模型，该模型是几乎可以解决大部分多线程问题。消费者必须要等生产者生产的资源才可以继续运行。）
#### 阻塞队列
|方法|正常动作|特殊情况下动作|
|----|----|----|
|add|添加一个元素|如果队列满，抛出IllegalStateException异常|
|element|返回队列的头元素|如果队列空，抛出NoSuchElementException异常|
|remove|移出并返回头元素|如果队列空，抛出NoSuchElementException异常|
|offer|添加一个元素并返回true|如果队列满，返回false|
|peek|返回队列的头元素|如果队列空，返回null|
|poll|移出并返回队列的头元素|如果队列空，返回null|
|put|添加一个元素|如果队列满，则阻塞|
|take|移出并返回队列的头元素|如果队列空，则阻塞|
一般我们使用offer/peek/pull来对队列进行添加/返回/移出操作，因为这些方法不过不能完成任务，会给出错误提示并且不会抛出异常。 
另外还有带超时参数的offer方法和poll方法的变体，例如：
`boolean success = q.offer(x,100,TimeUnit.MILLISECONDS);`
下面的例子还是来自于《Java核心技术》，其目的是为了搜索一个目录及其所有子目录中所有文件，并在文件中打印出包含指定关键字的行： 
思路如下：
- 确定入口和人机界面（菜单）
- 在目录中查找所有文件和子目录，如果是子目录那么返回第2步，再次进行查找，如果是文件，则添加该元素到阻塞队列中
- 新建一个线程，从阻塞队列中提取元素并删除，从该元素中查找关键字。
SearchTask.java
```java
package com.utopia.blockingQueue;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.concurrent.BlockingQueue;
public class SearchTask implements Runnable {
    private BlockingQueue<File> queue;
    private String keyword;
    public SearchTask(BlockingQueue<File> queue, String keyword) {
        this.queue = queue;
        this.keyword = keyword;
    }
    @Override
    public void run() {
        // TODO Auto-generated method stub
        try {
            boolean done = false;
            // 如果目录中文件没有每个都搜索
            while (!done) {
                // 从阻塞队列中读取文件
                File file = queue.take();
                // 如果该文件是空文件（即结束标志）
                if (file == FileEnumerationTask.DUMMY) {
                    // 因为在上面读取文件时，已经将结束标志文件取出，但是其他线程还没有读取到该文件
                    // 并不知道所有文件均已搜索完毕，所以要再次把标志文件放到循环队列当中
                    queue.put(file);
                    // 文件搜索完毕，跳出循环
                    done = true;
                } else {
                    // 从该文件中的内容当中查找关键字
                    search(file);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public void search(File file) throws IOException {
        try (Scanner in = new Scanner(file)) {
            int lineNumber = 0;
            while (in.hasNextLine()) {
                lineNumber++;
                String line = in.nextLine();
                if (line.contains(keyword)) {
                    System.out.printf("%s:%d:%s%n", file.getPath(), lineNumber, line);
                }
            }
        }
    }
}
```
FileEnumerationTask.java
```java
package com.utopia.blockingQueue;
import java.io.File;
import java.util.concurrent.BlockingQueue;
public class FileEnumerationTask implements Runnable {
    public static File DUMMY = new File("");
    private BlockingQueue<File> queue;
    private File startingDirectory;
    public FileEnumerationTask(BlockingQueue<File> queue, File startingDirectory) {
        this.queue = queue;
        this.startingDirectory = startingDirectory;
    }
    @Override
    public void run() {
        // TODO Auto-generated method stub
        try {
            enumerate(startingDirectory);
            // 如果所有文件搜索完毕，向阻塞队列中置入一个空文件夹作为搜索完毕信号，供消费者识别
            queue.put(DUMMY);
        } catch (InterruptedException e) {
        }
    }
    public void enumerate(File directory) throws InterruptedException {
        // 列出此文件夹下所有的子文件夹和子文件
        File[] files = directory.listFiles();
        for (File file : files) {
            if (file.isDirectory()) {
                // 如果是文件夹的话，那么遍历该文件夹
                enumerate(file);
            } else {
                // 否则（即如果是文件的话），将该文件放入阻塞队列中
                queue.put(file);
            }
        }
    }
}
```
BlockingQueueMain.java
```java
package com.utopia.blockingQueue;
import java.io.File;
import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
public class BlockingQueueMain {
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter base directory(e.g. /opt/jdk1.8.0_191/src):");
        String directory = scanner.nextLine();
        System.out.print("Enter keyword(e.g. volatile):");
        String keyword = scanner.nextLine();
        // 阻塞队列大小
        final int FILE_QUEUE_SIZE = 10;
        // 启动搜索线程的最大数量
        final int SEARCH_THREAD = 100;
        // 创建阻塞队列
        BlockingQueue<File> queue = new ArrayBlockingQueue<File>(
                FILE_QUEUE_SIZE);
        // 创建读取文档的线程，将目录下和其子目录下的文档写入到阻塞队列中（生产者）
        FileEnumerationTask enumeration = new FileEnumerationTask(queue,
                new File(directory));
        new Thread(enumeration).start();
        // 从阻塞队列中读取文档，并启动线程进行内容搜索（消费者）
        for (int i = 1; i < SEARCH_THREAD; i++) {
            new Thread(new SearchTask(queue, keyword)).start();
        }
        scanner.close();
    }
}
```
简单分析该程序源代码，就可以发现在这个程序中有个小问题，那么就是可能先打印出第1个文件中的某个关键字所在行，随后就会又打印出第2个文件中的，然后又打印出第一个文件中的关键字所在行。如图所示： 
![这里写图片描述](https://img-blog.csdn.net/20180125225315998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也就是说，他并不是按文件来输出的，因为在把文档搜索出来后，有多个关键字搜索线程同时启动，所以就会造成该现象。如果希望按文件来输出其实也很简单，第一种是只开启一个内容搜索线程，第二种就是把文件中的所有所在行都查出来，再进行打印。 
其实，在多线程下载中，用到这些方法的地方还是蛮多的。
