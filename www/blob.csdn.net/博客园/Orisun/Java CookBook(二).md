# Java CookBook(二) - Orisun - 博客园







# [Java CookBook(二)](https://www.cnblogs.com/zhangchaoyang/articles/2181403.html)





**通过HTTP连接访问资源**

URLConnection封装任何对URL连接的基本功能，HttpURLConnection基于HTTP的连接扩展URLConnection。

Step1.创建URL对象。URL(String rul) throws MalformedURLException

Step2.URL对象调用openConnection()，并将其返回的引用强制转换成HttpURLConnection。

Step3.HttpURLConnection对象调用getContentType()获取资源的内容类型，调用getContentLength()获取资源的内容长度，调用Map<String,List<String>> getHeaderFields()返回包含所有标题字段的键值对。

Step4.HttpURLConnection对象调用getInputStream()获取资源的输入流。读取这个流就能够下载资源。

**applet和servlet**

applet是一种小程序，它动态地通过Web发送，并在浏览器中运行。servlet是在服务器端运行的小程序。applet有两种：直接基于java.applet定义的Applet类；基于Swing的applet是现在最流行的。

**多线程**

虽然多线程增加了编程的复杂性，但现在的趋势是使用更多的线程。

基于任务的多进程并不在Java的控制之下。

对于涉及到I/O操作时，尤其适用于多线程。

在事件模型中，如果某个事件持续时间较长的动作，它必须由独立的线程执行，否则可能得不到及时地处理，显得应用程序缓慢，也可能事件丢失。

要创建一个新的线程，要么扩展Thread类，要么实现Runnable接口。

Thread(ThreadGroup thrdGroup,Runnable thrdObj,String thrdName)

Thread(Runnable thrdObj,String thrdName)

Thread(Runnable thrdObj)

Thread(String thrdName)

在开发、测试和调度中线程名thrdName是很有用的。

Long tid=Thread.currentThread().getId();

线程ID可以利用，当某个线程被终止时，新的线程可以被赋予旧线程的ID。

join()无限期地等待线程结束，也可以指定最长等待时间。

Final void join(long milliseconds) throws InterruptedException

Final void join(long milliseconds,int nanoseconds) throws InterruptedException

synchronized(objref){ } //同步块把对象 objref锁住

synchronized returnType methodName(arg-list){ }同步方法每次只能有一个对象调用它。

if(Character.isWhitespace(msg.charAt(i)))

new Thread(this).start();

线程调用调用wait()放弃对对象的控制，一直等待notify()通知。notify()会恢复某个正在等待的线程，notifyAll() 会恢复所有正在等待的线程。

同sleep()一样，wait()是无限期等待，我们也可为其设置时限：

fianl void wait(long milliseconds) throws InterruptedException

Final void wait(long milliseconds,int nanoseconds) throws InterruptedException

不推荐使用suspend(),resume()和stop()方法。

Final void setDaemon(boolean how) how为true时，把线程设为守护状态；为false时，设为用户状态。必须在启动线程之前调用。

Final boolean isDaemon()

一个线程想中断另一个线程可以对其调用interrupt()。判断线程是否被中断可调用interrupted()。

当线程因sleep(),wait(),join()而暂停时，对其调用interrupt()会抛出InterruptedException。

Static void yield()可使另外的线程，包括优先级更低的线程获取CPU访问权。

ThreadGroup(String name)

Thread(ThreadGroup thrdGroup,Runnable thrdObj,String thrdName)

ThreadGroup中的方法：

int activeCount() 组中活跃的线程数量

int enumerate(Thread[] thrds) 获得组中线程列表

final void interrupt()中断组中所有线程

****利用信号量****

package Nine;

/**
 * java并发实用工具对原始多线程和同步方法的补充而不是替换，这些原始方法是简练且高效的
 */
import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.Semaphore;

public class SemDemo {

