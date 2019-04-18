# HttpClient post 请求实例 - z69183787的专栏 - CSDN博客
2014年02月13日 10:29:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：85051
所需jar包：
commons-codec-1.3.jar
commons-httpclient-3.0.jar
commons-logging-1.1.1.jar
```java
/**
 * 
 */
package httpClient;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URLEncoder;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;
/** 
 * @ClassName: SimpleClient 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月13日 上午9:35:17 
 *  
 */
public class SimpleClient {
	public static void main(String[] args) throws IOException
    {
        HttpClient client = new HttpClient();   
        //设置代理服务器地址和端口     
     //client.getHostConfiguration().setProxy("proxy_host_addr",proxy_port);
        //使用GET方法，如果服务器需要通过HTTPS连接，那只需要将下面URL中的http换成https
       // HttpMethod method = new GetMethod("http://10.1.14.20:8088/workflowController/service/todo/addTask"); 
        //使用POST方法
        PostMethod method = new PostMethod("http://10.1.14.20:8088/workflowController/service/todo/addTask"); 
     
        
        String s = "http://10.1.48.16:8080/workflow/send-tDocSend/toFormPage.action?modelName=%E6%96%B0%E5%8F%91%E6%96%87%E6%B5%81%E7%A8%8B&incidentNo=65&processName=%E6%96%B0%E5%8F%91%E6%96%87%E6%B5%81%E7%A8%8B&pinstanceId=65&taskUserName=ST/G001000001612549&stepName=%E5%8F%91%E6%96%87%E9%80%9A%E7%9F%A5&taskId=12261064757e7498937e6b29ea80ca&taskuser=ST/G001000001612549&codeId=13";
		
     String ms = "{\"app\": \"standardWork\",\"type\": 0,"
 			+ "\"occurTime\": \"2013-11-14 11:22:02\",\"title\": \"-------流程标题-------\","
 			+ "\"loginName\": \"ST/G01008000311\",\"status\": 0,\"removed\": 0,"
 			+ " \"typename\": \"流程名称11\","
 			+ "\"url\": \""+URLEncoder.encode(s,"UTF-8")+"\","
 			+ "\"pname\": \"主流程名称\",\"pincident\": 1,"
 			+ "\"cname\": \"子流程实例号\",\"cincident\": 1,"
 			+ "\"stepName\": \"当前步骤\","
 			+ "\"initiator\": \"ST/G01008000311\"}";
    	 ((PostMethod) method).addParameter("data", ms);
    	 
		HttpMethodParams param = method.getParams();
		param.setContentCharset("UTF-8");
     
     client.executeMethod(method);
        //打印服务器返回的状态
     System.out.println(method.getStatusLine());
       //打印返回的信息
     System.out.println();
     InputStream stream = method.getResponseBodyAsStream();
     
     BufferedReader br = new BufferedReader(new InputStreamReader(stream, "UTF-8"));
     StringBuffer buf = new StringBuffer();
     String line;
     while (null != (line = br.readLine())) {
         buf.append(line).append("\n");
     }
     System.out.println(buf.toString());
       //释放连接
     method.releaseConnection();
    }
}
```
另附上 新浪微博 上的 调用demo中的一些 方法：
```java
public HttpClient(int maxConPerHost, int conTimeOutMs, int soTimeOutMs,
			int maxSize) {
		connectionManager = new MultiThreadedHttpConnectionManager();
		HttpConnectionManagerParams params = connectionManager.getParams();
		params.setDefaultMaxConnectionsPerHost(maxConPerHost);
		params.setConnectionTimeout(conTimeOutMs);
		params.setSoTimeout(soTimeOutMs);
		HttpClientParams clientParams = new HttpClientParams();
		// 忽略cookie 避免 Cookie rejected 警告
		clientParams.setCookiePolicy(CookiePolicy.IGNORE_COOKIES);
		client = new org.apache.commons.httpclient.HttpClient(clientParams,
				connectionManager);
		Protocol myhttps = new Protocol("https", new MySSLSocketFactory(), 443);
		Protocol.registerProtocol("https", myhttps);
		this.maxSize = maxSize;
		// 支持proxy
		if (proxyHost != null && !proxyHost.equals("")) {
			client.getHostConfiguration().setProxy(proxyHost, proxyPort);
			client.getParams().setAuthenticationPreemptive(true);
			if (proxyAuthUser != null && !proxyAuthUser.equals("")) {
				client.getState().setProxyCredentials(
						AuthScope.ANY,
						new UsernamePasswordCredentials(proxyAuthUser,
								proxyAuthPassword));
				log("Proxy AuthUser: " + proxyAuthUser);
				log("Proxy AuthPassword: " + proxyAuthPassword);
			}
		}
	}
	/**
	 * log调试
	 * 
	 */
	private static void log(String message) {
		if (DEBUG) {
			log.debug(message);
		}
	}
	/**
	 * 处理http getmethod 请求
	 * 
	 */
	public Response get(String url) throws WeiboException {
		return get(url, new PostParameter[0]);
	}
	public Response get(String url, PostParameter[] params)
			throws WeiboException {
		log("Request:");
		log("GET:" + url);
		if (null != params && params.length > 0) {
			String encodedParams = HttpClient.encodeParameters(params);
			if (-1 == url.indexOf("?")) {
				url += "?" + encodedParams;
			} else {
				url += "&" + encodedParams;
			}
		}
		GetMethod getmethod = new GetMethod(url);
		return httpRequest(getmethod);
	}
	public Response get(String url, PostParameter[] params, Paging paging)
			throws WeiboException {
		if (null != paging) {
			List<PostParameter> pagingParams = new ArrayList<PostParameter>(4);
			if (-1 != paging.getMaxId()) {
				pagingParams.add(new PostParameter("max_id", String
						.valueOf(paging.getMaxId())));
			}
			if (-1 != paging.getSinceId()) {
				pagingParams.add(new PostParameter("since_id", String
						.valueOf(paging.getSinceId())));
			}
			if (-1 != paging.getPage()) {
				pagingParams.add(new PostParameter("page", String
						.valueOf(paging.getPage())));
			}
			if (-1 != paging.getCount()) {
				if (-1 != url.indexOf("search")) {
					// search api takes "rpp"
					pagingParams.add(new PostParameter("rpp", String
							.valueOf(paging.getCount())));
				} else {
					pagingParams.add(new PostParameter("count", String
							.valueOf(paging.getCount())));
				}
			}
			PostParameter[] newparams = null;
			PostParameter[] arrayPagingParams = pagingParams
					.toArray(new PostParameter[pagingParams.size()]);
			if (null != params) {
				newparams = new PostParameter[params.length
						+ pagingParams.size()];
				System.arraycopy(params, 0, newparams, 0, params.length);
				System.arraycopy(arrayPagingParams, 0, newparams,
						params.length, pagingParams.size());
			} else {
				if (0 != arrayPagingParams.length) {
					String encodedParams = HttpClient
							.encodeParameters(arrayPagingParams);
					if (-1 != url.indexOf("?")) {
						url += "&" + encodedParams;
					} else {
						url += "?" + encodedParams;
					}
				}
			}
			return get(url, newparams);
		} else {
			return get(url, params);
		}
	}
	/**
	 * 处理http deletemethod请求
	 */
	public Response delete(String url, PostParameter[] params)
			throws WeiboException {
		if (0 != params.length) {
			String encodedParams = HttpClient.encodeParameters(params);
			if (-1 == url.indexOf("?")) {
				url += "?" + encodedParams;
			} else {
				url += "&" + encodedParams;
			}
		}
		DeleteMethod deleteMethod = new DeleteMethod(url);
		return httpRequest(deleteMethod);
	}
	/**
	 * 处理http post请求
	 * 
	 */
	public Response post(String url, PostParameter[] params)
			throws WeiboException {
		return post(url, params, true);
	}
	public Response post(String url, PostParameter[] params,
			Boolean WithTokenHeader) throws WeiboException {
		log("Request:");
		log("POST" + url);
		PostMethod postMethod = new PostMethod(url);
		for (int i = 0; i < params.length; i++) {
			postMethod.addParameter(params[i].getName(), params[i].getValue());
		}
		HttpMethodParams param = postMethod.getParams();
		param.setContentCharset("UTF-8");
		if (WithTokenHeader) {
			return httpRequest(postMethod);
		} else {
			return httpRequest(postMethod, WithTokenHeader);
		}
	}
	/**
	 * 支持multipart方式上传图片
	 * 
	 */
	public Response multPartURL(String url, PostParameter[] params,
			ImageItem item) throws WeiboException {
		PostMethod postMethod = new PostMethod(url);
		try {
			Part[] parts = null;
			if (params == null) {
				parts = new Part[1];
			} else {
				parts = new Part[params.length + 1];
			}
			if (params != null) {
				int i = 0;
				for (PostParameter entry : params) {
					parts[i++] = new StringPart(entry.getName(),
							(String) entry.getValue());
				}
				parts[parts.length - 1] = new ByteArrayPart(item.getContent(),
						item.getName(), item.getContentType());
			}
			postMethod.setRequestEntity(new MultipartRequestEntity(parts,
					postMethod.getParams()));
			return httpRequest(postMethod);
		} catch (Exception ex) {
			throw new WeiboException(ex.getMessage(), ex, -1);
		}
	}
	public Response multPartURL(String fileParamName, String url,
			PostParameter[] params, File file, boolean authenticated)
			throws WeiboException {
		PostMethod postMethod = new PostMethod(url);
		try {
			Part[] parts = null;
			if (params == null) {
				parts = new Part[1];
			} else {
				parts = new Part[params.length + 1];
			}
			if (params != null) {
				int i = 0;
				for (PostParameter entry : params) {
					parts[i++] = new StringPart(entry.getName(),
							(String) entry.getValue());
				}
			}
			FilePart filePart = new FilePart(fileParamName, file.getName(),
					file, new MimetypesFileTypeMap().getContentType(file),
					"UTF-8");
			filePart.setTransferEncoding("binary");
			parts[parts.length - 1] = filePart;
			postMethod.setRequestEntity(new MultipartRequestEntity(parts,
					postMethod.getParams()));
			return httpRequest(postMethod);
		} catch (Exception ex) {
			throw new WeiboException(ex.getMessage(), ex, -1);
		}
	}
	public Response httpRequest(HttpMethod method) throws WeiboException {
		return httpRequest(method, true);
	}
```
