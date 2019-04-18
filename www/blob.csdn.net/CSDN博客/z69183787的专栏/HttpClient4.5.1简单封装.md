# HttpClient4.5.1简单封装 - z69183787的专栏 - CSDN博客
2016年10月25日 17:04:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2051
摘要: HttpClient的简单封装，主要是方便自己在开发小工具的时候是使用。
1、maven依赖：
```xml
<dependency>
	<groupId>org.apache.httpcomponents</groupId>
	<artifactId>httpclient</artifactId>
	<version>4.5.1</version>
</dependency>
<dependency>
	<groupId>commons-io</groupId>
	<artifactId>commons-io</artifactId>
	<version>2.4</version>
</dependency>
```
2、HttpUtils.java：
```java
```java
package com.hjr.demo.util.http;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import org.apache.commons.lang.StringUtils;
import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicHeader;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import org.apache.log4j.Logger;
/**
 * Http工具类
 * 
 * @author hjr
 * @date 2016年2月24日 下午1:45:03
 */
public class HttpUtils {
	private static final Logger LOGGER = Logger.getLogger(HttpUtils.class);
	/**
	 * TODO 链接配置
	 * TODO 文件下载请求封装
	 * TODO GET请求封装
	 * 
	 */
	
	/**
	 * post请求
	 * 
	 * @param url 请求链接
	 * @param params 请求参数
	 * @param headers 请求头
	 * @return
	 */
	public static String post(String url, Map<String, Object> params, Map<String, Object> headers) {
		return post(url, params, headers, null, null);
	}
	
	/**
	 * post请求
	 * 
	 * @param url 请求链接
	 * @param params 请求参数
	 * @param headers 请求头
	 * @param hostname 请求代理域名或ip
	 * @param port 请求代理端口
	 * @return
	 */
	public static String post(String url, Map<String, Object> params, Map<String, Object> headers, String hostname,
			Integer port) {
		return post(url, params, headers, hostname, port);
	}
	/**
	 * post请求
	 * 
	 * @param url 请求链接
	 * @param params 请求参数
	 * @param headers 请求头
	 * @param hostname 请求代理域名或ip
	 * @param port 请求代理端口
	 * @param scheme
	 * @return
	 */
	public static String post(String url, Map<String, Object> params, Map<String, Object> headers, String hostname,
			Integer port, String scheme) {
		HttpResponse response = null;
		String responseBody = null;
		try {
			if (StringUtils.isBlank(hostname) || port == null) {
				response = postRaw(url, params, headers, null);
			} else {
				HttpHost proxy = new HttpHost(hostname, port, scheme);
				response = postRaw(url, params, headers, proxy);
			}
			HttpEntity entity = response.getEntity();
			responseBody = EntityUtils.toString(entity, "UTF-8");
		} catch (Exception e) {
			LOGGER.error("Http post error!", e);
		} finally {
			if (response != null) {
				try {
					EntityUtils.consume(response.getEntity());
				} catch (IOException e) {
					LOGGER.error("Consume response entity error!", e);
				}
			}
		}
		return responseBody;
	}
	/**
	 * post请求
	 * 
	 * @param url 请求链接
	 * @param params 请求参数
	 * @param headers 请求头
	 * @return
	 * @throws Exception
	 */
	public static HttpResponse postRaw(String url, Map<String, Object> params, Map<String, Object> headers) throws Exception {
		return postRaw(url, params, headers, null);
	}
	
	/**
	 * post请求
	 * 
	 * @param url 请求链接
	 * @param params 请求参数
	 * @param headers 请求头
	 * @param hostname 请求代理域名或ip
	 * @param port 请求代理端口
	 * @return
	 * @throws Exception
	 */
	public static HttpResponse postRaw(String url, Map<String, Object> params, Map<String, Object> headers,
			String hostname, int port) throws Exception {
		return postRaw(url, params, headers, hostname, port, "http");
	}
	/**
	 * post请求
	 * 
	 * @param url 请求链接
	 * @param params 请求参数
	 * @param headers 请求头
	 * @param hostname 请求代理域名或ip
	 * @param port 请求代理端口
	 * @param scheme 协议
	 * @return
	 * @throws Exception
	 */
	public static HttpResponse postRaw(String url, Map<String, Object> params, Map<String, Object> headers,
			String hostname, Integer port, String scheme) throws Exception {
		HttpHost proxy = new HttpHost(hostname, port, scheme);
		return postRaw(url, params, headers, proxy);
	}
	/**
	 * post请求
	 * 
	 * @param url 请求链接
	 * @param params 请求参数
	 * @param headers 请求头
	 * @param proxy 请求代理
	 * @return
	 * @throws Exception
	 */
	public static HttpResponse postRaw(String url, Map<String, Object> params, Map<String, Object> headers,
			HttpHost proxy) throws Exception {
		CloseableHttpClient httpClient = HttpClients.createDefault();
		HttpPost httpPost = new HttpPost(url);
		// 设置参数
		List<NameValuePair> pairList = new ArrayList<NameValuePair>(params.size());
		for (Map.Entry<String, Object> entry : params.entrySet()) {
			NameValuePair pair = new BasicNameValuePair(entry.getKey(), entry.getValue().toString());
			pairList.add(pair);
		}
		httpPost.setEntity(new UrlEncodedFormEntity(pairList, Charset.forName("UTF-8")));
		// 设置请求头
		List<Header> headerList = new ArrayList<Header>(headers.size());
		for (Map.Entry<String, Object> entry : headers.entrySet()) {
			Header header = new BasicHeader(entry.getKey(), entry.getValue().toString());
			headerList.add(header);
		}
		httpPost.setHeaders(headerList.toArray(new Header[headers.size()]));
		// 设置代理
		if (proxy != null) {
			RequestConfig config = RequestConfig.custom().setProxy(proxy).build();
			httpPost.setConfig(config);
		}
		return httpClient.execute(httpPost);
	}
}
```
```