	public static void main(String[] args) {
		Semaphore sem = new Semaphore(1, true);		//信号量设为1则为互斥锁
		FileWriter fw;
		try {
			fw = new FileWriter("Log.dat");
		} catch (IOException exe) {
			System.out.println("Error writing file.");
			return;
		}
		Thread thrA=new Thread(new SyncOutput(sem,fw,"Transfer Complete"));
		Thread thrB=new Thread(new SyncOutput(sem,fw,"Connection lost!"));
		thrA.start();
		thrB.start();
		try{
			thrA.join();
			thrB.join();
			fw.close();
		}catch (IOException exe) {
			System.out.println("Error Closing file.");
		} catch (InterruptedException exec) {
			System.out.println("Intrerupted");
		}
	}
}

class SyncOutput implements Runnable {
	Semaphore sem;
	String msg;
	FileWriter fw;

	SyncOutput(Semaphore s, FileWriter f, String m) {
		sem = s;
		msg = m;
		fw = f;
	}

	public void run() {
		try {
			sem.acquire(); // 申请获得通行证
			for (int i = 0; i < msg.length(); i++) {
				fw.write(msg.charAt(i));
				Thread.sleep(10); // 故意给其他线程运行的机会，但由于设置为互斥锁，其他线程是得不到运行机会的
			}
			fw.write("\n");
		} catch (IOException exe) {
			System.out.println("Error writing file.");
		} catch (InterruptedException exec) {
			System.out.println(exec);
		}
		sem.release(); // 释放通行证
	}
}

/*
 * 由于设置了互斥锁，线程A在执行的过程中线程B不能打断，所以Log.dat文件中的内容是：
 * Transfer Complete
 * Connection lost!
 * 如果把“sem.acquire();”注释掉，则线程AB交替执行，Log.dat文件内容很混乱：
 * TCroannnsefcetri oCno mlpolsett!e
 * 
*/

****从线程返回一个值****

package Nine;

import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class TheRetVal {

	public static void main(String[] args){
		ExecutorService es=Executors.newFixedThreadPool(3);	//使用纯种池来执行任务，池中有三个线程。线程可以重复使用，即线程执行完一个途程后可以去执行另外一个线程。线程的利用比创建多个独立的线程更有效率
		Future<Double> f;
		Future<Integer> f2;
		Future<Boolean> f3;
		
		Double nums[]={1.0,2.2,3.5,7.25};
		
		f=es.submit(new Avg(nums));
		f2=es.submit(new Factorial(4));
		f3=es.submit(new WriteField("test.dat","this is a test"));
		
		try{
			System.out.println(f.get());
			System.out.println(f2.get());
			System.out.println(f3.get());
		}catch(ExecutionException e){
			e.printStackTrace();
		}catch(InterruptedException exe){
			exe.printStackTrace();
		}
		es.shutdown();
	}
}

class Avg implements Callable<Double>{
	Double[] data;
	Avg(Double[] d){
		data=d;
	}

	@Override
	public Double call() throws Exception {
		double sum=0.0;
		// TODO Auto-generated method stub
		for(int i=0;i<data.length;i++){
			sum+=data[i];
		}
		return sum/data.length;
	}
}
class WriteField implements Callable<Boolean>{

	String fname;
	String msg;
	WriteField(String n,String info){
		fname=n;
		msg=info;
	}
	@Override
	public Boolean call() throws Exception {
		// TODO Auto-generated method stub
		FileWriter fw=null;
		boolean result=true;
		try{
			fw=new FileWriter(fname);
			fw.write(msg);
		}catch(IOException e){
			result=false;
		}
		try{
			if(fw!=null)fw.close();
		}catch(IOException e){
			result=false;
		}
		return result;
	}
}
class Factorial implements Callable<Integer>{

	int num;
	Factorial(int v){num=v;}
	@Override
	public Integer call() throws Exception {
		// TODO Auto-generated method stub
		int fact=1;
		for(int i=2;i<=num;i++){
			fact*=i;
		}
		return fact;
	}
	
}

****利用反射动态创建对象并调用其方法****

