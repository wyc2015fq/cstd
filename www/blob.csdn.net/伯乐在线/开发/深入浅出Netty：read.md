# 深入浅出Netty：read - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本系列：
- 《[深入浅出Netty（1）](http://blog.jobbole.com/105499/)》
- 《[深入浅出Netty：服务启动](http://blog.jobbole.com/105565/)》
- 《[深入浅出Netty：NioEventLoop](http://blog.jobbole.com/105564/)》
- 《[深入浅出Netty：ChannelPipeline](http://blog.jobbole.com/105605/)》
- 《[深入浅出Netty：accept](http://blog.jobbole.com/105658/)》
boss线程主要负责监听并处理accept事件，将socketChannel注册到work线程的selector，由worker线程来监听并处理read事件，本节主要分析Netty如何处理read事件。
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f7xfc9lbd1j20le0bxdgo.jpg)
accept->read
当work线程的selector检测到OP_READ事件发生时，触发read操作。


```
//NioEventLoop  
if ((readyOps & (SelectionKey.OP_READ | SelectionKey.OP_ACCEPT)) != 0 || readyOps == 0) {  
    unsafe.read();  
    if (!ch.isOpen()) {  
        // Connection already closed - no need to handle write.  
        return;  
    }  
}
```
该read方法定义在类NioByteUnsafe中。


```
//AbstractNioByteChannel.NioByteUnsafe
public final void read() {
    final ChannelConfig config = config();
    if (!config.isAutoRead() && !isReadPending()) {
        // ChannelConfig.setAutoRead(false) was called in the meantime
        removeReadOp();
        return;
    }
    final ChannelPipeline pipeline = pipeline();
    final ByteBufAllocator allocator = config.getAllocator();
    final int maxMessagesPerRead = config.getMaxMessagesPerRead();
    RecvByteBufAllocator.Handle allocHandle = this.allocHandle;
    if (allocHandle == null) {
        this.allocHandle = allocHandle = config.getRecvByteBufAllocator().newHandle();
    }
    ByteBuf byteBuf = null;
    int messages = 0;
    boolean close = false;
    try {
        int totalReadAmount = 0;
        boolean readPendingReset = false;
        do {
            byteBuf = allocHandle.allocate(allocator);
            int writable = byteBuf.writableBytes();
            int localReadAmount = doReadBytes(byteBuf);
            if (localReadAmount <= 0) {
                // not was read release the buffer
                byteBuf.release();
                byteBuf = null;
                close = localReadAmount < 0;
                break;
            }
            if (!readPendingReset) {
                readPendingReset = true;
                setReadPending(false);
            }
            pipeline.fireChannelRead(byteBuf);
            byteBuf = null;
            if (totalReadAmount >= Integer.MAX_VALUE - localReadAmount) {
                // Avoid overflow.
                totalReadAmount = Integer.MAX_VALUE;
                break;
            }
            totalReadAmount += localReadAmount;
            // stop reading
            if (!config.isAutoRead()) {
                break;
            }
            if (localReadAmount < writable) {
                // Read less than what the buffer can hold,
                // which might mean we drained the recv buffer completely.
                break;
            }
        } while (++ messages < maxMessagesPerRead);
        pipeline.fireChannelReadComplete();
        allocHandle.record(totalReadAmount);
        if (close) {
            closeOnRead(pipeline);
            close = false;
        }
    } catch (Throwable t) {
        handleReadException(pipeline, byteBuf, t, close);
    } finally {
        // Check if there is a readPending which was not processed yet.
        // This could be for two reasons:
        // * The user called Channel.read() or ChannelHandlerContext.read() in channelRead(...) method
        // * The user called Channel.read() or ChannelHandlerContext.read() in channelReadComplete(...) method
        //
        // See https://github.com/netty/netty/issues/2254
        if (!config.isAutoRead() && !isReadPending()) {
            removeReadOp();
        }
    }
}
```
1、allocHandle负责自适应调整当前缓存分配的大小，以防止缓存分配过多或过少，先看看AdaptiveRecvByteBufAllocator内部实现：


```
public class AdaptiveRecvByteBufAllocator implements RecvByteBufAllocator {
    static final int DEFAULT_MINIMUM = 64;
    static final int DEFAULT_INITIAL = 1024;
    static final int DEFAULT_MAXIMUM = 65536;
    private static final int INDEX_INCREMENT = 4;
    private static final int INDEX_DECREMENT = 1;
    private static final int[] SIZE_TABLE;
}
```
**SIZE_TABLE**：按照从小到大的顺序预先存储可以分配的缓存大小。
从16开始，每次累加16，直到496，接着从512开始，每次增大一倍，直到溢出。
**DEFAULT_MINIMUM**：最小缓存（64），在SIZE_TABLE中对应的下标为3。
**DEFAULT_MAXIMUM **：最大缓存（65536），在SIZE_TABLE中对应的下标为38。
**DEFAULT_INITIAL **：初始化缓存大小，第一次分配缓存时，由于没有上一次实际收到的字节数做参考，需要给一个默认初始值。
**INDEX_INCREMENT**：上次预估缓存偏小，下次index的递增值。
**INDEX_DECREMENT **：上次预估缓存偏大，下次index的递减值。
2、allocHandle.allocate(allocator) 申请一块指定大小的内存。


```
//AdaptiveRecvByteBufAllocator.HandleImpl
public ByteBuf allocate(ByteBufAllocator alloc) {
    return alloc.ioBuffer(nextReceiveBufferSize);
}
```
通过ByteBufAllocator的ioBuffer方法申请缓存。


```
//AbstractByteBufAllocator
public ByteBuf ioBuffer(int initialCapacity) {
    if (PlatformDependent.hasUnsafe()) {
        return directBuffer(initialCapacity);
    }
    return heapBuffer(initialCapacity);
}
```
根据平台是否支持unsafe，选择使用直接物理内存还是堆上内存。
direct buffer方案：


```
//AbstractByteBufAllocator
public ByteBuf directBuffer(int initialCapacity) {
    return directBuffer(initialCapacity, Integer.MAX_VALUE);
}
public ByteBuf directBuffer(int initialCapacity, int maxCapacity) {
    if (initialCapacity == 0 && maxCapacity == 0) {
        return emptyBuf;
    }
    validate(initialCapacity, maxCapacity);
    return newDirectBuffer(initialCapacity, maxCapacity);
}
//UnpooledByteBufAllocator
protected ByteBuf newDirectBuffer(int initialCapacity, int maxCapacity) {
    ByteBuf buf;
    if (PlatformDependent.hasUnsafe()) {
        buf = new UnpooledUnsafeDirectByteBuf(this, initialCapacity, maxCapacity);
    } else {
        buf = new UnpooledDirectByteBuf(this, initialCapacity, maxCapacity);
    }
    return toLeakAwareBuffer(buf);
}
```
UnpooledUnsafeDirectByteBuf是如何实现缓存管理的？对Nio的ByteBuffer进行了封装，通过ByteBuffer的allocateDirect方法实现缓存的申请。


```
protected UnpooledUnsafeDirectByteBuf(ByteBufAllocator alloc, ByteBuffer initialBuffer, int maxCapacity) {
    //判断逻辑已经忽略
   this.alloc = alloc;
   setByteBuffer(allocateDirect(initialCapacity));
}
protected ByteBuffer allocateDirect(int initialCapacity) {
    return ByteBuffer.allocateDirect(initialCapacity);
}
private void setByteBuffer(ByteBuffer buffer) {
    ByteBuffer oldBuffer = this.buffer;
    if (oldBuffer != null) {
        if (doNotFree) {
            doNotFree = false;
        } else {
            freeDirect(oldBuffer);
        }
    }
    this.buffer = buffer;
    memoryAddress = PlatformDependent.directBufferAddress(buffer);
    tmpNioBuf = null;
    capacity = buffer.remaining();
}
```
memoryAddress = PlatformDependent.directBufferAddress(buffer) 获取buffer的address字段值，指向缓存地址。
capacity = buffer.remaining() 获取缓存容量。
方法toLeakAwareBuffer(buf)对申请的buf又进行了一次包装：


```
protected static ByteBuf toLeakAwareBuffer(ByteBuf buf) {
    ResourceLeak leak;
    switch (ResourceLeakDetector.getLevel()) {
        case SIMPLE:
            leak = AbstractByteBuf.leakDetector.open(buf);
            if (leak != null) {
                buf = new SimpleLeakAwareByteBuf(buf, leak);
            }
            break;
        case ADVANCED:
        case PARANOID:
            leak = AbstractByteBuf.leakDetector.open(buf);
            if (leak != null) {
                buf = new AdvancedLeakAwareByteBuf(buf, leak);
            }
            break;
    }
    return buf;
}
```
Netty中使用引用计数机制来管理资源，ByteBuf实现了ReferenceCounted接口，当实例化一个ByteBuf时，引用计数为1， 代码中需要保持一个该对象的引用时需要调用retain方法将计数增1，对象使用完时调用release将计数减1。当引用计数变为0时，对象将释放所持有的底层资源或将资源返回资源池。
3、方法doReadBytes(byteBuf) 将socketChannel数据写入缓存。


```
//NioSocketChannel
@Override
protected int doReadBytes(ByteBuf byteBuf) throws Exception {
    return byteBuf.writeBytes(javaChannel(), byteBuf.writableBytes());
}
//WrappedByteBuf
@Override
public int writeBytes(ScatteringByteChannel in, int length) throws IOException {
    return buf.writeBytes(in, length);
}
//AbsractByteBuf
@Override
public int writeBytes(ScatteringByteChannel in, int length) throws IOException {
    ensureAccessible();
    ensureWritable(length);
    int writtenBytes = setBytes(writerIndex, in, length);
    if (writtenBytes > 0) {
        writerIndex += writtenBytes;
    }
    return writtenBytes;
}
//UnpooledUnsafeDirectByteBuf
@Override
public int setBytes(int index, ScatteringByteChannel in, int length) throws IOException {
    ensureAccessible();
    ByteBuffer tmpBuf = internalNioBuffer();
    tmpBuf.clear().position(index).limit(index + length);
    try {
        return in.read(tmpBuf);
    } catch (ClosedChannelException ignored) {
        return -1;
    }
}
private ByteBuffer internalNioBuffer() {
    ByteBuffer tmpNioBuf = this.tmpNioBuf;
    if (tmpNioBuf == null) {
        this.tmpNioBuf = tmpNioBuf = buffer.duplicate();
    }
    return tmpNioBuf;
}
```
最终底层采用ByteBuffer实现read操作，这里有一块逻辑不清楚，为什么要用tmpNioBuf？
int localReadAmount = doReadBytes(byteBuf);
1、如果返回0，则表示没有读取到数据，则退出循环。
2、如果返回-1，表示对端已经关闭连接，则退出循环。
3、否则，表示读取到了数据，数据读入缓存后，触发pipeline的ChannelRead事件，byteBuf作为参数进行后续处理，这时自定义Inbound类型的handler就可以进行业务处理了。


```
static class DiscardServerHandler extends ChannelInboundHandlerAdapter {
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        ByteBuf in = (ByteBuf) msg;
        try {
            while (in.isReadable()) { // (1)
                System.out.print((char) in.readByte());
                System.out.flush();
            }
        } finally {
            ReferenceCountUtil.release(msg); // (2)
        }
    }
}
```
其中参数msg，就是对应的byteBuf，当请求的数据量比较大时，会多次触发channelRead事件，默认最多触发16次，可以通过maxMessagesPerRead字段进行配置。
如果客户端传输的数据过大，可能会分成好几次传输，因为TCP一次传输内容大小有上限，所以同一个selectKey会触发多次read事件，剩余的数据会在下一轮select操作继续读取。
在实际应用中，应该把所有请求数据都缓存起来再进行业务处理。
所有数据都处理完，触发pipeline的ChannelReadComplete事件，并且allocHandle记录这次read的字节数，进行下次处理时缓存大小的调整。
到此为止，整个NioSocketChannel的read事件已经处理完成。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
