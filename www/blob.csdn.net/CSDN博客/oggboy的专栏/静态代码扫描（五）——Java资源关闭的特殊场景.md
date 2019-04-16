# 静态代码扫描（五）——Java资源关闭的特殊场景 - oggboy的专栏 - CSDN博客





2017年05月12日 18:32:45[oggboy](https://me.csdn.net/oggboy)阅读数：667








> 
在上一篇文章中，我主要介绍了为什么要手动关闭Java资源对象和怎样正确的手动关闭Java资源对象。这篇文章将继续分享在判断Java资源关闭时，有哪些特殊的场景。


#### 1. 当开发使用自定义类进行资源对象关闭时，需要跨类追踪资源对象是否关闭。

在使用火线扫描本公司的项目代码时，发现几乎所有的项目都会使用自定义的类来统一管理资源关闭。例如：

```java
/** * 资源回收工具类 */
public final class QHRecyleUtils {
    /**     * 回收InputStream类型     * @param inputStream 要回收的资源     */
    public static void close(InputStream inputStream) {
        if (inputStream != null) {
            try {
                inputStream.close();
            } catch (IOException e) {
            }
        }
    }
     /**     * 回收OutputStream类型     * @param outputStream 要回收的资源     */
    public static void close(OutputStream outputStream) {
        if (outputStream != null) {
            try {
                outputStream.close();
            } catch (IOException e) {
            }
        }
    }
    //还有Reader、Writer等等，这里就不一一列举了。
}
```

有个这个统一的工具类，开发在关闭资源对象时，只需要调用 `QHRecyleUtils.close(e)`即可。例如：

```
public void test_01(){  
        FileOutputStream fos;
        OutputStreamWriter osw=null;
        try {
            fos = new FileOutputStream("e:/a.txt");
            osw = new OutputStreamWriter(fos,"UTF-8");
            osw.append("55555");
        } catch (Exception e) {
            // TODO: handle exception
        }finally{
            QHRecyleUtils.close(osw);//这里调用了工具类进行关闭。
        }
    }
```

当静态代码分析遇到这种场景时，就需要追踪进入QHRecyleUtils类中的close()方法，确认该资源对象是否真的执行了关闭操作。

#### 2. 资源对象在方法中被return的情况下，该方法中不需要关闭。

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

#### 3. ByteArrayInputStream等不需要检查关闭的资源对象。

有一些资源对象时不用关闭的。这些对象包括：ByteArrayInputStream、ByteArrayOutputStream、StringBufferInputStream、CharArrayWriter、和StringWriter。 

看官方文档里面对象类对应的close()方法的解释： 
`Closing a ByteArrayInputStream has no effect. The methods in this class can be called after the stream has been closed without generating an IOException.`

关闭ByteArrayInputStream对象没有效果。即使资源对象已经关闭了，再调用这个关闭方法也不会生成IOException。 

既然关闭方法没有关闭的效果，那么在静态代码检测时就没有必要检查该资源对象是否调用了close()方法。 

更多详情请见[官方文档](https://docs.oracle.com/javase/8/docs/api/java/io/ByteArrayInputStream.html)。
#### 4. 资源对象在套接使用时，只需要手动关闭最后套接的对象。

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

#### 5. 数据库连接对象中当Statement被关闭后，由该Statement初始化的ResultSet对象也会自动关闭。

这个逻辑跟第二点套接流对象关闭的逻辑正好相反，我们结合代码来看一下。

```
Statement stmt = con.createStatement( ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);
ResultSet rs = stmt.executeQuery("SELECT a, b FROM TABLE2");
```

从代码可以看出Statement和ResultSet的关系，其中当Statement对象关闭之后，由Statement对象初始化的ResultSet对象rs也会被自动关闭。 
`When a Statement object is closed, its current ResultSet object, if one exists, is also closed.`

详情可查看[官方文档](https://docs.oracle.com/javase/8/docs/api/java/sql/Statement.html#close--)
#### 6. 使用socket获取的InputStream和OutputStream对象不需要关闭。

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
从以上6种特殊场景来看，想要判断一个资源对象是不是真正的关闭，还是非常复杂的。但是经过我们火线团队的努力，火线目前已经能够覆盖绝大部分场景，误报率和检出率均优于业界开源产品。我们将在下一篇文章中列出火线和其他开源产品横向的扫描结果对比报告，包括Sonar、Lint、PMD、Findbugs等，敬请期待。







