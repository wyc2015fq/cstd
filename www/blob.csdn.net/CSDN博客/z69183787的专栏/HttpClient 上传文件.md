# HttpClient 上传文件 - z69183787的专栏 - CSDN博客
2017年12月06日 10:31:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：154
个人分类：[Httpclient-简介](https://blog.csdn.net/z69183787/article/category/2185975)
```java
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.mime.MultipartEntityBuilder;
import org.apache.http.entity.mime.content.ByteArrayBody;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Date;
public class ImageUploadClient {
    private static final Logger logger = LoggerFactory.getLogger(ImageUploadClient.class);
    private String client_id = "";
    private String client_secret = "";
    private String url = "";
    public ImageUploadClient(String url, String client_id, String client_secret) {
        this.url = url;
        this.client_id = client_id;
        this.client_secret = client_secret;
    }
    public String postFile(HttpEntity httpEntity) {
        CloseableHttpClient httpclient = HttpClients.createDefault();
        HttpPost post = new HttpPost(url);
        try {
            Date date = new Date();
            URL u;
            try {
                u = new URL(url);
            } catch (MalformedURLException e) {
                logger.error(e.getMessage(), e);
                throw e;
            }
            // 设置Authorization
            if (client_id != null) {
                String uri = u.getPath();
                String dateString = "";
                String authorization = "";
                post.setHeader("Date", dateString);
                post.setHeader("Authorization", authorization);
            }
            if (httpEntity != null) {
                post.setEntity(httpEntity);
            }
            HttpResponse response = httpclient.execute(post);
            if (response.getStatusLine().getStatusCode() == 200) {
                return EntityUtils.toString(response.getEntity());
            }
            return null;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            return null;
        } finally {
            try {
                httpclient.close();
            } catch (Exception e) {
            }
        }
    }
    public String postFile(byte[] bytes, String filename) {
        if (bytes != null && filename != null) {
            HttpEntity httpEntity = MultipartEntityBuilder.create().addPart("file", new ByteArrayBody(bytes, ContentType.DEFAULT_BINARY, filename)).build();
            return this.postFile(httpEntity);
        } else {
            return null;
        }
    }
    public String postFile(File file) {
        if (file != null) {
            HttpEntity httpEntity = MultipartEntityBuilder.create().addPart("file", new FileBody(file)).build();
            return this.postFile(httpEntity);
        } else {
            return null;
        }
    }
}
```
