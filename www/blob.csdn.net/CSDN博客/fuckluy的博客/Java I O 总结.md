# Java I/O 总结 - fuckluy的博客 - CSDN博客












2017年01月15日 16:46:41[Zcc?](https://me.csdn.net/fuckluy)阅读数：849








本篇文章需要一定的I/O操作基础

## File类

与I/O紧密相关的类，File既可以代表一个目录又可以代表一个特定文件的名称，下面的代码示例 就是在G盘中先创建了目录directory接着在该目录下创建了指定的文件a.txt



```java
public static void main(String[] args) {
		String path="G:\\";
		String fileName="directory";
		//创建目录 G:\directory
		File directory=new File(path+fileName);
		if (!directory.exists()) {
			directory.mkdirs();
		}
		
		//在directory目录下创建a.txt
		File file=new File(directory.getAbsolutePath(),"a.txt");
		if(file.exists()){
			file.delete();
		}
		try {
			file.createNewFile();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
```


File类的方法请自行查看API









## 输入和输出：

按Java的I/O类库来区分，可分为传统的I/O（java.io.*），新I/O （java.nio.*   JDK1.4中添加）、标准的I/O（System.in、System.out、System.err）



### **传统I/O：**

以下内容都是Java的传统 I/O ，即 java.io.*中的类。

一般的我将I/O分为输入和输出两部分，而输入流，输出流又可以分为面向字节的和面向字符的



**面向字节的：**

**输入流 InputStream**，它的作用是用来表示那些从不同数据源产生输入的类，**类库的设计者限定了与输入有关的所有类都应该从InputStream继承。**

这些数据源包括：

1、字节数组（ByteArrayInputStream）

2、String对象（StringBufferInputStream）（已弃用）

3、文件（FileInputStream）

4、管道（PinpedInputSteam）（在多线程中用于任务之间的通信）

5、一个由其他种类的流组成的序列，便于我们将他们整合到一个流内（SequenceInputStream）

6、其他数据源，比如 Interent连接等



除了上面的类以外还有一个类FilterInputStream，它是一个装饰器的接口，主要是为了继承而存在的类，其构造器的修饰是protected，无法直接创建其实例，常用的子类有**BufferedInputStream**，内部带有缓冲区，可指定大小，缓冲区默认大小为8192字节，在缓冲区支持一些操作，具体请看API







**输出流OutputStream**，该类别的类决定了输出所要去往的目标，类库的设计者限定了与输出有关的所有类都应该从OutputStream继承。

这些目标包括：

1、字符数组（ByteArrayOutputStream）

2、文件（FileOutputStream）

3、管道（PipedOutputStream）（在多线程中用于任务之间的通信）



除了上面的类以外还有一个类FilterOutputStream，它是一个装饰器的接口，常用的子类有**BufferedOutputStream，PrintStream**，PrintStream用于格式化输出







**面向字符的（JDK 1.1中引入）：**

由于面向字节的流只能处理8位的字节，不能很好的处理16位的Unicode字符，因此添加了面向字符的输入流Reader和输出流Writer

，还有两个适配器类可以将字节流转换为字符流 ，分别为InputStreamReader将InputStream转换为Reader和OutputStreamWriter将OutputStream转换为Writer

下面是字节流和字符流的对应关系：



   字节流 ====================>>字符流

InputStream =======================>>  Reader（使用InputStreamReader）

OutputStream =======================>>   Write （OutputStreamWrite）



StringBufferInputStream(已弃用)  ==================>> StringReader

无对应的类==========================>>StringWriter




ByteArrayInputStream    =========================>> CharArrayReader

ByteArrayOutputStream ==========================>> CharArrayWriter



PipedInputStream  ==========================>>PipedReader（在多线程中用于任务之间的通信）

PipedOutputStream ==========================>>PipedWriter（在多线程中用于任务之间的通信）



BuffterInputStream ===========================>>BufferedReader

BufferOutputStream ===========================>>BufferedWriter



PrintStream  ==========================>>PrintWriter



下面是一个文件输入和输出的例子



```java
package Num_6;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class T2 {
	public static void main(String[] args) {

		// 向文件中写数据
		File file = new File("G:\\directory\\a.txt");
		if (file.exists()) {
			file.delete();
		}

		BufferedWriter writer = null;
		BufferedReader reader = null;

		try {
			file.createNewFile();
			writer = new BufferedWriter(new FileWriter(file));
			writer.write("A");
			writer.write("B");
			writer.write("C");
			writer.flush();// 刷新缓冲区,如果不刷新数据不会写入到文件中
			
			//从文件中读出数据
			reader = new BufferedReader(new FileReader(file));
			String string=String.valueOf((char)reader.read());
			reader.mark(1);//在下一个字符前面设置标记位
			string+=String.valueOf((char)reader.read());
			reader.reset();//将流重置到最小的标记位置
			string+=String.valueOf((char)reader.read());
			System.out.println(string); //ABB
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally {
			try {
				if (writer != null)
					writer.close();// close方法内部会调用刷新缓冲区的方法
				if (reader != null)
					reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}
}
```
**注意**上面的代码对异常的使用是不规范的，不要使用基类异常覆盖一个具体类型的异常，我这里为了方便所有使用了IOException


PrintWriter，在需要将数据写入文件时优先考虑这个类，在JDK 1.5时进行了改造，简化了将输出写入时的文件创建过程

下面的代码是使用PrintWriter向文件中写数据



```java
public static void main(String[] args) {
		File file = new File("G:\\directory\\b.txt");
		if (file.exists()) {
			file.delete();
		}
		try {
			file.createNewFile();
			PrintWriter writer=new PrintWriter(file);
			writer.write("A");
			writer.write("A");
			writer.write("A");
			writer.flush();// 刷新缓冲区,如果不刷新数据不会写入到文件中
			writer.close();//close方法内部会调用刷新缓冲区的方法
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
```





通过上面的介绍不管是面向字节的还是面向字符的对**文件操作**只有一种选择，要么读要么写，一个类不能既能读又能写，这样很不利于文件的操作，其实Java给我们提供了另一个**独立的类**对**文件**进行操作------->**RandomAccessFile**

**RandomAccessFile（面向字节的）**是一个完全独立的类，不使用InputStream和OutputStream类中已有的任何功能，RandomAccessFile支持对随机访问文件的读取和写入且提供了比别的I/O类型更强大的功能，比如我们可以在一个文件内向前和向后移动通过seek（）方法在文件内移至新的位置，length方法用于判断一个文件的最大尺寸

**举个简单的例子**，我们经常从网上下载各钟东西，下载到一半的时候突然有别的操作需要网速，这时候我们都会暂停当前的下载操作，等忙完以后我们都会点击继续按钮，继续下载，这时候已经下载过的部分文件怎么办？不可能丢弃掉重新下载，肯定是需要接着上次下载的地方继续加载，那对于已经下载过的部分文件就需要跳过，这时候RandomAccessFile就是一个很好的选择，下面就是一个断点续传Demo的部分代码

```java
//返回的不是完整的，而是已经跳过下载过的字节，即服务端返回的就是未下载的字节
		input = response.body().byteStream();//ByteArrayInputStream
                savedFile = new RandomAccessFile(file, "rw");
                savedFile.seek(downloadLength);//跳过已下载的字节
                byte[] b = new byte[1024];
               
                int len;
                while ((len = input.read(b)) != -1) {       //等价于 input.read(b,0,b.length)
                   
                    
                   
                    savedFile.write(b, 0, len);
         

                }
```






**对于字节流和字符流我们应该优先使用字符流，只有字符流搞不定的时候再使用字节流**

与I/O相关的类都是先了Closeable接口，因此可以单独写一个方法用于流的关闭



```java
public static void close(Closeable closeable) {
        try {
            if (closeable != null) {
                closeable.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
```







### 标准I/O：

标准I/O这个术语参考的是Unix中”程序所使用的单一信息流“这个概念。程序的所有输入都可以来自标准输入，它的所有输出也都可以发送到标准输出，以及所有的错误信息都可以发送到标准错误。



标准I/O的意义在于：我们可以很容易地把程序串联起来，一个程序的标准输出可以是另一个程序的标准输入。

按照标准I/O模型，Java提供了System.in、System.out、System.err







### **新I/O：**

JDK 1.4 的java.nio.* 包中引入了新的Java类库，其目的在于提高速度，目标是快速移动**大量数据**，实际上旧的I/O包已经使用nio重新实现过，速度的提高在文件I/O和网络I/O都可能发生

新I/O使用了通道和缓冲器两种结构，这两种结构更接近于操作系统执行I/O的方式，注意我们无法直接和通道交互，我们只能和缓冲器交互，通过缓冲器间接和通道交互，通道要么从缓冲器中获得数据，要么向缓冲器中发送数据。**唯一直接与通道交互的缓冲器**是**ByteBuffer**（用原始的字节形式或基本数据类型输输出和读取数据），ByteBuffer将数据移进移出通道的唯一方式，传统的I/O类库(java.io.*)有三个类被修改了用以产生**FileChannel**，这三个类分别是**FileInputStream**、**FileOutputStream**、**RandomAccessFile**，**这三个类都是面向字节的**，与底层的nio性质一致，这三个类可以产生通道，对于以Writer和Reader为代表的面向字符的流nio提供了Channels类可以在通道中产生Writer和Reader，具体请看API

下面的Demo 使用了上面三个被修改过的类产生可写的、即可写右可读的、可读的通道



```java
package Num_6;

import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class T4 {
	public static void main(String[] args) throws Exception {
		File file = new File("G:\\directory\\c.txt");
		FileChannel channel;
		if (!file.exists()) {
			file.createNewFile();
		}
		FileOutputStream out = new FileOutputStream(file);
		channel = out.getChannel();
		// 将字符串转换的byte数组包装到缓冲器，然后从缓冲器中获取数据写入此通道
		channel.write(ByteBuffer.wrap("hello".getBytes()));
		close(channel);
		close(out);
		
		
		RandomAccessFile rFile=new RandomAccessFile(file, "rw");
		channel=rFile.getChannel();
		channel.position(channel.size());//Move to end
		channel.write(ByteBuffer.wrap(" world".getBytes()));
		close(channel);
		close(rFile);
		
		
		FileInputStream in=new FileInputStream(file);
		channel=in.getChannel();
		ByteBuffer buffer=ByteBuffer.allocate(1024);
		//将通道中的数据发送给缓冲器
		channel.read(buffer);
		buffer.flip();//必须调用该方法，调用此方法为一系列通道写入或相对获取 操作做好准备
		while(buffer.hasRemaining()){
			System.out.print((char)buffer.get());
		}
		close(channel);		close(in);	
	}

	public static void close(Closeable closeable) {

		try {
			if (closeable != null)
				closeable.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
```


ByteBuffer中有一些asXXX方法，会创建字节缓冲器的视图返回的是其它基本数据类型的缓冲器，对视图的任何修改都会映射成为对ByteBuffer中数据的修改，如下面的Demo





```java
ByteBuffer bb=ByteBuffer.allocate(1024);
		IntBuffer ib=bb.asIntBuffer();
		ib.put(new int[]{1,2,3,1111,2222});//向缓冲区中添加数据
		ib.flip();
		while(ib.hasRemaining()){
			System.out.println(ib.get());
		}
```




下面介绍一个类MappedByteBuffer，它是ByteBuffer的子类，用于内存映射文件，内存映射文件允许我们创建和修改那些因为太大而不能放入内存的文件（有点操作系统中虚拟内存的意思），因为只有一部分文件放入了内存，文件的其他部分被交换了出去，通过内存映射的方式可以修改，创建很大的文件。下面的Demo会利用MappedByteBuffer创建一个500M的txt文件，注意一点，**映射文件中的所有输出必须使用RandomAccessFile**



```java
static int length = 1024 * 1024 * 500;//500M

	public static void main(String[] args) throws Exception {
		File file = new File("G:\\directory\\d.txt");
		if (!file.exists()) {
			file.createNewFile();
		}
		MappedByteBuffer out = new RandomAccessFile(file, "rw").getChannel().map(FileChannel.MapMode.READ_WRITE, 0,
				length);
		for(int i=0;i<length;i++)
			out.put((byte)'x');
		
	}
```























