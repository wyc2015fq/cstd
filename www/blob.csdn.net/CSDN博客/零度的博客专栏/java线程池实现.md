# java线程池实现 - 零度的博客专栏 - CSDN博客
2016年04月14日 09:16:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：372
个人分类：[Java并发编程](https://blog.csdn.net/zmx729618/article/category/6181602)
1、线程池类
```
package zmx.thread.pool;
import java.util.List;
import java.util.Vector;
public class ThreadPool {
	
	private static ThreadPool instance=null;
	
	//空闲的线程总数
	private List<PThread> idleThreads;
	
	//已有的线程总数
	private int threadCounter;
	
	private boolean isShutDown = false;
	
	private ThreadPool(){
		this.idleThreads = new Vector(5);
		threadCounter = 0;
	}
	
	public synchronized static ThreadPool getInstance(){
		if(instance ==null){
			instance = new ThreadPool();
		}
		return instance;
	}
	
	//获取线程总数
	public int getCreatedThreadsCount(){
		return threadCounter;
	}
    /**
     * 将线程放入池中
     * @param pThread
     */
	protected synchronized void repool(PThread pThread) {
		
		if(!isShutDown){
			idleThreads.add(pThread);
		}else{
			pThread.shutDown();
		}
		
		
	}
	
	/**
	 * 停止池中所有线程
	 */
	public synchronized void shutDown(){
		
		isShutDown = true;
		for(int threadIndex = 0; threadIndex < idleThreads.size(); threadIndex++){
			PThread idleThread = idleThreads.get(threadIndex);
			idleThread.shutDown();
		}
		
	}
	
	public synchronized void start(Runnable target){
		
		PThread thread = null;
		if(idleThreads.size() > 0){//如果有空闲线程，则直接使用
			
			int lastIndex =  idleThreads.size() - 1;
			thread = (PThread)idleThreads.get(lastIndex);
			idleThreads.remove(lastIndex);
			thread.setTarget(target);
			
		}else{//没有空闲线程，则创建新线程
			
			
			threadCounter++;
			
			thread = new PThread(this, target, "PThread #"+threadCounter) ;
			
			thread.start();
			
		}
		
	}
	
	
	
}
```
2、自定义线程
```
package zmx.thread.pool;
/**
 * 使用线程池 需要一个永不退出的线程
 * @author zhangwenchao
 *
 */
public class PThread extends Thread{
	/**
	 * 线程池
	 */
	private ThreadPool pool;
	
	//任务
	private Runnable target;
	
	private boolean isShutDown = false;
	
	private boolean isIdle = false;
	//构造函数
	public PThread(ThreadPool pool, Runnable target,String name) {
		super(name);
		this.pool = pool;
		this.target = target;
	}
	
	public Runnable getTarget(){
		
		return target;
		
	}
	
	public boolean isIdle(){
		
		return isIdle;
	}
	@Override
	public void run() {
		//只要没有关闭，则一直不结束该线程
		while(!isShutDown){
			isIdle = false;
			if(target!=null){
				target.run();
			}
			
			//任务执行完毕
			isIdle = true;
			try{
				//任务结束后，不关闭线程，而是放入线程池中
				pool.repool(this);  
				synchronized (this) {
					wait();  //线程等待
				}	
				
			}catch (Exception e) {
				
			}
			
			//isIdle = false;		
		}		
	}
	
	//设置任务
	public synchronized void setTarget(Runnable newTarget){
		
		target = newTarget;		
		notifyAll();
		
	}
	
	//关闭线程
	public synchronized void shutDown(){
		isShutDown = true;
		notifyAll();
	}
	
	
	
	
	
	
}
```
3、main测试类
```
package zmx.thread.pool;
public class Main {
	
	 static class MyThread implements Runnable{
		protected String name;
		
		public MyThread() {
			
		}
        public MyThread(String name) {
			this.name = name;
		}
		@Override
		public void run() {
		
			try {
				Thread.sleep(1000);
				System.out.println(name+"线程执行完毕");
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
		}
		
	}
	
	public static void main(String[] args) {
		
		for(int i=0;i<1000;i++ ){
			ThreadPool.getInstance().start(new MyThread("testPool"+i));
		}
	}
}
```
