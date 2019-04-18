# 微信公众号开发- 网页授权Oauth2.0及API开发 - z69183787的专栏 - CSDN博客
2016年02月16日 15:11:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4984
参考：[https://mp.weixin.qq.com/wiki](https://mp.weixin.qq.com/wiki) - 微信网页授权
**注：微信API接口地址可能有变更，具体参考微信开发者文档**
pom：
```
<dependencies>
        <dependency>
            <groupId>com.google.code.gson</groupId>
            <artifactId>gson</artifactId>
        </dependency>
        <dependency>
            <groupId>commons-codec</groupId>
            <artifactId>commons-codec</artifactId>
            <version>1.9</version>
        </dependency>
        <dependency>
            <groupId>commons-logging</groupId>
            <artifactId>commons-logging</artifactId>
            <version>1.1.3</version>
        </dependency>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
        </dependency>
        <dependency>
            <groupId>log4j</groupId>
            <artifactId>log4j</artifactId>
        </dependency>
    </dependencies>
```
回调URL jsp代码：
```java
<%
    String code = request.getParameter("code");
    System.out.println(code);
    AccessTokenResult accessTokenResult =  WxUtil.accessToken(code);
    UserInfoResult userInfoResult = WxUtil.userInfo(accessTokenResult.getAccess_token(),accessTokenResult.getOpenid());
    System.out.println(accessTokenResult.toString());
    System.out.println(userInfoResult.toString());
%>
```
设置头信息：
```java
CloseableHttpClient httpClient = HttpClientBuilder.create().
                setKeepAliveStrategy(new DefaultConnectionKeepAliveStrategy()).
                setRedirectStrategy(new DefaultRedirectStrategy()).setDefaultHeaders(new ArrayList<Header>()).
                setDefaultCookieStore(cookieStore).
                setDefaultRequestConfig(requestConfig).build();
        HttpGet httpGet = new HttpGet(url);
        httpGet.setHeader("Content-Type", "application/x-www-form-urlencoded");
```
HttpClientUtil：
```java
import org.apache.http.Consts;
import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.ParseException;
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
import org.apache.http.cookie.Cookie;
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
     * @throws IOException
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
     * @throws IOException
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
                setRedirectStrategy(new DefaultRedirectStrategy()).
                setDefaultCookieStore(cookieStore).
                setDefaultRequestConfig(requestConfig).build();
        HttpGet httpGet = new HttpGet(url);
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
AccessTokenResult：
```java
public class AccessTokenResult implements Serializable{
    private static final long serialVersionUID = -5429222736793133180L;
    private String access_token;
    private int expires_in;
    private String refresh_token;
    private String openid;
    private String scope;
    private String unionid;
    public String getAccess_token() {
        return access_token;
    }
    public void setAccess_token(String access_token) {
        this.access_token = access_token;
    }
    public int getExpires_in() {
        return expires_in;
    }
    public void setExpires_in(int expires_in) {
        this.expires_in = expires_in;
    }
    public String getRefresh_token() {
        return refresh_token;
    }
    public void setRefresh_token(String refresh_token) {
        this.refresh_token = refresh_token;
    }
    public String getOpenid() {
        return openid;
    }
    public void setOpenid(String openid) {
        this.openid = openid;
    }
    public String getScope() {
        return scope;
    }
    public void setScope(String scope) {
        this.scope = scope;
    }
    public String getUnionid() {
        return unionid;
    }
    public void setUnionid(String unionid) {
        this.unionid = unionid;
    }
    @Override
    public String toString() {
        return "AccessTokenResult{" +
                "access_token='" + access_token + '\'' +
                ", expires_in=" + expires_in +
                ", refresh_token='" + refresh_token + '\'' +
                ", openid='" + openid + '\'' +
                ", scope='" + scope + '\'' +
                ", unionid='" + unionid + '\'' +
                '}';
    }
}
```
ErrorInfoResult：
```java
public class ErrorInfoResult implements Serializable{
    private static final long serialVersionUID = -7644965943068191814L;
    private int errcode;
    private String errmsg;
    public int getErrcode() {
        return errcode;
    }
    public void setErrcode(int errcode) {
        this.errcode = errcode;
    }
    public String getErrmsg() {
        return errmsg;
    }
    public void setErrmsg(String errmsg) {
        this.errmsg = errmsg;
    }
}
```
UserInfoResult：
```java
public class UserInfoResult implements Serializable {
    private static final long serialVersionUID = 7897488657400830591L;
    private String openid;
    private String nickname;
    private String sex;
    private String province;
    private String city;
    private String country;
    private String headimgurl;
    private String unionid;
    public String getUnionid() {
        return unionid;
    }
    public void setUnionid(String unionid) {
        this.unionid = unionid;
    }
    public String getHeadimgurl() {
        return headimgurl;
    }
    public void setHeadimgurl(String headimgurl) {
        this.headimgurl = headimgurl;
    }
    public String getCountry() {
        return country;
    }
    public void setCountry(String country) {
        this.country = country;
    }
    public String getCity() {
        return city;
    }
    public void setCity(String city) {
        this.city = city;
    }
    public String getProvince() {
        return province;
    }
    public void setProvince(String province) {
        this.province = province;
    }
    public String getSex() {
        return sex;
    }
    public void setSex(String sex) {
        this.sex = sex;
    }
    public String getNickname() {
        return nickname;
    }
    public void setNickname(String nickname) {
        this.nickname = nickname;
    }
    public String getOpenid() {
        return openid;
    }
    public void setOpenid(String openid) {
        this.openid = openid;
    }
    @Override
    public String toString() {
        return "UserInfoResult{" +
                "openid='" + openid + '\'' +
                ", nickname='" + nickname + '\'' +
                ", sex='" + sex + '\'' +
                ", province='" + province + '\'' +
                ", city='" + city + '\'' +
                ", country='" + country + '\'' +
                ", headimgurl='" + headimgurl + '\'' +
                ", unionid='" + unionid + '\'' +
                '}';
    }
}
```
WxConstants：
```java
public class WxConstants {
    //appId;
    public final static String appId = "wxb2ebe42765aad029";
    //
    public final static String appSecret = "720661590f720b1f501ab3f390f80d52";
}
```
WxUtil：
```java
public class WxUtil {
    private static Gson gson = new GsonBuilder().setDateFormat("yyyy-MM-dd").disableHtmlEscaping().create();
    /**
     * 生成授权URL
     * @param callbackUrl
     * @return
     */
    public static String authorizeUrl(String callbackUrl){
        StringBuilder sb = new StringBuilder();
        try {
            sb.append("https://open.weixin.qq.com/connect/oauth2/authorize?appid=").
                    append(WxConstants.appId).
                    append("&redirect_uri=").
                    append(URLEncoder.encode(callbackUrl,"UTF-8")).
                    append("&response_type=code").
                    //append("&scope=snsapi_base ").
                    append("&scope=snsapi_userinfo").
                    append("&state=STATE").
                    append("#wechat_redirect");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return sb.toString();
    }
    /**
     * 获取access_token
     * @param code
     * @return
     */
    public static AccessTokenResult accessToken(String code){
        AccessTokenResult accessTokenResult = null;
        StringBuilder sb = new StringBuilder();
        sb.append("https://api.weixin.qq.com/sns/oauth2/access_token?appid=").
                append(WxConstants.appId).
                append("&secret=").
                append(WxConstants.appSecret).
                append("&code=").
                append(code).
                append("&grant_type=authorization_code");
        String result = HttpClientUtil.toString(HttpClientUtil.doHttpsGet(sb.toString(), ""));
        if(result != null){
            accessTokenResult = gson.fromJson(result,AccessTokenResult.class);
        }
        return accessTokenResult;
    }
    /**
     * refreshToken 重新获取 accessToken
     * @param refreshToken
     * @return
     */
    public static AccessTokenResult refreshToken(String refreshToken){
        AccessTokenResult accessTokenResult = null;
        StringBuilder sb = new StringBuilder();
        sb.append("https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=").
                append(WxConstants.appId).
                append("&refresh_token=").
                append(refreshToken).
                append("&grant_type=refresh_token");
        String result = HttpClientUtil.toString(HttpClientUtil.doHttpsGet(sb.toString(), ""));
        if(result != null){
            accessTokenResult = gson.fromJson(result,AccessTokenResult.class);
        }
        return accessTokenResult;
    }
    /**
     * 获取用户信息 需要snstype 为 userinfo 非 base
     * @param accessToken
     * @param openId
     * @return
     */
    public static UserInfoResult userInfo(String accessToken,String openId){
        UserInfoResult userInfoResult = null;
        StringBuilder sb = new StringBuilder();
        sb.append("https://api.weixin.qq.com/sns/userinfo?access_token=").
                append(accessToken).
                append("&openid=").
                append(openId).
                append("&lang=zh_CN");
        String result = HttpClientUtil.toString(HttpClientUtil.doHttpsGet(sb.toString(), ""));
        if(result != null){
            userInfoResult = gson.fromJson(result,UserInfoResult.class);
        }
        return userInfoResult;
    }
    /**
     * 判断accesstoken 是否有效
     * @param accessToken
     * @param openId
     * @return
     */
    public static boolean authAccessToken(String accessToken,String openId){
        ErrorInfoResult errorInfoResult = null;
        boolean isValid = false;
        StringBuilder sb = new StringBuilder();
        sb.append("https://api.weixin.qq.com/sns/auth?access_token=").
                append(accessToken).
                append("&openid=").
                append(openId);
        String result = HttpClientUtil.toString(HttpClientUtil.doHttpsGet(sb.toString(), ""));
        if(result != null){
            errorInfoResult = gson.fromJson(result,ErrorInfoResult.class);
            if(0 == errorInfoResult.getErrcode()){
                isValid = true;
            }
        }
        return isValid;
    }
    public static void main(String[] args){
        String url = WxUtil.authorizeUrl("http://zszs.ngrok.cc/index.jsp");
        System.out.println(url);
        System.out.println(HttpClientUtil.toString(HttpClientUtil.doGet("http://dpcola2.ngrok.cc/activityvip/customized/externalColaDrawLottery?activity=03D39BCAF4AAC406&prizePackage=FAB7B93B21645FAC&_dpKey=2656cola1455875399784&phoneNum=13917114404","")));
    }
}
```
