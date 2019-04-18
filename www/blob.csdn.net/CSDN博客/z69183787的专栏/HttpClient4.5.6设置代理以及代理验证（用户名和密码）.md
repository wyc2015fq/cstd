# HttpClient4.5.6设置代理以及代理验证（用户名和密码） - z69183787的专栏 - CSDN博客
2018年12月10日 17:55:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：140
[https://blog.csdn.net/manong_123/article/details/82531004](https://blog.csdn.net/manong_123/article/details/82531004)
HttpClient4.5.6设置代理以及代理验证（用户名和密码）
下载 httpcomponents-client-4.5.6-bin 在Apache官网有，贴上链接：http://hc.apache.org/downloads.cgi 下载以后把对应的包导入工程即可（java和maven工程都行），下面说明如何设置代理验证:
```java
import java.io.IOException;
import org.apache.http.HttpHost;
import org.apache.http.auth.AuthScope;
import org.apache.http.auth.UsernamePasswordCredentials;
import org.apache.http.client.CredentialsProvider;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.BasicCredentialsProvider;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.junit.jupiter.api.Test;
public class DownloadPage
{
    @Test
    public void downLoad()
    {
        try
        {
            // 设置代理HttpHost
            HttpHost proxy = new HttpHost("openproxy.huawei.com", 8080, "http");
            // 设置要访问的HttpHost,即是目标站点的HttpHost
            HttpHost target = new HttpHost("baidu.com", 80);
            // 设置认证
            CredentialsProvider provider = new BasicCredentialsProvider();
            provider.setCredentials(new AuthScope(proxy), new UsernamePasswordCredentials("username", "password"));
            CloseableHttpClient httpClient = HttpClients.custom().setDefaultCredentialsProvider(provider).build();
            RequestConfig config = RequestConfig.custom().setProxy(proxy).build();
            HttpGet httpGet = new HttpGet("/");
            httpGet.setConfig(config);
            CloseableHttpResponse resp = httpClient.execute(target, httpGet);
            if (resp.getStatusLine().getStatusCode() == 200)
            {
                System.out.println("OK");
            }
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
}
```
在执行请求时，请求设置用代理进行访问，但是代理需要验证，那么就会找到provider设置相关联的用户名和密码进行验证，所以设置provider时需要注意关联。说明一下setCredentials()这个方法可以添加多个验证账号和密码，看源代码可知就是ConcurrentHashMap（Map）的操作。
