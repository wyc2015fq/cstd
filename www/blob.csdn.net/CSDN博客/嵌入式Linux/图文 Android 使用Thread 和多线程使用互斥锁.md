
# 图文|Android 使用Thread 和多线程使用互斥锁 - 嵌入式Linux - CSDN博客

2019年03月04日 11:45:27[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：54



## 为什么需要多线程进行开发？
多线程不管是嵌入式系统RTOS，Linux，还是应用开发，中间件开发，都是必不可少的，做一个技术的时候，如果能做到举一反三，下次使用的时候不会再遇到坑，我这次给出的例子是Android 的多线程开发。
![](https://img-blog.csdnimg.cn/20190307185044611.png)
![](https://img-blog.csdnimg.cn/20190307190248267.png)
## 如何使用一个线程
在Android 应用程序里面如何使用一个线程呢?直接看下面的代码，代码不是很多，如果需要用的话直接摘抄过去就好了。
`//定义一个线程
private SendThread mSendThread = null;
/**
 * 线程实体
 */
private class SendThread extends Thread{
	public void run() {
	}
}
//实例化线程
if (mSendThread == null){
   mSendThread = new SendThread();
}
//启动线程
mSendThread.start();`![](https://img-blog.csdnimg.cn/20190307191624635.png)
## 多次调用start是同一个堆栈空间吗？
如果只new了一次线程，多次start，会出现怎么样的情况呢？
**android 线程start的函数原型如下**
`public synchronized void start() {
		/**
		 * This method is not invoked for the main method thread or "system"
		 * group threads created/set up by the VM. Any new functionality added
		 * to this method in the future may have to also be added to the VM.
		 *
		 * A zero status value corresponds to state "NEW".
		 */
		// Android-changed: throw if 'started' is true
		if (threadStatus != 0 || started)
			throw new IllegalThreadStateException();
		/* Notify the group that this thread is about to be started
		 * so that it can be added to the group's list of threads
		 * and the group's unstarted count can be decremented. */
		group.add(this);
		started = false;
		try {
			nativeCreate(this, stackSize, daemon);
			started = true;
		} finally {
			try {
				if (!started) {
					group.threadStartFailed(this);
				}
			} catch (Throwable ignore) {
				/* do nothing. If start0 threw a Throwable then
				  it will be passed up the call stack */
			}
		}
		}`**然后我做了下面的一个代码实验**
`/**
 * 发送线程实体
 */
private class SendThread extends Thread{
	int testCount = 20;
	public void run() {
		while(testCount >0)
		{
			Log.d(TAG,"testCount:"+testCount);
			testCount --;
		}
	}
}
//实例化线程
if (mSendThread == null){
   mSendThread = new SendThread();
}
//启动线程
mSendThread.start();
mSendThread.start();`**结果输出如下**
`D/ttyusb: testCount:20
D/ttyusb: testCount:20
D/ttyusb: testCount:19
D/ttyusb: testCount:19
D/ttyusb: testCount:18
D/ttyusb: testCount:18
D/ttyusb: testCount:17
D/ttyusb: testCount:16
D/ttyusb: testCount:17
D/ttyusb: testCount:15
D/ttyusb: testCount:16
D/ttyusb: testCount:14
D/ttyusb: testCount:15
D/ttyusb: testCount:13
D/ttyusb: testCount:14
D/ttyusb: testCount:12
D/ttyusb: testCount:13
D/ttyusb: testCount:11
D/ttyusb: testCount:12
D/ttyusb: testCount:10
D/ttyusb: testCount:9
D/ttyusb: testCount:11
D/ttyusb: testCount:8
D/ttyusb: testCount:10
D/ttyusb: testCount:7
D/ttyusb: testCount:9
D/ttyusb: testCount:6
D/ttyusb: testCount:5
D/ttyusb: testCount:8
D/ttyusb: testCount:4
D/ttyusb: testCount:7
D/ttyusb: testCount:3
D/ttyusb: testCount:6
D/ttyusb: testCount:2
D/ttyusb: testCount:1
D/ttyusb: testCount:5
D/ttyusb: testCount:4
D/ttyusb: testCount:3
D/ttyusb: testCount:2
D/ttyusb: testCount:1`可以看出线程每次start后，他们使用的堆栈空间是不相同的。
![](https://img-blog.csdnimg.cn/20190307193901826.png)
## 在双线程里面使用互斥锁
使用互斥锁的情况非常普遍，但是新手写代码肯定会有道意想不到的问题，我就是那个新手，我就遇到了那个意想不到的问题。
![](https://img-blog.csdnimg.cn/20190308100244863.png)
**给出下面一段代码**
`/**
     * 线程实体
     */
    public class SendThread extends Thread {

        public void run() {
            isStart = true;
            for(int i=0;i<20;i++)
            {
                lock.lock();
                successCount = i;
                lock.unlock();
                Log.d(TAG, "Write:testCount:" + successCount);
            }
            isStart = false;
        }
    }
    /**
     * 接收数据的线程
     */
    public class ReceiveThread extends Thread {
        @Override
        public void run() {
            int testCount = 20;
            super.run();
            //条件判断，只要条件为true，则一直执行这个线程
            while (isStart == true) {
                testCount = successCount;
                Log.d(TAG, "Read:testCount:" + testCount);
            }
        }
    }`**代码执行的流程大概如下**
![](https://img-blog.csdnimg.cn/20190308113248802.png)
![](https://img-blog.csdnimg.cn/20190308105412482.png)
**代码输出**
[
](https://img-blog.csdnimg.cn/20190308105412482.png)`03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:0
03-08 11:41:35.383 14866-14866/? D/TEST: 启动线程完成
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:1
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:2
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:1
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:3
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:3
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:4
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:4
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:5
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:5
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:6
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:6
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:7
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:7
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:8
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:8
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:9
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:9
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:10
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:10
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:11
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:11
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:12
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:12
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:13
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:13
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:14
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:14
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:15
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:15
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:16
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:16
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:17
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:17
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:18
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:18
03-08 11:41:35.383 14866-14906/? D/TEST: Write:testCount:19
03-08 11:41:35.383 14866-14907/? D/TEST: Read:testCount:19`[
](https://img-blog.csdnimg.cn/20190308105412482.png)**上述代码问题**
[
](https://img-blog.csdnimg.cn/20190308105412482.png)我们希望的代码流程是，我发送一个数据，你就接收一个数据，不能出现数据丢失的情况，但是上面的日子来看，接收数据是发生了丢失，这样的情况如果在实际应用中是非常危险的。
![](https://img-blog.csdnimg.cn/20190308114823118.png)
**所以代码需要类似下面这样修改**
`/**
     * 线程实体
     */
    public class SendThread extends Thread {

        public void run() {
            isStart = true;
            for(int i=0;i<20;i++)
            {
                Log.d(TAG,"开始写线程~");
                lock.lock();
                successCount = i;
                Log.d(TAG, "写数据:testCount:" + successCount);
                lock.unlock();
                Log.d(TAG,"写线程休眠~");
                sendTreadSleep(10);
            }
            isStart = false;
        }
    }
    /**
     * 接收数据的线程
     */
    public class ReceiveThread extends Thread {
        @Override
        public void run() {
            int testCount = 20;
            super.run();
            //条件判断，只要条件为true，则一直执行这个线程
            while (isStart == true) {
                Log.d(TAG,"开始读线程~");
                lock.lock();
                testCount = successCount;
                Log.d(TAG, "读数据~:testCount:" + testCount);
                lock.unlock();
                Log.d(TAG,"读线程休眠~");
                receiveTreadSleep(5);
            }
        }
    }`工程的Demo到时候在文末给出，写线程在写完后，就休眠10MS，然后再到读线程执行，加的日志非常方便大家观看两个线程之间的运行逻辑。
![](https://img-blog.csdnimg.cn/2019030814352827.png)
**日志输出如下**
`D/TEST: 启动线程完成
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:0
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:0
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:0
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:1
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:1
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:1
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:2
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:2
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:2
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:3
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:3
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 开始读线程~
 D/TEST: 写数据:testCount:4
 D/TEST: 写线程休眠~
 D/TEST: 读数据~:testCount:4
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:4
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:5
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:5
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:5
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:6
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:6
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:6
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:7
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:7
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:7
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:8
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:8
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:8
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:9
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:9
 D/TEST: 读线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:9
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:10
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:10
 D/TEST: 读线程休眠~
grande.ttyusb.test0001/com.evergrande.ttyusb.test0001.MainActivity] connect: already connected (cur=1 req=1)
 D/TEST: 开始读线程~
 D/TEST: 开始写线程~
 D/TEST: 读数据~:testCount:10
 D/TEST: 读线程休眠~
 D/TEST: 写数据:testCount:11
 D/TEST: 写线程休眠~
 D/mali_winsys: EGLint new_window_surface(egl_winsys_display*, void*, EGLSurface, EGLConfig, egl_winsys_surface**, egl_color_buffer_format*, EGLBoolean) returns 0x3000
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:11
 D/TEST: 读线程休眠~
 D/TEST: 开始写线程~
 D/TEST: 写数据:testCount:12
 D/TEST: 写线程休眠~
 D/TEST: 开始读线程~
 D/TEST: 读数据~:testCount:12`
## 使用synchronized来达到完成互斥
synchronized修饰的方法可以让两个方法之间完成互斥，比如写和读互斥，写和写互斥，读和读互斥，都可以用这个方法。
**使用代码如下**
`/**
     * 线程实体
     */
    public class SendThread extends Thread {

        public void run() {
            isStart = true;
            for(int i=0;i<10;i++)
            {
                Log.d(TAG,"开始写线程~");
                mDataValue.setData(i);
                Log.d(TAG, "写数据:testCount:" + successCount);
                Log.d(TAG,"写线程休眠~");
                sendTreadSleep(10);
            }
            isStart = false;
        }
    }
    /**
     * 接收数据的线程
     */
    public class ReceiveThread extends Thread {
        @Override
        public void run() {
            int testCount = 20;
            Log.d(TAG,"s开始读线程~");
            super.run();
            //条件判断，只要条件为true，则一直执行这个线程
            while (isStart == true) {
                Log.d(TAG,"开始读线程~");
                testCount = mDataValue.getData();
                Log.d(TAG, "读数据~:testCount:" + testCount);
                Log.d(TAG,"读线程休眠~");
                receiveTreadSleep(5);
            }
        }
    }
    /**
     * 发送线程延迟
     * @param millis 毫秒
     */
    private void sendTreadSleep(int millis)
    {
        try{
            mSendThread.sleep(millis);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    /**
     * 接收线程延迟
     * @param millis 毫秒
     */
    private void receiveTreadSleep(int millis)
    {
        try{
            mReceiveThread.sleep(millis);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private class DataValue{
        private synchronized void setData(int value){
            Log.d(TAG,"设置数据~setData");
            successCount = value;
        }
        private synchronized  int getData(){
            Log.d(TAG,"获取数据~getData");
            return successCount;
        }
    }`
## 读写锁ReentrantReadWriteLock
上面是使用互斥锁，这里介绍一个读写锁，也是用来完成互斥的。使用代码如下
`/**
     * 线程实体
     */
    public class SendThread extends Thread {

        public void run() {
            isStart = true;
            for(int i=0;i<10;i++)
            {
                mrwDataValue.setData(i);
                Log.d(TAG,Thread.currentThread().getName() +"写休眠");
                sendTreadSleep(10);
            }
            isStart = false;
        }
    }
    /**
     * 接收数据的线程
     */
    public class ReceiveThread extends Thread {
        @Override
        public void run() {
            int testCount = 20;
            Log.d(TAG,"s开始读线程~");
            super.run();
            //条件判断，只要条件为true，则一直执行这个线程
            while (isStart == true) {
                mrwDataValue.getData();
                Log.d(TAG,Thread.currentThread().getName() +"读休眠");
                receiveTreadSleep(5);
            }
        }
    }
    /*
     * 使用读写锁完成互斥ReadWriteLock
     */
    private class rwDataValue{
        private ReadWriteLock readWriteLock = new ReentrantReadWriteLock();
        private int Data;
        private void setData(int value){
            readWriteLock.writeLock().lock();
            try {
                Log.d(TAG, Thread.currentThread().getName() +"写数据~setData:"+value);
                Data = value;
                Thread.sleep(30);
            }catch (Exception i){
                Log.e(TAG,"error");
            }finally {
                readWriteLock.writeLock().unlock();
            }
        }
        private  void getData(){
            readWriteLock.readLock().lock();
            try {
                Log.d(TAG,Thread.currentThread().getName() +"获取数据~getData：" +Data);
                Thread.sleep(10);
            }catch (Exception i){
                Log.e(TAG,"error");
            }finally {
                readWriteLock.readLock().unlock();
            }
        }`![](https://img-blog.csdnimg.cn/20190308155212141.png)
![](https://img-blog.csdnimg.cn/20190308155401912.png)
![](https://img-blog.csdnimg.cn/20190308155733703.png)
![](https://img-blog.csdnimg.cn/20190308160432524.png)
## 参考
[https://blog.csdn.net/zy_style/article/details/53423877](https://blog.csdn.net/zy_style/article/details/53423877)
## Demo 代码
[https://github.com/weiqifa0/androitdThread](https://github.com/weiqifa0/androitdThread)
[
						](https://img-blog.csdnimg.cn/20190308155733703.png)
[
	](https://img-blog.csdnimg.cn/20190308155733703.png)
[
  ](https://img-blog.csdnimg.cn/20190308155401912.png)