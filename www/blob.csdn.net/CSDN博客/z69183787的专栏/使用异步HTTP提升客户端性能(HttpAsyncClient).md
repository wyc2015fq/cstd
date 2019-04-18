# 使用异步HTTP提升客户端性能(HttpAsyncClient) - z69183787的专栏 - CSDN博客
2016年11月03日 17:45:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4678
大家都知道，应用层的网络模型有同步、异步之分。
同步，意为着线程阻塞，只有等本次请求全部都完成了，才能进行下一次请求。
异步，好处是不阻塞当前线程，可以“万箭齐发”的将所有请求塞入缓冲区，然后谁的请求先完成就处理谁。
大家也注意到了，同步模式阻塞的只是“线程”。实际上，在异步模式流行之前，人们也经常用多线程的方式处理并发请求。然而，随着数据规模的不断加大，线程开销所带来的CPU、内存剧增，因此这种方法的应用比较有限。
近几年来，随着异步处理方案在node.js、Nginx等系统中的成功应用，异步模式的到了越来越多的关注。另外提一句：客户端与服务器端的异步处理是相互透明的，即允许客户端采用同步而服务器端采用异步。只是一般来说，异步的处理比同步要复杂许多。
下面回到实际问题上。
在近日的工作中，需要从Hadoop Job中调用一个Http计算服务以完成一些处理工作。我们使用了经典的HttpClient 3.x进行了实现。在一个HDFS文件分片上，性能数据大致是这样的：171237个文档、耗时305076ms。
备注：由于我们的Job跑在Hadoop上，在未来是会有N个Mapper同时运行，因此没有采用多线程的处理方式。
看看上面的数据，乍一看似乎还可以：平均每个文档的处理只需要1.8毫秒。然而从整个Map的角度来看，调用Http服务已经成为了整个Job的瓶颈，有必要进行一些优化。
在HttpClient进化到4.x后，官方提供了基于nio的异步版本：HttpAsyncClient。
这个异步版本的客户端，借助了Java并发库和nio进行封装，提供了非常方便的调用方式。
我们来看一下异步的代码：
```java
public void test2() throws InterruptedException, ExecutionException,
            IOException {
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
        // Start the client
        httpclient.start();
 
        // Execute 100 request in async
        final HttpGet request = new HttpGet(
                "http://xxxx");
        request.setHeader("Connection", "close");
        List<Future<HttpResponse>> respList = new LinkedList<Future<HttpResponse>>();
        for (int i = 0; i < 50; i++) {
            respList.add(httpclient.execute(request, null));
        }
 
        // Print response code
        for (Future<HttpResponse> response : respList) {
            response.get().getStatusLine();
            // System.out.println(response.get().getStatusLine());
        }
 
        httpclient.close();
    }
```
再来看一下同步的：
```java
public void test1() throws ClientProtocolException, IOException {
        CloseableHttpClient httpclient = HttpClients.createDefault();
 
        // Execute 500 request in async
        for (int i = 0; i < 50; i++) {
            HttpGet request = new HttpGet(
                    "http://xxxx");
            request.setHeader("Connection", "close");
            CloseableHttpResponse response = httpclient.execute(request);
            // System.out.println(response.getStatusLine());
            response.getStatusLine();
            response.close();
        }
 
        httpclient.close();
    }
```
不难发现，异步的代码使用了Future，使得最终的处理异常非常简单。
备注：我这里偷懒没有使用countdown latcher，所以future.get()实际还是会阻塞，但是发送http请求的client.execute阶段是非阻塞的。
下面来测一下性能。
我们采用了与Job中几乎相同的配置：每次batch发起50次请求，共50个batch。
结果如下：
```
async 2879 ms
sync 4190 ms
```
使用异步请求的方式，比同步的时间节约了31%！
当然，尽管使用异步可以提升客户端调用的性能，但实际上是以提升并发为代价的，也就是latency和qps的关系。
换句话说，客户端异步没问题，但服务器端的性能必须跟的上，在我们的系统中，会通过控制batch的数量以及同时并发的mapper数量限制并发，以防止压垮服务器：-）
====2014.11.13 更新====
昨天忘记写如何获取返回的正文了，实际还是用Future返回的，补充如下：
```
HttpResponse resp = respFuture.get();
InputStream input = resp.getEntity().getContent();
```
接下来，如何读取input的content就不再贴代码了。用IOUtil或者裸写BufferedInputStream/ByteArrayOutputStream都可以。
此外再补充下，实际线上效果比上面测试的要好，客户端大约节省了62%的时间开销。
