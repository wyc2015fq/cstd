# httpclient4.5的get和post方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月25日 10:59:33[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7195








```java
package an;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.methods.RequestBuilder;
import org.apache.http.cookie.Cookie;
import org.apache.http.impl.client.BasicCookieStore;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;

public class PostForm {
	
	/*
	 * 自动填单
	 */
	public String hcPost() throws ClientProtocolException, IOException, URISyntaxException  {
		
		BasicCookieStore cookieStore = new BasicCookieStore();
        CloseableHttpClient httpclient = HttpClients.custom()
                .setDefaultCookieStore(cookieStore)
                .build();
        try {
            HttpGet httpget = new HttpGet("https://passport.csdn.net/account/login");
            CloseableHttpResponse response1 = httpclient.execute(httpget);
            try {
                HttpEntity entity = response1.getEntity();

                System.out.println("Login form get: " + response1.getStatusLine());
                EntityUtils.consume(entity);
                
                System.out.println("Initial set of cookies:");
                List<Cookie> cookies = cookieStore.getCookies();
                if (cookies.isEmpty()) {
                    System.out.println("None");
                } else {
                    for (int i = 0; i < cookies.size(); i++) {
                        System.out.println("- " + cookies.get(i).toString());
                    }
                }
            } finally {
                response1.close();
            }

            HttpUriRequest login = RequestBuilder.post()
                    .setUri(new URI("https://passport.csdn.net/account/login"))
                    .addParameter("username", "my")
                    .addParameter("password", "123")
                    //.addParameter("IDToken2", "password")
                    .build();
            CloseableHttpResponse response2 = httpclient.execute(login);
            try {
                HttpEntity entity = response2.getEntity();

                System.out.println("Login form get: " + response2.getStatusLine());
                EntityUtils.consume(entity);
                
                System.out.println("Post logon cookies:");
                List<Cookie> cookies = cookieStore.getCookies();
                if (cookies.isEmpty()) {
                    System.out.println("None");
                } else {
                    for (int i = 0; i < cookies.size(); i++) {
                        System.out.println("- " + cookies.get(i).toString());
                    }
                }
            } finally {
                response2.close();
            }
        } finally {
            httpclient.close();
        }
        return "yes";
	}
	
	public static void main(String[]args) throws Exception{
		
		PostForm pf=new PostForm();	
        
        String ret=pf.hcPost();
        
        System.out.println("********"+ret);  
  
	}
}
```

httpclient提交后用htmlparser解析内容：



```java
package an;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.http.HttpEntity;
import org.apache.http.ParseException;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.methods.RequestBuilder;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.htmlparser.Parser;
import org.htmlparser.beans.StringBean;
import org.htmlparser.util.ParserException;

public class PostForm {
	
	public String hcPost() throws ClientProtocolException, IOException, URISyntaxException, ParserException, ParseException  {
		
        CloseableHttpClient httpclient = HttpClients.custom().build();
        HttpUriRequest post = RequestBuilder.post()
                    .setUri(new URI("http://mail.163.com/"))
                    .addParameter("username", "my")
                    .addParameter("password", "123")
                    //.addParameter("IDToken2", "password")
                    .build();
        CloseableHttpResponse response = httpclient.execute(post); 
        try {  
        	HttpEntity entity = response.getEntity();//中文乱码
            if( response.getStatusLine().getStatusCode() == 200){
            	StringBean sb = new StringBean ();
        		sb.setLinks (false);//设置不需要得到页面所包含的链接信息  
        		sb.setReplaceNonBreakingSpaces(true);//设置将不间断空格由正规空格所替代  
        		sb.setCollapse(true);//设置将一序列空格由一个单一空格所代替  
        		Parser parser = new Parser();
        		parser.setInputHTML(EntityUtils.toString(entity));
        		parser.reset ();
        		parser.visitAllNodesWith (sb);
        		String text = sb.getStrings ();
            	return text;
            }
            
        } finally { response.close();}
        return response.getStatusLine().toString();
	}
	
	public static void main(String[]args) throws Exception{
		
		PostForm pf=new PostForm();	
        
        String ret=pf.hcPost();
        
        System.out.println("********"+ret);  
  
	}
}
```







