# 各种同步方法性能比较(synchronized,ReentrantLock,Atomic) - 零度的博客专栏 - CSDN博客
2016年10月10日 14:47:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：355
5.0的多线程任务包对于同步的性能方面有了很大的改进，在原有synchronized关键字的基础上，又增加了ReentrantLock，以及各种Atomic类。了解其性能的优劣程度，有助与我们在特定的情形下做出正确的选择。 
总体的结论先摆出来：  
synchronized： 
在资源竞争不是很激烈的情况下，偶尔会有同步的情形下，synchronized是很合适的。原因在于，编译程序通常会尽可能的进行优化synchronize，另外可读性非常好，不管用没用过5.0多线程包的程序员都能理解。 
ReentrantLock: 
ReentrantLock提供了多样化的同步，比如有时间限制的同步，可以被Interrupt的同步（synchronized的同步是不能Interrupt的）等。在资源竞争不激烈的情形下，性能稍微比synchronized差点点。但是当同步非常激烈的时候，synchronized的性能一下子能下降好几十倍。而ReentrantLock确还能维持常态。 
Atomic: 
和上面的类似，不激烈情况下，性能比synchronized略逊，而激烈的时候，也能维持常态。激烈的时候，Atomic的性能会优于ReentrantLock一倍左右。但是其有一个缺点，就是只能同步一个值，一段代码中只能出现一个Atomic的变量，多于一个同步无效。因为他不能在多个Atomic之间同步。 
所以，我们写同步的时候，优先考虑synchronized，如果有特殊需要，再进一步优化。ReentrantLock和Atomic如果用的不好，不仅不能提高性能，还可能带来灾难。 
先贴测试结果：再贴代码（Atomic测试代码不准确，一个同步中只能有1个Actomic，这里用了2个，但是这里的测试只看速度） 
==========================
round:100000 thread:5
Sync  = 96515266
Lock  = 25434694
Atom = 22142464
==========================
round:200000 thread:10
Sync  = 363174894
Lock  = 92003568
Atom = 60405932
==========================
round:300000 thread:15
Sync  = 954456020
Lock  = 184936307
Atom = 141182490
==========================
round:400000 thread:20
Sync  = 1439020073
Lock  = 372073298
Atom = 328126317
==========================
round:500000 thread:25
Sync  = 2807426174
Lock  = 550143645
Atom = 427540885
```
package zmx.atomic.test;
import java.util.Random;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.locks.ReentrantLock;
public class TestSyncMethods {
	public static void test(int round, int threadNum,CyclicBarrier cyclicBarrier) {
		new SyncTest("Sync", round, threadNum, cyclicBarrier).testTime();
		new LockTest("Lock", round, threadNum, cyclicBarrier).testTime();
		new AtomicTest("Atom", round, threadNum, cyclicBarrier).testTime();
	}
	public static void main(String args[]) {
		for (int i = 0; i < 5; i++) {
			int round = 100000 * (i + 1);
			int threadNum = 5 * (i + 1);
			CyclicBarrier cb = new CyclicBarrier(threadNum * 2 + 1);
			System.out.println("==========================");
			System.out.println("round:" + round + " thread:" + threadNum);
			test(round, threadNum, cb);
		}
	}
}
class SyncTest extends TestTemplate {
	public SyncTest(String _id, int _round, int _threadNum, CyclicBarrier _cb) {
		super(_id, _round, _threadNum, _cb);
	}
	/**
	 * synchronized关键字不在方法签名里面，所以不涉及重载问题
	 */
	@Override
	synchronized long getValue() {
		return super.countValue;
	}
	@Override
	synchronized void sumValue() {
		super.countValue += preInit[index++ % round];
	}
}
class LockTest extends TestTemplate {
	
	ReentrantLock lock = new ReentrantLock();
	public LockTest(String _id, int _round, int _threadNum, CyclicBarrier _cb) {
		super(_id, _round, _threadNum, _cb);
	}
	/**
	 * synchronized关键字不在方法签名里面，所以不涉及重载问题
	 */
    @Override
	long getValue() {
		try {
			lock.lock();
			return super.countValue;
		} finally {
			lock.unlock();
		}
	}
	@Override
	void sumValue() {
		try {
			lock.lock();
			super.countValue += preInit[index++ % round];
		} finally {
			lock.unlock();
		}
	}
}
 
class AtomicTest extends TestTemplate {
	
	public AtomicTest(String _id, int _round, int _threadNum, CyclicBarrier _cb) {
		super(_id, _round, _threadNum, _cb);
	}
	
	/**
	 * synchronized关键字不在方法签名里面，所以不涉及重载问题
	 */
	@Override
	long getValue() {
		return super.countValueAtmoic.get();
	}
	@Override
	void sumValue() {
		super.countValueAtmoic.addAndGet(super.preInit[indexAtomic.get()% round]);
	}
}
abstract class TestTemplate {
	private String id;
	protected int round;
	private int threadNum;
	protected long countValue;
	protected AtomicLong countValueAtmoic = new AtomicLong(0);
	protected int[] preInit;
	protected int index;
	protected AtomicInteger indexAtomic = new AtomicInteger(0);
	Random r = new Random(47);
	// 任务栅栏，同批任务，先到达wait的任务挂起，一直等到全部任务到达制定的wait地点后，才能全部唤醒，继续执行
	private CyclicBarrier cb;
	public TestTemplate(String _id, int _round, int _threadNum,
			CyclicBarrier _cb) {
		this.id = _id;
		this.round = _round;
		this.threadNum = _threadNum;
		cb = _cb;
		preInit = new int[round];
		for (int i = 0; i < preInit.length; i++) {
			preInit[i] = r.nextInt(100);
		}
	}
	abstract void sumValue();
	/*
	 * 对long的操作是非原子的，原子操作只针对32位 long是64位，底层操作的时候分2个32位读写，因此不是线程安全
	 */
	abstract long getValue();
	public void testTime() {
		ExecutorService se = Executors.newCachedThreadPool();
		long start = System.nanoTime();
		// 同时开启2*ThreadNum个数的读写线程
		for (int i = 0; i < threadNum; i++) {
			se.execute(new Runnable() {
				public void run() {
					for (int i = 0; i < round; i++) {
						sumValue();
					}
					// 每个线程执行完同步方法后就等待
					try {
						cb.await();
					} catch (InterruptedException e) {
						e.printStackTrace();
					} catch (BrokenBarrierException e) {
						e.printStackTrace();
					}
				}
			});
			se.execute(new Runnable() {
				public void run() {
					getValue();
					try {
						// 每个线程执行完同步方法后就等待
						cb.await();
					} catch (InterruptedException e) {
						e.printStackTrace();
					} catch (BrokenBarrierException e) {
						e.printStackTrace();
					}
				}
			});
		}
		try {
			// 当前统计线程也wait,所以CyclicBarrier的初始值是threadNum*2+1
			cb.await();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (BrokenBarrierException e) {
			e.printStackTrace();
		}
		// 所有线程执行完成之后，才会跑到这一步
		long duration = System.nanoTime() - start;
		System.out.println(id + " = " + duration);
	}
}
```
另外看一下synchronized对性能的影响有多大:
```
class SyncTest {
    private static Object lock = newObject();  
    public static void main(String[] args) {
        for(int k = 0; k < 10; k++) {
            long start = System.currentTimeMillis();
            for(int j = 0; j < 10000000; j++) {
                synchronized(lock) {
                    inti = 1;
                }
            }
            System.out.println(System.currentTimeMillis() - start);
 
            start = System.currentTimeMillis();
            for(int j = 0; j < 10000000; j++) {
                int i = 1;
            }
            System.out.println(System.currentTimeMillis() - start);
        }
 
    }
}
```
运行结果:301 
4 
307 
5 
297 
5 
290 
4 
295 
3 
296 
4 
302 
4 
296 
4 
299 
4 
294 
4
