# HttpClient之HttpAsync使用 - 零度的博客专栏 - CSDN博客
2016年11月28日 13:41:31[零度anngle](https://me.csdn.net/zmx729618)阅读数：2894
pom.xml
```
<httpclient.version>4.5</httpclient.version>
<http-async-client.version>4.1</http-async-client.version>
<dependencies>
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>${lombok.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpasyncclient</artifactId>
            <version>${http-async-client.version}</version>
        </dependency>
        <!-- http client -->
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
            <version>${httpclient.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpmime</artifactId>
            <version>${httpclient.version}</version>
        </dependency>
    </dependencies>
```
async方式：
```
import org.apache.http.*;
import org.apache.http.client.CookieStore;
import org.apache.http.client.config.AuthSchemes;
import org.apache.http.client.config.CookieSpecs;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.protocol.HttpClientContext;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.config.Registry;
import org.apache.http.config.RegistryBuilder;
import org.apache.http.conn.socket.ConnectionSocketFactory;
import org.apache.http.conn.socket.PlainConnectionSocketFactory;
import org.apache.http.conn.ssl.NoopHostnameVerifier;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.impl.client.*;
import org.apache.http.impl.conn.PoolingHttpClientConnectionManager;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClientBuilder;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.apache.http.impl.nio.conn.PoolingNHttpClientConnectionManager;
import org.apache.http.impl.nio.reactor.DefaultConnectingIOReactor;
import org.apache.http.util.EntityUtils;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Future;
/**
 * Created by Administrator on 2015/11/28.
 */
public class HttpAsyncClientUtil {
    private static HttpClientContext context = HttpClientContext.create();
    /**
     * http async get
     *
     * @param url
     * @param data
     * @return
     */
    public static void doGet(String url, String data) {
        CookieStore cookieStore = new BasicCookieStore();
        final CloseableHttpAsyncClient httpClient = HttpAsyncClients.createDefault();
        httpClient.start();
        HttpGet httpGet = new HttpGet(url);
        //httpGet.setHeader("Content-Type", "application/x-www-form-urlencoded");
        try {
            httpClient.execute(httpGet, context, new FutureCallback<HttpResponse>() {
                @Override
                public void completed(HttpResponse result) {
                    String body="";
                    //这里使用EntityUtils.toString()方式时会大概率报错，原因：未接受完毕，链接已关
                    try {
                        HttpEntity entity = result.getEntity();
                        if (entity != null) {
                            final InputStream instream = entity.getContent();
                            try {
                                final StringBuilder sb = new StringBuilder();
                                final char[] tmp = new char[1024];
                                final Reader reader = new InputStreamReader(instream,"UTF-8");
                                int l;
                                while ((l = reader.read(tmp)) != -1) {
                                    sb.append(tmp, 0, l);
                                }
                                body = sb.toString();
                                System.out.println(body);
                            } finally {
                                instream.close();
                                EntityUtils.consume(entity);
                            }
                        }
                    } catch (ParseException e) {
                        e.printStackTrace();
                    }catch (IOException e) {
                        e.printStackTrace();
                    }finally {
                        close(httpClient);
                    }
                }
                @Override
                public void failed(Exception ex) {
                    System.out.println(ex.toString());
                    close(httpClient);
                }
                @Override
                public void cancelled() {
                }
            });
        } catch (Exception e) {
        }
        System.out.println("end-----------------------");
    }
    /**
     * http async post
     *
     * @param url
     * @param values
     * @return
     */
    public static void doPost(String url, List<NameValuePair> values) {
        CookieStore cookieStore = new BasicCookieStore();
        CloseableHttpAsyncClient httpClient = HttpAsyncClients.createDefault();
        HttpPost httpPost = new HttpPost(url);
        UrlEncodedFormEntity entity = new UrlEncodedFormEntity(values, Consts.UTF_8);
        httpPost.setEntity(entity);
        try {
            httpClient.execute(httpPost, context, new FutureCallback<HttpResponse>() {
                @Override
                public void completed(HttpResponse result) {
                    System.out.println(result.toString());
                }
                @Override
                public void failed(Exception ex) {
                    System.out.println(ex.toString());
                }
                @Override
                public void cancelled() {
                }
            });
        } catch (Exception e) {
        }
    }
    private static void close(CloseableHttpAsyncClient client) {
        try {
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    /**
     * 直接把Response内的Entity内容转换成String
     *
     * @param httpResponse
     * @return
     */
    public static String toString(CloseableHttpResponse httpResponse) {
        // 获取响应消息实体
        String result = null;
        try {
            HttpEntity entity = httpResponse.getEntity();
            if (entity != null) {
                result = EntityUtils.toString(entity, "UTF-8");
            }
        } catch (Exception e) {
        } finally {
            try {
                httpResponse.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return result;
    }
    public static void main(String[] args) throws Exception{
        CloseableHttpAsyncClient httpclient = HttpAsyncClients.createDefault();
        try {
            httpclient.start();
            HttpGet request = new HttpGet("http://www.apache.org/");
            Future<HttpResponse> future = httpclient.execute(request, null);
            HttpResponse response = future.get();
            System.out.println("Response: " + response.getStatusLine());
            System.out.println("Shutting down");
        } finally {
            httpclient.close();
        }
        System.out.println("Done");
    }
}
```
正常方式：
```
import org.apache.http.Consts;
import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.client.CookieStore;
import org.apache.http.client.config.AuthSchemes;
import org.apache.http.client.config.CookieSpecs;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.protocol.HttpClientContext;
import org.apache.http.config.Registry;
import org.apache.http.config.RegistryBuilder;
import org.apache.http.conn.socket.ConnectionSocketFactory;
import org.apache.http.conn.socket.PlainConnectionSocketFactory;
import org.apache.http.conn.ssl.NoopHostnameVerifier;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.impl.client.*;
import org.apache.http.impl.conn.PoolingHttpClientConnectionManager;
import org.apache.http.util.EntityUtils;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
/**
 * Created by Administrator on 2015/11/28.
 */
public class HttpClientUtil {
    private static HttpClientContext context = HttpClientContext.create();
    private static RequestConfig requestConfig = RequestConfig.custom().setConnectTimeout(120000).setSocketTimeout(60000)
            .setConnectionRequestTimeout(60000).setCookieSpec(CookieSpecs.STANDARD_STRICT).
                    setExpectContinueEnabled(true).
                    setTargetPreferredAuthSchemes(Arrays.asList(AuthSchemes.NTLM, AuthSchemes.DIGEST)).
                    setProxyPreferredAuthSchemes(Arrays.asList(AuthSchemes.BASIC)).build();
    //https
    private static SSLConnectionSocketFactory socketFactory;
    private static TrustManager manager = new X509TrustManager() {
        @Override
        public void checkClientTrusted(X509Certificate[] x509Certificates, String s) throws CertificateException {
        }
        @Override
        public void checkServerTrusted(X509Certificate[] x509Certificates, String s) throws CertificateException {
        }
        @Override
        public X509Certificate[] getAcceptedIssuers() {
            return null;
        }
    };
    private static void enableSSL() {
        try {
            SSLContext sslContext = SSLContext.getInstance("TLS");
            sslContext.init(null, new TrustManager[]{manager}, null);
            socketFactory = new SSLConnectionSocketFactory(sslContext, NoopHostnameVerifier.INSTANCE);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (KeyManagementException e) {
            e.printStackTrace();
        }
    }
    /**
     * https get
     * @param url
     * @param data
     * @return
     * @throws java.io.IOException
     */
    public static CloseableHttpResponse doHttpsGet(String url, String data){
        enableSSL();
        Registry<ConnectionSocketFactory> socketFactoryRegistry = RegistryBuilder.<ConnectionSocketFactory>create()
                .register("http", PlainConnectionSocketFactory.INSTANCE).register("https", socketFactory).build();
        PoolingHttpClientConnectionManager connectionManager = new PoolingHttpClientConnectionManager(socketFactoryRegistry);
        CloseableHttpClient httpClient = HttpClients.custom().setConnectionManager(connectionManager)
                .setDefaultRequestConfig(requestConfig).build();
        HttpGet httpGet = new HttpGet(url);
        CloseableHttpResponse response = null;
        try {
            response = httpClient.execute(httpGet, context);
        }catch (Exception e){
            e.printStackTrace();
        }
        return response;
    }
    /**
     * https post
     * @param url
     * @param values
     * @return
     * @throws java.io.IOException
     */
    public static CloseableHttpResponse doHttpsPost(String url, List<NameValuePair> values) {
        enableSSL();
        Registry<ConnectionSocketFactory> socketFactoryRegistry = RegistryBuilder.<ConnectionSocketFactory>create()
                .register("http", PlainConnectionSocketFactory.INSTANCE).register("https", socketFactory).build();
        PoolingHttpClientConnectionManager connectionManager = new PoolingHttpClientConnectionManager(socketFactoryRegistry);
        CloseableHttpClient httpClient = HttpClients.custom().setConnectionManager(connectionManager)
                .setDefaultRequestConfig(requestConfig).build();
        HttpPost httpPost = new HttpPost(url);
        UrlEncodedFormEntity entity = new UrlEncodedFormEntity(values, Consts.UTF_8);
        httpPost.setEntity(entity);
        CloseableHttpResponse response = null;
        try {
            response = httpClient.execute(httpPost, context);
        }catch (Exception e){}
        return response;
    }
    /**
     * http get
     *
     * @param url
     * @param data
     * @return
     */
    public static CloseableHttpResponse doGet(String url, String data) {
        CookieStore cookieStore = new BasicCookieStore();
        CloseableHttpClient httpClient = HttpClientBuilder.create().
                setKeepAliveStrategy(new DefaultConnectionKeepAliveStrategy()).
                setRedirectStrategy(new DefaultRedirectStrategy()).setDefaultHeaders(new ArrayList<Header>()).
                setDefaultCookieStore(cookieStore).
                setDefaultRequestConfig(requestConfig).build();
        HttpGet httpGet = new HttpGet(url);
        //httpGet.setHeader("Content-Type", "application/x-www-form-urlencoded");
        CloseableHttpResponse response = null;
        try {
            response = httpClient.execute(httpGet, context);
        }catch (Exception e){}
        return response;
    }
    /**
     * http post
     *
     * @param url
     * @param values
     * @return
     */
    public static CloseableHttpResponse doPost(String url, List<NameValuePair> values) {
        CookieStore cookieStore = new BasicCookieStore();
        CloseableHttpClient httpClient = HttpClientBuilder.create().
                setKeepAliveStrategy(new DefaultConnectionKeepAliveStrategy()).
                setRedirectStrategy(new DefaultRedirectStrategy()).
                setDefaultCookieStore(cookieStore).
                setDefaultRequestConfig(requestConfig).build();
        HttpPost httpPost = new HttpPost(url);
        UrlEncodedFormEntity entity = new UrlEncodedFormEntity(values, Consts.UTF_8);
        httpPost.setEntity(entity);
        CloseableHttpResponse response = null;
        try {
            response = httpClient.execute(httpPost, context);
        }catch (Exception e){}
        return response;
    }
    /**
     * 直接把Response内的Entity内容转换成String
     *
     * @param httpResponse
     * @return
     */
    public static String toString(CloseableHttpResponse httpResponse) {
        // 获取响应消息实体
        String result = null;
        try {
            HttpEntity entity = httpResponse.getEntity();
            if (entity != null) {
                result = EntityUtils.toString(entity,"UTF-8");
            }
        }catch (Exception e){}finally {
            try {
                httpResponse.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return result;
    }
    public static void main(String[] args){
        CloseableHttpResponse response = HttpClientUtil.doHttpsGet("https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wxb2ebe42765aad029&secret=720661590f720b1f501ab3f390f80d52","");
        System.out.println(HttpClientUtil.toString(response));
    }
}
```
