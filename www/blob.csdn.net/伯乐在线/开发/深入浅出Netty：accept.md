# 深入浅出Netty：accept - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本系列：
- 《[深入浅出Netty（1）](http://blog.jobbole.com/105499/)》
- 《[深入浅出Netty：服务启动](http://blog.jobbole.com/105565/)》
- 《[深入浅出Netty：NioEventLoop](http://blog.jobbole.com/105564/)》
- 《[深入浅出Netty：ChannelPipeline](http://blog.jobbole.com/105605/)》
本章节分析服务端如何accept客户端的connect请求。
在《[深入浅出Netty：NioEventLoop](http://blog.jobbole.com/105564/)》章节中，已经分析了NioEventLoop的工作机制，当有客户端connect请求，selector可以返回其对应的SelectionKey，方法processSelectedKeys进行后续的处理。
![2184951-d9cb43b57e0dfd11](http://jbcdn2.b0.upaiyun.com/2016/09/75cfc7f91dfa16926421ae11ae839f6d.png)


```
private void processSelectedKeys() {
    if (selectedKeys != null) {
        processSelectedKeysOptimized(selectedKeys.flip());
    } else {
        processSelectedKeysPlain(selector.selectedKeys());
    }
}
```
默认采用优化过的SelectedSelectionKeySet保存有事件发生的selectedKey。
1、SelectedSelectionKeySet内部使用两个大小为1024的SelectionKey数组keysA和keysB保存selectedKey。
2、把SelectedSelectionKeySet实例映射到selector的原生selectedKeys和publicSelectedKeys。


```
private void processSelectedKeysOptimized(SelectionKey[] selectedKeys) {
    for (int i = 0;; i ++) {
        final SelectionKey k = selectedKeys[i];
        if (k == null) {
            break;
        }
        // null out entry in the array to allow to have it GC'ed once the Channel close
        // See https://github.com/netty/netty/issues/2363
        selectedKeys[i] = null;
        final Object a = k.attachment();
        if (a instanceof AbstractNioChannel) {
            processSelectedKey(k, (AbstractNioChannel) a);
        } else {
            @SuppressWarnings("unchecked")
            NioTask<SelectableChannel> task = (NioTask<SelectableChannel>) a;
            processSelectedKey(k, task);
        }
        if (needsToSelectAgain) {
            // null out entries in the array to allow to have it GC'ed once the Channel close
            // See https://github.com/netty/netty/issues/2363
            for (;;) {
                i++;
                if (selectedKeys[i] == null) {
                    break;
                }
                selectedKeys[i] = null;
            }
            selectAgain();
            // Need to flip the optimized selectedKeys to get the right reference to the array
            // and reset the index to -1 which will then set to 0 on the for loop
            // to start over again.
            //
            // See https://github.com/netty/netty/issues/1523
            selectedKeys = this.selectedKeys.flip();
            i = -1;
        }
    }
}
```
因为selector的I/O多路复用机制，一次可以返回多个selectedKey，所以要用for循环处理全部selectionKey。
假设这时有请求进来，selectedKeys中就存在一个selectionKey，这块逻辑不清楚的可以回头看看[深入浅出Nio Socket](http://www.jianshu.com/p/0d497fe5484a)。
1、通过k.attachment()可以获取ServerSocketChannel注册时绑定上去的附件，其实这个附件就是ServerSocketChannel自身。
2、如果selectedKey的附件是AbstractNioChannel类型的，执行processSelectedKey(k, (AbstractNioChannel) a)方法进行下一步操作。


```
private static void processSelectedKey(SelectionKey k, AbstractNioChannel ch) {
    final NioUnsafe unsafe = ch.unsafe();
    if (!k.isValid()) {
        // close the channel if the key is not valid anymore
        unsafe.close(unsafe.voidPromise());
        return;
    }
    try {
        int readyOps = k.readyOps();
        // Also check for readOps of 0 to workaround possible JDK bug which may otherwise lead
        // to a spin loop
        if ((readyOps & (SelectionKey.OP_READ | SelectionKey.OP_ACCEPT)) != 0 || readyOps == 0) {
            unsafe.read();
            if (!ch.isOpen()) {
                // Connection already closed - no need to handle write.
                return;
            }
        }
        if ((readyOps & SelectionKey.OP_WRITE) != 0) {
            // Call forceFlush which will also take care of clear the OP_WRITE once there is nothing left to write
            ch.unsafe().forceFlush();
        }
        if ((readyOps & SelectionKey.OP_CONNECT) != 0) {
            // remove OP_CONNECT as otherwise Selector.select(..) will always return without blocking
            // See https://github.com/netty/netty/issues/924
            int ops = k.interestOps();
            ops &= ~SelectionKey.OP_CONNECT;
            k.interestOps(ops);
            unsafe.finishConnect();
        }
    } catch (CancelledKeyException ignored) {
        unsafe.close(unsafe.voidPromise());
    }
}
```
1、获取ServerSocketChannel的unsafe对象。
2、当前selectionKey发生的事件是SelectionKey.OP_ACCEPT，执行unsafe的read方法。
该read方法定义在NioMessageUnsafe类中：


```
private final List<Object> readBuf = new ArrayList<Object>();
@Override
public void read() {
    assert eventLoop().inEventLoop();
    final ChannelConfig config = config();
    if (!config.isAutoRead() && !isReadPending()) {
        // ChannelConfig.setAutoRead(false) was called in the meantime
        removeReadOp();
        return;
    }
    final int maxMessagesPerRead = config.getMaxMessagesPerRead();
    final ChannelPipeline pipeline = pipeline();
    boolean closed = false;
    Throwable exception = null;
    try {
        try {
            for (;;) {
                int localRead = doReadMessages(readBuf);
                if (localRead == 0) {
                    break;
                }
                if (localRead < 0) {
                    closed = true;
                    break;
                }
                // stop reading and remove op
                if (!config.isAutoRead()) {
                    break;
                }
                if (readBuf.size() >= maxMessagesPerRead) {
                    break;
                }
            }
        } catch (Throwable t) {
            exception = t;
        }
        setReadPending(false);
        int size = readBuf.size();
        for (int i = 0; i < size; i ++) {
            pipeline.fireChannelRead(readBuf.get(i));
        }
        readBuf.clear();
        pipeline.fireChannelReadComplete();
        if (exception != null) {
            if (exception instanceof IOException && !(exception instanceof PortUnreachableException)) {
                // ServerChannel should not be closed even on IOException because it can often continue
                // accepting incoming connections. (e.g. too many open files)
                closed = !(AbstractNioMessageChannel.this instanceof ServerChannel);
            }
            pipeline.fireExceptionCaught(exception);
        }
        if (closed) {
            if (isOpen()) {
                close(voidPromise());
            }
        }
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
1、readBuf 用来保存客户端NioSocketChannel，默认一次不超过16个。
2、方法doReadMessages进行处理ServerSocketChannel的accept操作。


```
protected int doReadMessages(List<Object> buf) throws Exception {
    SocketChannel ch = javaChannel().accept();
    try {
        if (ch != null) {
            buf.add(new NioSocketChannel(this, ch));
            return 1;
        }
    } catch (Throwable t) {
        logger.warn("Failed to create a new channel from an accepted socket.", t);
        try {
            ch.close();
        } catch (Throwable t2) {
            logger.warn("Failed to close a socket.", t2);
        }
    }
    return 0;
}
```
1、javaChannel()返回NioServerSocketChannel对应的ServerSocketChannel。
2、ServerSocketChannel.accept返回客户端的socketChannel 。
3、把 NioServerSocketChannel 和 socketChannel 封装成 NioSocketChannel，并缓存到readBuf。
4、遍历redBuf中的NioSocketChannel，触发各自pipeline的ChannelRead事件，从pipeline的head开始遍历，最终执行ServerBootstrapAcceptor的channelRead方法。


```
public void channelRead(ChannelHandlerContext ctx, Object msg) {
    final Channel child = (Channel) msg;
    child.pipeline().addLast(childHandler);
    for (Entry<ChannelOption<?>, Object> e: childOptions) {
        try {
            if (!child.config().setOption((ChannelOption<Object>) e.getKey(), e.getValue())) {
                logger.warn("Unknown channel option: " + e);
            }
        } catch (Throwable t) {
            logger.warn("Failed to set a channel option: " + child, t);
        }
    }
    for (Entry<AttributeKey<?>, Object> e: childAttrs) {
        child.attr((AttributeKey<Object>) e.getKey()).set(e.getValue());
    }
    try {
        childGroup.register(child).addListener(new ChannelFutureListener() {
            @Override
            public void operationComplete(ChannelFuture future) throws Exception {
                if (!future.isSuccess()) {
                    forceClose(child, future.cause());
                }
            }
        });
    } catch (Throwable t) {
        forceClose(child, t);
    }
}
```
1、child.pipeline().addLast(childHandler)添加childHandler到NioSocketChannel的pipeline。
其中childHandler是通过ServerBootstrap的childHandler方法进行配置的，和NioServerSocketChannel类似，NioSocketChannel在注册到selector后会触发其pipeline的fireChannelRegistered方法，并执行initChannel方法，为NioSocketChannel的pipeline添加更多自定义的handler，进行业务处理。
2、childGroup.register(child)将NioSocketChannel注册到work的eventLoop中，这个过程和NioServerSocketChannel注册到boss的eventLoop的过程一样，最终由work线程对应的selector进行read事件的监听。
当readBuf中缓存的NioSocketChannel都处理完成后，清空readBuf，并触发ChannelReadComplete。
到此为止，一次accept流程已经执行完。
END。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
