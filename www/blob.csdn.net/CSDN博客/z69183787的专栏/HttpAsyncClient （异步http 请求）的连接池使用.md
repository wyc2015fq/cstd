# HttpAsyncClient （异步http 请求）的连接池使用 - z69183787的专栏 - CSDN博客
2016年11月03日 17:11:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5828
代码示例
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public static void main(String[] args) throws Exception {
        ConnectingIOReactor ioReactor = new DefaultConnectingIOReactor();
        PoolingNHttpClientConnectionManager cm = new PoolingNHttpClientConnectionManager(ioReactor);
        cm.setMaxTotal(100);
        CloseableHttpAsyncClient httpAsyncClient = HttpAsyncClients.custom().setConnectionManager(cm).build();
        httpAsyncClient.start();
        String[] urisToGet = {
                "http://www.chinaso.com/",
                "http://www.so.com/",
                "http://www.qq.com/",
        };
        final CountDownLatch latch = new CountDownLatch(urisToGet.length);
        for (final String uri: urisToGet) {
            final HttpGet httpget = new HttpGet(uri);
            httpAsyncClient.execute(httpget, new FutureCallback<HttpResponse>() {
                public void completed(final HttpResponse response) {
                    latch.countDown();
                    System.out.println(httpget.getRequestLine() + "->" + response.getStatusLine());
                }
                public void failed(final Exception ex) {
                    latch.countDown();
                    System.out.println(httpget.getRequestLine() + "->" + ex);
                }
                public void cancelled() {
                    latch.countDown();
                    System.out.println(httpget.getRequestLine() + " cancelled");
                }
            });
        }
        latch.await();
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
流程分析
简要流程总结如下:
HttpAsyncClient有一个AbstractMultiworkerIOReactor和AbstractIOReactor, 前者和后者类似于netty的bossGroup和workerGroup, AbstractMultiworkerIOReactor负责channel的连接, AbstractIOReactor负责channel的读写
先要明白连接池的结构
AbstractNIOConnPool 下各个变量的含义
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    // 一个可复用的ioreactor, 负责生成SessionRequest并唤醒selector去做连接到目标网站的操作
    private final ConnectingIOReactor ioreactor;
    // 用来构造连接池的entry的工厂
    private final NIOConnFactory<T, C> connFactory;
    // 验证并生成目标连接socketAddress的类
    private final SocketAddressResolver<T> addressResolver;
    // 一个可复用的callBack类, 里面提供了一个调用SessionRequest的complete的方法
    private final SessionRequestCallback sessionRequestCallback;
    // 用域名区分的连接池
    private final Map<T, RouteSpecificPool<T, C, E>> routeToPool;
    // 没有成功拿到连接的请求列表
    private final LinkedList<LeaseRequest<T, C, E>> leasingRequests;
    // 已经拿到连接权利, 但是还没连接成功的连接集合
    private final Set<SessionRequest> pending;
    // 已经连接成功, 并被租借出去的连接集合
    private final Set<E> leased;
    // 当前连接池可用的连接集合
    private final LinkedList<E> available;
    // 已经连接完成, 但是不可用的连接集合, 例如因为异常连接失败等待, 他们会在队列中等待被调用回调方法做后续处理
    private final ConcurrentLinkedQueue<LeaseRequest<T, C, E>> completedRequests;
    // 每个route的最大连接数
    private final Map<T, Integer> maxPerRoute;
    // 锁对象
    private final Lock lock;
    // 是否关闭
    private final AtomicBoolean isShutDown;
    // 每个route最大连接数默认值
    private volatile int defaultMaxPerRoute;
    // 整个连接池最大连接数
    private volatile int maxTotal;
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 
1. 发起请求
 a. 根据请求route查看连接池, 如果连接池不为空, 直接返回跟池中connection绑定的future, 并把该conn放入leased列表
 b. 如果因为某些原因导致当前请求无法取得连接, 但是没有发生致命错误的, 请求将被放入一个 leasing 列表, 这个列表会在后续动作中被取出来做连接重试
 c. 如果实在连接过程中出现了移除等不可恢复的错误, 则将request标记为completed, 退出方法后调用fireCallBack, 进行回调清理, 这次请求就算是失败结束了
 d. 如果是因为连接池没有可用连接, 但是可以新建连接的情况, 则会将request 加入pending列表, 并调用 selector的wakeup()方法, selector在wakeup以后会使用AbstractMultiworkerIOReactor(bossGroup)来进行连接操作, 并注册到selector中, 后续的connectable事件监听和channel连接成功注册也是由他完成的
2. AbstractIOReactor监听读写事件
3. 通过decoder检测response已经完成, 最后将连接release到连接池中, 此时将连接从leased列表除去, 并加入到available中
 
## 连接阶段 
调用
```
Future<HttpResponse> execute(
HttpUriRequest request, 
FutureCallback<org.apache.http.HttpResponse> callback)
```
请求开始, 里面会调用 execute(request, new BasicHttpContext(), callback)
调用
```
Future<HttpResponse> execute(
            final HttpUriRequest request,
            final HttpContext context,
            final FutureCallback<HttpResponse> callback)
```
context 代表了一次请求的上下文, 里面实际上就是一个用来存储 attribute 的结构, 默认的实现 BasicHttpContext 实际上就是一个 ConcurrentHashMap
context 是可以嵌套的, 代码如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@ThreadSafe
public class BasicHttpContext implements HttpContext {
    private final HttpContext parentContext;
    private final Map<String, Object> map;
    public BasicHttpContext() {
        this(null);
    }
    public BasicHttpContext(final HttpContext parentContext) {
        super();
        this.map = new ConcurrentHashMap<String, Object>();
        this.parentContext = parentContext;
    }
    public Object getAttribute(final String id) {
        Args.notNull(id, "Id");
        Object obj = this.map.get(id);
        if (obj == null && this.parentContext != null) {
            obj = this.parentContext.getAttribute(id);
        }
        return obj;
    }
    public void setAttribute(final String id, final Object obj) {
        Args.notNull(id, "Id");
        if (obj != null) {
            this.map.put(id, obj);
        } else {
            this.map.remove(id);
        }
    }
    public Object removeAttribute(final String id) {
        Args.notNull(id, "Id");
        return this.map.remove(id);
    }
    /**
     * @since 4.2
     */
    public void clear() {
        this.map.clear();
    }
    @Override
    public String toString() {
        return this.map.toString();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
接着看执行流程
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public Future<HttpResponse> execute(
            final HttpUriRequest request,
            final HttpContext context,
            final FutureCallback<HttpResponse> callback) {
        final HttpHost target;
        try {
            target = determineTarget(request); // 这一步是取出目标host
        } catch (final ClientProtocolException ex) {
            final BasicFuture<HttpResponse> future = new BasicFuture<HttpResponse>(callback);
            future.failed(ex);
            return future;
        }
        return execute(target, request, context, callback);
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 
调用
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public Future<HttpResponse> execute(
            final HttpHost target, final HttpRequest request, final HttpContext context,
            final FutureCallback<HttpResponse> callback) {
        return execute(
                HttpAsyncMethods.create(target, request),
                HttpAsyncMethods.createConsumer(),
                context, callback);
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
位于HttpAsyncClient接口下的
    /**
     * Initiates asynchronous HTTP request execution using the given context.
     * <p/>
     * The request producer passed to this method will be used to generate
     * a request message and stream out its content without buffering it
     * in memory. The response consumer passed to this method will be used
     * to process a response message without buffering its content in memory.
     * <p/>
     * Please note it may be unsafe to interact with the context instance
     * while the request is still being executed.
     *
     * @param <T> the result type of request execution.
     * @param requestProducer request producer callback.
     * @param responseConsumer response consumer callaback.
     * @param context HTTP context
     * @param callback future callback.
     * @return future representing pending completion of the operation.
     */
    <T> Future<T> execute(
            HttpAsyncRequestProducer requestProducer,
            HttpAsyncResponseConsumer<T> responseConsumer,
            HttpContext context,
            FutureCallback<T> callback);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 
这里会通过原来的请求信息生成一个requestProducer跟responseConsumer, 默认会调用HttpAsyncClient的InternalHttpAsyncClient的实现, 如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public <T> Future<T> execute(
            final HttpAsyncRequestProducer requestProducer,
            final HttpAsyncResponseConsumer<T> responseConsumer,
            final HttpContext context,
            final FutureCallback<T> callback) {
        final Status status = getStatus();
        Asserts.check(status == Status.ACTIVE, "Request cannot be executed; " +
                "I/O reactor status: %s", status);
        final BasicFuture<T> future = new BasicFuture<T>(callback);
        final HttpClientContext localcontext = HttpClientContext.adapt(
            context != null ? context : new BasicHttpContext());
        setupContext(localcontext);
        @SuppressWarnings("resource")
        final DefaultClientExchangeHandlerImpl<T> handler = new DefaultClientExchangeHandlerImpl<T>(
            this.log,
            requestProducer,
            responseConsumer,
            localcontext,
            future,
            this.connmgr,
            this.exec);
        try {
            handler.start(); // 请求开始
        } catch (final Exception ex) {
            handler.failed(ex);
        }
        return future;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 
 这里通过生成一个ExchangeHandler来实现请求开始, 查看 handler.start()
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public void start() throws HttpException, IOException {
        final HttpHost target = this.requestProducer.getTarget();
        final HttpRequest original = this.requestProducer.generateRequest();
        if (original instanceof HttpExecutionAware) {
            ((HttpExecutionAware) original).setCancellable(this);
        }
        this.exec.prepare(this.state, target, original); // 准备动作, 往state里设置各种状态
        requestConnection(); // 实际发送请求的地方
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 接着往下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
private void requestConnection() {
        if (this.log.isDebugEnabled()) {
            this.log.debug("[exchange: " + this.state.getId() + "] Request connection for " +
                this.state.getRoute());
        }
        discardConnection();
        this.state.setValidDuration(0);
        this.state.setNonReusable();
        this.state.setRouteEstablished(false);
        this.state.setRouteTracker(null);
        final HttpRoute route = this.state.getRoute();
        final Object userToken = this.localContext.getUserToken();
        final RequestConfig config = this.localContext.getRequestConfig();
        this.connmgr.requestConnection( // 此处调用ConenctionManager的requestConnection方法
                route,
                userToken,
                config.getConnectTimeout(),
                config.getConnectionRequestTimeout(),
                TimeUnit.MILLISECONDS,
                new FutureCallback<NHttpClientConnection>() {
                    public void completed(final NHttpClientConnection managedConn) {
                        connectionAllocated(managedConn);
                    }
                    public void failed(final Exception ex) {
                        connectionRequestFailed(ex);
                    }
                    public void cancelled() {
                        connectionRequestCancelled();
                    }
                });
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
再看NHttpClientConnectionManager下的
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/**
     * Returns a {@link Future} for a {@link NHttpClientConnection}.
     * <p/>
     * Please note that the consumer of that connection is responsible
     * for fully establishing the route the to the connection target
     * by calling {@link #startRoute(org.apache.http.nio.NHttpClientConnection,
     *   org.apache.http.conn.routing.HttpRoute,
     *   org.apache.http.protocol.HttpContext) startRoute} in order to start
     * the process of connection initialization, optionally calling
     * {@link #upgrade(org.apache.http.nio.NHttpClientConnection,
     *   org.apache.http.conn.routing.HttpRoute,
     *   org.apache.http.protocol.HttpContext) upgrade} method to upgrade
     * the connection after having executed <code>CONNECT</code> method to
     * all intermediate proxy hops and and finally calling
     * {@link #routeComplete(org.apache.http.nio.NHttpClientConnection,
     *   org.apache.http.conn.routing.HttpRoute,
     *   org.apache.http.protocol.HttpContext) routeComplete} to mark the route
     * as fully completed.
     *
     * @param route HTTP route of the requested connection.
     * @param state expected state of the connection or <code>null</code>
     *              if the connection is not expected to carry any state.
     * @param connectTimeout connect timeout.
     * @param connectionRequestTimeout  connection request timeout.
     * @param timeUnit time unit of the previous two timeout values.
     * @param callback future callback.
     */
    Future<NHttpClientConnection> requestConnection(
            HttpRoute route,
            Object state,
            long connectTimeout,
            long connectionRequestTimeout,
            TimeUnit timeUnit,
            FutureCallback<NHttpClientConnection> callback);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
它调用了PoolingNHttpClientConnectionManager的实现
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public Future<NHttpClientConnection> requestConnection(
            final HttpRoute route,
            final Object state,
            final long connectTimeout,
            final long leaseTimeout,
            final TimeUnit tunit,
            final FutureCallback<NHttpClientConnection> callback) {
        Args.notNull(route, "HTTP route");
        if (this.log.isDebugEnabled()) {
            this.log.debug("Connection request: " + format(route, state) + formatStats(route));
        }
        final BasicFuture<NHttpClientConnection> future = new BasicFuture<NHttpClientConnection>(callback);
        final HttpHost host;
        if (route.getProxyHost() != null) {
            host = route.getProxyHost();
        } else {
            host = route.getTargetHost();
        }
        final SchemeIOSessionStrategy sf = this.iosessionFactoryRegistry.lookup(
                host.getSchemeName());
        if (sf == null) {
            future.failed(new UnsupportedSchemeException(host.getSchemeName() +
                    " protocol is not supported"));
            return future;
        }
        this.pool.lease(route, state,
                connectTimeout, leaseTimeout, tunit != null ? tunit : TimeUnit.MILLISECONDS,
                new InternalPoolEntryCallback(future)); // 这里就是实际运用连接池的地方
        return future;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
看 AbstractNIOConnPool 的 lease 方法
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public Future<E> lease(
            final T route, final Object state,
            final long connectTimeout, final long leaseTimeout, final TimeUnit tunit,
            final FutureCallback<E> callback) {
        Args.notNull(route, "Route");
        Args.notNull(tunit, "Time unit");
        Asserts.check(!this.isShutDown.get(), "Connection pool shut down");
        final BasicFuture<E> future = new BasicFuture<E>(callback);
        this.lock.lock(); // 同步
        try {
            final long timeout = connectTimeout > 0 ? tunit.toMillis(connectTimeout) : 0;
            final LeaseRequest<T, C, E> request = new LeaseRequest<T, C, E>(route, state, timeout, leaseTimeout, future);
            final boolean completed = processPendingRequest(request); // 1) 获取连接的方法
            if (!request.isDone() && !completed) {  // 2) 因为连接池满而不能马上获得连接的的, 加入到一个leasing的LinkedList中, 他会在后续的某些操作中被取出来重新尝试连接发送请求
                this.leasingRequests.add(request);
            }
            if (request.isDone()) {  // 3) 已经完成连接动作(注意是连接动作完成, 不是请求完成获得响应, 这里的连接完成包括从连接池获取到连接, 或者是因为异常request被设置为fail)的请求, 加入到一个ConcurrentLinkedQueue中, 这个队列的唯一作用就是标记连接完成以后, 调用fireCallBack方法会从里面把这些连接完成的request做一遍回调处理
                this.completedRequests.add(request);
            }
        } finally {
            this.lock.unlock();
        }
        fireCallbacks();
        return future;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 
这里主要涉及到连接池 AbstractNIOConnPool 以及连接池下得实际存储连接的 RouteSpecificPool, 
然后开始分析连接流程
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
private boolean processPendingRequest(final LeaseRequest<T, C, E> request) {
        final T route = request.getRoute();
        final Object state = request.getState();
        final long deadline = request.getDeadline();
        final long now = System.currentTimeMillis();
        if (now > deadline) {
            request.failed(new TimeoutException());
            return false;
        }
        final RouteSpecificPool<T, C, E> pool = getPool(route);
        E entry;
        for (;;) { // 租借连接池连接
            entry = pool.getFree(state); // getFree即是从available中获取一个state匹配的连接
            if (entry == null) { // 没有可用连接退出循环
                break;
            }
            // 清除不可用连接
            if (entry.isClosed() || entry.isExpired(System.currentTimeMillis())) {
                entry.close();
                this.available.remove(entry);
                pool.free(entry, false);
            } else {
                break;
            }
        }
        if (entry != null) { // 找到连接退出
            this.available.remove(entry);
            this.leased.add(entry);
            request.completed(entry);
            onLease(entry);
            return true;
        }
        // 需要新连接的情况
        // New connection is needed
        final int maxPerRoute = getMax(route);
        // 已经分配的连接超出可分配限制
        // Shrink the pool prior to allocating a new connection
        final int excess = Math.max(0, pool.getAllocatedCount() + 1 - maxPerRoute);
        // 对连接池进行缩减, 将上次使用的连接关闭并删除, 直到超出的连接全被清除
        if (excess > 0) {
            for (int i = 0; i < excess; i++) {
                final E lastUsed = pool.getLastUsed(); // 这个方法是取到 available 里的最后一个连接, 也就是说会出现所有连接都被租借出去了的情况, 这样的话就相当于连接池满, 到下一步的 if (pool.getAllocatedCount() < maxPerRoute) 即会 false, 最后导致request进入 leasingRequest 列表
                if (lastUsed == null) {
                    break;
                }
                lastUsed.close();
                this.available.remove(lastUsed);
                pool.remove(lastUsed);
            }
        }
        // 已分配连接数 < 最大连接数限制, 开始新建
        if (pool.getAllocatedCount() < maxPerRoute) {
            // 总共被使用的数量等于 正在等待连接数 + 已经租借出去的连接数
            final int totalUsed = this.pending.size() + this.leased.size();
            final int freeCapacity = Math.max(this.maxTotal - totalUsed, 0);
            if (freeCapacity == 0) {
                return false;
            }
            // 需要注意的是pool里available不为空, 也有可能拿不到可用连接, 因为state不匹配
            final int totalAvailable = this.available.size();
            // 总的available > 连接空位时, 会随机选择最后一次使用的连接, 并把它关掉.. 没搞明白这一步是干嘛用的
            if (totalAvailable > freeCapacity - 1) {
                if (!this.available.isEmpty()) {
                    final E lastUsed = this.available.removeLast();
                    lastUsed.close();
                    final RouteSpecificPool<T, C, E> otherpool = getPool(lastUsed.getRoute());
                    otherpool.remove(lastUsed);
                }
            }
            // 创建连接监视器阶段, 创建了一个监时此次请求的监视对象 SessionRequest, 并调用selector的wakeup(), 出发实际的连接操作
            final SocketAddress localAddress;
            final SocketAddress remoteAddress;
            try {
                remoteAddress = this.addressResolver.resolveRemoteAddress(route);
                localAddress = this.addressResolver.resolveLocalAddress(route);
            } catch (final IOException ex) {
                request.failed(ex);
                return false;
            }
　　　　　　　// 重点关注一下这个connect方法
            final SessionRequest sessionRequest = this.ioreactor.connect(
                    remoteAddress, localAddress, route, this.sessionRequestCallback);
            final int timout = request.getConnectTimeout() < Integer.MAX_VALUE ?
                    (int) request.getConnectTimeout() : Integer.MAX_VALUE;
            sessionRequest.setConnectTimeout(timout);
            // 加入到总pending集合
            this.pending.add(sessionRequest);
            // 加入到route连接池pending集合
            pool.addPending(sessionRequest, request.getFuture());
            return true;
        } else {
            return false;
        }
    }
    // 检查最后一个完成的request的结果, 并设置future的状态
    private void fireCallbacks() {
        LeaseRequest<T, C, E> request;
        while ((request = this.completedRequests.poll()) != null) {
            final BasicFuture<E> future = request.getFuture();
            final Exception ex = request.getException();
            final E result = request.getResult();
            if (ex != null) {
                future.failed(ex);
            } else if (result != null) {
                future.completed(result);
            } else {
                future.cancel();
            }
        }
    }    
![复制代码](http://common.cnblogs.com/images/copycode.gif)
看看DefaultConnectingIOReactor的connect方法
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public SessionRequest connect(
            final SocketAddress remoteAddress,
            final SocketAddress localAddress,
            final Object attachment,
            final SessionRequestCallback callback) {
        Asserts.check(this.status.compareTo(IOReactorStatus.ACTIVE) <= 0,
            "I/O reactor has been shut down");
        final SessionRequestImpl sessionRequest = new SessionRequestImpl(
                remoteAddress, localAddress, attachment, callback);
        sessionRequest.setConnectTimeout(this.config.getConnectTimeout());
        this.requestQueue.add(sessionRequest);
        this.selector.wakeup(); // 去看看wakeup()以后会发生什么事情
        return sessionRequest;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
在AbstractMultiworkerIOReactor中有一个execute()方法
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 /**
     * Activates the main I/O reactor as well as all worker I/O reactors.
     * The I/O main reactor will start reacting to I/O events and triggering
     * notification methods. The worker I/O reactor in their turn will start
     * reacting to I/O events and dispatch I/O event notifications to the given
     * {@link IOEventDispatch} interface.
     * <p>
     * This method will enter the infinite I/O select loop on
     * the {@link Selector} instance associated with this I/O reactor and used
     * to manage creation of new I/O channels. Once a new I/O channel has been
     * created the processing of I/O events on that channel will be delegated
     * to one of the worker I/O reactors.
     * <p>
     * The method will remain blocked unto the I/O reactor is shut down or the
     * execution thread is interrupted.
     *
     * @see #processEvents(int)
     * @see #cancelRequests()
     *
     * @throws InterruptedIOException if the dispatch thread is interrupted.
     * @throws IOReactorException in case if a non-recoverable I/O error.
     */
    public void execute(
            final IOEventDispatch eventDispatch) throws InterruptedIOException, IOReactorException {
        Args.notNull(eventDispatch, "Event dispatcher");
        synchronized (this.statusLock) {
            if (this.status.compareTo(IOReactorStatus.SHUTDOWN_REQUEST) >= 0) {
                this.status = IOReactorStatus.SHUT_DOWN;
                this.statusLock.notifyAll();
                return;
            }
            Asserts.check(this.status.compareTo(IOReactorStatus.INACTIVE) == 0,
                    "Illegal state %s", this.status);
            this.status = IOReactorStatus.ACTIVE;
            // Start I/O dispatchers
            for (int i = 0; i < this.dispatchers.length; i++) {
                final BaseIOReactor dispatcher = new BaseIOReactor(this.selectTimeout, this.interestOpsQueueing);
                dispatcher.setExceptionHandler(exceptionHandler);
                this.dispatchers[i] = dispatcher;
            }
            for (int i = 0; i < this.workerCount; i++) {
                final BaseIOReactor dispatcher = this.dispatchers[i];
                this.workers[i] = new Worker(dispatcher, eventDispatch);
                this.threads[i] = this.threadFactory.newThread(this.workers[i]);
            }
        }
        try {
            // 启动所有worker线程, 连接的事情是交给 AbstractMultiworkerIOReactor 来做的, 但是连接成功后的事情则是交给 AbstractIOReactor Worker 线程来处理, 前者类似于 bossGroup, 后者类似于 workerGroup
            for (int i = 0; i < this.workerCount; i++) {
                if (this.status != IOReactorStatus.ACTIVE) {
                    return;
                }
                this.threads[i].start();
            }
            // 使用无限循环监听事件
            for (;;) {
                final int readyCount;
                try {
                    // 阻塞, 直到超时或者调用 wakeup()
                    readyCount = this.selector.select(this.selectTimeout);
                } catch (final InterruptedIOException ex) {
                    throw ex;
                } catch (final IOException ex) {
                    throw new IOReactorException("Unexpected selector failure", ex);
                }
                
                // 如果有需要处理的事件, 则进入processEvents流程, 实际的连接过程就在这里
                if (this.status.compareTo(IOReactorStatus.ACTIVE) == 0) {
                    processEvents(readyCount);
                }
                // Verify I/O dispatchers
                for (int i = 0; i < this.workerCount; i++) {
                    final Worker worker = this.workers[i];
                    final Exception ex = worker.getException();
                    if (ex != null) {
                        throw new IOReactorException(
                                "I/O dispatch worker terminated abnormally", ex);
                    }
                }
                if (this.status.compareTo(IOReactorStatus.ACTIVE) > 0) {
                    break;
                }
            }
        } catch (final ClosedSelectorException ex) {
            addExceptionEvent(ex);
        } catch (final IOReactorException ex) {
            if (ex.getCause() != null) {
                addExceptionEvent(ex.getCause());
            }
            throw ex;
        } finally {
            doShutdown();
            synchronized (this.statusLock) {
                this.status = IOReactorStatus.SHUT_DOWN;
                this.statusLock.notifyAll();
            }
        }
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
首次连接的时候, 触发的是 DefaultConnectingIOReactor 的 processEvents 方法
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    @Override
    protected void processEvents(final int readyCount) throws IOReactorException {
        processSessionRequests(); // 这里就是实际连接的地方
        if (readyCount > 0) {
            final Set<SelectionKey> selectedKeys = this.selector.selectedKeys();
            for (final SelectionKey key : selectedKeys) {
                
                processEvent(key);
            }
            selectedKeys.clear();
        }
        final long currentTime = System.currentTimeMillis();
        if ((currentTime - this.lastTimeoutCheck) >= this.selectTimeout) {
            this.lastTimeoutCheck = currentTime;
            final Set<SelectionKey> keys = this.selector.keys();
            processTimeouts(keys);
        }
    }
private void processSessionRequests() throws IOReactorException {
        SessionRequestImpl request;
        // wakeup 一次将队列的所有request处理(发起连接)掉
        while ((request = this.requestQueue.poll()) != null) {
            if (request.isCompleted()) {
                continue;
            }
            final SocketChannel socketChannel;
            try {
                socketChannel = SocketChannel.open();
            } catch (final IOException ex) {
                throw new IOReactorException("Failure opening socket", ex);
            }
            try {
                socketChannel.configureBlocking(false);
                validateAddress(request.getLocalAddress());
                validateAddress(request.getRemoteAddress());
                if (request.getLocalAddress() != null) {
                    final Socket sock = socketChannel.socket();
                    sock.setReuseAddress(this.config.isSoReuseAddress());
                    sock.bind(request.getLocalAddress());
                }
                prepareSocket(socketChannel.socket());
                final boolean connected = socketChannel.connect(request.getRemoteAddress());
                if (connected) { // 马上连接成功, 处理下一个
                    final ChannelEntry entry = new ChannelEntry(socketChannel, request);
                    addChannel(entry);
                    continue;
                }
            } catch (final IOException ex) {
                closeChannel(socketChannel);
                request.failed(ex);
                return;
            }
            // 还未连接成功, 则注册到selector, 等待connect事件的触发, 再用processEvent来处理
            final SessionRequestHandle requestHandle = new SessionRequestHandle(request);
            try {
                final SelectionKey key = socketChannel.register(this.selector, SelectionKey.OP_CONNECT,
                        requestHandle);
                request.setKey(key);
            } catch (final IOException ex) {
                closeChannel(socketChannel);
                throw new IOReactorException("Failure registering channel " +
                        "with the selector", ex);
            }
        }
    }
    // 这个方法是连接成功以后注册channel的方法
    private void processEvent(final SelectionKey key) {
        try {
            if (key.isConnectable()) {
                final SocketChannel channel = (SocketChannel) key.channel();
                // Get request handle
                final SessionRequestHandle requestHandle = (SessionRequestHandle) key.attachment();
                final SessionRequestImpl sessionRequest = requestHandle.getSessionRequest();
                // Finish connection process
                try {
                    channel.finishConnect();
                } catch (final IOException ex) {
                    sessionRequest.failed(ex);
                }
                key.cancel();
                key.attach(null);
                if (!sessionRequest.isCompleted()) {
                    // 注册新channel, 这些channel后来会被worker线程处理, 他们来进行io读写
                    addChannel(new ChannelEntry(channel, sessionRequest));
                } else {
                    try {
                        channel.close();
                    } catch (IOException ignore) {
                    }
                }
            }
        } catch (final CancelledKeyException ex) {
            final SessionRequestHandle requestHandle = (SessionRequestHandle) key.attachment();
            key.attach(null);
            if (requestHandle != null) {
                final SessionRequestImpl sessionRequest = requestHandle.getSessionRequest();
                if (sessionRequest != null) {
                    sessionRequest.cancel();
                }
            }
        }
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
接下来看连接成功后的IOReactor如何处理, 如下 BaseIOReactor 的 execute 方法
 
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 /**
     * Activates the I/O reactor. The I/O reactor will start reacting to
     * I/O events and triggering notification methods.
     * <p>
     * This method will enter the infinite I/O select loop on
     * the {@link Selector} instance associated with this I/O reactor.
     * <p>
     * The method will remain blocked unto the I/O reactor is shut down or the
     * execution thread is interrupted.
     *
     * @see #acceptable(SelectionKey)
     * @see #connectable(SelectionKey)
     * @see #readable(SelectionKey)
     * @see #writable(SelectionKey)
     * @see #timeoutCheck(SelectionKey, long)
     * @see #validate(Set)
     * @see #sessionCreated(SelectionKey, IOSession)
     * @see #sessionClosed(IOSession)
     *
     * @throws InterruptedIOException if the dispatch thread is interrupted.
     * @throws IOReactorException in case if a non-recoverable I/O error.
     */
    protected void execute() throws InterruptedIOException, IOReactorException {
        this.status = IOReactorStatus.ACTIVE;
        try {
            for (;;) {
                final int readyCount;
                try {
                    readyCount = this.selector.select(this.selectTimeout);
                } catch (final InterruptedIOException ex) {
                    throw ex;
                } catch (final IOException ex) {
                    throw new IOReactorException("Unexpected selector failure", ex);
                }
                if (this.status == IOReactorStatus.SHUT_DOWN) {
                    // Hard shut down. Exit select loop immediately
                    break;
                }
                if (this.status == IOReactorStatus.SHUTTING_DOWN) {
                    // Graceful shutdown in process
                    // Try to close things out nicely
                    closeSessions();
                    closeNewChannels();
                }
                // Process selected I/O events
                if (readyCount > 0) {
                    processEvents(this.selector.selectedKeys());
                }
                // Validate active channels
                validate(this.selector.keys());
                // Process closed sessions
                processClosedSessions();
                // If active process new channels
                if (this.status == IOReactorStatus.ACTIVE) {
                    processNewChannels();
                }
                // Exit select loop if graceful shutdown has been completed
                if (this.status.compareTo(IOReactorStatus.ACTIVE) > 0
                        && this.sessions.isEmpty()) {
                    break;
                }
                if (this.interestOpsQueueing) {
                    // process all pending interestOps() operations
                    processPendingInterestOps();
                }
            }
        } catch (final ClosedSelectorException ignore) {
        } finally {
            hardShutdown();
            synchronized (this.statusMutex) {
                this.statusMutex.notifyAll();
            }
        }
    }
    private void processEvents(final Set<SelectionKey> selectedKeys) {
        for (final SelectionKey key : selectedKeys) {
            processEvent(key);
        }
        selectedKeys.clear();
    }
    /**
     * Processes new event on the given selection key.
     *
     * @param key the selection key that triggered an event.
     */
    protected void processEvent(final SelectionKey key) {
        final IOSessionImpl session = (IOSessionImpl) key.attachment();
        try {
            if (key.isAcceptable()) {
                acceptable(key);
            }
            if (key.isConnectable()) {
                connectable(key);
            }
            if (key.isReadable()) {
                session.resetLastRead();
                readable(key);
            }
            if (key.isWritable()) {
                session.resetLastWrite();
                writable(key);
            }
        } catch (final CancelledKeyException ex) {
            queueClosedSession(session);
            key.attach(null);
        }
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
这个就跟AbstractMultiworkerIOReactor类似, 只不过两个人的兴趣集事件不太一样, 看看 AbstractIOReactor 的实现类 BaseIOReactor就知道了
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/**
     * This I/O reactor implementation does not react to the
     * {@link SelectionKey#OP_ACCEPT} event.
     * <p>
     * Super-classes can override this method to react to the event.
     */
    @Override
    protected void acceptable(final SelectionKey key) {
    }
    /**
     * This I/O reactor implementation does not react to the
     * {@link SelectionKey#OP_CONNECT} event.
     * <p>
     * Super-classes can override this method to react to the event.
     */
    @Override
    protected void connectable(final SelectionKey key) {
    }
    /**
     * Processes {@link SelectionKey#OP_READ} event on the given selection key.
     * This method dispatches the event notification to the
     * {@link IOEventDispatch#inputReady(IOSession)} method.
     */
    @Override
    protected void readable(final SelectionKey key) {
        final IOSession session = getSession(key);
        try {
            this.eventDispatch.inputReady(session);
            if (session.hasBufferedInput()) {
                this.bufferingSessions.add(session);
            }
        } catch (final CancelledKeyException ex) {
            queueClosedSession(session);
            key.attach(null);
        } catch (final RuntimeException ex) {
            handleRuntimeException(ex);
        }
    }
    /**
     * Processes {@link SelectionKey#OP_WRITE} event on the given selection key.
     * This method dispatches the event notification to the
     * {@link IOEventDispatch#outputReady(IOSession)} method.
     */
    @Override
    protected void writable(final SelectionKey key) {
        final IOSession session = getSession(key);
        try {
            this.eventDispatch.outputReady(session);
        } catch (final CancelledKeyException ex) {
            queueClosedSession(session);
            key.attach(null);
        } catch (final RuntimeException ex) {
            handleRuntimeException(ex);
        }
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
它实际上只会处理 read 跟 write 事件
 
这里特别注意一下 AbstractNIOConnPool requestComplete 并不是整个请求结束, 而是连接成功的意思, 看看他的调用的地方和触发的东西
最开始他是从 AbstractIOReactor 的processNewChannels中来的, 这个方法在execute里被触发
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
rivate void processNewChannels() throws IOReactorException {
        ChannelEntry entry;
        while ((entry = this.newChannels.poll()) != null) { // 记得上面连接成功后调用addChannel加到的这个队列, 现在取出来
            final SocketChannel channel;
            final SelectionKey key;
            try {
                channel = entry.getChannel();
                channel.configureBlocking(false);
                key = channel.register(this.selector, SelectionKey.OP_READ);
            } catch (final ClosedChannelException ex) {
                final SessionRequestImpl sessionRequest = entry.getSessionRequest();
                if (sessionRequest != null) {
                    sessionRequest.failed(ex);
                }
                return;
            } catch (final IOException ex) {
                throw new IOReactorException("Failure registering channel " +
                        "with the selector", ex);
            }
            final SessionClosedCallback sessionClosedCallback = new SessionClosedCallback() {
                public void sessionClosed(final IOSession session) {
                    queueClosedSession(session);
                }
            };
            InterestOpsCallback interestOpsCallback = null;
            if (this.interestOpsQueueing) {
                interestOpsCallback = new InterestOpsCallback() {
                    public void addInterestOps(final InterestOpEntry entry) {
                        queueInterestOps(entry);
                    }
                };
            }
            final IOSession session;
            try {
                session = new IOSessionImpl(key, interestOpsCallback, sessionClosedCallback);
                int timeout = 0;
                try {
                    timeout = channel.socket().getSoTimeout();
                } catch (final IOException ex) {
                    // Very unlikely to happen and is not fatal
                    // as the protocol layer is expected to overwrite
                    // this value anyways
                }
                session.setAttribute(IOSession.ATTACHMENT_KEY, entry.getAttachment());
                session.setSocketTimeout(timeout);
            } catch (final CancelledKeyException ex) {
                continue;
            }
            try {
                this.sessions.add(session);
                final SessionRequestImpl sessionRequest = entry.getSessionRequest();
                if (sessionRequest != null) {
                    // 就是在这里调用了completed, 最后进入连接池的completed方法
                    sessionRequest.completed(session);
                }
                key.attach(session);
                sessionCreated(key, session);
            } catch (final CancelledKeyException ex) {
                queueClosedSession(session);
                key.attach(null);
            }
        }
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
看看连接池 AbstractNIOConnPool 的completed 方法做了什么
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
protected void requestCompleted(final SessionRequest request) {
        if (this.isShutDown.get()) {
            return;
        }
        @SuppressWarnings("unchecked")
        final
        T route = (T) request.getAttachment();
        this.lock.lock();
        try {
            this.pending.remove(request); // 从peding列表中去掉这个连接
            final RouteSpecificPool<T, C, E> pool = getPool(route);
            final IOSession session = request.getSession();
            try {
                final C conn = this.connFactory.create(route, session);
                final E entry = pool.createEntry(request, conn);
                this.leased.add(entry); // 连接加入到被租借集合
                pool.completed(request, entry); // 调用 perRoute 连接池的complte
                onLease(entry); // 这个 onLease 就是设置了一下超时时间
            } catch (final IOException ex) {
                pool.failed(request, ex);
            }
        } finally {
            this.lock.unlock();
        }
        fireCallbacks();
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
主要做的事情就是设置了一下连接的sotimeout, 还有就是将连接从 pending列表移到了 lease 集合
最后, 来看看 连接是如何归还的
通过对AbstractNIOConnPool的release方法的跟踪, 最后找了还是在 BaseIOReactor 监听到 readable 时间的时候, 调用了HttpAsyncRequestExecutor的inputReady
方法
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public void inputReady(
            final NHttpClientConnection conn,
            final ContentDecoder decoder) throws IOException, HttpException {
        final State state = ensureNotNull(getState(conn));
        final HttpAsyncClientExchangeHandler handler = ensureNotNull(getHandler(conn));
        handler.consumeContent(decoder, conn);
        state.setResponseState(MessageState.BODY_STREAM);
        if (decoder.isCompleted()) { // 检测到内容已结束, 进入complete流程
            processResponse(conn, state, handler);
        }
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
## 请求完成阶段
当检测到response已经完了, 就会进入complete流程, 最后回到releaseConnection流程, 最后到达连接池的release
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public void release(final E entry, final boolean reusable) {
        if (entry == null) {
            return;
        }
        if (this.isShutDown.get()) {
            return;
        }
        this.lock.lock();
        try {
            if (this.leased.remove(entry)) { // 从租借集合中删除
                final RouteSpecificPool<T, C, E> pool = getPool(entry.getRoute());
                pool.free(entry, reusable); // 重新加入到pool的available中
                if (reusable) {
                    this.available.addFirst(entry); // 加入到available
                    onRelease(entry); // 重新这是soTimeout
                } else {
                    entry.close();
                }
                processNextPendingRequest(); // 处理下一个在leasing队列中等待的请求
            }
        } finally {
            this.lock.unlock();
        }
        fireCallbacks();
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
完
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
