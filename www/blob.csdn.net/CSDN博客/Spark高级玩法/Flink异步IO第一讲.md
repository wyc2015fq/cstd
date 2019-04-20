# Flink异步IO第一讲 - Spark高级玩法 - CSDN博客
2018年11月06日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：606
Async I/O 是阿里巴巴贡献给社区的一个呼声非常高的特性，于1.2版本引入。主要目的是为了解决与外部系统交互时网络延迟成为了系统瓶颈的问题。
对于实时处理，当需要使用外部存储数据染色的时候，需要小心对待，不能让与外部系统之间的交互延迟对流处理的整个工作进度起决定性的影响。
在mapfunction等算子里访问外部存储，实际上该交互过程是同步的：比如请求a发送到数据库，那么mapfunction会一直等待响应。在很多案例中，这个等待过程是非常浪费函数时间的。
与数据库异步交互，意味着单个函数实例可以并发处理很多请求，同时并发接收响应。那么，等待时间由于发送其它请求和接收其它响应，被重复使用而节省了。至少，等待时间在多个请求上被摊销。这就使得很多使用案例具有更高的吞吐量。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW7W9cIicVC0bNvE3wHbYuqencwXlAE7yypOIF0rlVqeWf9P4LZibrUZvaBbUezVI2bg6y8pmibAkZRA/640)
注意：通过增加MapFunction的到一个较大的并行度也是可以改善吞吐量的，但是这就意味着更高的资源开销：更多的MapFunction实例意味着更多的task，线程，flink内部网络连接，数据库的链接，缓存，更多内部状态开销。
**1. 前提**
正确的实现flink的异步IO功能，需要所连接的数据库支持异步客户端。幸运的是很多流行的数据库支持这样的客户端。
假如没有异步客户端，也可以创建多个同步客户端，放到线程池里，使用线程池来完成异步功能。当然，该种方式相对于异步客户端更低效。
**2. 异步IO API**
flink异步IO的API支持用户在data stream中使用异步请求客户端。API自身处理与数据流的整合，消息顺序，时间时间，容错等。
假如有目标数据库的异步客户端，使用异步IO，需要实现一下三步：
- 
实现AsyncFunction，该函数实现了请求分发的功能。
- 
一个callback回调，该函数取回操作的结果，然后传递给ResultFuture。
- 
对DataStream使用异步IO操作。
下面的代码，生名了一个基本模板：
// This example implements the asynchronous request and callback with Futures that have the
// interface of Java 8's futures (which is the same one followed by Flink's Future)
/**
 * An implementation of the 'AsyncFunction' that sends requests and sets the callback.
 */
class AsyncDatabaseRequest extends RichAsyncFunction<String, Tuple2<String, String>> {
/** The database specific client that can issue concurrent requests with callbacks */
private transient DatabaseClient client;
@Override
publicvoid open(Configuration parameters) throws Exception {
        client = new DatabaseClient(host, post, credentials);
    }
@Override
publicvoid close() throws Exception {
        client.close();
    }
@Override
publicvoid asyncInvoke(String key, final ResultFuture<Tuple2<String, String>> resultFuture) throws Exception {
// issue the asynchronous request, receive a future for result
        final Future<String> result = client.query(key);
// set the callback to be executed once the request by the client is complete
// the callback simply forwards the result to the result future
        CompletableFuture.supplyAsync(new Supplier<String>() {
@Override
publicStringget() {
try {
return result.get();
                } catch (InterruptedException | ExecutionException e) {
// Normally handled explicitly.
returnnull;
                }
            }
        }).thenAccept( (String dbResult) -> {
            resultFuture.complete(Collections.singleton(new Tuple2<>(key, dbResult)));
        });
    }
}
// create the original stream
DataStream<String> stream = ...;
// apply the async I/O transformation
DataStream<Tuple2<String, String>> resultStream =
    AsyncDataStream.unorderedWait(stream, new AsyncDatabaseRequest(), 1000, TimeUnit.MILLISECONDS, 100);
