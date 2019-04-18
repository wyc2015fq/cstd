# IPC通信之共享内存（文件锁）Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月30日 16:19:50[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3595








                
1、操作系统的IPC（InterProcess Communication）机制，包括共享内存、信号灯操作、消息队列、信号处理等，在分布式共享对象的应用中对保证读写共享和互斥有很重要作用。其中共享内存IPC机制，具有数据共享、系统快 速查询、动态配置、减少资源耗费等优点。

   一般来说，共享内存的主要用于：独占的写操作共享的读操作，保证数据一致性；

   共享内存的使用特点：

   1）多进程可访问，这是基本要求；

   2）读写操作的进程在执行读写操作时具有排他性，其他进程不能进行写操作；

   3）多个进程可以交替对某一共享内存执行写操作；

   4）一个进程执行了内存的写操作后，不影响其他进程对该内存的访问，同时其他进程对更新后的内存具有可见性。

   5）在进程执行写操作时如果异常退出，对其他进程写操作禁止应自动解除。

   这些特点决定了共享内存在进程间可以起到共享和互斥资源的目的，对资源的读写具有加锁意义。



2、Java中共享内存的实现机制

   Windows 系统进程间的共享内存是通过内存映射文件实现，是通过把同一块内存区域映射到不同进程的地址空间中。

   Java进程间的共享内存是通过内存映射文件NIO(MappedByteBuffer)实现，不同进程的内存映射文件关联到同一个物理文件，并保持内存与文件即时的双向同步。

   简单来说，Java共享内存是让进程和物理文件形成映射，通过物理交换来实现。

   这个和操作系统让进程和内存中地址空间映射有区别，操作系统的内存共享是让进程共享一个内存区域，

   而Java NIO是让进程(都有独立的内存区域来同步物理文件)共享一份物理文件。

   共享内存的主要应用场景：

   1）永久对象配置，如启动参数需要永久保存共享，这类应用主要是读共享；

   2）对象同步互斥，多进程读写共享和独占，这类应用主要是读写独占；



3、Java NIO代码案例

   案例设计上，分别有NIOWrite写进程和NIORead读进程，共享物理文件swap.nio；

   并设置第一个字节为读写标志来实现读写独占的目的，分别是0-可读，1-正在写，2-可读。

```java
package cn.ipc;

import java.io.RandomAccessFile;  
import java.nio.MappedByteBuffer;  
import java.nio.channels.FileChannel;  
import java.nio.channels.FileChannel.MapMode; 

public class NIOWrite {
	
	private static RandomAccessFile raf;
	public static void main(String[] args) throws Exception {  
		//建立文件和内存的映射，即时双向同步
		raf = new RandomAccessFile("D:/tmp/swap.nio", "rw");  
        FileChannel fc = raf.getChannel();  
        MappedByteBuffer mbb = fc.map(MapMode.READ_WRITE, 0, 1024);  
          
        //清除文件内容 ，对 MappedByteBuffer就是对文件的操作
        for(int i=0;i<1024;i++){  
            mbb.put(i,(byte)0);  
        }  
          
        //从文件的第二个字节开始，依次写入 A-Z 字母，第一个字节指明了当前操作的位置  
        for(int i=65;i<91;i++){  
            int index = i-63;  
            int flag = mbb.get(0); //可读标置第一个字节为 0  
            if(flag != 0){ //不是可写标示 0，则重复循环，等待  
                i--;  
                continue;  
            }  
            mbb.put(0,(byte)1); //正在写数据，标志第一个字节为 1  
            mbb.put(1,(byte)(index)); //文件第二个字节说明，写数据的位置  
              
            System.out.println(System.currentTimeMillis() +  ":position:" + index +"write:" + (char)i);  
              
            mbb.put(index,(byte)i);//index 位置写入数据  
            mbb.put(0,(byte)2); //置可读数据标志第一个字节为 2  
            
            Thread.sleep(3000);  
        }  
    }  
}
```


