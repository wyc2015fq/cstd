# HttpClient 获取 Cookie 的一次踩坑实录 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [fullstackyang](http://www.jobbole.com/members/2653yy) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
在使用HttpClient进行抓取一些网页的时候，经常会保留从服务器端发回的Cookie信息，以便发起其他需要这些Cookie的请求。大多数情况下，我们使用内置的cookie策略，便能够方便直接地获取这些cookie。
下面的一小段代码，就是访问[http://www.baidu.com](http://www.baidu.com)，并获取对应的cookie：


```
@ Test
public void getCookie(){
    CloseableHttpClient httpClient = HttpClients.createDefault();
    HttpGet get=new HttpGet("http://www.baidu.com");
    HttpClientContext context = HttpClientContext.create();
    try {
        CloseableHttpResponse response = httpClient.execute(get, context);
        try{
            System.out.println(">>>>>>headers:");
            Arrays.stream(response.getAllHeaders()).forEach(System.out::println);
            System.out.println(">>>>>>cookies:");
            context.getCookieStore().getCookies().forEach(System.out::println);
        }
        finally {
            response.close();
        }
    } catch (IOException e) {
        e.printStackTrace();
    }finally {
        try {
            httpClient.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
打印结果


```
>>>>>>headers:
Server: bfe/1.0.8.18
Date: Tue, 12 Sep 2017 06:19:06 GMT
Content-Type: text/html
Last-Modified: Mon, 23 Jan 2017 13:28:24 GMT
Transfer-Encoding: chunked
Connection: Keep-Alive
Cache-Control: private, no-cache, no-store, proxy-revalidate, no-transform
Pragma: no-cache
Set-Cookie: BDORZ=27315; max-age=86400; domain=.baidu.com; path=/
>>>>>>cookies:
[version: 0][name: BDORZ][value: 27315][domain: baidu.com][path: /][expiry: null]
```
但是也有一些网站返回的cookie并不一定完全符合规范，例如下面这个例子，从打印出的header中可以看到，这个cookie中的Expires属性是时间戳形式，并不符合标准的时间格式，因此，httpclient对于cookie的处理失效，最终无法获取到cookie，并且发出了一条警告信息：“Invalid ‘expires’ attribute: 1505204523”


```
警告: Invalid cookie header: "Set-Cookie: yd_cookie=90236a64-8650-494b332a285dbd886e5981965fc4a93f023d; Expires=1505204523; Path=/; HttpOnly". Invalid 'expires' attribute: 1505204523
>>>>>>headers:
Date: Tue, 12 Sep 2017 06:22:03 GMT
Content-Type: text/html
Connection: keep-alive
Set-Cookie: yd_cookie=90236a64-8650-494b332a285dbd886e5981965fc4a93f023d; Expires=1505204523; Path=/; HttpOnly
Cache-Control: no-cache, no-store
Server: WAF/2.4-12.1
>>>>>>cookies:
```
虽然我们可以利用header的数据，重新构造一个cookie出来，也有很多人确实也是这么做的，但这种方法不够优雅，那么如何解决这个问题？网上相关的资料又很少，所以就只能先从官方文档入手。在官方文档3.4小节custom cookie policy中讲到允许自定义的cookie策略，自定义的方法是实现CookieSpec接口，并通过CookieSpecProvider来完成在httpclient中的初始化和注册策略实例的工作。好了，关键的线索在于CookieSpec接口，我们来看一下它的源码：


```
public interface CookieSpec {
……
    /**
      * Parse the {@ code "Set-Cookie"} Header into an array of Cookies.
      *
      * <p> This method will not perform the validation of the resultant<code>
      * {@link Cookie}s</p>
      *
      * @ see #validate
      *
      * @param header the {@ code Set-Cookie} received from the server
      * @param origin details of the cookie origin
      * @ return an array of {@ code Cookie}s parsed from the header
      * @throws MalformedCookieException if an exception occurs during parsing
      */
    List parse(Header header, CookieOrigin origin) throws MalformedCookieException;
……
}
```
在源码中我们发现了一个parse方法，看注释就知道正是这个方法，将Set-Cookie的header信息解析为Cookie对象，自然地再了解一下在httplcient中的默认实现DefaultCookieSpec，限于篇幅，源码就不贴了。在默认的实现中，DefaultCookieSpec主要的工作是判断header中Cookie规范的类型，然后再调用具体的某一个实现。像上述这种Cookie，最终是交由NetscapeDraftSpec的实例来做解析，而在NetscapeDraftSpec的源码中，定义了默认的expires时间格式为“EEE, dd-MMM-yy HH:mm:ss z”


```
public class NetscapeDraftSpec extends CookieSpecBase {
    protected static final String EXPIRES_PATTERN = "EEE, dd-MMM-yy HH:mm:ss z";
    /** Default constructor */
    public NetscapeDraftSpec(final String[] datepatterns) {
        super(new BasicPathHandler(),
                new NetscapeDomainHandler(),
                new BasicSecureHandler(),
                new BasicCommentHandler(),
                new BasicExpiresHandler(
                        datepatterns != null ? datepatterns.clone() : new String[]{EXPIRES_PATTERN}));
    }
    NetscapeDraftSpec(final CommonCookieAttributeHandler... handlers) {
        super(handlers);
    }
    public NetscapeDraftSpec() {
        this((String[]) null);
    }
……
}
```
到这里已经比较清楚了，我们只需要将Cookie中expires的时间转换为正确的格式，然后再送入默认的解析器就可以了。
**解决方法：**
- 自定义一个CookieSpec类，继承DefaultCookieSpec
- 重写parser方法
- 将Cookie中的expires转换为正确的时间格式
- 调用默认的解析方法
实现如下（URL就不公开了，已经隐去）


```
public class TestHttpClient {
    
    String url = sth;
    class MyCookieSpec extends DefaultCookieSpec {
        @ Override
        public List parse(Header header, CookieOrigin cookieOrigin) throws MalformedCookieException {
            String value = header.getValue();
            String prefix = "Expires=";
            if (value.contains(prefix)) {
                String expires = value.substring(value.indexOf(prefix) + prefix.length());
                expires = expires.substring(0, expires.indexOf(";"));
                String date = DateUtils.formatDate(new Date(Long.parseLong(expires) * 1000L),"EEE, dd-MMM-yy HH:mm:ss z");
                value = value.replaceAll(prefix + "\\d{10};", prefix + date + ";");
            }
            header = new BasicHeader(header.getName(), value);
            return super.parse(header, cookieOrigin);
        }
    }
    @ Test
    public void getCookie() {
        CloseableHttpClient httpClient = HttpClients.createDefault();
        Registry cookieSpecProviderRegistry = RegistryBuilder.create()
                .register("myCookieSpec", context -> new MyCookieSpec()).build();//注册自定义CookieSpec
        HttpClientContext context = HttpClientContext.create();
        context.setCookieSpecRegistry(cookieSpecProviderRegistry);
        HttpGet get = new HttpGet(url);
        get.setConfig(RequestConfig.custom().setCookieSpec("myCookieSpec").build());
        try {
            CloseableHttpResponse response = httpClient.execute(get, context);
            try{
                System.out.println(">>>>>>headers:");
                Arrays.stream(response.getAllHeaders()).forEach(System.out::println);
                System.out.println(">>>>>>cookies:");
                context.getCookieStore().getCookies().forEach(System.out::println);
            }
            finally {
                response.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                httpClient.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```
再次运行，顺利地打印出正确的结果，完美！


```
>>>>>>headers:
Date: Tue, 12 Sep 2017 07:24:10 GMT
Content-Type: text/html
Connection: keep-alive
Set-Cookie: yd_cookie=9f521fc5-0248-4ab3ee650ca50b1c7abb1cd2526b830e620f; Expires=1505208250; Path=/; HttpOnly
Cache-Control: no-cache, no-store
Server: WAF/2.4-12.1
>>>>>>cookies:
[version: 0][name: yd_cookie][value: 9f521fc5-0248-4ab3ee650ca50b1c7abb1cd2526b830e620f][domain: www.sth.com][path: /][expiry: Tue Sep 12 17:24:10 CST 2017]
```
本文也发我的个人博客：[fullstackyang](http://www.fullstackyang.com/archives/416.html)
