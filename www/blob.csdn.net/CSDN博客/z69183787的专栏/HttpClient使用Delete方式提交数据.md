# HttpClient使用Delete方式提交数据 - z69183787的专栏 - CSDN博客
2018年11月02日 11:50:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：510
[https://blog.csdn.net/HaleyLiu123/article/details/81303508](https://blog.csdn.net/HaleyLiu123/article/details/81303508)
1. http请求主要有以下几种方法来对指定资源做不同操作：
HTTP/1.1协议中共定义了八种方法（有时也叫“动作”）来表明Request-URI指定的资源的不同操作方式：
. OPTIONS - 返回服务器针对特定资源所支持的HTTP请求方法。
                   也可以利用向Web服务器发送'*'的请求来测试服务器的功能性。
. HEAD    - 向服务器索要与GET请求相一致的响应，只不过响应体将不会被返回。
                这一方法可以在不必传输整个响应内容的情况下，就可以获取包含在响应消息头中的元信息。
. GET     - 向特定的资源发出请求。
                注意：GET方法不应当被用于产生“副作用”的操作中，例如在web app.中。
                其中一个原因是GET可能会被网络蜘蛛等随意访问。
. POST    - 向指定资源提交数据进行处理请求（例如提交表单或者上传文件）。
                数据被包含在请求体中。POST请求可能会导致新的资源的建立和/或已有资源的修改。
. PUT     - 向指定资源位置上传其最新内容。
. DELETE  - 请求服务器删除Request-URI所标识的资源。
. TRACE   - 回显服务器收到的请求，主要用于测试或诊断。
. CONNECT - HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器。
. PATCH   - 用来将局部修改应用于某一资源，添加于规范RFC5789。
2.HttpDelete的方法中本身并没有setEntity方法，参考HttpPost的setEntity方法，自定义一个HttpDeleteWithBody类
import java.net.URI;
import org.apache.http.client.methods.HttpEntityEnclosingRequestBase;
public class HttpDeleteWithBody extends HttpEntityEnclosingRequestBase{
    public static final String METHOD_NAME = "DELETE";
    /**
     * 获取方法（必须重载）
     *
     * @return
     */
    @Override
    public String getMethod() {
        return METHOD_NAME;
    }
    public HttpDeleteWithBody(final String uri) {
        super();
        setURI(URI.create(uri));
    }
    public HttpDeleteWithBody(final URI uri) {
        super();
        setURI(uri);
    }
    public HttpDeleteWithBody() {
        super();
    }
}
3. 用HttpClient 调用 HttpDeleteWithBody的方法，就可以进行body的操作了
    public static String doDelete(String data, String url) throws IOException {
        CloseableHttpClient client = null;
        HttpDeleteWithBody httpDelete = null;
        String result = null;
        try {
            client = HttpClients.createDefault();
            httpDelete = new HttpDeleteWithBody(url);
            httpDelete.addHeader("Content-type","application/json; charset=utf-8");
            httpDelete.setHeader("Accept", "application/json; charset=utf-8");
            httpDelete.setEntity(new StringEntity(data));
            CloseableHttpResponse response = client.execute(httpDelete);
            HttpEntity entity = response.getEntity();
            result = EntityUtils.toString(entity);
            if (200 == response.getStatusLine().getStatusCode()) {
                LOGGER.info("远程调用成功.msg={}", result);
            }
        } catch (Exception e) {
            LOGGER.error("远程调用失败,errorMsg={}", e.getMessage());
        } finally {
            client.close();
        }
        return result;
