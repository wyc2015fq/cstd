# 为 httpclient 设置代理, 设置http头. - z69183787的专栏 - CSDN博客
2015年01月21日 11:05:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：22531
在学校园网，要代理才可上网。写程序获取网上资源时，怎么办呢？同样可以为程序设置代理。我用HttpClient的3.1版本。有些网站要浏览器才可以访问，但程序可以仿浏览器，主要是设置http头。
在HostConfiguration.setProxy里设置。如下面代码：
- package com.chenlb;  
- 
- import java.io.IOException;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;  
- import org.apache.commons.httpclient.Header;  
- import org.apache.commons.httpclient.HttpClient;  
- import org.apache.commons.httpclient.HttpException;  
- import org.apache.commons.httpclient.HttpStatus;  
- import org.apache.commons.httpclient.UsernamePasswordCredentials;  
- import org.apache.commons.httpclient.auth.AuthScope;  
- import org.apache.commons.httpclient.methods.GetMethod;  
- import org.apache.commons.httpclient.params.HttpMethodParams;  
- 
- publicclass HttpClientUse {  
- 
- publicstaticvoid main(String[] args) throws HttpException, IOException {  
-         HttpClient httpClient = new HttpClient();  
- 
-         httpClient.getHostConfiguration().setProxy("localhost", 808);  
- 
- /*//需要验证
-         UsernamePasswordCredentials creds = new UsernamePasswordCredentials("chenlb", "123456");
- 
-         httpClient.getState().setProxyCredentials(AuthScope.ANY, creds);
-         */
- 
- //设置http头
-         List<Header> headers = new ArrayList<Header>();  
-         headers.add(new Header("User-Agent", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"));  
-         httpClient.getHostConfiguration().getParams().setParameter("http.default-headers", headers);  
- 
-         GetMethod method = new GetMethod("http://www.baidu.com");  
-         method.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,  
- new DefaultHttpMethodRetryHandler(3, false));  
- try {  
- int statusCode = httpClient.executeMethod(method);  
- 
- if (statusCode != HttpStatus.SC_OK) {  
-                 System.out.println("Method failed code="+statusCode+": " + method.getStatusLine());  
- 
-             } else {  
-                 System.out.println(new String(method.getResponseBody(), "gb2312"));  
-             }  
-         } finally {  
-             method.releaseConnection();  
-         }  
-     }  
- }  
如果要用户名与密码验证的，请把/* */注释去掉。使验证有效。
验证，关键是：
- UsernamePasswordCredentials creds = new UsernamePasswordCredentials("chenlb", "123456");  
- 
- httpClient.getState().setProxyCredentials(AuthScope.ANY, creds);  
设置http请求头.
- List<Header> headers = new ArrayList<Header>();  
- headers.add(new Header("User-Agent", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"));  
- httpClient.getHostConfiguration().getParams().setParameter("http.default-headers", headers);  
