# 【一】HttpClient4.3.1简单入门实例 - z69183787的专栏 - CSDN博客
2015年09月11日 14:27:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1318
**1、先看一下示例代码**
**[java]**[view
 plain](http://blog.csdn.net/mr_tank_/article/details/17454315#)[copy](http://blog.csdn.net/mr_tank_/article/details/17454315#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/121428/fork)
- publicclass HttpClientTest {  
- publicstaticvoid main(String args[]) {  
- //创建HttpClientBuilder
-         HttpClientBuilder httpClientBuilder = HttpClientBuilder.create();  
- //HttpClient
-         CloseableHttpClient closeableHttpClient = httpClientBuilder.build();  
- 
-         HttpGet httpGet = new HttpGet("http://www.gxnu.edu.cn/default.html");  
-         System.out.println(httpGet.getRequestLine());  
- try {  
- //执行get请求
-             HttpResponse httpResponse = closeableHttpClient.execute(httpGet);  
- //获取响应消息实体
-             HttpEntity entity = httpResponse.getEntity();  
- //响应状态
-             System.out.println("status:" + httpResponse.getStatusLine());  
- //判断响应实体是否为空
- if (entity != null) {  
-                 System.out.println("contentEncoding:" + entity.getContentEncoding());  
-                 System.out.println("response content:" + EntityUtils.toString(entity));  
-             }  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         } finally {  
- try {  
**[java]**[view
 plain](http://blog.csdn.net/mr_tank_/article/details/17454315#)[copy](http://blog.csdn.net/mr_tank_/article/details/17454315#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/121428/fork)
- //关闭流并释放资源
-             closeableHttpClient.close();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
以下内容来自HttpClient4.3.1 API文档：[http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/overview-summary.html](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/overview-summary.html)
**2、HttpClientBuilder**
HttpClientBuilder用于创建CloseableHttpClient实例。看了一下API文档，[AbstractHttpClient](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/client/AbstractHttpClient.html)、[AutoRetryHttpClient](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/client/AutoRetryHttpClient.html)、[DefaultHttpClient](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/client/DefaultHttpClient.html)等都被弃用了，使用HttpClientBuilder代替。
**3、CloseableHttpClient**
实现接口：[Closeable](http://download.oracle.com/javase/1.5.0/docs/api/java/io/Closeable.html?is-external=true), [AutoCloseable](http://download.oracle.com/javase/1.5.0/docs/api/java/lang/AutoCloseable.html?is-external=true), [HttpClient](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/client/HttpClient.html)；子类：[AbstractHttpClient](http://hc.apache.org/httpcomponents-client-ga/httpclient/apidocs/org/apache/http/impl/client/AbstractHttpClient.html)
**4、HttpGet**
非线程安全；HttpGet有三个构造方法：HttpGet()、HttpGet(String uri)、HttpGet(URI uri)
**5、HttpResponse**
服务器在接收和解释请求之后返回一个HTTP响应信息
     Response      = Status-Line
                     *(( general-header
                      | response-header
                      | entity-header ) CRLF)
                     CRLF
                     [ message-body ]
 
