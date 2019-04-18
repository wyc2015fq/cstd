# HttpClient工具正确使用方式 - z69183787的专栏 - CSDN博客
2018年01月09日 14:15:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1170
                
虽然现在基于netty和thift的微服务非常的流行，但是公司内部的蛮多系统仍然对外提供http接口。在调用http接口工具类中，apache httpclient工具用的比较多。下面列举一些正确使用httpclient的一些要点。
设置socketTimeout
可以在RequestConfig 中设置socketTimeout，表示数据传输处理时间，如下：
 RequestConfig requestConfig = RequestConfig.custom().setSocketTimeout(4000).build()
设置connectionRequestTimeout
可以在RequestConfig 中设置socketTimeout，表示从连接池中后去连接的timeout时间，如下：
RequestConfig requestConfig = RequestConfig.custom().setConnectionRequestTimeout(1000)
设置connectTimeout
可以在RequestConfig 中设置connectTimeout，表示建立连接的timeout时间，如下：
RequestConfig requestConfig = RequestConfig.custom().setConnectTimeout(3000).build();
设置MaxTotal
MaxTotal表示连接池最大并发连接数，代码如下：
PoolingHttpClientConnectionManager pccm = new PoolingHttpClientConnectionManager();
    pccm.setMaxTotal(300); 
设置DefaultMaxPerRout
DefaultMaxPerRout表示单路由的最大并发连接数，假设你的业务系统需要调用A和B这两个外部系统的http接口，那么如果DefaultMaxPerRout=100，那么调用A系统的http接口时，最大并发数就是100。代码如下：
PoolingHttpClientConnectionManager pccm = new PoolingHttpClientConnectionManager();
    pccm.setDefaultMaxPerRoute(100); // 单路由最大并发数
