# Java深入 - Java 流 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年09月09日 13:58:32[initphp](https://me.csdn.net/initphp)阅读数：1498
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









### 数据流的基本概念

数据流是一串连续不断的数据的集合。就好比是水管中的水源源不断的从一端流向另外一端。

Java类库中，IO部分类库是非常庞大的，包括输入输出流，文件流，网络上的数据流，ZIP流等等。Java中将输入输出都抽象称为流。


![](https://img-blog.csdn.net/20140909134701318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

输入（图）

![](https://img-blog.csdn.net/20140909134715623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

输出（图）




### Java IO 的层次体系

Java IO包中最重要的5个类和接口：File、OutputStream、InputStream、Writer、Reader、接口Serializable。

我们将Java的IO部分分成三个层次：

**1. 流式部分。**IO的主体部分，包括OutputStream、InputStream、Writer、Reader及衍生的子类。

**2. 非流式部分。**主要用于辅助流式部分，最常用的就是我们简单的File文件操作类。

**3. 其它类。**文件读取部分的与安全相关的类，如：SerializablePermission类，以及与本地操作系统相关的文件系统的类，如：FileSystem类和Win32FileSystem类和WinNTFileSystem类。




我们又将流式部分分为两个部分：字节流和字符流。

**1. 字节流：**主要是OutputStream、InputStream （抽象类，字节流的操作主要是其子类完成）


**2. 字符流：**主要是Writer、Reader（抽象类）

下面一张图，展示了字节流和字符流各自实现的子类：

![](https://img-blog.csdn.net/20140909135604718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 按IO类型来分类

1. 内存操作。例如：CharArrayReader、 CharArrayWriter、ByteArrayInputStream、ByteArrayOutputStream

2. Pipe管道。PipedReader、PipedWriter、PipedInputStream、PipedOutputStream

3. File文件流。文件的读写操作。FileReader、FileWriter、FileInputStream、FileOutputStream

4. 对象输入输出。ObjectInputStream、ObjectOutputStream。

5. 打印。PrintWriter、PrintStream

6. Buffering缓冲。在读入或写出时，对数据进行缓存，以减少I/O的次数：BufferedReader、BufferedWriter、BufferedInputStream、BufferedOutputStream

7. Filtering 过滤流。在数据进行读或写时进行过滤：FilterReader、FilterWriter、FilterInputStream、FilterOutputStream过

8. Counting计数，在读入数据时对行记数 ：LineNumberReader、LineNumberInputStream

等。




### 字节输入流InputStream

InputStream是字节输入流。InputStream本身是一个抽象类，继承该抽象类的子类，必须实现InputStream定义的一个read方法。

我们可以看下InputStream中定义的抽象方法read：



```java
/**
     * Reads the next byte of data from the input stream. The value byte is
     * returned as an <code>int</code> in the range <code>0</code> to
     * <code>255</code>. If no byte is available because the end of the stream
     * has been reached, the value <code>-1</code> is returned. This method
     * blocks until input data is available, the end of the stream is detected,
     * or an exception is thrown.
     *
     * <p> A subclass must provide an implementation of this method.
     *
     * @return     the next byte of data, or <code>-1</code> if the end of the
     *             stream is reached.
     * @exception  IOException  if an I/O error occurs.
     */
    public abstract int read() throws IOException;
```


InputStream类中还定义了几个比较重要的方法：



```java
1. public int read(byte b[]) throws IOException  直接读取byte的数据

2. public int read(byte b[], int off, int len) 从输入流中最多读取len个字节的数据，存放到偏移量为off的b数组中。

3. public void close() throws IOException {}  用于关闭读取的流。
```


常用的InputStream流子类：

1.  FileInputStream把一个文件作为InputStream，实现对文件的读取操作     

2. ByteArrayInputStream：把内存中的一个缓冲区作为InputStream使用 

3. StringBufferInputStream：把一个String对象作为InputStream 

4.  PipedInputStream：实现了pipe的概念，主要在线程中使用 

5. SequenceInputStream：把多个InputStream合并为一个InputStream 




流结束标志：**当read()方法返回-1的时候标志着流读取完毕**




一个FileInputStream的例子：



```java
public static void main(String[] args) {
        System.out.println("IO流 测试");
        try {
            //打开一个文件流
            FileInputStream in = new FileInputStream("D:/test.txt");
            //文件按行读取，并且打印读取到的文件内容
            int n = 512;
            byte[] buf = new byte[n];
            while (in.read(buf, 0, n) != -1) {
                System.out.println(new String(buf));
            }
            //最后关闭这个流
            in.close();
        } catch (Exception e) {
            System.out.println("Error" + e.getMessage());
        }
    }
```






### 字节输出流OutputStream



OutputStream字节输入流。OutputStream本身是一个抽象类，继承该抽象类的子类，必须实现OutputStream定义的一个write方法。

字节输出流的实现必须先实现OutputStream中的write抽象方法：



```java
/**
     * Writes the specified byte to this output stream. The general 
     * contract for <code>write</code> is that one byte is written 
     * to the output stream. The byte to be written is the eight 
     * low-order bits of the argument <code>b</code>. The 24 
     * high-order bits of <code>b</code> are ignored.
     * <p>
     * Subclasses of <code>OutputStream</code> must provide an 
     * implementation for this method. 
     *
     * @param      b   the <code>byte</code>.
     * @exception  IOException  if an I/O error occurs. In particular, 
     *             an <code>IOException</code> may be thrown if the 
     *             output stream has been closed.
     */
    public abstract void write(int b) throws IOException;
```


OutputStream中定义的比较重要的方法：





```java
1. public void write(byte b[]) throws IOException //直接写入byte数据
2. public void write(byte b[], int off, int len)
3. public void close() //关闭流
4. public void flush() throws IOException //将数据缓冲区中数据全部输出，并清空缓冲区。
```




重要的子类：

1.  ByteArrayOutputStream：把信息存入内存中的一个缓冲区中 

2.  FileOutputStream：把信息存入文件中 

3.  PipedOutputStream：实现了pipe的概念，主要在线程中使用 

4.  SequenceOutputStream：把多个OutStream合并为一个OutStream 





一个FileOutputStream的例子：



```java
public static void main(String[] args) {
        System.out.println("IO流 测试");
        try {
            //打开一个文件流
            FileOutputStream on = new FileOutputStream("D:/test.txt");
            //将字符串转成byte 并且写入文件
            String string = "Hello Wolrd\r\nTest";
            byte[] by = string.getBytes();
            on.write(by);
            //最后关闭这个流
            on.close();
            System.out.println("写入成功");
        } catch (Exception e) {
            System.out.println("Error" + e.getMessage());
        }
    }
```



### 字符输入和输出流Reader和Writer

Reader是字符输入抽象类。Reader和InputStream有些类似，但是Reader操作的字符串流，而InputStream操作的是二进制流。

常用Reader子类：

1. FileReader :与FileInputStream对应  

2. CharArrayReader：与ByteArrayInputStream对应

3. StringReader : 与StringBufferInputStream对应 

4. InputStreamReader  从输入流读取字节

5. FilterReader: 允许过滤字符流 

6. BufferReader :接受Reader对象作为参数，并对其添加字符缓冲器，使用readline()方法可以读取一行


Reader的子类需要实现两个抽象方法：



```java
/**
     * Reads characters into a portion of an array.  This method will block
     * until some input is available, an I/O error occurs, or the end of the
     * stream is reached.
     *
     * @param      cbuf  Destination buffer
     * @param      off   Offset at which to start storing characters
     * @param      len   Maximum number of characters to read
     *
     * @return     The number of characters read, or -1 if the end of the
     *             stream has been reached
     *
     * @exception  IOException  If an I/O error occurs
     */
    abstract public int read(char cbuf[], int off, int len) throws IOException;

    /**
     * Closes the stream and releases any system resources associated with
     * it.  Once the stream has been closed, further read(), ready(),
     * mark(), reset(), or skip() invocations will throw an IOException.
     * Closing a previously closed stream has no effect.
     *
     * @exception  IOException  If an I/O error occurs
     */
     abstract public void close() throws IOException;
```


Writer是字符输出抽象类。Writer就和OutputStream有些类似了。



常用Writer子类：

1. FileWrite: 与FileOutputStream对应  

2. chararrayWrite:与ByteArrayOutputStream对应 ,将字符缓冲器用作输出。 

3. PrintWrite:生成格式化输出 

4. filterWriter:用于写入过滤字符流 

5. PipedWriter：与PipedOutputStream对应   

6. StringWriter：无与之对应的以字节为导向的stream  



Writer的子类需要实现三个抽象方法：



```java
/**
     * Writes a portion of an array of characters.
     *
     * @param  cbuf
     *         Array of characters
     *
     * @param  off
     *         Offset from which to start writing characters
     *
     * @param  len
     *         Number of characters to write
     *
     * @throws  IOException
     *          If an I/O error occurs
     */
    abstract public void write(char cbuf[], int off, int len) throws IOException;

    /**
     * Flushes the stream.  If the stream has saved any characters from the
     * various write() methods in a buffer, write them immediately to their
     * intended destination.  Then, if that destination is another character or
     * byte stream, flush it.  Thus one flush() invocation will flush all the
     * buffers in a chain of Writers and OutputStreams.
     * 
     * <p> If the intended destination of this stream is an abstraction provided
     * by the underlying operating system, for example a file, then flushing the
     * stream guarantees only that bytes previously written to the stream are
     * passed to the operating system for writing; it does not guarantee that
     * they are actually written to a physical device such as a disk drive.
     *
     * @throws  IOException
     *          If an I/O error occurs
     */
    abstract public void flush() throws IOException;

    /**
     * Closes the stream, flushing it first. Once the stream has been closed,
     * further write() or flush() invocations will cause an IOException to be
     * thrown. Closing a previously closed stream has no effect.
     *
     * @throws  IOException
     *          If an I/O error occurs
     */
    abstract public void close() throws IOException;
```


一个BufferedReader和BufferedWriter的例子：





```java
public static void main(String[] args) {
        System.out.println("IO流 测试");
        try {
            //使用BufferedReader读取文件内容
            BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream("D:/test.txt"), "GBK"));
            String buf = null;
            //BufferedWriter 用于输出到文件上
            BufferedWriter out = new BufferedWriter(new FileWriter("D:/test2.txt"));
            while ((buf = in.readLine()) != null) {
                System.out.println(buf);
                out.write(buf + "\r\n"); //写入文件
            }
            in.close();
            out.close();
        } catch (IOException e) {
        }
    }
```




### IO异常子类

1.public class  EOFException ：

   非正常到达文件尾或输入流尾时，抛出这种类型的异常。



2.public class FileNotFoundException：

   当文件找不到时，抛出的异常。



3.public class InterruptedIOException：

   当I/O操作被中断时，抛出这种类型的异常。




### File类

File类提供了描述文件和目录的操作与管理方法。但File类不是InputStream、OutputStream或Reader、Writer的子类，因为它不负责数据的输入输出，而专门用来管理磁盘文件与目录。

常用方法：

创建方法：

1.boolean createNewFile() 不存在返回true 存在返回false

2.boolean mkdir() 创建目录

3.boolean mkdirs() 创建多级目录

删除方法：

1.boolean delete()

2.boolean deleteOnExit() 文件使用完成后删除

判断方法：

1.boolean canExecute()判断文件是否可执行

2.boolean canRead()判断文件是否可读

3.boolean canWrite() 判断文件是否可写

4.boolean exists() 判断文件是否存在

5.boolean isDirectory() 

6.boolean isFile()

7.boolean isHidden()

8.boolean isAbsolute()判断是否是绝对路径 文件不存在也能判断

获取方法：

1.String getName()

2.String getPath()

3.String getAbsolutePath()

4.String getParent()//如果没有父目录返回null

5.long lastModified()//获取最后一次修改的时间

6.long length()

7.boolean renameTo(File f)

8.File[] liseRoots()//获取机器盘符

9.String[] list() 

10.String[] list(FilenameFilter filter)



一个例子:



```java
public static void main(String[] args) {
        System.out.println("IO流 测试");
        try {
            //判断文件是否存在
            File file = new File("D:/test.txt");
            if (file.exists()) {
                System.out.println("这个文件存在");
            }
            file.delete(); //删除文件
        } catch (Exception e) {

        }

    }
```










