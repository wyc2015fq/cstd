# HttpClient实现CSDN自动登录 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年05月20日 12:21:23[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3395








参考网上代码，通过httpclient实现CSDN自动登录，问题是对于网页表单JS动态加载的还没找到办法自动填写和提交。

1、HttpUtils类封装httpclient的get和post方法



```java
package gddx;

import java.io.IOException;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.protocol.HttpClientContext;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.apache.http.NameValuePair;

public class HttpUtils {
	
	private static CloseableHttpClient httpClient = HttpClients.createDefault();  
    private static HttpClientContext context = new HttpClientContext();  
    
    private HttpUtils() {  
    	  
    } 
    
    public static String sendGet(String url) {  
        CloseableHttpResponse response = null;  
        String content = null;  
        try {  
            HttpGet get = new HttpGet(url);  
            response = httpClient.execute(get, context);  
            HttpEntity entity = response.getEntity();  
            content = EntityUtils.toString(entity);  
            EntityUtils.consume(entity);  
            return content;  
        } catch (Exception e) {  
            e.printStackTrace();  
            if (response != null) {  
                try {  
                    response.close();  
                } catch (IOException e1) {  
                    e1.printStackTrace();  
                }  
            }  
        }  
        return content;  
    }  
  
    public static String sendPost(String url, List<NameValuePair> nvps) {  
        CloseableHttpResponse response = null;  
        String content = null;  
        try {  
            //　HttpClient中的post请求包装类  
            HttpPost post = new HttpPost(url);  
            // nvps是包装请求参数的list  
            if (nvps != null) {  
                post.setEntity(new UrlEncodedFormEntity(nvps, "UTF-8"));  
            }  
            // 执行请求用execute方法，content用来帮我们附带上额外信息  
            response = httpClient.execute(post, context);  
            // 得到相应实体、包括响应头以及相应内容  
            HttpEntity entity = response.getEntity();  
            // 得到response的内容  
            content = EntityUtils.toString(entity);  
            //输出html
            System.out.println(content);
            //　关闭输入流  
            EntityUtils.consume(entity);  
            return content;  
        } catch (Exception e) {  
            e.printStackTrace();  
        } finally {  
            if (response != null) {  
                try {  
                    response.close();  
                } catch (IOException e) {  
                    e.printStackTrace();  
                }  
            }  
        }  
        return content;  
    }  
}
```

2、CSDNLogin实现自动登录



```java
package gddx;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;

/**
 * @author Jason.F
 * Date 2016年5月19日
 * **/
public class CSDNLogin {
	
	private String loginURL = "https://passport.csdn.net/account/login";
	private String lt=null;
	private String execution=null;
	private String _eventId=null;
	/** 
	 * 获取必要的登陆参数信息 
	 *  
	 * @throws IOException 
	 */  
	private void fetchNecessaryParam(){  
	    // 查看CSDN登陆页面源码发现登陆时需要post5个参数  
	    // name、password，另外三个在页面的隐藏域中，a good start  
		System.out.println("获取登录表单参数。。。。。");  
	    // 这样登陆不行，因为第一次需要访问需要拿到上下文context  
	    // Document doc = Jsoup.connect(LOGIN_URL).get();  
	    String html = HttpUtils.sendGet(loginURL);  
	    Document doc = Jsoup.parse(html);  
	    Element form = doc.select(".user-pass").get(0);  
	    lt = form.select("input[name=lt]").get(0).val();  
	    execution = form.select("input[name=execution]").get(0).val();  
	    _eventId = form.select("input[name=_eventId]").get(0).val();  
	    System.out.println("获取成功。。。。。");  
	}  
	
	private void login() {  
		fetchNecessaryParam();
		
		System.out.println("开始登陆。。。。。");  
	    List<NameValuePair> nvps = new ArrayList<NameValuePair>();  
	    nvps.add(new BasicNameValuePair("username", "fjssharpsword"));  
	    nvps.add(new BasicNameValuePair("password", "xyz"));  
	    nvps.add(new BasicNameValuePair("lt", lt));  
	    nvps.add(new BasicNameValuePair("execution", execution));  
	    nvps.add(new BasicNameValuePair("_eventId", _eventId));  
	    String ret = HttpUtils.sendPost(loginURL, nvps);  
	    if (ret.indexOf("redirect_back") > -1) {  
	    	System.out.println("登陆成功。。。。。");  

	    } else if (ret.indexOf("登录太频繁") > -1) {  
	    	System.out.println("登录太频繁，请稍后再试。。。。。");  
	    } else {  
	    	System.out.println("登陆失败。。。。。");  
	    }  
	}  
	
	public static void main(String[] args) {
		CSDNLogin lession = new CSDNLogin();
        lession.login();
    }
}
```