尽量不要设置超时重试次数
如果要设置的话，可以如下设置：
```java
// 设置默认时间 
    RequestConfig params = RequestConfig.custom().setConnectTimeout(3000).setConnectionRequestTimeout(1000).setSocketTimeout(4000)
        .setExpectContinueEnabled(true).build();
    PoolingHttpClientConnectionManager pccm = new PoolingHttpClientConnectionManager();
    pccm.setMaxTotal(300); // 连接池最大并发连接数
    pccm.setDefaultMaxPerRoute(50); // 单路由最大并发数
HttpRequestRetryHandler retryHandler = new HttpRequestRetryHandler() {
      public boolean retryRequest(IOException exception , int executionCount , HttpContext context) {
        if (executionCount > 3) {
          return false;
        }
        if (exception instanceof NoHttpResponseException) {
          LOGGER.info(
              "[NoHttpResponseException has retry request:" + context.toString() + "][executionCount:" + executionCount + "]");
          return true;
        }
        else if (exception instanceof SocketException) {
          LOGGER.info("[SocketException has retry request:" + context.toString() + "][executionCount:" + executionCount + "]");
          return true;
        }
        return false;
      }
    };
    httpclient = HttpClients.custom().setConnectionManager(pccm).setDefaultRequestConfig(params).setRetryHandler(retryHandler)
        .build();
```
**注：看源码得知，默认是不会重发的Assigns org.apache.http.client.HttpRequestRetryHandler instance.Please note this value can be overridden by the disableAutomaticRetries() method. //Disables automatic request recovery and re-execution.public final HttpClientBuilder disableAutomaticRetries() {automaticRetriesDisabled = true;return this;}**
完整：、
```java
package test;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import org.apache.http.HttpEntity;
import org.apache.http.HttpStatus;
import org.apache.http.NameValuePair;
import org.apache.http.NoHttpResponseException;
import org.apache.http.client.HttpRequestRetryHandler;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.impl.conn.PoolingHttpClientConnectionManager;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.HttpContext;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import com.alibaba.fastjson.JSON;
public class HttpClientUtils {
  private final static Logger LOGGER = LoggerFactory.getLogger(HttpClientUtils.class);
  private final static String UTF8 = "UTF-8";
  public static CloseableHttpClient httpclient = null;
  static {
    // 初始化线程池
    RequestConfig params = RequestConfig.custom().setConnectTimeout(3000).setConnectionRequestTimeout(1000).setSocketTimeout(4000)
        .setExpectContinueEnabled(true).build();
    PoolingHttpClientConnectionManager pccm = new PoolingHttpClientConnectionManager();
    pccm.setMaxTotal(300); // 连接池最大并发连接数
    pccm.setDefaultMaxPerRoute(50); // 单路由最大并发数
    HttpRequestRetryHandler retryHandler = new HttpRequestRetryHandler() {
      public boolean retryRequest(IOException exception , int executionCount , HttpContext context) {
        // 重试1次,从1开始
        if (executionCount > 1) {
          return false;
        }
        if (exception instanceof NoHttpResponseException) {
          LOGGER.info(
              "[NoHttpResponseException has retry request:" + context.toString() + "][executionCount:" + executionCount + "]");
          return true;
        }
        else if (exception instanceof SocketException) {
          LOGGER.info("[SocketException has retry request:" + context.toString() + "][executionCount:" + executionCount + "]");
          return true;
        }
        return false;
      }
    };
    httpclient = HttpClients.custom().setConnectionManager(pccm).setDefaultRequestConfig(params).setRetryHandler(retryHandler)
        .build();
  }
  public static String post(String urlToRequest , Map<String, Object> parameters , Integer connectionRequestTimeout ,
      Integer connectTimeout , Integer socketTimeout) {
    Long startTs = System.currentTimeMillis();
    List<NameValuePair> nvps = new ArrayList<NameValuePair>();
    if (parameters != null && !parameters.isEmpty()) {
      for (Entry<String, Object> entry : parameters.entrySet()) {
        nvps.add(new BasicNameValuePair(entry.getKey(), String.valueOf(entry.getValue())));
      }
    }
    try {
      LOGGER.info("post-req:url:{},param:{}", urlToRequest, JSON.toJSONString(parameters));
      HttpPost post = new HttpPost(urlToRequest);
      if (connectionRequestTimeout != null && connectTimeout != null && socketTimeout != null) {
        RequestConfig requestConfig = RequestConfig.custom().setConnectTimeout(connectTimeout)
            .setConnectionRequestTimeout(connectionRequestTimeout).setSocketTimeout(socketTimeout).build();
        post.setConfig(requestConfig);
      }
      post.setEntity(new UrlEncodedFormEntity(nvps, UTF8));
      String result = invoke(post);
      Long endTs = System.currentTimeMillis();
      Long currentMethodCallTime = endTs - startTs;
      if (currentMethodCallTime > 5000) {
        LOGGER.warn("url:{},call time {} ms", urlToRequest, currentMethodCallTime);
        LOGGER.info("所有存活线程="+Thread.getAllStackTraces().size());
      }
      LOGGER.info("post-rps:{}", result);
      return result;
    }
    catch (UnsupportedEncodingException e) {
      LOGGER.error("[HttpClientUtils][post][Unsupported Encoding Exception]", e);
    }
    return null;
  }
  private static String invoke(HttpUriRequest request) {
    CloseableHttpResponse response = null;
    try {
      response = httpclient.execute(request);
      if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
        HttpEntity entity = response.getEntity();
        return EntityUtils.toString(entity, UTF8);
      }
    }
    catch (IOException e) {
      LOGGER.error(
          "[HttpClientUtils][invoke][method:" + request.getMethod() + " URI:" + request.getURI() + "] is request exception", e);
    }
    finally {
      if (response != null) {
        try {
          response.close();
        }
        catch (IOException e) {
          LOGGER.error(
              "[HttpClientUtils][invoke][method:" + request.getMethod() + " URI:" + request.getURI() + "] is closed exception",
              e);
        }
      }
    }
    return null;
  }
}
```

