# java中httpclient实现digest验证的请求 - 零度的博客专栏 - CSDN博客
2016年05月11日 10:11:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：4112
1、首先介绍如何使用HttpClient发起GET和POST请求
```
GET 方式：
//先将参数放入List，再对参数进行URL编码
List<BasicNameValuePair> params = new LinkedList<BasicNameValuePair>();
params.add(new BasicNameValuePair("param1", "中国"));
params.add(new BasicNameValuePair("param2", "value2"));
 
//对参数编码
String param = URLEncodedUtils.format(params, "UTF-8");
 
//baseUrl           
String baseUrl = "
 
 
//将URL与参数拼接
HttpGet getMethod = new HttpGet(baseUrl + "?" + param);
             
HttpClient httpClient = new DefaultHttpClient();
 
try {
    HttpResponse response = httpClient.execute(getMet搜索hod); //发起GET请求
 
    Log.i(TAG, "resCode = " + response.getStatusLine().getStatusCode()); //获取响应码
    Log.i(TAG, "result = " + EntityUtils.toString(response.getEntity(), "utf-8"));//获取服务器响应内容
} catch (ClientProtocolException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
} catch (IOException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
} 
 
POST方式：
//和GET方式一样，先将参数放入List
params = new LinkedList<BasicNameValuePair>();
params.add(new BasicNameValuePair("param1", "Post方法"));
params.add(new BasicNameValuePair("param2", "第二个参数"));
             
try {
    HttpPost postMethod = new HttpPost(baseUrl);
    postMethod.setEntity(new UrlEncodedFormEntity(params, "utf-8")); //将参数填入POST Entity中
                 
    HttpResponse response = httpClient.execute(postMethod); //执行POST方法
    Log.i(TAG, "resCode = " + response.getStatusLine().getStatusCode()); //获取响应码
    Log.i(TAG, "result = " + EntityUtils.toString(response.getEntity(), "utf-8")); //获取响应内容
                 
} catch (UnsupportedEncodingException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
} catch (ClientProtocolException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
} catch (IOException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
}
```
 2、httpclient的digest验证，需要tomcat服务器配置digest验证，具体配置参见先前博文。                     
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/47171059#)[copy](http://blog.csdn.net/z69183787/article/details/47171059#)[print](http://blog.csdn.net/z69183787/article/details/47171059#)[?](http://blog.csdn.net/z69183787/article/details/47171059#)
- package test.util;  
- 
- import java.net.URI;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.apache.commons.httpclient.auth.AuthPolicy;  
- import org.apache.http.HttpResponse;  
- import org.apache.http.auth.AuthScope;  
- import org.apache.http.auth.Credentials;  
- import org.apache.http.auth.UsernamePasswordCredentials;  
- import org.apache.http.client.methods.HttpUriRequest;  
- import org.apache.http.impl.auth.DigestSchemeFactory;  
- import org.apache.http.impl.client.DefaultHttpClient;  
- 
- /**
-  * @author Administrator
-  * 
-  */
- publicclass HTTPDigestClient {  
- private URI serverURI = null;  
-     String response = null;  
- private DefaultHttpClient httpClient = new DefaultHttpClient();  
- 
- /**
-      * constructor
-      */
- public HTTPDigestClient(String userName, String passWord, String url) {  
- try {  
-             serverURI = new URI(url);  
-             Credentials creds = new UsernamePasswordCredentials(userName,  
-                     passWord);  
- 
-             httpClient.getCredentialsProvider().setCredentials(  
- new AuthScope(serverURI.getHost(), serverURI.getPort), (Credentials) creds);  
- 
-             httpClient.getParams().setParameter(  
-                     AuthPolicy.AUTH_SCHEME_PRIORITY, Collections.singleton(AuthPolicy.DIGEST));  
-             httpClient.getAuthSchemes().register(AuthPolicy.DIGEST,  
- new DigestSchemeFactory());  
- 
-         } catch (Exception e) {  
- 
-         }  
-     }  
- 
- /**
-      * send request to server
-      * 
-      * @param httpClient
-      * @param httpUriRequest
-      * @return response HttpResponse
-      */
- public HttpResponse send(HttpUriRequest httpUriRequest) {  
-         HttpResponse response = null;  
- try {  
- if (null == httpClient) {  
- 
- return response;  
-             }  
-             response = httpClient.execute(httpUriRequest);  
- 
-         } catch (Exception e) {  
- 
-         }  
- return response;  
-     }  
- 
- }  
```java
package test.util;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import org.apache.commons.httpclient.auth.AuthPolicy;
import org.apache.http.HttpResponse;
import org.apache.http.auth.AuthScope;
import org.apache.http.auth.Credentials;
import org.apache.http.auth.UsernamePasswordCredentials;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.auth.DigestSchemeFactory;
import org.apache.http.impl.client.DefaultHttpClient;
/**
 * @author Administrator
 * 
 */
public class HTTPDigestClient {
	private URI serverURI = null;
	String response = null;
	private DefaultHttpClient httpClient = new DefaultHttpClient();
	/**
	 * constructor
	 */
	public HTTPDigestClient(String userName, String passWord, String url) {
		try {
			serverURI = new URI(url);
			Credentials creds = new UsernamePasswordCredentials(userName,
					passWord);
			httpClient.getCredentialsProvider().setCredentials(
					new AuthScope(serverURI.getHost(), serverURI.getPort), (Credentials) creds);
			httpClient.getParams().setParameter(
					AuthPolicy.AUTH_SCHEME_PRIORITY, Collections.singleton(AuthPolicy.DIGEST));
			httpClient.getAuthSchemes().register(AuthPolicy.DIGEST,
					new DigestSchemeFactory());
		} catch (Exception e) {
			
		}
	}
	/**
	 * send request to server
	 * 
	 * @param httpClient
	 * @param httpUriRequest
	 * @return response HttpResponse
	 */
	public HttpResponse send(HttpUriRequest httpUriRequest) {
		HttpResponse response = null;
		try {
			if (null == httpClient) {
				
				return response;
			}
			response = httpClient.execute(httpUriRequest);
		} catch (Exception e) {
			
		}
		return response;
	}
}
```
服务器端必须加入域
设置realm 为空或者当前域名 测试成功
用户名 密码用的是域账户
下面为basic验证方式 ，测试通过
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/47171059#)[copy](http://blog.csdn.net/z69183787/article/details/47171059#)[print](http://blog.csdn.net/z69183787/article/details/47171059#)[?](http://blog.csdn.net/z69183787/article/details/47171059#)
- package test.util;  
- 
- import java.net.URI;  
- import java.util.Collections;  
- 
- import org.apache.commons.httpclient.auth.AuthPolicy;  
- import org.apache.http.HttpResponse;  
- import org.apache.http.auth.AuthScope;  
- import org.apache.http.auth.Credentials;  
- import org.apache.http.auth.UsernamePasswordCredentials;  
- import org.apache.http.client.CredentialsProvider;  
- import org.apache.http.client.methods.HttpUriRequest;  
- import org.apache.http.impl.auth.BasicSchemeFactory;  
- import org.apache.http.impl.client.DefaultHttpClient;  
- 
- /**
-  * @author Administrator
-  * 
-  */
- publicclass HTTPBasicClient {  
- 
- private URI serverURI = null;  
-     String response = null;  
- private DefaultHttpClient httpClient = new DefaultHttpClient();  
- 
- /**
-      * constructor
-      * 
-      */
- public HTTPBasicClient(String userName, String passWord, String url) {  
- try {  
-             serverURI = new URI(url);  
- 
-             UsernamePasswordCredentials creds = new UsernamePasswordCredentials(  
-                     userName, passWord);  
-             CredentialsProvider credsProvider = httpClient  
-                     .getCredentialsProvider();  
-             credsProvider.setCredentials(new AuthScope(serverURI.getHost(),  
-                     serverURI.getPort()), (Credentials) creds);  
-             httpClient.getParams().setParameter(  
-                     AuthPolicy.AUTH_SCHEME_PRIORITY,  
-                     Collections.singleton(AuthPolicy.BASIC));  
-             httpClient.getAuthSchemes().register(AuthPolicy.BASIC,  
- new BasicSchemeFactory());  
- 
-         } catch (Exception e) {  
- 
-         }  
-     }  
- 
- /**
-      * send request to server
-      * 
-      * @param httpClient
-      * @param httpUriRequest
-      * @return response HttpResponse
-      */
- public HttpResponse send(HttpUriRequest httpUriRequest) {  
-         HttpResponse response = null;  
- try {  
- if (null == httpClient) {  
- 
- return response;  
-             }  
-             response = httpClient.execute(httpUriRequest);  
- 
-         } catch (Exception e) {  
- 
-         }  
- return response;  
-     }  
- 
- }  