**重要提示：**
第一次调用 ResultFuture.complete的时候 ResultFuture就会完成。所有后续的complete调用都会被忽略。
下面也有两个参数需要注意一下：
**Timeout**
异步IO请求被视为失败的超时时间，超过该时间异步请求就算失败。该参数主要是为了剔除死掉或者失败的请求。
**Capacity**
该参数定义了同时最多有多少个异步请求在处理。即使异步IO的方式会导致更高的吞吐量，但是对于实时应用来说该操作也是一个瓶颈。限制并发请求数，算子不会积压过多的未处理请求，但是一旦超过容量的显示会触发背压。
**3. 超时处理**
当一个异步IO请求多次超时，默认情况下会抛出一个异常，然后重启job。如果想处理超时，可以覆盖AsyncFunction#timeout方法。
**4. 结果的顺序**
AsyncFunction发起的并发请求完成的顺序是不可预期的。为了控制结果发送的顺序，flink提供了两种模式：
**1). Unordered**
结果记录在异步请求结束后立刻发送。流中的数据在经过该异步IO操作后顺序就和以前不一样了。当使用处理时间作为基础时间特性的时候，该方式具有极低的延迟和极低的负载。调用方式
AsyncDataStream.unorderedWait(...)
**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW7W9cIicVC0bNvE3wHbYuqeo0bCxiaDgWG8LrEY3UbpE0p55AyAIzc1T3ToQWUbWD6DZhpWpgv4oWA/640)**
（处理时间无序图）
**2).  Ordered**
该种方式流的顺序会被保留。结果记录发送的顺序和异步请求被触发的顺序一样，该顺序就是愿意流中事件的顺序。为了实现该目标，操作算子会在该结果记录之前的记录为发送之前缓存该记录。这往往会引入额外的延迟和一些Checkpoint负载，因为相比于无序模式结果记录会保存在Checkpoint状态内部较长的时间。调用方式
AsyncDataStream.orderedWait(...)
**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW7W9cIicVC0bNvE3wHbYuqeibibITEYQQUT9y0VjsXcJliaxvXqOsticNJFtjDcZnMkCsbu3Oib3Qby7Rw/640)**
**5. 事件时间**
当使用事件时间的时候，异步IO操作也会正确的处理watermark机制。这就意味着两种order模式的具体操作如下：
**1). Unordered**
watermark不会超过记录，反之亦然，意味着watermark建立了一个order边界。记录仅会在两个watermark之间无序发射。watermark之后的记录仅会在watermark发送之后发送。watermark也仅会在该watermark之前的所有记录发射完成之后发送。
这就意味着在存在watermark的情况下，无序模式引入了一些与有序模式相同的延迟和管理开销。开销的大小取决于watermark的频率。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW7W9cIicVC0bNvE3wHbYuqebWgiad2M3Qyz0dl55fyNibVrNqBVUshHQ8ibJ8Gnj7rGJ4Dh5GZ0mtDsw/640)
（事件时间无序图）
**2). Ordered**
watermark的顺序就如记录的顺序一样被保存。与处理时间相比，开销没有显著变化。
请记住，注入时间 Ingestion Time是基于源处理时间自动生成的watermark事件时间的特殊情况。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW7W9cIicVC0bNvE3wHbYuqeibibITEYQQUT9y0VjsXcJliaxvXqOsticNJFtjDcZnMkCsbu3Oib3Qby7Rw/640)
**6. 容错担保**
异步IO操作提供了仅一次处理的容错担保。它会将在传出的异步IO请求保存于Checkpoint，然后故障恢复的时候从Checkpoint中恢复这些请求。
详细的案例公众号回复 **异步** 即可得到详细的案例。
**推荐阅读：**
[推荐两个不错的flink项目](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485652&idx=1&sn=170ece64334979fc5ea5fc5628a33759&chksm=9f38ebfca84f62ea0c1aaaeb8410ec15aec31b1122558662175aa70a2d953f9a3f0e6e904c8f&scene=21#wechat_redirect)
[重要 : 优化flink的四种方式](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485344&idx=1&sn=cde8915c6983dcfe541043cd6db8dbe6&chksm=9f38e488a84f6d9eb7b961a2c4fa05f2bd85fa82df81e4d8e56d0c22d86ff413a0baaacea01e&scene=21#wechat_redirect)
[flink 有状态udf 引起血案一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485723&idx=1&sn=6900dfd8e86318b13deabb247d8f83bc&chksm=9f38ea33a84f6325d62df7f984018bf5f16b18f185d83a5c6dd4ba91808356eb5e69183234df&scene=21#wechat_redirect)
[flink超越Spark的Checkpoint机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485181&idx=1&sn=05623930a6bcb09596d1a252d440fd4e&chksm=9f38e5d5a84f6cc3c56ae158a005dbe9122f16a6d27782a682bf1c9c5f1ac9f022d75c89a95b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
