# 解决httpclient超时设置不生效的问题 - z69183787的专栏 - CSDN博客
2018年01月09日 10:42:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1709
[http://m.blog.csdn.net/kakaweb/article/details/51974139](http://m.blog.csdn.net/kakaweb/article/details/51974139)
最近公司有项目需要通过http调用第三方服务，且第三方服务偶有超时，故需要设置一定的超时时间防止不响应的情况出现。
初始设置如下：
```java
//超时设置
		RequestConfig requestConfig = RequestConfig.custom()
		        .setConnectTimeout(400).setSocketTimeout(400).build();
		request.setEntity(s);
		request.setConfig(requestConfig);
		
		HttpClient client = new HttpClient();
		HttpResponse httpResponse = client.execute(request);
```
发现设置并未生效，后发现是由于没有设置重试选项，将代码修改如下：
```java
private HttpRequestRetryHandler myRetryHandler = new HttpRequestRetryHandler() {
		    @Override
		    public boolean retryRequest(IOException exception,
			    	int executionCount, HttpContext context) {
			    return false;
		    }};
		//超时设置
		RequestConfig requestConfig = RequestConfig.custom()
		        .setConnectTimeout(connectTImeoutInMills).setSocketTimeout(socketTImeoutInMills).build();
		request.setEntity(s);
		request.setConfig(requestConfig);
		
		HttpClient client = HttpClients.custom().setRetryHandler(myRetryHandler).build();
		HttpResponse httpResponse = client.execute(request);
```
超时设置即生效。
备注：
httpclient版本为4.5。
