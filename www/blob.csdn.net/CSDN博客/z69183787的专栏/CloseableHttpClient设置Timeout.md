# CloseableHttpClient设置Timeout - z69183787的专栏 - CSDN博客
2015年09月11日 14:09:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11967
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
