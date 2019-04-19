# 从日常开发说起，浅谈HTTP协议是做什么的 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [左潇龙](http://www.jobbole.com/members/zuoxiaolong) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**引言**
HTTP协议作为Web开发的基础一直被大多数人所熟知，不过相信有很多人只知其一不知其二。比如咱们经常用到的session会话机制是如何实现的，可能很多人都说不出来吧。其实session会话就是HTTP协议中的一个header属性cookie所支持的，在你了解了HTTP协议之后，其实这些都非常容易理解。
本文会尝试从各位的日常开发去解释一下HTTP到底是做什么的，文章篇幅有限，如果有什么本文没有提到的，各位请自行百度或者看书补脑。接下来，咱们先来看一个小A和小B的故事。
### 小故事：两个人的任务
小A和小B是中国著名的互联网公司BAT的两个员工，时间走到2015年4月15日下午6点半，北京风沙漫天，仿佛是妖怪来临，小A和小B兴奋的正准备收拾回家，顺便享受一下免费的风沙晚餐。
就在这时，项目经理大S的声音不合时宜的响了起来，“小A，小B，你们俩先别走，给你们俩一个任务”。
两人听到这个声音，苦逼的相视一笑，之后便异口同声的说道，“头儿，有事儿您说话”。
“你俩也别不乐意，自从来到BAT，你俩单身问题解决了，房子也买了，存款也有了，加会班也是应该的”。大S注意到二人苦逼的眼神，淡定的说道。看到二人心悦诚服的点了点头，大S也就不再多说，吩咐道：“咱们项目里有不少地方需要判断String是否为空，是否为空串等等，小B你写一个工具类，小A你把调用的地方改一下。”
两人一听，这还不简单，赶紧点头哈腰的说，“包在我俩身上”。
看到二人的反应，大S满意的点了点头，正色说道：“记得我一直教导你们的，bug毁一时，重复毁一生，重构要趁早。”说完这句话，大S便头也不回的离开二人，数十秒后便钻进了风沙之中。
小A和小B简单商量了一下，由小B来编写String的工具类，小A来调用小B的工具类方法。工具类名叫StringUtils，里面共有两个方法，一个叫isNull，一个叫isEmpty，参数都是一个String，返回值都是一个boolean。
由于这个类非常简单，小B很快就搞定了它，代码如下。


```
/**
 * 工具类
 *
 * @author 小B
 * <a href="http://www.jobbole.com/members/chchxinxinjun">@since</a> 4/17/2015 2:47 PM
 */
public abstract class StringUtils {
    public static boolean isNull(String s) {
        return s == null;
    }
    public static boolean isEmpty(String s) {
        return isNull(s) || s.length() == 0;
    }
}
```
小B把工具类写好以后，小A也很快把其中一个用于验证身份证号的类改成了调用小B工具类的方式，代码如下。


```
/**
 * 身份证号验证工具类
 *
 * @author 小A
 * <a href="http://www.jobbole.com/members/chchxinxinjun">@since</a> 4/17/2015 2:50 PM
 */
public class CardNumberValidator {
    /**
     * 判断身份证号是否有效 (忽略身份证号复杂的判断规则)
     */
    public boolean valid(String cardNumber) {
        if (StringUtils.isNull(cardNumber)) {
            throw new NullPointerException("cardNumber is null!");
        } else if (StringUtils.isEmpty(cardNumber)) {
            throw new IllegalArgumentException("cardNumber is empty!");
        } else if (cardNumber.length() == 18) {
            return true;
        } else {
            throw new IllegalArgumentException("the length of cardNumber must be 18!");
        }
    }
}
```
最后小A又写了一个简单的测试类测试了一下，测试类如下。


```
/**
 * 客户端
 *
 * <a href="http://www.jobbole.com/members/chchxinxinjun">@since</a> 4/17/2015 3:01 PM
 */
public class Client {
    /**
     * 主函数
     *
     * @param args
     */
    public static void main(String[] args) {
        CardNumberValidator validator = new CardNumberValidator();
        String cardNumber = "433182198803211232";
        if (validator.valid(cardNumber)) {
            System.out.println(cardNumber + "是一个有效的身份证号！");
        } else {
            System.out.println(cardNumber + "是一个无效的身份证号！");
        }
    }
}
```
运行之后，程序运行的结果为正确的，如下。


