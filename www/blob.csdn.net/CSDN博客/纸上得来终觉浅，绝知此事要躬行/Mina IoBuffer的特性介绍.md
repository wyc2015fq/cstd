# Mina IoBuffer的特性介绍 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月07日 11:54:25[boonya](https://me.csdn.net/boonya)阅读数：3972
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina IoBuffer(Apache Mina user guide Chapter8 IoBuffer)](http://mina.apache.org/mina-project/userguide/ch8-iobuffer/ch8-iobuffer.html)

一个字节缓冲区被Mina的应用程序所使用。这是一个[ByteBuffer](http://java.sun.com/j2se/1.5.0/docs/api/java/nio/ByteBuffer.html)的替代类。Mina不直接使用NIOByteBuffer,有两个原因:
- 它不提供有用的getters和putters方法,如fill/ putString,get/ putAsciiInt()。
- 由于其固定容量很难写变长数据.



这将在Mina3改变。Mina有自己的包装主要原因是拓展nio ByteBuffer实现可扩展的缓冲区。这是一个非常糟糕的决定。缓冲区缓冲:临时存储临时数据,之前使用它。存在许多其他的解决方案,比如定义一个包装器,依靠NIO
 bytebuffer的列表,而不是复制现有的缓冲区更大的一个仅仅因为我们希望延长缓冲能力。



它也可能是更舒适使用InputStream而不是一个字节缓冲区的过滤器,因为它并不意味着对存储数据的性质:它可以是一个字节数组,字符串,消息……



最后,同样重要的是,当前实现失败的一个目标:零拷贝策略(例如,一旦我们从套接字读取数据,稍后我们不想做一个副本)。我们使用可扩展字节缓冲区,我们将肯定复制这些数据如果我们有处理大消息。假设Mina的ByteBuffer只是一个包装器NIO
 ByteBuffer之上,在使用直接缓冲区时这可能是一个真正的问题。

### 1.1.1. **IoBuffer操作**

#### 1.1.1.1. **分配一个新的缓冲区**

IoBuffer是一个抽象类,因此不能直接实例化。分配IoBuffer,我们需要使用两个allocate()方法中的一个。

```java
<span style="font-size:12px;">// Allocates a new buffer with a specific size, defining its type (direct or heap)
public static IoBuffer allocate(int capacity, boolean direct)

// Allocates a new buffer with a specific size
public static IoBuffer allocate(int capacity)</span>
```

allocate()方法接受一个或两个参数。第一个结构需要两个参数:

· **capacity** - 缓冲区的容量大小

· **direct** - 
类型的缓冲区。true直接使用缓冲区,false使用堆缓冲区

默认的缓冲区分配是由[SimpleBufferAllocator](http://mina.apache.org/mina-project/xref/org/apache/mina/core/buffer/SimpleBufferAllocator.html)处理的。



另外,还可以使用以下形式：

```java
<span style="font-size:12px;">// Allocates heap buffer by default.
IoBuffer.setUseDirectBuffer(false);

// A new heap buffer is returned.
IoBuffer buf = IoBuffer.allocate(1024);</span>
```

使用第二种形式时,别忘了之前设置默认缓冲类型,否则您将得到默认堆缓冲区。

### 1.1.2. **创建自动扩展的缓冲区**

创建自动扩大缓冲与java NIO API不是很容易,因为固定大小的缓冲区。有一个缓冲区,可以自动扩展需要的是网络应用的一大亮点。为了解决这个问题,IoBuffer引入了autoExpand属性。它会自动扩大容量和极限值。

让我们看看如何创建一个自动扩大缓冲区:

```java
<span style="font-size:12px;">IoBuffer buffer = IoBuffer.allocate(8);

buffer.setAutoExpand(true);

buffer.putString("12345678", encoder);

// Add more to this buffer

buffer.put((byte)10);</span>
```

底层ByteBuffer在幕后重新分配是IoBuffer。如果编码数据大于8个字节在上面的例子中。容量大小将翻倍,其限制将增加到最后位置的字符串。这种行为非常类似于StringBuffer类的工作方式。

这种机制很可能是远离Mina3.0,因为它不是最好的方式来处理增加的缓冲区大小。取而代之的应该是类似InputStream隐藏列表或一个固定大小的bytebuffer)数组。

### 1.1.3. **创建自动收缩的缓冲区**

有些情况下，调用释放另外分配的字节缓冲区以保存到内存。IoBuffer提供autoShrink属性处理的需要。如果autoShrink打开,IoBuffer半紧凑时缓冲区的容量()调用,只有1/4或少使用当前的能力。手动收缩缓冲区,使用shrink()方法。

让我们看看这种方式：



```java
<span style="font-size:12px;">IoBuffer buffer = IoBuffer.allocate(16);
buffer.setAutoShrink(true);
buffer.put((byte)1);
System.out.println("Initial Buffer capacity = "+buffer.capacity());
buffer.shrink();
System.out.println("Initial Buffer capacity after shrink = "+buffer.capacity());
buffer.capacity(32);
System.out.println("Buffer capacity after incrementing capacity to 32 = "+buffer.capacity());
buffer.shrink();
System.out.println("Buffer capacity after shrink= "+buffer.capacity());</span>
```

我们最初分配一个容量为16,autoShrink属性设置为true。

我们来看它的输出：



```java
<span style="font-size:12px;">Initial Buffer capacity = 16
Initial Buffer capacity after shrink = 16
Buffer capacity after incrementing capacity to 32 = 32
Buffer capacity after shrink= 16</span>
```

让我们休息一下和分析输出：
- 初始缓冲容量是16,我们创建的缓冲能力。内部这变成了最低的缓冲能力
- 调用后shrink(),能力仍然是16,能力永远不会小于最小能力
- 增加32的能力后,变成了32的能力
- shrink()的调用,减少能力16,从而消除额外的存储



同样，这种应该是一个默认机制,无需影响告诉缓冲,它可以缩小。

### 1.1.4. **缓冲区分配**

IoBufferAllocater负责分配和管理缓冲区。精确控制缓冲区分配策略,实现IoBufferAllocater接口。

Mina附带IoBufferAllocater的实现：
- SimpleBufferAllocator(默认),每次都创建一个新的缓冲区
- CachedBufferAllocator——缓存缓冲区可能被重用扩张



在新的可用的JVM中,使用缓存IoBuffer很可能提高性能。



您可以实现您自己的实现IoBufferAllocator并调用setAllocator()在IoBuffer使用相同。





