# JAVA利用HttpClient进行POST请求（HTTPS） - z69183787的专栏 - CSDN博客
2015年08月26日 15:23:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7813
目前，要为另一个项目提供接口，接口是用HTTP URL实现的，最初的想法是另一个项目用JQuery post进行请求。
但是，很可能另一个项目是部署在别的机器上，那么就存在跨域问题，而JQuery的post请求是不允许跨域的。
这时，就只能够用HttpClient包进行请求了，同时由于请求的URL是HTTPS的，为了避免需要证书，所以用一个类继承DefaultHttpClient类，忽略校验过程。
1.写一个SSLClient类，继承至HttpClient
**[java]**[view
 plain](http://blog.csdn.net/rongyongfeikai2/article/details/41659353#)[copy](http://blog.csdn.net/rongyongfeikai2/article/details/41659353#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/538971/fork)
- import java.security.cert.CertificateException;  
- import java.security.cert.X509Certificate;  
- import javax.net.ssl.SSLContext;  
- import javax.net.ssl.TrustManager;  
- import javax.net.ssl.X509TrustManager;  
- import org.apache.http.conn.ClientConnectionManager;  
- import org.apache.http.conn.scheme.Scheme;  
- import org.apache.http.conn.scheme.SchemeRegistry;  
- import org.apache.http.conn.ssl.SSLSocketFactory;  
- import org.apache.http.impl.client.DefaultHttpClient;  
- //用于进行Https请求的HttpClient
- publicclass SSLClient extends DefaultHttpClient{  
- public SSLClient() throws Exception{  
- super();  
-         SSLContext ctx = SSLContext.getInstance("TLS");  
-         X509TrustManager tm = new X509TrustManager() {  
- @Override
- publicvoid checkClientTrusted(X509Certificate[] chain,  
-                         String authType) throws CertificateException {  
-                 }  
- @Override
- publicvoid checkServerTrusted(X509Certificate[] chain,  
-                         String authType) throws CertificateException {  
-                 }  
- @Override
- public X509Certificate[] getAcceptedIssuers() {  
- returnnull;  
-                 }  
-         };  
-         ctx.init(null, new TrustManager[]{tm}, null);  
-         SSLSocketFactory ssf = new SSLSocketFactory(ctx,SSLSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER);  
-         ClientConnectionManager ccm = this.getConnectionManager();  
-         SchemeRegistry sr = ccm.getSchemeRegistry();  
-         sr.register(new Scheme("https", 443, ssf));  
-     }  
- }  
2.写一个利用HttpClient发送post请求的类
**[java]**[view
 plain](http://blog.csdn.net/rongyongfeikai2/article/details/41659353#)[copy](http://blog.csdn.net/rongyongfeikai2/article/details/41659353#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/538971/fork)
- import java.util.ArrayList;  
- import java.util.Iterator;  
- import java.util.List;  
- import java.util.Map;  
- import java.util.Map.Entry;  
- import org.apache.http.HttpEntity;  
- import org.apache.http.HttpResponse;  
- import org.apache.http.NameValuePair;  
- import org.apache.http.client.HttpClient;  
- import org.apache.http.client.entity.UrlEncodedFormEntity;  
- import org.apache.http.client.methods.HttpPost;  
- import org.apache.http.message.BasicNameValuePair;  
- import org.apache.http.util.EntityUtils;  
- /*
-  * 利用HttpClient进行post请求的工具类
-  */
- publicclass HttpClientUtil {  
- public String doPost(String url,Map<String,String> map,String charset){  
-         HttpClient httpClient = null;  
-         HttpPost httpPost = null;  
-         String result = null;  
- try{  
-             httpClient = new SSLClient();  
-             httpPost = new HttpPost(url);  
- //设置参数
-             List<NameValuePair> list = new ArrayList<NameValuePair>();  
-             Iterator iterator = map.entrySet().iterator();  
- while(iterator.hasNext()){  
-                 Entry<String,String> elem = (Entry<String, String>) iterator.next();  
-                 list.add(new BasicNameValuePair(elem.getKey(),elem.getValue()));  
-             }  
- if(list.size() > 0){  
-                 UrlEncodedFormEntity entity = new UrlEncodedFormEntity(list,charset);  
-                 httpPost.setEntity(entity);  
-             }  
-             HttpResponse response = httpClient.execute(httpPost);  
- if(response != null){  
-                 HttpEntity resEntity = response.getEntity();  
- if(resEntity != null){  
-                     result = EntityUtils.toString(resEntity,charset);  
-                 }  
-             }  
-         }catch(Exception ex){  
-             ex.printStackTrace();  
-         }  
- return result;  
-     }  
- }  
3.调用post请求的测试代码
**[java]**[view
 plain](http://blog.csdn.net/rongyongfeikai2/article/details/41659353#)[copy](http://blog.csdn.net/rongyongfeikai2/article/details/41659353#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/538971/fork)
- import java.util.HashMap;  
- import java.util.Map;  
- //对接口进行测试
- publicclass TestMain {  
- private String url = "https://192.168.1.101/";  
- private String charset = "utf-8";  
- private HttpClientUtil httpClientUtil = null;  
- 
- public TestMain(){  
-         httpClientUtil = new HttpClientUtil();  
-     }  
- 
- publicvoid test(){  
-         String httpOrgCreateTest = url + "httpOrg/create";  
-         Map<String,String> createMap = new HashMap<String,String>();  
-         createMap.put("authuser","*****");  
-         createMap.put("authpass","*****");  
-         createMap.put("orgkey","****");  
-         createMap.put("orgname","****");  
-         String httpOrgCreateTestRtn = httpClientUtil.doPost(httpOrgCreateTest,createMap,charset);  
-         System.out.println("result:"+httpOrgCreateTestRtn);  
-     }  
- 
- publicstaticvoid main(String[] args){  
-         TestMain main = new TestMain();  
-         main.test();  
-     }  
- }  
httpClient4.2的jar包下载路径：http://download.csdn.net/detail/hqmryang/4582440#comment