```
433182198803211232 是一个有效的身份证号！
```
看到程序很快就正确运行，小A和小B都欣喜不已，感觉自己的技术水平又得到了质的提升。由于两人住的比较近，于是干完活之后，二人便一同钻进了风沙之中。
### 花絮与故事分析
一个小小的京城故事，两个快乐的2B程序员。没有复杂的故事情节，没有高深的技术含量，但却蕴含着一个像极了WEB资源请求的过程。回想一下刚才小A的程序调用小B程序的过程，其实包含了以下三个步骤。
1，小A的程序通过【类名.方法名(参数列表)】的形式找到了小B程序当中的方法。
2，小A的程序传给小B的程序一个参数，小B的程序对这个请求进行相应的处理，比如判断是否为空等等。
3，小B的程序根据处理结果返回给小A的程序，小A的程序根据返回结果进行后续的处理。
这整个过程与一个简单的WEB资源请求如出一撤，具体的内容咱们在接下来的过程中再去讨论。这里，咱们先简单的回顾一下，小A和小B都商量了哪些东西以后，开始了各自的编程。
1，首先小A和小B定义了类名，方法名和参数类型。根据这三个内容，小A就可以通过命名找到小B的两个方法（比如StringUtils.isNull）。那么简单点说，类名,方法名以及参数类型就可以解决“小A怎么找到小B的方法”。
2，其次，由于规定了小A需要给小B传送String类型的数据，那么小B就可以按照String类型进行相应的处理。因此，小A和小B对于方法参数类型的约定，就可以解决“小A传什么数据和小B接到以后按照什么类型去处理”。
3，最后，小B和小A约定返回的类型为boolean，那么小A这边收到结果以后就可以按照boolean类型去处理。返回结果的约定，就可以解决“小B返回什么数据和小A接收到以后按照什么类型去处理结果”。
### HTTP与小故事
一次WEB资源请求的过程，其实就和一次方法调用特别相似，上面小A的程序其实就相当于浏览器，小B的程序就相当于服务器，而小B提供的方法就相当于服务器上的资源。上面咱们分析了方法调用的过程，咱们来看看一次WEB资源请求大致分为哪三步。
1，浏览器需要根据某种字符串格式（类似于故事当中的【类名.方法(参数列表)】的方式）找到服务器当中的资源。
2，浏览器传给服务器一个请求，服务器对这个请求进行相应的处理（比如增删改查）。
3，服务器根据处理结果返回给浏览器，浏览器根据返回结果进行相应的处理（比如显示网页，显示图片等）。
可以看出，这三个步骤是非常相似的。既然是相似的步骤，那么就会存在相似的问题。接下来，咱们简单的分析一下都有哪些问题，以及这些问题如何处理。
【1】**如何找到服务器当中的资源**
故事当中，小A根据【类名.方法(参数列表)】的方式找到小B的方法，那么在WEB资源请求当中，浏览器如何找到服务器的资源呢？
相信大部分人脑子里已经浮现出了那三个字母。
是的，就是URL。URL就是专门用来定位资源的。URL的一般格式如下。


```
protocol :// hostname[:port] / path / [;parameters][?query]#fragment
```
其中各个部分的含义相信大部分人都知道，这里咱们就不过多解释了。最重要的就是protocol，hostname和port，分别代表着应用层的协议（比如http，https，ftp等等），主机名或IP以及服务端口。
【2】**浏览器和服务器互相传输的数据如何解析**
这个问题其实就是第二和第三步所面临的问题，浏览器要给服务器发请求，但是服务器哪知道你发的是什么玩意。同理，如果服务器给浏览器返回数据，浏览器同样也不知道服务器返回的是什么东西。
在故事当中，小A和小B商量好了，小A给小B传String，小B给小A返回boolean，这就很好的解决了程序之间数据交换的解析工作。当然，由于上面的程序非常简单，所以解析的工作还不是体现的特别明显。
假设小B的方法是一个save(Map user)的形式，返回值也是一个Map。这时候，如果小B和小A不商量好Map里面都需要put点啥东西的话，估计这程序也没法写下去了。
所以问题就出现在这里，如果不给浏览器和服务器制定好一个规则的话，不管是开发浏览器的程序员，还是开发服务的程序员，都会出现程序不知道怎么写的情况。最可怕的是，开发浏览器的程序员和开发服务的程序员可不一定是同事，他们无法面对面或者通过通讯工具去商量你给我传什么，我给你传什么这种问题。
所以HTTP协议就应运而生了，这是一群外国人勾搭以后产生的（听说这群外国人叫World Wide Web Consortium和Internet Engineering Task Force）。HTTP协议自出现以来，主要解决的就是浏览器和服务器数据交换的格式问题。
既然是解决数据交换的格式问题，所以不用去想，也知道HTTP其实是定义了一套数据格式。咱们来看一个实际的例子，一个HTTP请求到底都有哪些数据，以及这些数据是什么格式。


