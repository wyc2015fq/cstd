# 使用ConcurrentLinkedQueue惨痛的教训 - z69183787的专栏 - CSDN博客
2018年07月16日 14:43:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3600
url： http://m.blog.csdn.net/blog/kanepan/5706488
服务端原本有个定时任务对一个集合ArrayList 中的消息做处理。 因为考虑到处理消息是先进先出原则，所以优化的时候考虑改用ConcurrentLinkedQueue 当时没仔细深入研究过这个集合就匆匆上线了。结果刚上线第二天就出问题了。服务端一次优化演变成了一个缺陷，还好及时回退了版本，后果才不是很严重。
回退后对ConcurrentLinkedQueue 做了一个简单的测试代码如下：
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
public class ConcurrentLinkedQueueTest {
    private static ConcurrentLinkedQueue<Integer> queue = new ConcurrentLinkedQueue<Integer>();
    private static int count = 100000;
    private static int count2 = 2; // 线程个数
    private static CountDownLatch cd = new CountDownLatch(count2);
    public static void dothis() {
        for (int i = 0; i < count; i++) {
            queue.offer(i);
        }
    }
    public static void main(String[] args) throws InterruptedException {
        long timeStart = System.currentTimeMillis();
        ExecutorService es = Executors.newFixedThreadPool(4);
        ConcurrentLinkedQueueTest.dothis();
        for (int i = 0; i < count2; i++) {
            es.submit(new Poll());
        }
        cd.await();
        System.out.println("cost time "
                + (System.currentTimeMillis() - timeStart) + "ms");
        es.shutdown();
    }
    static class Poll implements Runnable {
        @Override
        public void run() {
//            while (queue.size()>0) {
            while (!queue.isEmpty()) {
                System.out.println(queue.poll());
            }
            cd.countDown();
        }
    }
}
运行结果：
costtime 2360ms
改用while (queue.size()>0)后
运行结果：
cost time 46422ms
结果居然相差那么大，看了下ConcurrentLinkedQueue的API 原来.size() 是要遍历一遍集合的，难怪那么慢，所以尽量要避免用size而改用isEmpty().
总结了下， 在单位缺乏性能测试下，对自己的编程要求更加要严格，特别是在生产环境下更是要小心谨慎。
