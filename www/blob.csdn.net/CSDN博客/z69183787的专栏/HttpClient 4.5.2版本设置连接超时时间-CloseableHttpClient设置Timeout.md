# HttpClient 4.5.2版本设置连接超时时间-CloseableHttpClient设置Timeout - z69183787的专栏 - CSDN博客
2017年09月11日 10:20:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：943
HttpClient  4.5版本设置连接超时时间-CloseableHttpClient设置Timeout(区别于4.3.2)
HttpClient升级到4.5版本后，API有很多变化，HttpClient 4之后，API一直没有太稳定，我感觉4.5版本抽象后，很多API应该快稳定了。
       使用HttpClient，一般都需要设置连接超时时间和获取数据超时时间。这两个参数很重要，目的是为了防止访问其他http时，由于超时导致自己的应用受影响。
       4.5版本中，这两个参数的设置都抽象到了RequestConfig中，由相应的Builder构建，具体的例子如下：
```java
CloseableHttpClient httpclient = HttpClients.createDefault();  
HttpGet httpGet = new HttpGet("http://stackoverflow.com/");  
RequestConfig requestConfig = RequestConfig.custom()  
        .setConnectTimeout(5000).setConnectionRequestTimeout(1000)  
        .setSocketTimeout(5000).build();  
httpGet.setConfig(requestConfig);  
CloseableHttpResponse response = httpclient.execute(httpGet);  
System.out.println("得到的结果:" + response.getStatusLine());//得到请求结果  
HttpEntity entity = response.getEntity();//得到请求回来的数据
```
setConnectTimeout：设置连接超时时间，单位毫秒。
setConnectionRequestTimeout：设置从connect Manager获取Connection 超时时间，单位毫秒。这个属性是新加的属性，因为目前版本是可以共享连接池的。
setSocketTimeout：请求获取数据的超时时间，单位毫秒。 如果访问一个接口，多少时间内无法返回数据，就直接放弃此次调用。
===========================================
昨天遇到一个问题需要设置CloseableHttpClient的超时时间，查了官方文档如下。
新建一个RequestConfig：
```
RequestConfig defaultRequestConfig = RequestConfig.custom()
    .setSocketTimeout(5000)
    .setConnectTimeout(5000)
    .setConnectionRequestTimeout(5000)
    .setStaleConnectionCheckEnabled(true)
    .build();
```
这个超时可以设置为客户端级别，作为所有请求的默认值：
```
CloseableHttpClient httpclient = HttpClients.custom()
    .setDefaultRequestConfig(defaultRequestConfig)
    .build();
```
Request不会继承客户端级别的请求配置，所以在自定义Request的时候，需要将客户端的默认配置拷贝过去：
```
HttpGet httpget = new HttpGet("http://www.apache.org/");
RequestConfig requestConfig = RequestConfig.copy(defaultRequestConfig)
    .setProxy(new HttpHost("myotherproxy", 8080))
    .build();
httpget.setConfig(requestConfig);
```
[伦理片](http://www.dotdy.com/)http://www.dotdy.com/
4.3版本的超时是这样的：
public static String httpPost(String url, String jsonString) {
    // 设置HTTP请求参数
String result = null;
    HttpClient httpClient = new DefaultHttpClient();
    HttpPost httpPost = new HttpPost(url);
    try {
        httpClient.getParams().setParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, 10000);//设置请求超时时间 10s
StringEntity entity = new StringEntity(jsonString);
        entity.setContentEncoding("UTF-8");
        entity.setContentType("application/json");
        httpPost.setEntity(entity);
        HttpEntity resEntity = httpClient.execute(httpPost).getEntity();
        result = EntityUtils.toString(resEntity, "UTF-8");
    } catch (Exception e) {
        logger.error("http接口调用异常：url is::" + url, e);
        return null;
    } finally {
        httpClient.getConnectionManager().shutdown();
    }
    return result;
}
4.5.2版本是这样的：
 public static String testTimeout(String url) {
        // 设置HTTP请求参数
        String result = null;
        CloseableHttpClient client = HttpClients.createDefault();
        HttpGet httpGet = new HttpGet(url);
        RequestConfig requestConfig = RequestConfig.custom()
                .setConnectTimeout(50000).setConnectionRequestTimeout(10000)
                .setSocketTimeout(50000).build();
        httpGet.setConfig(requestConfig);
        try {
            CloseableHttpResponse response = client.execute(httpGet);
            result = EntityUtils.toString(response.getEntity(), "UTF-8");
        } catch (ClientProtocolException e) {
            logger.error("http接口调用异常：url is::" + url, e);
            return null;
        } catch (Exception e) {
            logger.error("http接口调用异常：url is::" + url, e);
            return null;
        } finally {
            try {
                client.close();
            } catch (IOException e) {
                logger.error("http接口调用异常：url is::" + url, e);
            }
        }
        return result;
    }
