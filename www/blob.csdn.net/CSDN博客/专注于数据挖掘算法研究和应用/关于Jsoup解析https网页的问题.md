# 关于Jsoup解析https网页的问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月16日 10:46:59[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：10147








针对Jsoup解析https网页，网上的一段源码执行后并不能实现成功访问。



```java
import java.net.MalformedURLException;  
import java.net.URL;  
import java.security.SecureRandom;  
import java.security.cert.CertificateException;  
import java.security.cert.X509Certificate;
import java.util.Map;  
  
import javax.net.ssl.HostnameVerifier;  
import javax.net.ssl.HttpsURLConnection;  
import javax.net.ssl.SSLContext;  
import javax.net.ssl.SSLSession;  
import javax.net.ssl.X509TrustManager;  
  
import org.jsoup.Connection;
import org.jsoup.helper.HttpConnection;

public class HTTPCommonUtil {
	
	public static void trustEveryone() { 
		try {  
            HttpsURLConnection.setDefaultHostnameVerifier(new HostnameVerifier() {  
                public boolean verify(String hostname, SSLSession session) {  
                    return true;  
                }  
            });  
  
            SSLContext context = SSLContext.getInstance("TLS");  
            context.init(null, new X509TrustManager[] { new X509TrustManager() {  
                public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {  
                }  
  
                public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {  
                }  
  
                public X509Certificate[] getAcceptedIssuers() {  
                    return new X509Certificate[0];  
                }  
            } }, new SecureRandom());  
            HttpsURLConnection.setDefaultSSLSocketFactory(context.getSocketFactory());  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }  
  
    public static Object getHttpHeaders(URL url, int timeout) {  
        try {  
            trustEveryone(); 
            Connection conn = HttpConnection.connect(url);  
            conn.timeout(timeout);  
            conn.header("Accept-Encoding", "gzip,deflate,sdch");  
            conn.header("Connection", "close");  
            conn.get();  
            //String result=conn.response().body();
            Map<String, String> result = conn.response().headers();  
            result.put("title", conn.response().parse().title());  
            return result; 
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
        return null;  
    }  
  
    public static void main(String[] args) {  
        try {  
            URL url = new URL("https", "www.icbc-axa.com", -1, "");  
            System.out.println(getHttpHeaders(url, 10000));  
        } catch (MalformedURLException e) {  
            e.printStackTrace();  
        }  
    }  
}
```



执行结果：{Content-Length=187, Connection=close, Pragma=no-cache, Cache-Control=no-cache, title=Request Rejected}

需要进一步寻找方案，通过java自带HttpsURLConnection可实现https访问，实际上上面这段代码的trustEveryone()函数也是通过HttpsURLConnection，只是如何结合到jsoup尚未找到有效办法，先转到htmlparser来实现。