```
GET http://www.cnblogs.com/mvc/Follow/GetFollowStatus.aspx HTTP/1.1
Host: www.cnblogs.com
User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:37.0) Gecko/20100101 Firefox/37.0
Accept: text/plain, */*; q=0.01
Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3
Accept-Encoding: gzip, deflate
X-Requested-With: XMLHttpRequest
Referer: http://www.cnblogs.com
Cookie: _ga=GA1.2.1892107667.1429258898; __gads=ID=ab3bc12f51ce0821:T=1429258914:S=ALNI_MYk8_dptVuXGkQkQbTMuiNN4DdPcA; .CNBlogsCookie=D84CF73E03B7891789045C601A78AAF52AB67268B93E15C1138FE9A9CE858E98FE769CE0249E5C5C68D6E84DB2CBC9D9BA77ACA3993260AA8671E17F2C3AC7B3267298C6160C09A737AF94A353F58D0B7FF5C0AD6287EFC4DBE122883CBDD2CB42C3052C; _gat=1; CNZZDATA1684828=cnzz_eid%3D1208659555-1429265886-%26ntime%3D1429265886
Connection: keep-alive
```
可以看到，最上面的那一行其实是协议当中定义的首行。第一个GET代表的是，这是一个get请求。后面紧跟着的是访问的URL，最后是HTTP协议版本。
再往下就是HTTP当中定义的header了，具体每个header都代表什么意思这里就不一一解释了，这不是本文的重点，可以去参考网络上其它的文章。这些Header其实就相当于HTTP协议提供的一些方便的功能，你设置相应的header，可以让服务器产生相应的行为。
举个例子，比如Cookie这个header，大家应该再熟悉不过。它的作用就是告诉服务器当前请求者的身份，而大部分的服务器也都会自动去管理Cookie。
除了上面出现的首行和header以外，对于一些特定的请求，HTTP还有特定的数据格式。比如post请求的时候，在【Connection: keep-alive】下面会多出来一个json格式的字符串，这个字符串就是post请求时所发送的表单数据。
同样的，服务器返回的数据格式也是相似的。一个比较实际的例子如下。


```
HTTP/1.1 200 OK
Date: Fri, 17 Apr 2015 10:18:06 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 128
Connection: keep-alive
Cache-Control: private
X-UA-Compatible: IE=10
<a href="javascript:void(0);" onclick="cnblogs.UserManager.FollowBlogger('83f9460b-63cf-dd11-9e4d-001cf0cd104b')">+加关注</a>
```
响应当中依然有首行，而首行就是协议版本，加上状态码和状态描述。接下来就是一堆header，这点与请求相同。不同的是，请求和响应所支持的header并不一样。比如请求的时候，浏览器给服务器传送Cookie时使用的header是Cookie。但是当服务器返回响应给浏览器时，如果要更新Cookie的话，对应的header是Set-Cookie。最后一行则是服务器所返回的内容，格式是由Content-Type所指定的，类型为html，字符编码为UTF-8。对于其它的header这里就不一一解释了，请各位自行补脑。
### HTTP与WEB开发的联系
说到这里，需要简单提一下HTTP与WEB开发的联系。比如大家在做J2EE开发时所熟知的request和response对象，咱们来看一下request和response接口都有哪些方法。


```
public interface HttpServletRequest extends ServletRequest {
    String BASIC_AUTH = "BASIC";
    String FORM_AUTH = "FORM";
    String CLIENT_CERT_AUTH = "CLIENT_CERT";
    String DIGEST_AUTH = "DIGEST";
    String getAuthType();
    Cookie[] getCookies();
    long getDateHeader(String var1);
    String getHeader(String var1);
    Enumeration getHeaders(String var1);
    Enumeration getHeaderNames();
    int getIntHeader(String var1);
    String getMethod();
    String getPathInfo();
    String getPathTranslated();
    String getContextPath();
    String getQueryString();
    String getRemoteUser();
    boolean isUserInRole(String var1);
    Principal getUserPrincipal();
    String getRequestedSessionId();
    String getRequestURI();
    StringBuffer getRequestURL();
    String getServletPath();
    HttpSession getSession(boolean var1);
    HttpSession getSession();
    boolean isRequestedSessionIdValid();
    boolean isRequestedSessionIdFromCookie();
    boolean isRequestedSessionIdFromURL();
    /** @deprecated */
    boolean isRequestedSessionIdFromUrl();
}
```


