# 多线程中的"断点"续传《notify()和wait()》 - DEVELOPER - CSDN博客





2014年06月01日 21:27:49[学术袁](https://me.csdn.net/u012827205)阅读数：1801标签：[android																[notify和wait的使用																[下载的断点续传																[IllegalMonitorStateE](https://so.csdn.net/so/search/s.do?q=IllegalMonitorStateE&t=blog)
个人分类：[使用notify和wait实现下载的断点续传](https://blog.csdn.net/u012827205/article/category/2302841)





目前在做一个项目，关于软件管理与下载的，估计项目提交日期定在6月9号。项目做了有20天了，但是在一个功能上卡住了。在这个项目中有一个功能————APK的下载需要实现。相信大家都玩过很多关于下载APK的软件，在这个下载功能上，应该能够清楚的知道有：断点续传；也就是当你点击下载之后的下载过程中，可以点击暂停来临时控制此时要不要继续下载。当点击继续下载的时候，这个软件会接着暂停之前的进度继续下载。

由于第一次接触这种功能的实现，在网络上搜索到的都是使用Sqlite来记录下载的进度，然后通过sqlite所记录的下载进度控制。读了三天时间，没有什么进展。别人写的代码充斥着各种的嵌套、包装，看得我晕头转向。最后决定，还是靠自己写吧。

最后突然想到我可以使用：*wait*** 和*** notify(notifyall)*   来控制。然后，思路瞬间清晰了很多。当然，这个东西我不常用，不是很熟。不过没关系，有网络嘛，自学一下就OK了

***首先介绍一下wait 和 notify(notifyall)的规范说明及使用方法：***



wait()、notify()、notifyAll()是三个定义在Object类里的方法，可以用来控制线程的状态。这三个方法最终调用的都是jvm级的native方法，随着jvm运行平台的不同可能有些许差异。

1.如果对象调用了wait方法就会使持有该对象的线程把该对象的控制权交出去，然后处于等待状态。


2.如果对象调用了notify方法就会通知某个正在等待这个对象的控制权的线程可以继续运行。


3.如果对象调用了notifyAll方法就会通知所有等待这个对象控制权的线程继续运行。

其中wait方法有三个重载的方法：<wait 方法通过参数可以指定等待的时长。如果没有指定参数，默认一直等待直到被通知。>

  wait()、wait(long)、wait(long,int)




以下是一个演示代码，以最简洁的方式说明复杂的问题：

NotifyThread是用来模拟3秒钟后通知其他等待状态的线程的线程类；

WaitThread是用来模拟等待的线程类；

等待的中间对象是flag，一个String对象；

main方法中同时启动一个Notify线程和三个wait线程；

```java
public class NotifyTest {
	private  String flag = "true";

	class NotifyThread extends Thread{
		public NotifyThread(String name) {
			super(name);
		}
		public void run() {		
			try {
				sleep(3000);//推迟3秒钟通知
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
				flag = "false";
				flag.notify();
		}
	};

	class WaitThread extends Thread {
		public WaitThread(String name) {
			super(name);
		}

		public void run() {
			
				while (flag!="false") {
					System.out.println(getName() + " begin waiting!");
					long waitTime = System.currentTimeMillis();
					try {
						flag.wait();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					waitTime = System.currentTimeMillis() - waitTime;
					System.out.println("wait time :"+waitTime);
				}
				System.out.println(getName() + " end waiting!");
			
		}
	}

	public static void main(String[] args) throws InterruptedException {
		System.out.println("Main Thread Run!");
		NotifyTest test = new NotifyTest();
		NotifyThread notifyThread =test.new NotifyThread("notify01");
		WaitThread waitThread01 = test.new WaitThread("waiter01");
		WaitThread waitThread02 = test.new WaitThread("waiter02");
		WaitThread waitThread03 = test.new WaitThread("waiter03");
		notifyThread.start();
		waitThread01.start();
		waitThread02.start();
		waitThread03.start();
	}

}
```
如果你拿这段程序去运行下的话， 会发现根本运行不了，它会报下面的错误：

java.lang.IllegalMonitorStateException:object not locked by thread before notify()



首先，这儿要非常注意的几个事实是：
- 任何一个时刻，对象的控制权（monitor）只能被一个线程拥有。
- 无论是执行对象的wait、notify还是notifyAll方法，必须保证当前运行的线程取得了该对象的控制权（monitor）
- 如果在没有控制权的线程里执行对象的以上三种方法，就会报java.lang.IllegalMonitorStateException异常。
- JVM基于多线程，默认情况下不能保证运行时线程的时序性

基于以上几点事实，我们需要确保让线程拥有对象的控制权。也就是说在waitThread中执行wait方法时，要保证waitThread对flag有控制权；在notifyThread中执行notify方法时，要保证notifyThread对flag有控制权。

线程取得控制权的方法有三：
- 执行对象的某个同步实例方法。
- 执行对象对应类的同步静态方法。
- 执行对该对象加同步锁的同步块。

我们用第三种方法来做说明：将以上notify和wait方法包在同步块中

```java
synchronized (flag) {
				flag = "false";
				flag.notify();
			}
```


```java
synchronized (flag) {
				while (flag!="false") {
					System.out.println(getName() + " begin waiting!");
					long waitTime = System.currentTimeMillis();
					try {
						flag.wait();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					waitTime = System.currentTimeMillis() - waitTime;
					System.out.println("wait time :"+waitTime);
				}
				System.out.println(getName() + " end waiting!");
			}
```


问题解决了吗？当然没有。

好像运行还是报错java.lang.IllegalMonitorStateException。怎么办？

这时的异常是由于在针对flag对象同步块中，更改了flag对象的状态所导致的。如下：

flag = "false";    flag != "false";    flag.notify();

对在同步块中对flag进行了赋值操作，使得flag引用的对象改变，这时候再调用notify方法时，因为没有控制权所以抛出异常。

我们可以改进一下，将flag改成一个数组，然后更改它的属性不会影响到flag的引用。

我们这里改成数组来试试，也可以达到同样的效果：

```java
private   String flag[] = {"true"};
```


```java
synchronized (flag) {
				flag[0] = "false";
				flag.notify();
			}
```


```java
synchronized (flag) {
				while (flag[0]!="false") {
					System.out.println(getName() + " begin waiting!");
					long waitTime = System.currentTimeMillis();
					try {
						flag.wait();
						
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
```


这时候再运行，不再报异常，但是线程没有结束是吧，没错，还有线程堵塞，处于wait状态。

原因很简单，我们有三个wait线程，只有一个notify线程，notify线程运行notify方法的时候，是随机通知一个正在等待的线程，所以，现在应该还有两个线程在waiting。

我们只需要将NotifyThread线程类中的flag.notify()方法改成notifyAll()就可以了。notifyAll方法会通知所有正在等待对象控制权的线程。

最终完成版如下：



```java
public class NotifyTest {
	private String flag[] = { "true" };

	class NotifyThread extends Thread {
		public NotifyThread(String name) {
			super(name);
		}

		public void run() {
			try {
				sleep(3000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			synchronized (flag) {
				flag[0] = "false";
				flag.notifyAll();
			}
		}
	};

	class WaitThread extends Thread {
		public WaitThread(String name) {
			super(name);
		}

		public void run() {
			synchronized (flag) {
				while (flag[0] != "false") {
					System.out.println(getName() + " begin waiting!");
					long waitTime = System.currentTimeMillis();
					try {
						flag.wait();

					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					waitTime = System.currentTimeMillis() - waitTime;
					System.out.println("wait time :" + waitTime);
				}
				System.out.println(getName() + " end waiting!");
			}
		}
	}

	public static void main(String[] args) throws InterruptedException {
		System.out.println("Main Thread Run!");
		NotifyTest test = new NotifyTest();
		NotifyThread notifyThread = test.new NotifyThread("notify01");
		WaitThread waitThread01 = test.new WaitThread("waiter01");
		WaitThread waitThread02 = test.new WaitThread("waiter02");
		WaitThread waitThread03 = test.new WaitThread("waiter03");
		notifyThread.start();
		waitThread01.start();
		waitThread02.start();
		waitThread03.start();
	}

}
```


其实通过上面的就能够学会怎么使用wait和notigy(notifyall)了，因为它的使用上就这么多。应用在断点续传上已经可以解决问题了。


***最后的最后来看看我的部分代码通过notify()和wait()实现断点续传：***



```java
/**
 * DownloadTaskAsnc extends AsyncTask<String, Integer, LoadInfobean>
 * 该线程类是完成对  DownFileTask extends AsyncTask<String, Integer, String> 线程类的调用。
 * 这个类在对下载调用之前判断，如果处于暂停状态则激活使其继续下载
 */
	@Override
	protected void onPostExecute(LoadInfobean loadInfobean) {
		// TODO Auto-generated method stub
		super.onPostExecute(loadInfobean);
//		if(DownFileTask.stateDownload==DownFileTask.DOWNLOADING) return ;
//		DownFileTask.stateDownload=DownFileTask.DOWNLOADING;

                     /**
                      *当下载文件的线程处于等待状态时，激活线程使其继续下载文件
                      * 看看notify()的使用就会懂得怎么用了，呵呵...
                      */
                    synchronized (DownFileTask.DOWNLOAD_PAUSE) {
		      if(DownFileTask.stateDownload==DownFileTask.DOWNLOAD_PAUSE){
			DownFileTask.stateDownload=DownFileTask.DOWNLOADING;
			DownFileTask.DOWNLOAD_PAUSE.notifyAll();
			return;
			}
		}
		downFileTask.execute(url);
		showProgress(loadInfobean,vi);
	}
```







```java
/**
	 * DownFileTask extends AsyncTask<String, Integer, String>
	 * 
	 * 定义了属性： public static final String DOWNLOAD_INIT = "1",DOWNLOADING =
	 * "2",DOWNLOAD_PAUSE = "3"; public static String stateDownload =
	 * DOWNLOAD_INIT;
	 * 
	 */
	@SuppressWarnings("resource")
	@Override
	protected String doInBackground(String... params) {

		url = params[0];
		if (!checkWorkNet(context)) {
			Toast.makeText(context, "无网络连接", Toast.LENGTH_SHORT).show();
			return "无网络联接";
		}

		if (!TextUtils.isEmpty(url)) {
			URLConnection connection = null;
			InputStream inputStream = null;
			RandomAccessFile randomAccessFile = null;
			try {

				URL myUrl = new URL(url);
				connection = myUrl.openConnection();
				connection.setConnectTimeout(5 * 1000);
				// 设置范围，格式为Range：bytes x-y;
				int startPos = downloadBeans.get(0).getStartPos();
				int endPos = downloadBeans.get(0).getEndPos();
				connection.setRequestProperty("Range", "bytes="
						+ (startPos + completeSize) + "-" + endPos);
				connection.connect();
				inputStream = connection.getInputStream();
				fileSize = connection.getContentLength();
				Log.d("vv", "downfiletask中的filezise==" + fileSize);

				if (fileSize <= 0)
					return "文件为空";

				else {
					downloadFileBean.setFileName(fileName);
					downloadFileBean.setFileSize(fileSize);
					downloadFileBean.setDownLoadAddress(url);
					downloadFileBean.setDownloadState("下载");

					randomAccessFile = new RandomAccessFile(localfilePath, "rw");
					randomAccessFile.seek(startPos + completeSize);

					// 存储文件缓存
					byte buf[] = new byte[2 * 1024];
					int length = 0;

					while ((length = inputStream.read(buf)) != -1) {
						// if (isDown) {
						// outputStream.write(buf, 0, length);

						randomAccessFile.write(buf, 0, length);
						completeSize += length;

						// 实时更新数据库下载信息
						DataDbHelper.getInstance(context).updataInfos(threadId,
								completeSize, url);
						Log.d("vv", "读写数据while (completeSize)==" + completeSize);
						// 原稿的进度保存
						downloadFileBean.setDownloadSize(completeSize);
						fileMap.put(fileName, downloadFileBean);

                          //通过wait()使下载文件的线程处于等待的状态
                              synchronized (DOWNLOAD_PAUSE) {
				if (stateDownload == DOWNLOAD_PAUSE) {

				 DOWNLOAD_PAUSE.wait();
					}
				}
					}
				}
			} catch (Exception e) {
				Log.d("vv", "randomAccessFile.seek downloadtask 中出错了=="
						+ completeSize);
				return "出错了";
			} finally {
				try {

					if (inputStream != null || randomAccessFile != null) {
						randomAccessFile.close();
						inputStream.close();
					}
				} catch (Exception e2) {
				}
			}

		}

		return "下载完成";
	}
```](https://so.csdn.net/so/search/s.do?q=下载的断点续传&t=blog)](https://so.csdn.net/so/search/s.do?q=notify和wait的使用&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




