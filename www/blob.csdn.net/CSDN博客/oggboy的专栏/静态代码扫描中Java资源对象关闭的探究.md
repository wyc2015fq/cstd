# 静态代码扫描中Java资源对象关闭的探究 - oggboy的专栏 - CSDN博客





2016年12月13日 19:18:31[oggboy](https://me.csdn.net/oggboy)阅读数：658
个人分类：[技术积累](https://blog.csdn.net/oggboy/article/category/2120537)









> 
本人在探究静态代码扫描中资源对象关闭规则遇到了一些疑惑，本文将这些疑惑记录下来并逐一找出解答。 

  包括在哪些情况下，这些资源对象需要手动关闭，怎么正确的关闭，哪些情况下不需要手动关闭，以及为什么。 

  资源对象包括很多种，本文重点关注输入输出流对象和数据库连接对象。
### 一、正确的关闭资源对象的写法
- 第一种写法：

```
public static void main(String[] args) throws Exception{
    FileOutputStream fileOutputStream = null;
    BufferedOutputStream bufferedOutputStream=null;
    DataOutputStream out=null;
    byte[] data1 = "1这个例子测试文件写".getBytes("GB2312");
    try {
        fileOutputStream = new FileOutputStream("E:\\A.txt");
        bufferedOutputStream = new BufferedOutputStream(fileOutputStream);
        out = new DataOutputStream(bufferedOutputStream);
        out.write(data1);
    } catch (Exception e) {
        // TODO: handle exception
    } finally {
        if (out!=null) {
            out.close();
        }
    }
}
```

在finally 中进行关闭可以保证在程序运行出现异常的情况下，资源对象仍然会被正常关闭。
- 第二种写法：

```
public static void main(String[] args) throws Exception{
    byte[] data1 = "这个例子测试文件写".getBytes("GB2312");
    try (
            FileOutputStream fileOutputStream = new FileOutputStream("E:\\A.txt");
            BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(fileOutputStream);
            DataOutputStream out = new DataOutputStream(bufferedOutputStream)
            )
    {       
        out.write(data1);
    } catch (Exception e) {
        // TODO: handle exception
    }
}
```

从Java1.7开始，支持了try-with-resources写法，即将资源对象声明的过程放在try()的括号里面，这样java在资源对象使用完成之后会自动关闭。详情请见[官方文档](http://docs.oracle.com/javase/tutorial/essential/exceptions/tryResourceClose.html)。
- 第三种写法：

```
import org.apache.commons.io.IOUtils;
public static void main(String[] args) throws Exception{
    FileOutputStream fileOutputStream = null;
    BufferedOutputStream bufferedOutputStream=null;
    DataOutputStream out=null;
    byte[] data1 = "这个例子测试文件写".getBytes("GB2312");
    try {       
        fileOutputStream = new FileOutputStream("E:\\A.txt");
        bufferedOutputStream = new BufferedOutputStream(fileOutputStream);
        out = new DataOutputStream(bufferedOutputStream);
        out.write(data1);
    } catch (Exception e) {
        // TODO: handle exception
    } finally {
        IOUtils.closeQuietly(out);
    }
}
```

在关闭资源对象时调用了第三方的类进行关闭操作。[apache官方文档](http://commons.apache.org/proper/commons-io/javadocs/api-release/org/apache/commons/io/IOUtils.html)。

```java
public static void closeQuietly(final Closeable closeable) {
337       try {
338            if (closeable != null) {
339                closeable.close();
340            }
341        } catch (final IOException ioe) {
342            // ignore
343        }
344    }
```

实际就是加了个非空判断后再调用java的关闭方法。[源码实现文档](http://commons.apache.org/proper/commons-io/javadocs/api-release/src-html/org/apache/commons/io/IOUtils.html#line.336)。 

同理我们自己也可以自定义类对资源关闭方法进行封装，不再赘述。

### 二、资源对象在套接使用时，只需要手动关闭最后套接的对象。

先来看一段代码来理解资源对象的套接是什么：

```
FileOutputStream fileOutputStream = new FileOutputStream("A.txt");
BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(fileOutputStream);
DataOutputStream out = new DataOutputStream(bufferedOutputStream);
```

以上代码中，首先定义了fileOutputStream，选择A.txt作为输出文件。 

接着第二行bufferedOutputStream套接了fileOutputStream，为输出提供缓冲功能。 

第三行out套接了bufferedOutputStream，提供基本数据类型的写入功能。 

在两次套接后，我们最后只需要关闭最后套接的out对象，即调用`out.close()`，之前被套接的两个对象也会自动被关闭。
为什么被套接的两个对象也会自动被关闭？ 

重点在于close()方法的实现，out在关闭时实际是先调用了java.io.FilterOutputStream.close()方法，该方法的具体实现如下：

```java
/** * Closes this output stream and releases any system resources * associated with the stream. * <p> * The <code>close</code> method of <code>FilterOutputStream</code> * calls its <code>flush</code> method, and then calls the * <code>close</code> method of its underlying output stream. * * @exception  IOException  if an I/O error occurs. * @see        java.io.FilterOutputStream#flush() * @see        java.io.FilterOutputStream#out */
 public void close() throws IOException {
     try {
       flush();
     } catch (IOException ignored) {
     }
     out.close();
 }
```

这段代码意思是先调用了flush()方法，保证之前写入到内存的数据刷到硬盘。接着调用java.io.OutputStream.close()方法，继续看java.io.OutputStream.close()的实现：

```java
/**  * Closes this output stream and releases any system resources  * associated with this stream. The general contract of <code>close</code>  * is that it closes the output stream. A closed stream cannot perform  * output operations and cannot be reopened.  * <p>  * The <code>close</code> method of <code>OutputStream</code> does nothing.  *  * @exception  IOException  if an I/O error occurs.  */
 public void close() throws IOException {
 }
```

注释中`Closes this output stream and releases any system resources associated with this stream`这句非常重要，意思是关闭这个输出流并释放任何与之相关的系统资源。 

大家还可以看到方法里面什么都没有做，但是java.io.OutputStream 实现了 Closeable接口，接着Closeable接口集成了AutoCloseable接口，最后定位到AutoCloseable接口中，注释里有这样一句`Closes this resource, relinquishing any underlying resources.`，大意为关闭这个资源，放弃任何底层的资源。有兴趣深入研究的同学，另附[官方文档](http://docs.oracle.com/javase/8/docs/api/java/lang/AutoCloseable.html#close--)

依据已经找到了，我们需要验证一下是否真的会关闭：

```
public static void main(String[] args) throws Exception{    
    FileOutputStream fileOutputStream = null;
    BufferedOutputStream bufferedOutputStream=null;
    DataOutputStream out=null;
    byte[] data1 = "测试".getBytes("GB2312");
    byte[] data2 = "这个".getBytes("GB2312");
    byte[] data3 = "案例".getBytes("GB2312");
    try {
        fileOutputStream = new FileOutputStream("E:\\A.txt");
        bufferedOutputStream = new BufferedOutputStream(fileOutputStream);
        out = new DataOutputStream(bufferedOutputStream);
        //out.close();//在该位置关闭时，A.txt没有内容
        fileOutputStream.write(data1);
        bufferedOutputStream.write(data2);
        out.write(data3);
    } catch (Exception e) {
        // TODO: handle exception
    } finally {
        if (out!=null) {
            out.close();
        }
    }
}
```

正常的情况下，即out.close();方法最后在finally中执行时，A.txt的内容为“测试这个案例”。当out对象提前关闭，即文中中文注释的位置，A.txt的内容为空。说明out对象关闭后，其相关的fileOutputStream和bufferedOutputStream都被关闭了，无法再向A.txt中写入内容。大家可以亲自动手实验一下，完全可以验证之前的逻辑。

### 三、资源对象在方法中被return的情况下，该方法中不需要关闭。

存在一种这样的写法，把针对资源对象做的操作封装起来，方法最终返回的是操作完成后的资源对象。这种情况下，是不需要关闭该资源对象的。例如：

```
private static InputStream getNewInputStream() throws IOException {
    InputStream in = null;
    try {
        in = new URL("http://www.so.com").openStream();
    } catch (MalformedURLException e) {
        e.printStackTrace();
    } catch (IOException e) {
        e.printStackTrace();
    }
    return in;
}
```

这是在静态代码扫描中比较容易遗漏的情况。最终该资源对象在调用该方法的方法中使用完成后，仍然是需要关闭的。

### 四、ByteArrayInputStream等不需要检查关闭的资源对象。

有一些资源对象时不用关闭的。这些对象包括：ByteArrayInputStream、ByteArrayOutputStream、StringBufferInputStream、CharArrayWriter、和StringWriter。 

看官方文档里面对象类对应的close()方法的解释： 
`Closing a ByteArrayInputStream has no effect. The methods in this class can be called after the stream has been closed without generating an IOException.`

关闭ByteArrayInputStream对象没有效果。即使资源对象已经关闭了，再调用这个关闭方法也不会生成IOException。 

既然关闭方法没有关闭的效果，那么在静态代码检测时就没有必要检查该资源对象是否调用了close()方法。 

更多详情请见[官方文档](https://docs.oracle.com/javase/8/docs/api/java/io/ByteArrayInputStream.html)。
### 五、数据库连接对象中当Statement被关闭后，由该Statement初始化的ResultSet对象也会自动关闭。

这个逻辑跟第二点套接流对象关闭的逻辑正好相反，我们结合代码来看一下。

```
Statement stmt = con.createStatement( ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);
ResultSet rs = stmt.executeQuery("SELECT a, b FROM TABLE2");
```

从代码可以看出Statement和ResultSet的关系，其中当Statement对象关闭之后，由Statement对象初始化的ResultSet对象rs也会被自动关闭。 
`When a Statement object is closed, its current ResultSet object, if one exists, is also closed.`

详情可查看[官方文档](https://docs.oracle.com/javase/8/docs/api/java/sql/Statement.html#close--)
### 六、使用socket获取的InputStream和OutputStream对象。

使用socket创建出的InputStream和OutputStream，当socket关闭时，这两个流也会自动关闭。 

同时，如果关闭InputStream，将会同时关闭与之相关的Socket。

```
Socket socket = new Socket("127.0.0.1", 8001);
InputStream input = socket.getInputStream();
OutputStream output = socket.getOutputStream();
```


```
Closing this socket will also close the socket's InputStream and OutputStream. 

If this socket has an associated channel then the channel is closed as well.
```

`Closing the returned InputStream will close the associated socket.`

详情可查看[官方文档](https://docs.oracle.com/javase/8/docs/api/java/net/Socket.html#close--)
### 七、使用后需要关闭的资源对象列表。
- 
输入输出流对象： 

BufferedInputStream, 

BufferedOutputStream,  

BufferedReader, 

BufferedWriter,  

CharArrayReader,  

CharArrayWriter,  

CheckedInputStream, 

CheckedOutputStream, 

CipherOutputStream,  

DataInputStream,  

DataOutputStream,  

DeflaterInputStream,  

DeflaterOutputStream,  

DigestInputStream,  

DigestOutputStream,  

FileCacheImageInputStream, 

FileCacheImageOutputStream,  

FileImageInputStream,  

FileImageOutputStream,  

FileInputStream,  

FileOutputStream,  

FileReader,  

FileWriter,  

FilterInputStream,  

FilterOutputStream,  

FilterReader,  

FilterWriter,  

GZIPInputStream,  

GZIPOutputStream,  

ImageInputStreamImpl,  

ImageOutputStreamImpl,  

InflaterInputStream,  

InflaterOutputStream,  

InputStream,  

InputStream,  

InputStream,  

InputStreamReader,  

JarInputStream,  

JarOutputStream,  

LineNumberInputStream,  

LineNumberReader,  

LogStream,  

MemoryCacheImageInputStream,  

MemoryCacheImageOutputStream,  

ObjectInputStream,  

ObjectOutputStream,  

OutputStream,  

OutputStream,  

OutputStream,  

OutputStreamWriter,  

PipedInputStream,  

PipedOutputStream,  

PipedReader,  

PipedWriter,  

PrintStream,  

PrintWriter,  

ProgressMonitorInputStream,  

PushbackInputStream,  

PushbackReader,  

Reader,  

SequenceInputStream,  

StringBufferInputStream,  

StringReader,  

URLClassLoader, 

Writer,  

ZipFile,  

ZipInputStream,  

ZipOutputStream- 
数据连接对象： 

Connection， 

Statement， 

CallableStatement， 

PreparedStatement， 

ResultSet， 

CachedRowSet， 

FilteredRowSet， 

JdbcRowSet,  

JoinRowSet,  

RowSet,  

SyncResolver,  

WebRowSet 

更多内容请见[官方文档](https://docs.oracle.com/javase/8/docs/api/java/lang/AutoCloseable.html)：