package Nine;
/**
 * 利用反射在运行时动态地创建实例并执行其方法，这样在编译时并不需要使用类
 */
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

class MyClass{
	private int count;
	
	MyClass(int c){
		System.out.println("Executing MyClass(int)."+" Argument is "+c);
		count=c;
	}
	MyClass(){
		System.out.println("Executing MyClass().");
		count=0;
	}
	void setCount(int c){
		System.out.println("Executing setCount(int)."+" Argument is "+c);
		count=c;
	}
	int getCount(){
		System.out.println("Executing getCount().");
		return count;
	}
	void showcount(){
		System.out.println("Executing showCount()."+" count is "+count);
	}
}
public class UseReflection {

	public static void main(String[] args){
		try{
			System.out.println("Use Relfection to execute MyClass constructors and methods.\n");
			System.out.println("Create an instance of MyClass.");
			Class c=Class.forName("MyClass");		//通过Class.forName获取一个Class实例（也可获取接口的信息）
			Constructor constructors[]=c.getConstructors();		//获取类的构造函数信息
			Object obj=null;
			for(Constructor cons:constructors){
				Class[] params=cons.getParameterTypes();	//获取构造函数的参数类型
				if(params.length==1 && params[0]==int.class){
					obj=cons.newInstance(10);		//通过反射获取的构造函数创建类的一个实例
					break;
				}
			}
			if(obj==null){
				System.out.println("Can't create MyClass object.");
				return;
			}
			System.out.println("\nExecute all MyClass methods.");
			Method[] methods=c.getDeclaredMethods();	//通过反射获取类的方法信息
			for(Method meth:methods){
				Class[] params=meth.getParameterTypes();	//获取方法的参数信息
				if(params.length==1 && params[0]==int.class)
					meth.invoke(obj, 99);		//执行方法
				else if(params.length==0){
					if(meth.getReturnType()==int.class){		//获取方法的返回类型
						Object retval=meth.invoke(obj);		//执行方法并获得其返回值
						System.out.println("Method returns:"+(Integer)retval);
					}else if(meth.getReturnType()==void.class){
						meth.invoke(obj);
					}
				}
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
}

****安排未来执行的任务****

/**
 * Author: Orisun
 * Date: Sep 16, 2011
 * FileName: SchedTask.java
 * Function: TimerTask定义要执行的任务，而Timer提供调度安排
 */
package Nine;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

public class SchedTask {

	public static void main(String[] args){
		AutoBkUp bkupTask=new AutoBkUp("Mydata.dat","Mydata.bak");
		Timer bkTimer=new Timer();
		bkTimer.schedule(bkupTask, 2000,2000);		//每隔2秒执行一次，每次持续2秒
		for(int i=0;i<5;i++){
			try{
				Thread.sleep(2100);		//主线程休息2.1秒，让给AutoBkUp任务去执行
			}catch(InterruptedException exe){}
			if(bkupTask.isSuccessful()){
				System.out.println("Last backup was successful.");
			}else{
				System.out.println("Last backup attempt failed.");
			}
		}
		bkTimer.cancel();		//取消调度的任务
	}
}
class AutoBkUp extends TimerTask{
	private String bkupName;
	private String fName;
	private boolean success;
	
	AutoBkUp(String fn,String bk){
		fName=fn;
		bkupName=bk;
		success=false;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		FileInputStream fin=null;
		FileOutputStream fout=null;
		int len;
		byte[] b=new byte[1024];
		try{
			fin=new FileInputStream(fName);
			fout=new FileOutputStream(bkupName);
			for(;;){
				len=fin.read(b);
				if(len==-1)break;
				else fout.write(b,0,len);
			}
			success=true;
		}catch(IOException e){
			success=false;
		}
		if(fin!=null){
			try{
				fin.close();
			}catch(IOException e){
				success=false;
			}
		}
		if(fout!=null){
			try{
				fout.close();
			}catch(IOException e){
				success=false;
			}
		}
		fin=null;
		fout=null;
	}
	boolean isSuccessful(){
		return success;
	}
}













