# HttpAsyncClient之模拟post请求示例 - z69183787的专栏 - CSDN博客
2016年11月03日 18:13:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2056
如果看到过我前些天写过的《[轻松把玩HttpClient之模拟post请求示例](http://blog.csdn.net/xiaoxian8023/article/details/49863967)》这篇文章，你再看本文就是小菜一碟了，如果你顺便懂一些NIO，基本上是毫无压力了。因为HttpAsyncClient相对于HttpClient，就多了一个NIO，这也是为什么支持异步的原因。
不过我有一个疑问，虽说NIO是同步非阻塞IO，但是HttpAsyncClient提供了回调的机制，这点儿跟netty很像，所以可以模拟类似于AIO的效果。但是[官网上的例子](http://hc.apache.org/httpcomponents-asyncclient-4.1.x/httpasyncclient/examples/org/apache/http/examples/nio/client/AsyncClientHttpExchange.java)却基本上都是使用Future<HttpResponse> future = httpclient.execute(request, null);来同步获得执行结果。
```
<properties>
		<!-- 文件拷贝时的编码 -->
		<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
		<project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
		<!-- 编译时的编码 -->
		<maven.compiler.encoding>UTF-8</maven.compiler.encoding>
		<httpclient.version>4.4.1</httpclient.version>
		<maven-javadoc-plugin.version>2.10.3</maven-javadoc-plugin.version>
		<maven-source-plugin.version>2.4</maven-source-plugin.version>
		<maven-gpg-plugin.version>1.6</maven-gpg-plugin.version>
	</properties>
	<dependencies>
		<dependency>
			<groupId>org.apache.httpcomponents</groupId>
			<artifactId>httpclient</artifactId>
			<version>${httpclient.version}</version>
		</dependency>
		<dependency>
			<groupId>org.apache.httpcomponents</groupId>
			<artifactId>httpmime</artifactId>
			<version>${httpclient.version}</version>
		</dependency>
		<dependency>
			<groupId>org.apache.httpcomponents</groupId>
			<artifactId>httpasyncclient</artifactId>
			<version>4.1</version>
		</dependency>
		<!-- <dependency> <groupId>org.apache.httpcomponents</groupId> <artifactId>httpasyncclient</artifactId> 
			<version>4.1</version> </dependency> -->
		<dependency>
			<groupId>log4j</groupId>
			<artifactId>log4j</artifactId>
			<version>1.2.17</version>
		</dependency>
	</dependencies>
```
```java
package com.tgb.ccl.http.simpledemo;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.ParseException;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.config.Registry;
import org.apache.http.config.RegistryBuilder;
import org.apache.http.conn.ssl.TrustSelfSignedStrategy;
import org.apache.http.impl.conn.DefaultProxyRoutePlanner;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClientBuilder;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.apache.http.impl.nio.conn.PoolingNHttpClientConnectionManager;
import org.apache.http.impl.nio.reactor.DefaultConnectingIOReactor;
import org.apache.http.impl.nio.reactor.IOReactorConfig;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.nio.conn.NoopIOSessionStrategy;
import org.apache.http.nio.conn.SchemeIOSessionStrategy;
import org.apache.http.nio.conn.ssl.SSLIOSessionStrategy;
import org.apache.http.nio.reactor.ConnectingIOReactor;
import org.apache.http.ssl.SSLContexts;
import org.apache.http.util.EntityUtils;
/** 
 * HttpAsyncClient模拟post请求简单示例
 * 
 * @author arron
 * @date 2015年11月1日 下午2:23:18 
 * @version 1.0 
 */
public class SimpleHttpAsyncClientDemo {
	
	/**
	 * 设置信任自定义的证书
	 * 	
	 * @param keyStorePath		密钥库路径
	 * @param keyStorepass		密钥库密码
	 * @return
	 */
	public static SSLContext custom(String keyStorePath, String keyStorepass) {
		SSLContext sc = null;
		FileInputStream instream = null;
		KeyStore trustStore = null;
		try {
			trustStore = KeyStore.getInstance(KeyStore.getDefaultType());
			instream = new FileInputStream(new File(keyStorePath));
			trustStore.load(instream, keyStorepass.toCharArray());
			// 相信自己的CA和所有自签名的证书
			sc = SSLContexts.custom().loadTrustMaterial(trustStore, new TrustSelfSignedStrategy()).build();
		} catch (KeyStoreException | NoSuchAlgorithmException| CertificateException | IOException | KeyManagementException e) {
			e.printStackTrace();
		} finally {
			try {
				instream.close();
			} catch (IOException e) {
			}
		}
		return sc;
	}
	
	/**
	 * 绕过验证
	 * 	
	 * @return
	 * @throws NoSuchAlgorithmException 
	 * @throws KeyManagementException 
	 */
	public static SSLContext createIgnoreVerifySSL() throws NoSuchAlgorithmException, KeyManagementException {
		SSLContext sc = SSLContext.getInstance("SSLv3");
		// 实现一个X509TrustManager接口，用于绕过验证，不用修改里面的方法
		X509TrustManager trustManager = new X509TrustManager() {
			@Override
			public void checkClientTrusted(
					java.security.cert.X509Certificate[] paramArrayOfX509Certificate,
					String paramString) throws CertificateException {
			}
			@Override
			public void checkServerTrusted(
					java.security.cert.X509Certificate[] paramArrayOfX509Certificate,
					String paramString) throws CertificateException {
			}
			@Override
			public java.security.cert.X509Certificate[] getAcceptedIssuers() {
				return null;
			}
		};
		sc.init(null, new TrustManager[] { trustManager }, null);
		return sc;
	}
	/**
	 * 设置代理
	 * @param builder
	 * @param hostOrIP
	 * @param port
	 */
	public static HttpAsyncClientBuilder proxy(String hostOrIP, int port){
		// 依次是代理地址，代理端口号，协议类型  
		HttpHost proxy = new HttpHost(hostOrIP, port, "http");  
		DefaultProxyRoutePlanner routePlanner = new DefaultProxyRoutePlanner(proxy);
		return HttpAsyncClients.custom().setRoutePlanner(routePlanner);
	}
	
	/**
	 * 模拟请求
	 * 
	 * @param url				资源地址
	 * @param map			参数列表
	 * @param encoding	编码
	 * @param handler		结果处理类
	 * @return
	 * @throws NoSuchAlgorithmException 
	 * @throws KeyManagementException 
	 * @throws IOException 
	 * @throws ClientProtocolException 
	 */
	public static void send(String url, Map<String,String> map,final String encoding, final AsyncHandler handler) throws KeyManagementException, NoSuchAlgorithmException, ClientProtocolException, IOException {
		//绕过证书验证，处理https请求
		SSLContext sslcontext = createIgnoreVerifySSL();
		
        // 设置协议http和https对应的处理socket链接工厂的对象
		Registry<SchemeIOSessionStrategy> sessionStrategyRegistry = RegistryBuilder.<SchemeIOSessionStrategy>create()
                .register("http", NoopIOSessionStrategy.INSTANCE)
                .register("https", new SSLIOSessionStrategy(sslcontext))
                .build();
		//配置io线程
        IOReactorConfig ioReactorConfig = IOReactorConfig.custom().setIoThreadCount(Runtime.getRuntime().availableProcessors()).build();
		//设置连接池大小
        ConnectingIOReactor ioReactor;
		ioReactor = new DefaultConnectingIOReactor(ioReactorConfig);
        PoolingNHttpClientConnectionManager connManager = new PoolingNHttpClientConnectionManager(ioReactor, null, sessionStrategyRegistry, null);
        
        //创建自定义的httpclient对象
		final CloseableHttpAsyncClient client = proxy("127.0.0.1", 8087).setConnectionManager(connManager).build();
//		CloseableHttpAsyncClient client = HttpAsyncClients.createDefault();
		
		//创建post方式请求对象
		HttpPost httpPost = new HttpPost(url);
		
		//装填参数
		List<NameValuePair> nvps = new ArrayList<NameValuePair>();
		if(map!=null){
			for (Entry<String, String> entry : map.entrySet()) {
				nvps.add(new BasicNameValuePair(entry.getKey(), entry.getValue()));
			}
		}
		//设置参数到请求对象中
		httpPost.setEntity(new UrlEncodedFormEntity(nvps, encoding));
		System.out.println("请求地址："+url);
		System.out.println("请求参数："+nvps.toString());
		
		//设置header信息
		//指定报文头【Content-type】、【User-Agent】
		httpPost.setHeader("Content-type", "application/x-www-form-urlencoded");
		httpPost.setHeader("User-Agent", "Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt)");
		
		// Start the client
		client.start();
		//执行请求操作，并拿到结果（异步）
		client.execute(httpPost, new FutureCallback<HttpResponse>() {
			
			@Override
			public void failed(Exception ex) {
				handler.failed(ex);
				close(client);
			}
			
			@Override
			public void completed(HttpResponse resp) {
				String body="";
				//这里使用EntityUtils.toString()方式时会大概率报错，原因：未接受完毕，链接已关
				try {
					HttpEntity entity = resp.getEntity();
					if (entity != null) {
						final InputStream instream = entity.getContent();
						try {
							final StringBuilder sb = new StringBuilder();
							final char[] tmp = new char[1024];
							final Reader reader = new InputStreamReader(instream,encoding);
							int l;
							while ((l = reader.read(tmp)) != -1) {
								sb.append(tmp, 0, l);
							}
							body = sb.toString();
						} finally {
							instream.close();
							EntityUtils.consume(entity);
						}
					}
				} catch (ParseException | IOException e) {
					e.printStackTrace();
				}
				handler.completed(body);
				close(client);
			}
			
			@Override
			public void cancelled() {
				handler.cancelled();
				close(client);
			}
		});
	}
	
	/**
	 * 关闭client对象
	 * 
	 * @param client
	 */
	private static void close(CloseableHttpAsyncClient client) {
		try {
			client.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	static class AsyncHandler implements IHandler{
		
		@Override
		public Object failed(Exception e) {
			System.err.println(Thread.currentThread().getName()+"--失败了--"+e.getClass().getName()+"--"+e.getMessage());
			return null;
		}
		@Override
		public Object completed(String respBody) {
			System.out.println(Thread.currentThread().getName()+"--获取内容："+respBody);
			return null;
		}
		@Override
		public Object cancelled() {
			System.out.println(Thread.currentThread().getName()+"--取消了");
			return null;
		}
	}
	
	/**
	 * 回调处理接口
	 * 
	 * @author arron
	 * @date 2015年11月10日 上午10:05:40 
	 * @version 1.0
	 */
	public interface IHandler {
		
		/**
		 * 处理异常时，执行该方法
		 * @return
		 */
		Object failed(Exception e);
		
		/**
		 * 处理正常时，执行该方法
		 * @return
		 */
		Object completed(String respBody);
		
		/**
		 * 处理取消时，执行该方法
		 * @return
		 */
		Object cancelled();
	}
	
}
```
来一个测试类：
```java
public static void main(String[] args) throws KeyManagementException, NoSuchAlgorithmException, ClientProtocolException, IOException {  
    AsyncHandler handler = new AsyncHandler();  
    String url = "http://php.weather.sina.com.cn/iframe/index/w_cl.php";  
    Map<String, String> map = new HashMap<String, String>();  
    map.put("code", "js");  
    map.put("day", "0");  
    map.put("city", "上海");  
    map.put("dfc", "1");  
    map.put("charset", "utf-8");  
    send(url, map, "utf-8", handler);  
      
    System.out.println("-----------------------------------");  
      
    map.put("city", "北京");  
    send(url, map, "utf-8", handler);  
      
    System.out.println("-----------------------------------");  
      
}
```
好吧，反正我是用回调的方式实现的。代码基本上跟httpClient那篇一致。不一样的地方主要有这么2个地方：配置ssl时不一样；调用execute方式时，使用回调。具体代码如下：测试结果如下：
![](https://img-blog.csdn.net/20151120183337371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
很简单吧，其实基于HttpAsyncClient的工具类我也进行了封装，跟HttpClient工具类差不多。代码都已经提交至：[https://github.com/Arronlong/httpclientUtil](https://github.com/Arronlong/httpclientUtil)。有兴趣的自行下载，博客中就不再分享了
版权声明：本文为博主原创文章，未经博主允许不得转载。如需转载请声明：【转自 http://blog.csdn.net/xiaoxian8023 】
官方例子：
```java
/**
 * This example demonstrates a basic asynchronous HTTP request / response exchange.
 * Response content is buffered in memory for simplicity.
 */
public class AsyncClientHttpExchange {
    public static void main(final String[] args) throws Exception {
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
        try {
            httpclient.start();
            HttpGet request = new HttpGet("http://www.apache.org/");
            Future<HttpResponse> future = httpclient.execute(request, null);
            HttpResponse response = future.get();
            System.out.println("Response: " + response.getStatusLine());
            System.out.println("Shutting down");
        } finally {
            httpclient.close();
        }
        System.out.println("Done");
    }
}
```
