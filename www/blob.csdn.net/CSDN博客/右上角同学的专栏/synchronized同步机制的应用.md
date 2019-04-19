# synchronized同步机制的应用 - 右上角同学的专栏 - CSDN博客
2016年06月28日 21:57:31[右上角徐](https://me.csdn.net/u011032983)阅读数：196
```java
public class TextThread {
	/**
	 * 该程序是为了说明synchronized锁住对象的条件，
	 * 即同步的两个或多个线程必须是对同一对象上锁
	 * （不管是this表示的当前对象，还是其他定义的
	 * 对象如String对象等）
	 * 如果每个线程新建不同的对象，然后对不同对象上锁
	 * 如TxtThread tt0 = new TxtThread();TxtThread tt1 = new TxtThread();
	 * 然后对当前对象this上锁，将会失效。
	 */
	public static void main(String[] args) {
		// TODO 自动生成方法存根
		TxtThread tt0 = new TxtThread();
		// TxtThread tt1 = new TxtThread();
		// TxtThread tt2 = new TxtThread();
		// TxtThread tt3 = new TxtThread();
		tt0.flag = false;
		new Thread(tt0).start();
		TxtThread tt1 = new TxtThread();
		//tt1.flag = true;
		new Thread(tt0).start();
		// new Thread(tt0).start();
		// new Thread(tt0).start();
		// new Thread(tt1).start();
		// new Thread(tt2).start();
		// new Thread(tt3).start();
	}
}
class TxtThread implements Runnable {
	static String str = new String();
	boolean flag;
	public void run() {
		if (flag == true) {
			for (int i = 1; i <= 20; i++) {
				try {
					Thread.sleep(10);
				} catch (Exception e) {
					e.getMessage();
				}
				System.out.println("鎖之外");
			}
			synchronized (this) {
				int num = 100;
				while (num > 0) {
					try {
						Thread.sleep(10);
					} catch (Exception e) {
						e.getMessage();
					}
					System.out.println(Thread.currentThread().getName()
							+ "this is 模块1 " + num--);
				}
			}
		}
		if (flag == false) {
			synchronized (this) {
				int num = 100;
				while (num > 0) {
					try {
						Thread.sleep(10);
					} catch (Exception e) {
						e.getMessage();
					}
					System.out.println(Thread.currentThread().getName()
							+ "this is模块2 " + num--);
				}
			}
		}
	}
}
```
