# spring StopWatch用法 - z69183787的专栏 - CSDN博客
2018年08月06日 11:17:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：128
### 背景
有时我们在做开发的时候需要记录每个任务执行时间，或者记录一段代码执行时间，最简单的方法就是打印当前时间与执行完时间的差值，然后这样如果执行大量测试的话就很麻烦，并且不直观，如果想对执行的时间做进一步控制，则需要在程序中很多地方修改，目前spring-framework提供了一个StopWatch类可以做类似任务执行时间控制，也就是封装了一个对开始时间，结束时间记录操作的Java类，小例一则如下
[回到顶部](https://www.cnblogs.com/kaituorensheng/p/7105639.html#_labelTop)
### 实例
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.example.stopwatch;
import org.springframework.util.StopWatch;
public class TestStopWatch {
    private void test() throws InterruptedException {
        StopWatch sw = new StopWatch();
        sw.start("起床");
        Thread.sleep(1000);
        sw.stop();
        sw.start("洗漱");
        Thread.sleep(2000);
        sw.stop();
        sw.start("锁门");
        Thread.sleep(500);
        sw.stop();
        System.out.println(sw.prettyPrint());
        System.out.println(sw.getTotalTimeMillis());
        System.out.println(sw.getLastTaskName());
        System.out.println(sw.getLastTaskInfo());
        System.out.println(sw.getTaskCount());
    }
    public static void main(String []argv) throws InterruptedException {
        TestStopWatch testStopWatch = new TestStopWatch();
        testStopWatch.test();
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
[回到顶部](https://www.cnblogs.com/kaituorensheng/p/7105639.html#_labelTop)
### 结果
|123456789101112|`StopWatch ``''``: running time (millis) = 3518``-----------------------------------------``ms     %     Task name``-----------------------------------------``00998  028%  起床``02020  057%  洗漱``00500  014%  锁门``3518``锁门``org.springframework.util.StopWatch$TaskInfo@5b2133b1``3`|
