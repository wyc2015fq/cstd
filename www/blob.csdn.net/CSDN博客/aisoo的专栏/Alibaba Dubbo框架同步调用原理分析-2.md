# Alibaba Dubbo框架同步调用原理分析-2 - aisoo的专栏 - CSDN博客
2012年12月12日 15:26:12[aisoo](https://me.csdn.net/aisoo)阅读数：9136
接上一篇，看一下Dubbo的相关代码
关键代码：
**com.taobao.remoting.impl.DefaultClient.java**
**//同步调用远程接口**
public Object invokeWithSync(Object appRequest, RequestControl control) throws RemotingException, InterruptedException {
        byte protocol = getProtocol(control);
        if (!TRConstants.isValidProtocol(protocol)) {
            throw new RemotingException("Invalid serialization protocol [" + protocol + "] on invokeWithSync.");
        }
        ResponseFuture future = invokeWithFuture(appRequest, control);
        return future.get(); ** //****获取结果时让当前线程等待，ResponseFuture其实就是前面说的callback**
}
public ResponseFuture **invokeWithFuture**(Object appRequest, RequestControl control) {
         byte protocol = getProtocol(control);
         long timeout = getTimeout(control);
         ConnectionRequest request = new ConnectionRequest(appRequest);
         request.setSerializeProtocol(protocol);
         Callback2FutureAdapter adapter = new Callback2FutureAdapter(request);
         connection.sendRequestWithCallback(request, adapter, timeout);
         return adapter;
}
**Callback2FutureAdapter implements ResponseFuture**
public Object get() throws RemotingException, InterruptedException {
synchronized (this) {  // 旋锁
    while (!isDone) {  **// 是否有结果了**
wait();** //没结果是释放锁，让当前线程处于等待状态**
    }
}
if (errorCode == TRConstants.RESULT_TIMEOUT) {
    throw new TimeoutException("Wait response timeout, request["
    + connectionRequest.getAppRequest() + "].");
}
else if (errorCode > 0) {
    throw new RemotingException(errorMsg);
}
else {
    return appResp;
}
}
**客户端收到服务端结果后，回调时相关方法，即设置isDone = true并notifyAll()**
public void handleResponse(Object _appResponse) {
         appResp = _appResponse; **//****将远程调用结果设置到callback中来**
         setDone();
}
public void onRemotingException(int _errorType, String _errorMsg) {
         errorCode = _errorType;
         errorMsg = _errorMsg;
         setDone();
}
private void setDone() {
         isDone = true;
         synchronized (this) { **//****获取锁，因为前面wait()已经释放了callback的锁了**
             notifyAll();** // ****唤醒处于等待的线程**
         }
}
**com.taobao.remoting.impl.DefaultConnection.java**
**// 用来存放请求和回调的MAP**
private final ConcurrentHashMap<Long, Object[]> requestResidents;
**//发送消息出去**
void sendRequestWithCallback(ConnectionRequest connRequest, ResponseCallback callback, long timeoutMs) {
         long requestId = connRequest.getId();
         long waitBegin = System.currentTimeMillis();
         long waitEnd = waitBegin + timeoutMs;
         Object[] queue = new Object[4];
         int idx = 0;
         queue[idx++] = waitEnd;
         queue[idx++] = waitBegin;   //用于记录日志
         queue[idx++] = connRequest; //用于记录日志
         queue[idx++] = callback;
requestResidents.put(requestId, queue); // 记录响应队列
write(connRequest);
         // 埋点记录等待响应的Map的大小
         StatLog.addStat("TBRemoting-ResponseQueues", "size", requestResidents.size(),
                   1L);
}
public void write(final Object connectionMsg) {
**//mina里的IoSession.write()发送消息**
         WriteFuture writeFuture = ioSession.write(connectionMsg);
         // 注册FutureListener，当请求发送失败后，能够立即做出响应
         writeFuture.addListener(new MsgWrittenListener(this, connectionMsg));
}
/**
* 在得到响应后，删除对应的请求队列，并执行回调
* 调用者：MINA线程
*/
public void putResponse(final ConnectionResponse connResp) {
         final long requestId = connResp.getRequestId();
         Object[] queue = requestResidents.remove(requestId);
         if (null == queue) {
             Object appResp = connResp.getAppResponse();
             String appRespClazz = (null == appResp) ? "null" : appResp.getClass().getName();
             StringBuilder sb = new StringBuilder();
             sb.append("Not found response receiver for requestId=[").append(requestId).append("],");
             sb.append("from [").append(connResp.getHost()).append("],");
             sb.append("response type [").append(appRespClazz).append("].");
             LOGGER.warn(sb.toString());
             return;
         }
         int idx = 0;
         idx++;
         long waitBegin = (Long) queue[idx++];
         ConnectionRequest connRequest = (ConnectionRequest) queue[idx++];
         ResponseCallback callback = (ResponseCallback) queue[idx++];
         // ** 把回调任务交给业务提供的线程池执行 **
         Executor callbackExecutor = callback.getExecutor();
         callbackExecutor.execute(new CallbackExecutorTask(connResp, callback));
         long duration = System.currentTimeMillis() - waitBegin; // 实际读响应时间
         logIfResponseError(connResp, duration, connRequest.getAppRequest());
}
**CallbackExecutorTask**
static private class CallbackExecutorTask implements Runnable {
         final ConnectionResponse resp;
         final ResponseCallback callback;
         final Thread createThread;
         CallbackExecutorTask(ConnectionResponse _resp, ResponseCallback _cb) {
             resp = _resp;
             callback = _cb;
             createThread = Thread.currentThread();
         }
         public void run() {
             // 预防这种情况：业务提供的Executor，让调用者线程来执行任务
             if (createThread == Thread.currentThread()
                       && callback.getExecutor() != DIYExecutor.getInstance()) {
                   StringBuilder sb = new StringBuilder();
                   sb.append("The network callback task [" + resp.getRequestId() + "] cancelled, cause:");
                   sb.append("Can not callback task on the network io thhread.");
                   LOGGER.warn(sb.toString());
                   return;
             }
             if (TRConstants.RESULT_SUCCESS == resp.getResult()) {
callback.handleResponse(resp.getAppResponse());**//****设置调用结果**
             }
             else {
callback.onRemotingException(resp.getResult(), resp
                            .getErrorMsg());**//****处理调用异常**
             }
         }
}
**另外：**
**1，服务端在处理客户端的消息，然后再处理时，使用了线程池来并行处理，不用一个一个消息的处理**
**同样，客户端接收到服务端的消息，也是使用线程池来处理消息，再回调**
