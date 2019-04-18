# 从HttpClient3迁移到HttpClient4 - z69183787的专栏 - CSDN博客
2015年07月17日 11:49:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1096
网上很多HttpClient的学习源码都是3版本的，不过，既然现在HttpClient已经升级成Apache的顶级项目，自然想使用新版本。但是4版本完全是重新编写，而不能向下兼容。所以在学习调试源码时，利用的4的库，就不能直接运行3.1下的源码。
结合网上资源或自己实践，把3迁移4的一些资料收集整理于此，于己于人都会有帮助。
- jar包的变更，可以在此下载最新的jar包：http://hc.apache.org/downloads.cgi
- 把org.apache.commons.httpclient.*修改到import org.apache.http.*。具体变更哪些，就要看利用了HttpClient的哪些功能
比如：
Java代码  ![收藏代码](http://hugh-wangp.iteye.com/images/icon_star.png)
- import org.apache.commons.httpclient.HttpClient;  
- import org.apache.commons.httpclient.HttpStatus;  
- import org.apache.commons.httpclient.HttpException;  
- import org.apache.commons.httpclient.methods.GetMethod;  
- import org.apache.commons.httpclient.ConnectTimeoutException;  
To
Java代码  ![收藏代码](http://hugh-wangp.iteye.com/images/icon_star.png)
- import org.apache.http.client.HttpClient;  
- import org.apache.http.HttpStatus;  
- import org.apache.http.HttpException;  
- import org.apache.http.client.methods.HttpGet;  
- import org.apache.http.conn.ConnectTimeoutException;  
- import org.apache.http.HttpResponse;  
- import org.apache.http.impl.client.DefaultHttpClient;  
- 使用的各个HttpClient的功能的代码修改。这部分的整理，在使用过程中慢慢补全。本次先介绍Get方法的修改。
**GET方法：**
HttpClient3.1的代码：
HttpClient client = new HttpClient();
Java代码  ![收藏代码](http://hugh-wangp.iteye.com/images/icon_star.png)
- GetMethod method = new GetMethod(url);  
- int statusCode = client.executeMethod(method);  
- if (statusCode == HttpStatus.SC_OK) {  
-     InputStream is = method.getResponseBodyAsStream();  
- // do something with the input stream
- }  
- method.releaseConnection();  
To
HttpClient4.0的代码：
HttpClient client = new DefaultHttpClient();
Java代码  ![收藏代码](http://hugh-wangp.iteye.com/images/icon_star.png)
- HttpGet method = new HttpGet(url);  
- HttpResponse httpResponse = client.execute(method);  
- int statusCode = httpResponse.getStatusLine().getStatusCode();  
- if (statusCode == HttpStatus.SC_OK) {  
-     InputStream is = httpResponse.getEntity().getContent();  
- // do something with the input stream
- }  
哪位有更完整的资料，求分享！
参考：
 http://blog.teamextension.com/migrating-from-httpclient-3-1-to-4-0-34
