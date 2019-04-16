# 静态代码扫描（四）——Java资源关闭研究 - oggboy的专栏 - CSDN博客





2017年05月05日 19:00:08[oggboy](https://me.csdn.net/oggboy)阅读数：322








> 
最近一直在研究java资源关闭的检查规则，发现市面上开源的工具针对资源关闭的检测都存在一定不足，同时也无法满足我们业务的需求。所以火线团队针对资源关闭进行了深度的研究，取得了一些不错的进展，但是过程的艰辛也远超了我们的预料。现在就跟大家聊聊我们的心路历程，从为什么开始。


#### 1. 为什么要手动关闭Java资源对象？

首先解释Java的资源对象，它主要包括IO对象，数据库连接对象。比如常见的InputStream、OutputStream、Reader、Writer、Connection、Statement、ResultSet、Socket等等，先代码列举一个示例：

```
FileInputStream f = new FileInputStream("sample.txt");
f.close();//f对象即需要手动关闭的资源对象
```

上述代码中f对象即需要手动关闭的资源对象。 

如果类似的资源对象没有及时的手动关闭，这个对象就会一直占据内存，当这样的对象越来越多，那内存被占用的就会越来越多，久而久之就可能造成OutOfMemory，俗称内存溢出。

![内存溢出案例](https://img-blog.csdn.net/20170508112848674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2dnYm95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这时应该有人会问，Java不是有自己的垃圾回收机制GC么？不是可以自动回收么？ 

这个问题问的好，我也一度非常困惑。 

首先我们先了解一下GC的原理： 

在Java中，当没有对象引用指向原先分配给某个对象的内存时，该内存便成为垃圾。JVM的一个系统级线程会自动释放该内存块。垃圾回收意味着程序不再需要的对象是”无用信息”，这些信息将被丢弃。当一个对象不再被引用的时候，内存回收它占领的空间，以便空间被后来的新对象使用。
![这里写图片描述](https://img-blog.csdn.net/20170508113654397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2dnYm95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

首先GC只能回收内存。至于各种stream之类，他们下边一般还开启了各种其他的系统资源，比如文件，比如输入输出设备（键盘/屏幕等），等等。而这些设备第一是不能自动关闭（因为谁知道你程序要用它到什么时候啊），另一个系统内数量有限（比如键盘/屏幕同一时间只有一个）。最后，文件和数据库连接之类的东西还存在读写锁定的问题。这些都导致用户必须手动处理这些资源的开启和关闭。

其次为了“避免”程序员忘了自己释放那些资源，Java提供了finalizer、PhantomReference之类的机制来让程序员向GC注册“自动回调释放资源”的功能。但GC回调它们的时机不确定，所以只应该作为最后手段来使用，主要手段还是自己关闭最好。

PS：关于GC其实有很多的知识可以深度挖掘，比如各种回收算法，finalize()方法等等，大家感兴趣的话可以自行搜索研究，我就不班门弄斧了。

#### 2. 怎样正确的手动关闭Java资源对象？

先说一种最常见的关闭方式，在finally中进行关闭：

```
FileInputStream f;
try{
    f= new FileInputStream("sample.txt");
    //something that uses f and sometimes throws an exception
}
catch(IOException ex){
    /* Handle it somehow */
}
finally{
    f.close();
}
```

这里在finally中进行资源对象关闭属于Best Practice。因为即使对象f在使用的过程中出现异常，也能保证程序不会跳过后续的关闭操作。

特别注意，自从Java1.7开始，支持了try-with-resources写法，即将资源对象声明的过程放在try()的括号里面，这样java在资源对象使用完成之后会自动关闭。

```
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
```

另外还有一些第三方库提供了一些统一的关闭处理方法，例如

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

这个apache提供的IOUtils类库可以通过IOUtils.closeQuietly(e)的形式关闭资源对象，实际内部实现依然是调用.close()方法。内部实现代码如下：

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

以上就是手动关闭Java资源对象的几种推荐写法，希望对大家有所帮助。

为防止篇幅过长，这只是系列文章的第一篇，我将在下一篇继续讲述在判断资源关闭时，有哪些不为人知的特殊情况需要考虑。 

敬请期待。

#### 参考文献：

> 
CSDN.[Java垃圾回收机制](http://blog.csdn.net/zsuguangh/article/details/6429592)

  知乎. [为什么Java有GC还需要自己来关闭某些资源？](https://www.zhihu.com/question/29265003)

  Oracle.[Java Garbage Collection Basics](http://www.oracle.com/webfolder/technetwork/tutorials/obe/java/gc01/index.html)

  stackoverflow.[Why do I need to use finally to close resources?](http://stackoverflow.com/questions/7652050/why-do-i-need-to-use-finally-to-close-resources)





