# Http请求连接池 - HttpClient 的 PoolingHttpClientConnectionManager - z69183787的专栏 - CSDN博客
2017年09月13日 11:51:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2050
个人分类：[Httpclient-简介																[Httpclient-连接池](https://blog.csdn.net/z69183787/article/category/7424963)](https://blog.csdn.net/z69183787/article/category/2185975)
[http://blog.csdn.net/catoop/article/details/50352334](http://blog.csdn.net/catoop/article/details/50352334)
[http://sharong.iteye.com/blog/2250777](http://sharong.iteye.com/blog/2250777)
只有一个httpclient的实例，你可以看看CloseableHttpClient和PoolingHttpClientConnectionManager的源码,你会发现httpclient实例通过execute执行get或post获取连接的时候，会通过实例关联的connectionManager的connect()来建立连接，这里的connectionManager是一个连接池的实现, 因此connect方法调用的是PoolingHttpClientConnectionManager重写的实现，这就是一个连接池建立连接的实现，具体还是看源码吧
HttpClient 3.x,4.x都提供http连接池管理器,当使用了请求连接池管理器（比如PoolingHttpClientConnectionManager）后，HttpClient就可以同时执行多个线程的请求了。 
hc3.x和4.x的早期版本,提供了PoolingClientConnectionManager,DefaultHttpClient等类来实现http连接池,但这些类在4.3.x版本之后大部分就已经过时,本文使用4.3.x提供的最新的PoolingHttpClientConnectionManager等类进行http连接池的实现. 
废话不多说,下面是全部代码: 
Java代码  ![收藏代码](http://sharong.iteye.com/images/icon_star.png)
- publicclass PoolTest {  
- privatestaticvoid config(HttpRequestBase httpRequestBase) {  
-         httpRequestBase.setHeader("User-Agent", "Mozilla/5.0");  
-         httpRequestBase.setHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");  
-         httpRequestBase.setHeader("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3");//"en-US,en;q=0.5");
-         httpRequestBase.setHeader("Accept-Charset", "ISO-8859-1,utf-8,gbk,gb2312;q=0.7,*;q=0.7");  
- 
- // 配置请求的超时设置
-         RequestConfig requestConfig = RequestConfig.custom()  
-                 .setConnectionRequestTimeout(3000)  
-                 .setConnectTimeout(3000)  
-                 .setSocketTimeout(3000)  
-                 .build();  
-         httpRequestBase.setConfig(requestConfig);         
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         ConnectionSocketFactory plainsf = PlainConnectionSocketFactory.getSocketFactory();  
-         LayeredConnectionSocketFactory sslsf = SSLConnectionSocketFactory.getSocketFactory();  
-         Registry<ConnectionSocketFactory> registry = RegistryBuilder.<ConnectionSocketFactory>create()  
-                 .register("http", plainsf)  
-                 .register("https", sslsf)  
-                 .build();  
- 
-         PoolingHttpClientConnectionManager cm = new PoolingHttpClientConnectionManager(registry);  
- // 将最大连接数增加到200
-         cm.setMaxTotal(200);  
- // 将每个路由基础的连接增加到20
-         cm.setDefaultMaxPerRoute(20);  
- 
- // 将目标主机的最大连接数增加到50
-         HttpHost localhost = new HttpHost("http://blog.csdn.net/gaolu",80);  
-         cm.setMaxPerRoute(new HttpRoute(localhost), 50);  
- 
- //请求重试处理
-         HttpRequestRetryHandler httpRequestRetryHandler = new HttpRequestRetryHandler() {  
- publicboolean retryRequest(IOException exception,int executionCount, HttpContext context) {  
- if (executionCount >= 5) {// 如果已经重试了5次，就放弃                   
- returnfalse;  
-                 }  
- if (exception instanceof NoHttpResponseException) {// 如果服务器丢掉了连接，那么就重试                  
- returntrue;  
-                 }  
- if (exception instanceof SSLHandshakeException) {// 不要重试SSL握手异常                 
- returnfalse;  
-                 }                 
- if (exception instanceof InterruptedIOException) {// 超时                 
- returnfalse;  
-                 }  
- if (exception instanceof UnknownHostException) {// 目标服务器不可达                 
- returnfalse;  
-                 }  
- if (exception instanceof ConnectTimeoutException) {// 连接被拒绝                 
- returnfalse;  
-                 }  
- if (exception instanceof SSLException) {// ssl握手异常                  
- returnfalse;  
-                 }  
- 
-                 HttpClientContext clientContext = HttpClientContext.adapt(context);  
-                 HttpRequest request = clientContext.getRequest();  
- // 如果请求是幂等的，就再次尝试
- if (!(request instanceof HttpEntityEnclosingRequest)) {                   
- returntrue;  
-                 }  
- returnfalse;  
-             }  
-         };    
- 
-         CloseableHttpClient httpClient = HttpClients.custom()  
-                 .setConnectionManager(cm)  
-                 .setRetryHandler(httpRequestRetryHandler)  
-                 .build();  
- 
- // URL列表数组
-         String[] urisToGet = {        
- "http://blog.csdn.net/gaolu/article/details/48466059",  
- "http://blog.csdn.net/gaolu/article/details/48243103",  
- "http://blog.csdn.net/gaolu/article/details/47656987",  
- "http://blog.csdn.net/gaolu/article/details/47055029",  
- 
- "http://blog.csdn.net/gaolu/article/details/46400883",  
- "http://blog.csdn.net/gaolu/article/details/46359127",  
- "http://blog.csdn.net/gaolu/article/details/46224821",  
- "http://blog.csdn.net/gaolu/article/details/45305769",  
- 
- "http://blog.csdn.net/gaolu/article/details/43701763",  
- "http://blog.csdn.net/gaolu/article/details/43195449",  
- "http://blog.csdn.net/gaolu/article/details/42915521",  
- "http://blog.csdn.net/gaolu/article/details/41802319",  
- 
- "http://blog.csdn.net/gaolu/article/details/41045233",  
- "http://blog.csdn.net/gaolu/article/details/40395425",  
- "http://blog.csdn.net/gaolu/article/details/40047065",  
- "http://blog.csdn.net/gaolu/article/details/39891877",  
- 
- "http://blog.csdn.net/gaolu/article/details/39499073",  
- "http://blog.csdn.net/gaolu/article/details/39314327",  
- "http://blog.csdn.net/gaolu/article/details/38820809",  
- "http://blog.csdn.net/gaolu/article/details/38439375",  
-         };  
- 
- long start = System.currentTimeMillis();          
- try {  
- int pagecount = urisToGet.length;  
-             ExecutorService executors = Executors.newFixedThreadPool(pagecount);  
-             CountDownLatch countDownLatch = new CountDownLatch(pagecount);        
- for(int i = 0; i< pagecount;i++){  
-                 HttpGet httpget = new HttpGet(urisToGet[i]);  
-                 config(httpget);  
- 
- //启动线程抓取
-                 executors.execute(new GetRunnable(httpClient,httpget,countDownLatch));  
-             }  
- 
-             countDownLatch.await();  
-             executors.shutdown();  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         } finally {  
-             System.out.println("线程" + Thread.currentThread().getName() + "," + System.currentTimeMillis() + ", 所有线程已完成，开始进入下一步！");  
-         }         
- 
- long end = System.currentTimeMillis();  
-         System.out.println("consume -> " + (end - start));  
-     }  
- 
- staticclass GetRunnable implements Runnable {  
- private CountDownLatch countDownLatch;  
- privatefinal CloseableHttpClient httpClient;  
- privatefinal HttpGet httpget;  
- 
- public GetRunnable(CloseableHttpClient httpClient, HttpGet httpget, CountDownLatch countDownLatch){  
- this.httpClient = httpClient;  
- this.httpget = httpget;  
- 
- this.countDownLatch = countDownLatch;  
-         }  
- 
- @Override
- publicvoid run() {  
-             CloseableHttpResponse response = null;  
- try {  
-                 response = httpClient.execute(httpget,HttpClientContext.create());  
-                 HttpEntity entity = response.getEntity();  
-                 System.out.println(EntityUtils.toString(entity, "utf-8")) ;  
-                 EntityUtils.consume(entity);  
-             } catch (IOException e) {  
-                 e.printStackTrace();  
-             } finally {  
-                 countDownLatch.countDown();  
- 
- try {  
- if(response != null)  
-                         response.close();  
-                 } catch (IOException e) {  
-                     e.printStackTrace();  
-                 }  
-             }  
-         }  
-     }     
- }  
主要参考文档: 
http://free0007.iteye.com/blog/2012308 
一开始我是使用传统的 HttpURLConnection 来做网络请求的，查了很多资料，有不少说 HttpURLConnection 效率高的。可是经过我修改实现方法后，HttpClient 连接池版本的网络请求相对比较稳定。这也说明，我们并不请尽信他人解说，凡事还是要寻找适合自己的方法，真正的解决自己的问题，才是王道。
===========================================
在使用 HttpURLConnection 的时候，大并发对外做网络请求的时候，前期请求耗时还好，后面耗时越来越高。下面是我之前的实现代码：
```
@Deprecated
    protected JSONObject callRestfulOld(String url, Map<String, Object> params) 
    {
        String temp;
        String ret="";
        JSONObject jsonRet=null;
        String sign = generateSign("POST", url, params);// 对参数进行加密签名
        if(sign.isEmpty()) return new JSONObject("{\"ret_code\":-1,\"err_msg\":\"generateSign error\"}");
        params.put("sign", sign);
        try{
            URL u = new URL(url);
            HttpURLConnection conn = (HttpURLConnection)u.openConnection();
            conn.setRequestMethod("POST");
            conn.setConnectTimeout(10000);
            conn.setDoOutput(true);
            conn.setDoInput(true);
            conn.setUseCaches(false);
            StringBuffer param = new StringBuffer();
            for (String key: params.keySet())
            {
                param.append(key).append("=").append(URLEncoder.encode(params.get(key).toString(), "UTF-8")).append("&");
            }
            conn.getOutputStream().write(param.toString().getBytes("UTF-8"));
            //System.out.println(param);
            conn.getOutputStream().flush();
            conn.getOutputStream().close();
            InputStreamReader isr = new InputStreamReader(conn.getInputStream());  
            BufferedReader br = new BufferedReader(isr);  
            while((temp = br.readLine()) != null){  
                ret += temp;  
            }     
            br.close();  
            isr.close();
            conn.disconnect();
            //System.out.println(ret);
            jsonRet = new JSONObject(ret);
        } catch(java.net.SocketTimeoutException e) {
            //e.printStackTrace();
            jsonRet = new JSONObject("{\"ret_code\":-1,\"err_msg\":\"call restful timeout\"}");
        } catch(Exception e) {
            //e.printStackTrace();
            jsonRet = new JSONObject("{\"ret_code\":-1,\"err_msg\":\"call restful error\"}");
        }
        return jsonRet;
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
（完）
