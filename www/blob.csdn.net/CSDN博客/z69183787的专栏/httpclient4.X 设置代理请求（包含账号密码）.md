# httpclient4.X 设置代理请求（包含账号密码） - z69183787的专栏 - CSDN博客
2016年10月25日 17:09:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6486
最近需要使用Httpclient做后台请求，使用的是httpclient4.3版本，apache网站上有，我这里就不提供下载链接了，搜一下就可以了，废话少说，直接上代码：
Java代码  ![收藏代码](http://zhangzhaoaaa.iteye.com/images/icon_star.png)
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.apache.http.HttpEntity;  
- import org.apache.http.HttpHost;  
- import org.apache.http.NameValuePair;  
- import org.apache.http.client.config.RequestConfig;  
- import org.apache.http.client.entity.UrlEncodedFormEntity;  
- import org.apache.http.client.methods.CloseableHttpResponse;  
- import org.apache.http.client.methods.HttpPost;  
- import org.apache.http.impl.client.CloseableHttpClient;  
- import org.apache.http.impl.client.HttpClientBuilder;  
- import org.apache.http.message.BasicNameValuePair;  
- import org.apache.http.util.EntityUtils;  
- 
- publicclass HttpClientTest {  
- publicstaticvoid main(String args[]) throws Exception {  
- // 创建HttpClientBuilder
-         HttpClientBuilder httpClientBuilder = HttpClientBuilder.create();  
- // HttpClient
-         CloseableHttpClient closeableHttpClient = httpClientBuilder.build();  
- // 依次是目标请求地址，端口号,协议类型
-         HttpHost target = new HttpHost("10.10.100.102:8080/mytest", 8080,  
- "http");  
- // 依次是代理地址，代理端口号，协议类型
-         HttpHost proxy = new HttpHost("yourproxy", 8080, "http");  
-         RequestConfig config = RequestConfig.custom().setProxy(proxy).build();  
- 
- // 请求地址
-         HttpPost httpPost = new HttpPost("http://10.10.100.102:8080/mytest");  
-         httpPost.setConfig(config);  
- // 创建参数队列
-         List<NameValuePair> formparams = new ArrayList<NameValuePair>();  
- // 参数名为pid，值是2
-         formparams.add(new BasicNameValuePair("pid", "2"));  
- 
-         UrlEncodedFormEntity entity;  
- try {  
-             entity = new UrlEncodedFormEntity(formparams, "UTF-8");  
-             httpPost.setEntity(entity);  
-             CloseableHttpResponse response = closeableHttpClient.execute(  
-                     target, httpPost);  
- // getEntity()
-             HttpEntity httpEntity = response.getEntity();  
- if (httpEntity != null) {  
- // 打印响应内容
-                 System.out.println("response:"
-                         + EntityUtils.toString(httpEntity, "UTF-8"));  
-             }  
- // 释放资源
-             closeableHttpClient.close();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
您好，您知道这么设置以后，如何设置代理服务器的用户名和密码么
好久没搞这个了,手边也没有这个环境,但是我帮你查了一下,请试试
```java
HttpHost proxy = new HttpHost("proxy", 8080);
BasicScheme proxyAuth = new BasicScheme();
// Make client believe the challenge came form a proxy
proxyAuth.processChallenge(new BasicHeader(AUTH.PROXY_AUTH, "BASIC realm=default"));
BasicAuthCache authCache = new BasicAuthCache();
authCache.put(proxy, proxyAuth);
CredentialsProvider credsProvider = new BasicCredentialsProvider();
credsProvider.setCredentials(
        new AuthScope(proxy),
        new UsernamePasswordCredentials("username", "password"));
HttpClientContext context = HttpClientContext.create();
context.setAuthCache(authCache);
context.setCredentialsProvider(credsProvider);
CloseableHttpClient httpclient = HttpClients.createDefault();
try {
    CloseableHttpResponse response = httpclient.execute(new HttpGet("/stuff"), context);
    try {
        // ...
    } finally {
        response.close();
    }
} finally {
    httpclient.close();
}
```
使用代理需要导入：commons-logging-1.1.jar，httpclient-4.0-beta2.jar ，httpcore-4.1-alpha1.jar 和 commons-codec-1.4.jar架包。
在连接代理时需要使用用户名和密码构造UsernamePasswordCredentials对象并作为参数传递给HttpClient对象。
具体用法如下：
```java
public static void main(String args[])
{
 StringBuffer sb = new StringBuffer();
 //创建HttpClient实例
 HttpClient client = getHttpClient();
 //创建httpGet
 HttpGet httpGet = new HttpGet("http://www.csdn.net");
 //执行
 try {
  HttpResponse response = client.execute(httpGet);
  
  HttpEntity entry = response.getEntity();
  
  if(entry != null)
  {
   InputStreamReader is = new InputStreamReader(entry.getContent());
   BufferedReader br = new BufferedReader(is);
   String str = null;
   while((str = br.readLine()) != null)
   {
    sb.append(str.trim());
   }
   br.close();
  }
  
 } catch (ClientProtocolException e) {
  // TODO Auto-generated catch block
  e.printStackTrace();
 } catch (IOException e) {
  // TODO Auto-generated catch block
  e.printStackTrace();
 }
 System.out.println(sb.toString());
}
//设置代理
public static HttpClient getHttpClient() {
 DefaultHttpClient httpClient = new DefaultHttpClient();
 String proxyHost = "proxycn2.huawei.com";
 int proxyPort = 8080;
 String userName = "china\\******";
 String password = "*******“
 httpClient.getCredentialsProvider().setCredentials(
   new AuthScope(proxyHost, proxyPort),
   new UsernamePasswordCredentials(userName, password));
 HttpHost proxy = new HttpHost(proxyHost,proxyPort);
 httpClient.getParams().setParameter(ConnRouteParams.DEFAULT_PROXY, proxy);
 return httpClient;
}
```
