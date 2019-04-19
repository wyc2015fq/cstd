# ScheduledExecutorService中scheduleAtFixedRate方法同步 - 零度的博客专栏 - CSDN博客
2016年05月20日 11:25:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：2839
       由于ScheduledExecutorServices.cheduleAtFixedRate是指定频率执行方法，若方法的执行时间大于指定的间隔时间，将可能会发生，同一时间点 ，执行方法N 次。
所以需要使用同步方法才确保并发操作的安全性。
```
import java.util.concurrent.Executors;
 import java.util.concurrent.ScheduledExecutorService;
 import java.util.concurrent.Semaphore;
 import java.util.concurrent.TimeUnit;
 public class TodoTimer {
	static Logger log = SimpleLogger.getLogger(TodoTimer.class);
	private TodoTimer() {
	}
	/**
	 * 定时操作线程
	 */
	private final static Runnable TodoOperation = new Runnable() {
		public void run() {
			try {
				semaphore.acquire();
 
				TodoManager.operate();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} finally {
				semaphore.release();
			}
		}
	};
	/**
	 * 同步信号量
	 */
	private static final Semaphore semaphore = new Semaphore(1);
	/**
	 * 定时执行
	 */
	private static ScheduledExecutorService scheduler = null;
	/**
	 * 启动定时器
	 */
	public static void start() {
		
		if (scheduler == null) {
			scheduler = Executors.newScheduledThreadPool(1);
			
			synchronized (scheduler) {
			
				/*
				 * TODO 设置定时器启动时间和间隔
				 */
				scheduler.scheduleAtFixedRate(TodoOperation, 20, 60*5, TimeUnit.SECONDS);
				log.debug("启动TODO定时器!");
			}
			
		} else {
			log.warn("TODO定时器正在运行!");
		}
		
	}
	/**
	 * 停止定时器
	 */
	public static void stop() {
		synchronized (scheduler) {
			if (scheduler != null && !scheduler.isShutdown()) {
				scheduler.shutdown();
				scheduler = null;
				log.debug("关闭TODO定时器!");
			}
		}
	}
	/**
	 * 重启定时器
	 */
	public static void restart() {
		stop();
		start();
	}
	/**
	 * 手动触发
	 */
	public static void trigger() {
		TodoOperation.run();
	}
}
```
