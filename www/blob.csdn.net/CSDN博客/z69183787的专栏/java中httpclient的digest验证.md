# java中httpclient的digest验证 - z69183787的专栏 - CSDN博客
2015年07月31日 15:41:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5102
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
```java
package test.util;
import java.net.URI;
import java.util.Collections;
import org.apache.commons.httpclient.auth.AuthPolicy;
import org.apache.http.HttpResponse;
import org.apache.http.auth.AuthScope;
import org.apache.http.auth.Credentials;
import org.apache.http.auth.UsernamePasswordCredentials;
import org.apache.http.client.CredentialsProvider;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.auth.BasicSchemeFactory;
import org.apache.http.impl.client.DefaultHttpClient;
/**
 * @author Administrator
 * 
 */
public class HTTPBasicClient {
	private URI serverURI = null;
	String response = null;
	private DefaultHttpClient httpClient = new DefaultHttpClient();
	/**
	 * constructor
	 * 
	 */
	public HTTPBasicClient(String userName, String passWord, String url) {
		try {
			serverURI = new URI(url);
			UsernamePasswordCredentials creds = new UsernamePasswordCredentials(
					userName, passWord);
			CredentialsProvider credsProvider = httpClient
					.getCredentialsProvider();
			credsProvider.setCredentials(new AuthScope(serverURI.getHost(),
					serverURI.getPort()), (Credentials) creds);
			httpClient.getParams().setParameter(
					AuthPolicy.AUTH_SCHEME_PRIORITY,
					Collections.singleton(AuthPolicy.BASIC));
			httpClient.getAuthSchemes().register(AuthPolicy.BASIC,
					new BasicSchemeFactory());
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