```
public interface HttpServletResponse extends ServletResponse {
    int SC_CONTINUE = 100;
    int SC_SWITCHING_PROTOCOLS = 101;
    int SC_OK = 200;
    int SC_CREATED = 201;
    int SC_ACCEPTED = 202;
    int SC_NON_AUTHORITATIVE_INFORMATION = 203;
    int SC_NO_CONTENT = 204;
    int SC_RESET_CONTENT = 205;
    int SC_PARTIAL_CONTENT = 206;
    int SC_MULTIPLE_CHOICES = 300;
    int SC_MOVED_PERMANENTLY = 301;
    int SC_MOVED_TEMPORARILY = 302;
    int SC_FOUND = 302;
    int SC_SEE_OTHER = 303;
    int SC_NOT_MODIFIED = 304;
    int SC_USE_PROXY = 305;
    int SC_TEMPORARY_REDIRECT = 307;
    int SC_BAD_REQUEST = 400;
    int SC_UNAUTHORIZED = 401;
    int SC_PAYMENT_REQUIRED = 402;
    int SC_FORBIDDEN = 403;
    int SC_NOT_FOUND = 404;
    int SC_METHOD_NOT_ALLOWED = 405;
    int SC_NOT_ACCEPTABLE = 406;
    int SC_PROXY_AUTHENTICATION_REQUIRED = 407;
    int SC_REQUEST_TIMEOUT = 408;
    int SC_CONFLICT = 409;
    int SC_GONE = 410;
    int SC_LENGTH_REQUIRED = 411;
    int SC_PRECONDITION_FAILED = 412;
    int SC_REQUEST_ENTITY_TOO_LARGE = 413;
    int SC_REQUEST_URI_TOO_LONG = 414;
    int SC_UNSUPPORTED_MEDIA_TYPE = 415;
    int SC_REQUESTED_RANGE_NOT_SATISFIABLE = 416;
    int SC_EXPECTATION_FAILED = 417;
    int SC_INTERNAL_SERVER_ERROR = 500;
    int SC_NOT_IMPLEMENTED = 501;
    int SC_BAD_GATEWAY = 502;
    int SC_SERVICE_UNAVAILABLE = 503;
    int SC_GATEWAY_TIMEOUT = 504;
    int SC_HTTP_VERSION_NOT_SUPPORTED = 505;
    void addCookie(Cookie var1);
    boolean containsHeader(String var1);
    String encodeURL(String var1);
    String encodeRedirectURL(String var1);
    /** @deprecated */
    String encodeUrl(String var1);
    /** @deprecated */
    String encodeRedirectUrl(String var1);
    void sendError(int var1, String var2) throws IOException;
    void sendError(int var1) throws IOException;
    void sendRedirect(String var1) throws IOException;
    void setDateHeader(String var1, long var2);
    void addDateHeader(String var1, long var2);
    void setHeader(String var1, String var2);
    void addHeader(String var1, String var2);
    void setIntHeader(String var1, int var2);
    void addIntHeader(String var1, int var2);
    void setStatus(int var1);
    /** @deprecated */
    void setStatus(int var1, String var2);
}
```
可以看到，request和response里面有好几个方法都和header有关，使用这些方法就可以取到相应的HTTP请求当中的header内容，也可以返回相应的header内容给浏览器。还有一点，就是response接口当中定义了一大把状态码和状态描述，比如200对应OK，404对应NOT_FOUND，500对应内部错误等等。
可以预见的是，在编写HttpServletRequest和HttpServletResponse这两个接口的时候，一定是参照HTTP协议去定义的，而且每当HTTP协议进行一次大的变更，这两个接口都要跟着进行相应的变化。
总的来说，把对HTTP的了解和日常的开发联系起来，更加有助于你理解HTTP协议，而且有时候也可以利用HTTP协议扩展一些功能，比如授权服务，自定义的状态保持等等。
### 小结
到此，大家应该基本上了解HTTP主要是用来做什么的了，具体HTTP协议当中都规定了哪些内容，大家可以去找各种资料翻阅。个人觉得，只要深刻理解HTTP协议是做什么的，了解一些常用的协议内容就行了，你并不需要把HTTP所有的header都给背下来并记住它们的作用。
最后，重复一下那句与HTTP无关的话：bug毁一时，重复毁一生，重构要趁早。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/78805a221a988e79ef3f42d7c5bfd41811.jpeg)![](http://www.jobbole.com/wp-content/uploads/2016/04/78805a221a988e79ef3f42d7c5bfd41812.jpeg)