```java
package cn.ipc;

import java.io.RandomAccessFile;  
import java.nio.MappedByteBuffer;  
import java.nio.channels.FileChannel;  
import java.nio.channels.FileChannel.MapMode; 

public class NIORead {
	private static RandomAccessFile raf;

	public static void main(String[] args) throws Exception {  
		
        raf = new RandomAccessFile("D:/tmp/swap.nio", "rw");  
        FileChannel fc = raf.getChannel();  
        MappedByteBuffer mbb = fc.map(MapMode.READ_WRITE, 0, 1024);  
        int lastIndex = 0;  
  
        for(int i=1;i<27;i++){  
            int flag = mbb.get(0); //取读写数据的标志  
            int index = mbb.get(1); //读取数据的位置,2为可读  
  
            if(flag != 2 || index == lastIndex){ //假如不可读，或未写入新数据时重复循环  
                i--;  
                continue;  
            }  
              
            lastIndex = index;  
            System.out.println( System.currentTimeMillis() +  ":position:" + index +"read:" + (char)mbb.get(index));  
              
            mbb.put(0,(byte)0); //置第一个字节为可读标志为 0  
              
            if(index == 27){ //读完数据后退出  
                break;  
            }  
        }  
    }  
}
```
![](https://img-blog.csdn.net/20160830162050374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



4、Java NIO代码案例文件锁

   在实现读写操作排他性上，可以使用Java文件锁机制，而不是通过文件第一个字节备注读写标志。

   文件锁四个方案：阻塞独占锁、阻塞共享锁、非阻塞独占锁、非阻塞共享锁；

   阻塞锁没有获取到锁就会挂起进程，非阻塞则返回NULL值。

   执行结果上，如果NIOReadLock抢到锁，可能读取到空内容。



```java
package cn.ipc;

import java.io.RandomAccessFile;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileChannel.MapMode;
import java.nio.channels.FileLock;

public class NIOWriteLock {
	private static RandomAccessFile raf;
	public static void main(String[] args) throws Exception {  
		//建立文件和内存的映射，即时双向同步
		raf = new RandomAccessFile("D:/tmp/swap.nio", "rw");  
        FileChannel fc = raf.getChannel();  
        MappedByteBuffer mbb = fc.map(MapMode.READ_WRITE, 0, 1024);
        FileLock flock=null;
        //阻塞方法一：非阻塞独占锁，当文件锁不可用时，tryLock()会得到null值
        //do {
        //	flock=fc.tryLock();
        //} while(null == flock);
        //阻塞方法二：非阻塞共享锁，当文件锁不可用时，tryLock()会得到null值
        //fc.tryLock(0L, Long.MAX_VALUE, true);
        //阻塞方法三：阻塞共享锁，有写操作会报异常
        //flock = fc.lock(0L, Long.MAX_VALUE, true);
        
        for(int i=65;i<91;i++){ 
        	//阻塞方法四：阻塞独占锁，当文件锁不可用时，当前进程会被挂起    
        	flock=fc.lock();
        	System.out.println(System.currentTimeMillis() +  ":write:" + (char)i); 
            mbb.put(i-65,(byte)i);//从文件第一个字节位置开始写入数据  
            flock.release();//释放锁
            Thread.sleep(1000);
        }
        
    }  
}
```


```java
package cn.ipc;

import java.io.RandomAccessFile;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.FileChannel.MapMode;

public class NIOReadLock {
	private static RandomAccessFile raf;

	public static void main(String[] args) throws Exception {  
		
        raf = new RandomAccessFile("D:/tmp/swap.nio", "rw");  
        FileChannel fc = raf.getChannel();  
        MappedByteBuffer mbb = fc.map(MapMode.READ_WRITE, 0, 1024);  
        FileLock flock=null;
       
        for(int i=0;i<26;i++){   
        	flock=fc.lock();//上锁
            System.out.println( System.currentTimeMillis() +  ":read:" + (char)mbb.get(i));             
            flock.release();//释放锁
            Thread.sleep(3000);            
        }  
    }  
}
```
![](https://img-blog.csdn.net/20160830162151657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





