# HttpClient学习整理 - weixin_33985507的博客 - CSDN博客
2017年09月05日 00:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
1 HttpClient简介
  2 HttpClient 功能介绍
  3 1． 读取网页(HTTP/HTTPS)内容
  4 2、使用POST方式提交数据(httpClient3)
  5 3． 处理页面重定向
  6 4． 模拟登录开心网
  7 5． 提交XML格式参数
  8 6． 访问启用认证的页面
  9 7． 多线程模式下使用httpclient
 10 httpClient完整封装
 11
```
## HttpClient简介
HTTP 协议可能是现在 Internet 上使用得最多、最重要的协议了，越来越多的 Java 应用程序需要直接通过 HTTP 协议来访问网络资源。虽然在 JDK 的 java.net 包中已经提供了访问 HTTP 协议的基本功能，但是对于大部分应用程序来说，JDK 库本身提供的功能还不够丰富和灵活。HttpClient 是 Apache Jakarta Common 下的子项目，用来提供高效的、最新的、功能丰富的支持 HTTP 协议的客户端编程工具包，并且它支持 HTTP 协议最新的版本和建议。HttpClient 已经应用在很多的项目中，比如 Apache Jakarta 上很著名的另外两个开源项目 Cactus 和 HTMLUnit 都使用了 HttpClient。更多信息请关注[http://hc.apache.org/](http://hc.apache.org/)
## HttpClient 功能介绍
以下列出的是 HttpClient 提供的主要的功能，要知道更多详细的功能可以参见 HttpClient 的主页。
- 
实现了所有 HTTP 的方法（GET,POST,PUT,HEAD 等）
- 
支持自动转向
- 
支持 HTTPS 协议
- 
支持代理服务器等
**应用HttpClient来对付各种顽固的WEB服务器**转自：[http://blog.csdn.net/ambitiontan/archive/2006/01/06/572171.aspx](http://blog.csdn.net/ambitiontan/archive/2006/01/06/572171.aspx)
一般的情况下我们都是使用IE或者Navigator浏览器来访问一个WEB服务器，用来浏览页面查看信息或者提交一些数据等等。所访问的这些页面有的仅仅是一些普通的页面，有的需要用户登录后方可使用，或者需要认证以及是一些通过加密方式传输，例如HTTPS。目前我们使用的浏览器处理这些情况都不会构成问题。不过你可能在某些时候需要通过程序来访问这样的一些页面，比如从别人的网页中“偷”一些数据；利用某些站点提供的页面来完成某种功能，例如说我们想知道某个手机号码的归属地而我们自己又没有这样的数据，因此只好借助其他公司已有的网站来完成这个功能，这个时候我们需要向网页提交手机号码并从返回的页面中解析出我们想要的数据来。如果对方仅仅是一个很简单的页面，那我们的程序会很简单，本文也就没有必要大张旗鼓的在这里浪费口舌。但是考虑到一些服务授权的问题，很多公司提供的页面往往并不是可以通过一个简单的URL就可以访问的，而必须经过注册然后登录后方可使用提供服务的页面，这个时候就涉及到COOKIE问题的处理。我们知道目前流行的动态网页技术例如ASP、JSP无不是通过COOKIE来处理会话信息的。为了使我们的程序能使用别人所提供的服务页面，就要求程序首先登录后再访问服务页面，这过程就需要自行处理cookie，想想当你用java.net.HttpURLConnection来完成这些功能时是多么恐怖的事情啊！况且这仅仅是我们所说的顽固的WEB服务器中的一个很常见的“顽固”！再有如通过HTTP来上传文件呢？不需要头疼，这些问题有了“它”就很容易解决了！
我们不可能列举所有可能的顽固，我们会针对几种最常见的问题进行处理。当然了，正如前面说到的，如果我们自己使用java.net.HttpURLConnection来搞定这些问题是很恐怖的事情，因此在开始之前我们先要介绍一下一个开放源码的项目，这个项目就是Apache开源组织中的httpclient，它隶属于Jakarta的commons项目，目前的版本是2.0RC2。commons下本来已经有一个net的子项目，但是又把httpclient单独提出来，可见http服务器的访问绝非易事。
Commons-httpclient项目就是专门设计来简化HTTP客户端与服务器进行各种通讯编程。通过它可以让原来很头疼的事情现在轻松的解决，例如你不再管是HTTP或者HTTPS的通讯方式，告诉它你想使用HTTPS方式，剩下的事情交给httpclient替你完成。本文会针对我们在编写HTTP客户端程序时经常碰到的几个问题进行分别介绍如何使用httpclient来解决它们，为了让读者更快的熟悉这个项目我们最开始先给出一个简单的例子来读取一个网页的内容，然后循序渐进解决掉前进中的所有问题。
### 1． 读取网页(HTTP/HTTPS)内容
下面是我们给出的一个简单的例子用来访问某个页面
```
1 /**
  2  *最简单的HTTP客户端,用来演示通过GET或者POST方式访问某个页面
  3   *@authorLiudong
  4 */
  5 public class SimpleClient {
  6 public static void main(String[] args) throws IOException
  7 {
  8   HttpClient client = new HttpClient();
  9       // 设置代理服务器地址和端口      
 10       //client.getHostConfiguration().setProxy("proxy_host_addr",proxy_port); 
 11       // 使用 GET 方法 ，如果服务器需要通过 HTTPS 连接，那只需要将下面 URL 中的 http 换成 https 
 12          HttpMethod method=new GetMethod("http://java.sun.com");
 13       //使用POST方法
 14       //HttpMethod method = new PostMethod("http://java.sun.com");
 15       client.executeMethod(method);
 16 
 17       //打印服务器返回的状态
 18       System.out.println(method.getStatusLine());
 19       //打印返回的信息
 20       System.out.println(method.getResponseBodyAsString());
 21       //释放连接
 22       method.releaseConnection();
 23    }
 24 }
```
在这个例子中首先创建一个HTTP客户端(HttpClient)的实例，然后选择提交的方法是GET或者POST，最后在HttpClient实例上执行提交的方法，最后从所选择的提交方法中读取服务器反馈回来的结果。这就是使用HttpClient的基本流程。其实用一行代码也就可以搞定整个请求的过程，非常的简单！
### 2、使用POST方式提交数据(httpClient3)
httpclient使用了单独的一个HttpMethod子类来处理文件的上传，这个类就是MultipartPostMethod，该类已经封装了文件上传的细节，我们要做的仅仅是告诉它我们要上传文件的全路径即可，下面这里将给出关于两种模拟上传方式的代码
#### 第一种：模拟上传url文件(该方式也适合做普通post请求):
```
1 /**
  2      * 上传url文件到指定URL
  3      * @param fileUrl 上传图片url
  4      * @param postUrl 上传路径及参数,注意有些中文参数需要使用预先编码 eg : URLEncoder.encode(appName, "UTF-8")
  5      * @return
  6      * @throws IOException
  7      */
  8     public static String doUploadFile(String postUrl) throws IOException {
  9         if(StringUtils.isEmpty(postUrl))
 10             return null;
 11         String response = "";
 12         PostMethod postMethod = new PostMethod(postUrl);
 13         try {
 14             HttpClient client = new HttpClient();
 15             client.getHttpConnectionManager().getParams()
 16                     .setConnectionTimeout(50000);// 设置连接时间
 17             int status = client.executeMethod(postMethod);
 18             if (status == HttpStatus.SC_OK) {
 19                 InputStream inputStream = postMethod.getResponseBodyAsStream();
 20                 BufferedReader br = new BufferedReader(new InputStreamReader(
 21                         inputStream));
 22                 StringBuffer stringBuffer = new StringBuffer();
 23                 String str = "";
 24                 while ((str = br.readLine()) != null) {
 25                     stringBuffer.append(str);
 26                 }
 27                 response = stringBuffer.toString();
 28             } else {
 29                 response = "fail";
 30             }
 31         } catch (Exception e) {
 32             e.printStackTrace();
 33         } finally {
 34             // 释放连接
 35             postMethod.releaseConnection();
 36         }
 37         return response;
 38     }
```
#### 第二种：模拟文件上传到指定位置
```
1 /**
  2      * 上传文件到指定URL
  3      * @param file
  4      * @param url
  5      * @return
  6      * @throws IOException
  7      */
  8     public static String doUploadFile(File file, String url) throws IOException {
  9         String response = "";
 10         if (!file.exists()) {
 11             return "file not exists";
 12         }
 13         PostMethod postMethod = new PostMethod(url);
 14         try {
 15             //----------------------------------------------
 16             // FilePart：用来上传文件的类,file即要上传的文件
 17             FilePart fp = new FilePart("file", file);
 18             Part[] parts = { fp };
 19 
 20             // 对于MIME类型的请求，httpclient建议全用MulitPartRequestEntity进行包装
 21             MultipartRequestEntity mre = new MultipartRequestEntity(parts,
 22                     postMethod.getParams());
 23             postMethod.setRequestEntity(mre);
 24             //---------------------------------------------
 25             HttpClient client = new HttpClient();
 26             client.getHttpConnectionManager().getParams()
 27                     .setConnectionTimeout(50000);// 由于要上传的文件可能比较大 , 因此在此设置最大的连接超时时间
 28             int status = client.executeMethod(postMethod);
 29             if (status == HttpStatus.SC_OK) {
 30                 InputStream inputStream = postMethod.getResponseBodyAsStream();
 31                 BufferedReader br = new BufferedReader(new InputStreamReader(
 32                         inputStream));
 33                 StringBuffer stringBuffer = new StringBuffer();
 34                 String str = "";
 35                 while ((str = br.readLine()) != null) {
 36                     stringBuffer.append(str);
 37                 }
 38                 response = stringBuffer.toString();
 39             } else {
 40                 response = "fail";
 41             }
 42         } catch (Exception e) {
 43             e.printStackTrace();
 44         } finally {
 45             // 释放连接
 46             postMethod.releaseConnection();
 47         }
 48         return response;
 49     }
```
### 3． 处理页面重定向
在JSP/Servlet编程中response.sendRedirect方法就是使用HTTP协议中的重定向机制。它与JSP中的<jsp:forward …>的区别在于后者是在服务器中实现页面的跳转，也就是说应用容器加载了所要跳转的页面的内容并返回给客户端；而前者是返回一个状态码，这些状态码的可能值见下表，然后客户端读取需要跳转到的页面的URL并重新加载新的页面。就是这样一个过程，所以我们编程的时候就要通过HttpMethod.getStatusCode()方法判断返回值是否为下表中的某个值来判断是否需要跳转。如果已经确认需要进行页面跳转了，那么可以通过读取HTTP头中的location属性来获取新的地址。
![image](https://images2017.cnblogs.com/blog/672567/201709/672567-20170905003441007-1484250007.png)
下面的代码片段演示如何处理页面的重定向
```
1 client.executeMethod(post);
  2 System.out.println(post.getStatusLine().toString());
  3 post.releaseConnection();
  4 // 检查是否重定向
  5 int statuscode = post.getStatusCode();
  6 if ((statuscode == HttpStatus.SC_MOVED_TEMPORARILY) || (statuscode == HttpStatus.SC_MOVED_PERMANENTLY) ||
  7 (statuscode ==HttpStatus.SC_SEE_OTHER) || (statuscode == HttpStatus.SC_TEMPORARY_REDIRECT)) {
  8 // 读取新的 URL 地址 
  9    Header header=post.getResponseHeader("location");
 10    if (header!=null){
 11       Stringnewuri=header.getValue();
 12       if((newuri==null)||(newuri.equals("")))
 13          newuri="/";
 14          GetMethodredirect=newGetMethod(newuri);
 15          client.executeMethod(redirect);
 16          System.out.println("Redirect:"+redirect.getStatusLine().toString());
 17          redirect.releaseConnection();
 18    }else
 19     System.out.println("Invalid redirect");
 20 }
```
我们可以自行编写两个JSP页面，其中一个页面用response.sendRedirect方法重定向到另外一个页面用来测试上面的例子。
### 4． 模拟登录开心网
本小节应该说是HTTP客户端编程中最常碰见的问题，很多网站的内容都只是对注册用户可见的，这种情况下就必须要求使用正确的用户名和口令登录成功后，方可浏览到想要的页面。因为HTTP协议是无状态的，也就是连接的有效期只限于当前请求，请求内容结束后连接就关闭了。在这种情况下为了保存用户的登录信息必须使用到Cookie机制。以JSP/Servlet为例，当浏览器请求一个JSP或者是Servlet的页面时，应用服务器会返回一个参数，名为jsessionid（因不同应用服务器而异），值是一个较长的唯一字符串的Cookie，这个字符串值也就是当前访问该站点的会话标识。浏览器在每访问该站点的其他页面时候都要带上jsessionid这样的Cookie信息，应用服务器根据读取这个会话标识来获取对应的会话信息。
对于需要用户登录的网站，一般在用户登录成功后会将用户资料保存在服务器的会话中，这样当访问到其他的页面时候，应用服务器根据浏览器送上的Cookie中读取当前请求对应的会话标识以获得对应的会话信息，然后就可以判断用户资料是否存在于会话信息中，如果存在则允许访问页面，否则跳转到登录页面中要求用户输入帐号和口令进行登录。这就是一般使用JSP开发网站在处理用户登录的比较通用的方法。
这样一来，对于HTTP的客户端来讲，如果要访问一个受保护的页面时就必须模拟浏览器所做的工作，首先就是请求登录页面，然后读取Cookie值；再次请求登录页面并加入登录页所需的每个参数；最后就是请求最终所需的页面。当然在除第一次请求外其他的请求都需要附带上Cookie信息以便服务器能判断当前请求是否已经通过验证。说了这么多，可是如果你使用httpclient的话，你甚至连一行代码都无需增加，你只需要先传递登录信息执行登录过程，然后直接访问想要的页面，跟访问一个普通的页面没有任何区别，因为类HttpClient已经帮你做了所有该做的事情了，太棒了！下面的例子实现了模拟登陆开心网并向自己好友发送消息的功能。
```
1 import java.io.BufferedReader;
  2 import java.io.IOException;
  3 import java.io.InputStream;
  4 import java.io.InputStreamReader;
  5 
  6 import org.apache.commons.httpclient.Cookie;
  7 import org.apache.commons.httpclient.Header;
  8 import org.apache.commons.httpclient.HttpClient;
  9 import org.apache.commons.httpclient.HttpStatus;
 10 import org.apache.commons.httpclient.NameValuePair;
 11 import org.apache.commons.httpclient.cookie.CookiePolicy;
 12 import org.apache.commons.httpclient.methods.GetMethod;
 13 import org.apache.commons.httpclient.methods.PostMethod;
 14 import org.apache.commons.httpclient.params.HttpClientParams;
 15 import org.apache.commons.httpclient.params.HttpMethodParams;
 16 
 17 class Login {
 18     public static String loginurl = "https://security.kaixin001.com/login/login_post.php";
 19     static Cookie[] cookies = {};
 20 
 21     static HttpClient httpClient = new HttpClient();
 22 
 23     static String email = "xxx@qq.com";//你的email
 24     static String psw = "xxx";//你的密码
 25     // 消息发送的action
 26     String url = "http://www.kaixin001.com/home/";
 27 
 28     public static void getUrlContent()
 29             throws Exception {
 30 
 31         HttpClientParams httparams = new HttpClientParams();
 32         httparams.setSoTimeout(30000);
 33         httpClient.setParams(httparams);
 34 
 35         httpClient.getHostConfiguration().setHost("www.kaixin001.com", 80);
 36 
 37         httpClient.getParams().setParameter(
 38                 HttpMethodParams.HTTP_CONTENT_CHARSET, "UTF-8");
 39 
 40         PostMethod login = new PostMethod(loginurl);
 41         login.addRequestHeader("Content-Type",
 42                 "application/x-www-form-urlencoded; charset=UTF-8");
 43 
 44         NameValuePair Email = new NameValuePair("loginemail", email);// 邮箱
 45         NameValuePair password = new NameValuePair("password", psw);// 密码
 46         // NameValuePair code = new NameValuePair( "code"
 47         // ,"????");//有时候需要验证码，暂时未解决
 48 
 49         NameValuePair[] data = { Email, password };
 50         login.setRequestBody(data);
 51 
 52         httpClient.executeMethod(login);
 53         int statuscode = login.getStatusCode();
 54         System.out.println(statuscode + "-----------");
 55         String result = login.getResponseBodyAsString();
 56         System.out.println(result+"++++++++++++");
 57 
 58         cookies = httpClient.getState().getCookies();
 59         System.out.println("==========Cookies============");
 60         int i = 0;
 61         for (Cookie c : cookies) {
 62             System.out.println(++i + ":   " + c);
 63         }
 64         httpClient.getState().addCookies(cookies);
 65 
 66         // 当state为301或者302说明登陆页面跳转了，登陆成功了
 67         if ((statuscode == HttpStatus.SC_MOVED_TEMPORARILY)
 68                 || (statuscode == HttpStatus.SC_MOVED_PERMANENTLY)
 69                 || (statuscode == HttpStatus.SC_SEE_OTHER)
 70                 || (statuscode == HttpStatus.SC_TEMPORARY_REDIRECT)) {
 71             // 读取新的 URL 地址
 72             Header header = login.getResponseHeader("location");
 73             // 释放连接
 74             login.releaseConnection();
 75             System.out.println("获取到跳转header>>>" + header);
 76             if (header != null) {
 77                 String newuri = header.getValue();
 78                 if ((newuri == null) || (newuri.equals("")))
 79                     newuri = "/";
 80                 GetMethod redirect = new GetMethod(newuri);
 81                 // ////////////
 82                 redirect.setRequestHeader("Cookie", cookies.toString());
 83                 httpClient.executeMethod(redirect);
 84                 System.out.println("Redirect:"
 85                         + redirect.getStatusLine().toString());
 86                 redirect.releaseConnection();
 87 
 88             } else
 89                 System.out.println("Invalid redirect");
 90         } else {
 91             // 用户名和密码没有被提交，当登陆多次后需要验证码的时候会出现这种未提交情况
 92             System.out.println("用户没登陆");
 93             System.exit(1);
 94         }
 95 
 96     }
 97 
 98     public static void sendMsg() throws Exception {
 99         // 登录后发消息
100         System.out.println("*************发消息***********");
101 
102         String posturl = "http://www.kaixin001.com/msg/post.php";
103         PostMethod poster = new PostMethod(posturl);
104 
105         poster.addRequestHeader("Content-Type",
106                 "application/x-www-form-urlencoded; charset=UTF-8");
107         poster.setRequestHeader("Cookie", cookies.toString());
108 
109         NameValuePair uids = new NameValuePair("uids", "89600585");// 发送的好友对象的id,此处换成你的好友id
110         NameValuePair content = new NameValuePair("content", "你好啊!");// 需要发送的信息的内容
111         NameValuePair liteeditor_0 = new NameValuePair("liteeditor_0", "你好啊!");// 需要发送的信息的内容
112         NameValuePair texttype = new NameValuePair("texttype", "plain");
113         NameValuePair send_separate = new NameValuePair("send_separate", "0");
114         NameValuePair service = new NameValuePair("service", "0");
115         NameValuePair[] msg = { uids, content, texttype, send_separate, service,liteeditor_0 };
116 
117         poster.setRequestBody(msg);
118         httpClient.executeMethod(poster);
119 
120         String result = poster.getResponseBodyAsString();
121         System.out.println(result+"++++++++++++");
122         //System.out.println(StreamOut(result, "iso8859-1"));
123         int statuscode = poster.getStatusCode();
124         System.out.println(statuscode + "-----------");
125         if(statuscode == 301 || statuscode == 302){
126             // 读取新的 URL 地址
127             Header header = poster.getResponseHeader("location");
128             System.out.println("获取到跳转header>>>" + header);
129             if (header != null) {
130                 String newuri = header.getValue();
131                 if ((newuri == null) || (newuri.equals("")))
132                     newuri = "/";
133                 GetMethod redirect = new GetMethod(newuri);
134                 // ////////////
135                 redirect.setRequestHeader("Cookie", cookies.toString());
136                 httpClient.executeMethod(redirect);
137                 System.out.println("Redirect:"
138                         + redirect.getStatusLine().toString());
139                 redirect.releaseConnection();
140 
141             } else
142                 System.out.println("Invalid redirect");
143         }
144 
145             poster.releaseConnection();
146     }
147 
148     public static String StreamOut(InputStream txtis, String code)
149             throws IOException {
150         BufferedReader br = new BufferedReader(new InputStreamReader(txtis,
151                 code));
152         String tempbf;
153         StringBuffer html = new StringBuffer(100);
154         while ((tempbf = br.readLine()) != null) {
155             html.append(tempbf + "\n");
156         }
157         return html.toString();
158 
159     }
160 }
```
### 5． 提交XML格式参数
提交XML格式的参数很简单，仅仅是一个提交时候的ContentType问题，下面的例子演示从文件文件中读取XML信息并提交给服务器的过程，该过程可以用来测试Web服务。
```
1 import java.io.File;
  2 import java.io.FileInputStream;
  3 import org.apache.commons.httpclient.HttpClient;
  4 import org.apache.commons.httpclient.methods.EntityEnclosingMethod;
  5 import org.apache.commons.httpclient.methods.PostMethod;
  6 /**
  7  *用来演示提交XML格式数据的例子
  8 */
  9 public class PostXMLClient {
 10 
 11    public static void main(String[] args) throws Exception {
 12       File input = new File(“test.xml”);
 13       PostMethod post = new PostMethod(“http://localhost:8080/httpclient/xml.jsp”);
 14 
 15       // 设置请求的内容直接从文件中读取
 16       post.setRequestBody( new FileInputStream(input));
 17       if (input.length() < Integer.MAX_VALUE)
 18          post.setRequestContentLength(input.length());
 19       else
 20          post.setRequestContentLength(EntityEnclosingMethod.CONTENT_LENGTH_CHUNKED);
 21 
 22       // 指定请求内容的类型
 23       post.setRequestHeader( "Content-type" , "text/xml; charset=GBK" );
 24       HttpClient httpclient = new HttpClient();
 25       int result = httpclient.executeMethod(post);
 26       System.out.println( "Response status code: " + result);
 27       System.out.println( "Response body: " );
 28       System.out.println(post.getResponseBodyAsString());
 29       post.releaseConnection();
 30    }
 31 }
```
### 6． 访问启用认证的页面
我们经常会碰到这样的页面，当访问它的时候会弹出一个浏览器的对话框要求输入用户名和密码后方可，这种用户认证的方式不同于我们在前面介绍的基于表单的用户身份验证。这是HTTP的认证策略，httpclient支持三种认证方式包括：基本、摘要以及NTLM认证。其中基本认证最简单、通用但也最不安全；摘要认证是在HTTP 1.1中加入的认证方式，而NTLM则是微软公司定义的而不是通用的规范，最新版本的NTLM是比摘要认证还要安全的一种方式。
下面例子是从httpclient的CVS服务器中下载的，它简单演示如何访问一个认证保护的页面：
```
1 import org.apache.commons.httpclient.HttpClient;
  2 import org.apache.commons.httpclient.UsernamePasswordCredentials;
  3 import org.apache.commons.httpclient.methods.GetMethod;
  4 
  5 public class BasicAuthenticationExample {
  6 
  7    public BasicAuthenticationExample() {
  8    }
  9 
 10    public static void main(String[] args) throws Exception {
 11       HttpClient client = new HttpClient();
 12       client.getState().setCredentials( "www.verisign.com" , "realm" , new UsernamePasswordCredentials( "username" , "password" ) );
 13 
 14       GetMethod get = new GetMethod( "https://www.verisign.com/products/index.html" );
 15       get.setDoAuthentication( true );
 16       int status = client.executeMethod( get );
 17       System.out.println(status+ "\n" + get.getResponseBodyAsString());
 18       get.releaseConnection();
 19    }
 20 }
```
### 7． 多线程模式下使用
多线程同时访问httpclient，例如同时从一个站点上下载多个文件。对于同一个HttpConnection同一个时间只能有一个线程访问，为了保证多线程工作环境下不产生冲突，httpclient使用了一个多线程连接管理器的类：MultiThreadedHttpConnectionManager，要使用这个类很简单，只需要在构造HttpClient实例的时候传入即可，代码如下：
MultiThreadedHttpConnectionManager connectionManager = **new **MultiThreadedHttpConnectionManager();
HttpClient client = **new **HttpClient(connectionManager);
以后尽管访问client实例即可。
## httpClient完整封装
HttpInvoke.java:封装了HttpClient调度的必要参数设置,以及post,get等常用方法
```
1 import org.apache.commons.logging.Log;
  2 import org.apache.commons.logging.LogFactory;
  3 import org.apache.commons.httpclient.*;
  4 import org.apache.commons.httpclient.auth.AuthScope;
  5 import org.apache.commons.httpclient.methods.PostMethod;
  6 import org.apache.commons.httpclient.methods.GetMethod;
  7 
  8 import java.util.Iterator;
  9 import java.util.Map;
 10 import java.net.SocketTimeoutException;
 11 import java.io.BufferedReader;
 12 import java.io.InputStreamReader;
 13 
 14 public class HttpInvoker {
 15     private Log logger = LogFactory.getLog(HttpInvoker.class);
 16     private static HttpInvoker httpInvoker = new HttpInvoker();
 17     private HttpClient client = null;
 18     private String charset = "gbk";
 19     private int timeout = 10000;
 20     private boolean useProxy = false;
 21     private String proxyHost = null;
 22     private int proxyPort;
 23     private String proxyUsername = null;
 24     private String proxyPassword = null;
 25     private boolean initialized = false;
 26 
 27     public static HttpInvoker getInstance() {
 28         return httpInvoker;
 29     }
 30 
 31     private HttpInvoker() {
 32         client = new HttpClient(new MultiThreadedHttpConnectionManager());
 33         client.getParams().setParameter("http.protocol.content-charset", "gbk");
 34         client.getParams().setContentCharset("gbk");
 35         client.getParams().setSoTimeout(timeout);
 36     }
 37 
 38     public HttpInvoker(String charset, int timeout, boolean useProxy,
 39                        String proxyHost, int proxyPort, String proxyUsername,
 40                        String proxyPassword) {
 41         client = new HttpClient(new MultiThreadedHttpConnectionManager());
 42         if(charset != null && !charset.trim().equals("")) {
 43             this.charset = charset;
 44         }
 45         if(timeout > 0) {
 46             this.timeout = timeout;
 47         }
 48         client.getParams().setParameter("http.protocol.content-charset", charset);
 49         client.getParams().setContentCharset(charset);
 50         client.getParams().setSoTimeout(timeout);
 51         if(useProxy && proxyHost != null &&
 52                 !proxyHost.trim().equals("") && proxyPort > 0) {
 53             HostConfiguration hc = new HostConfiguration();
 54             hc.setProxy(proxyHost, proxyPort);
 55             client.setHostConfiguration(hc);
 56             if (proxyUsername != null && !proxyUsername.trim().equals("") &&
 57                     proxyPassword != null && !proxyPassword.trim().equals("")) {
 58                 client.getState().setProxyCredentials(AuthScope.ANY,
 59                     new UsernamePasswordCredentials(proxyUsername, proxyPassword));
 60             }
 61         }
 62         initialized = true;
 63         logger.debug("HttpInvoker初始化完成");
 64     }
 65 
 66     public synchronized void init() {
 67         if(charset != null && !charset.trim().equals("")) {
 68             client.getParams().setParameter("http.protocol.content-charset", charset);
 69             client.getParams().setContentCharset(charset);
 70         }
 71         if(timeout > 0) {
 72             client.getParams().setSoTimeout(timeout);
 73         }
 74         if(useProxy && proxyHost != null &&
 75                 !proxyHost.trim().equals("") && proxyPort > 0) {
 76             HostConfiguration hc = new HostConfiguration();
 77             hc.setProxy(proxyHost, proxyPort);
 78             client.setHostConfiguration(hc);
 79             if (proxyUsername != null && !proxyUsername.trim().equals("") &&
 80                     proxyPassword != null && !proxyPassword.trim().equals("")) {
 81                 client.getState().setProxyCredentials(AuthScope.ANY,
 82                     new UsernamePasswordCredentials(proxyUsername, proxyPassword));
 83             }
 84         }
 85         initialized = true;
 86         logger.debug("HttpInvoker初始化完成");
 87     }
 88 
 89     public String invoke(String url) throws Exception {
 90         return invoke(url, null, false);
 91     }
 92 
 93     public String invoke(String url, Map params, boolean isPost) throws Exception {
 94         logger.debug("HTTP调用[" + (isPost?"POST":"GET") + "][" + url + "][" + params + "]");
 95         HttpMethod httpMethod = null;
 96         String result = "";
 97         try {
 98             if(isPost && params != null && params.size() > 0) {
 99                 Iterator paramKeys = params.keySet().iterator();
100                 httpMethod = new PostMethod(url);
101                 NameValuePair[] form = new NameValuePair[params.size()];
102                 int formIndex = 0;
103                 while(paramKeys.hasNext()) {
104                     String key = (String)paramKeys.next();
105                     Object value = params.get(key);
106                     if(value != null && value instanceof String && !value.equals("")) {
107                         form[formIndex] = new NameValuePair(key, (String)value);
108                         formIndex++;
109                     } else if(value != null && value instanceof String[] &&
110                             ((String[])value).length > 0) {
111                         NameValuePair[] tempForm =
112                                 new NameValuePair[form.length + ((String[])value).length - 1];
113                         for(int i=0; i<formIndex; i++) {
114                             tempForm[i] = form[i];
115                         }
116                         form = tempForm;
117                         for(String v : (String[])value) {
118                             form[formIndex] = new NameValuePair(key, (String)v);
119                             formIndex++;
120                         }
121                     }
122                 }
123                 ((PostMethod)httpMethod).setRequestBody(form);
124             } else {
125                 if(params != null && params.size() > 0) {
126                     Iterator paramKeys = params.keySet().iterator();
127                     StringBuffer getUrl = new StringBuffer(url.trim());
128                     if(url.trim().indexOf("?") > -1) {
129                         if(url.trim().indexOf("?") < url.trim().length()-1 &&
130                                 url.trim().indexOf("&")  < url.trim().length()-1) {
131                             getUrl.append("&");
132                         }
133                     } else {
134                         getUrl.append("?");
135                     }
136                     while(paramKeys.hasNext()) {
137                         String key = (String)paramKeys.next();
138                         Object value = params.get(key);
139                         if(value != null && value instanceof String && !value.equals("")) {
140                             getUrl.append(key).append("=").append(value).append("&");
141                         } else if(value != null && value instanceof String[] &&
142                                 ((String[])value).length > 0) {
143                             for(String v : (String[])value) {
144                                 getUrl.append(key).append("=").append(v).append("&");
145                             }
146                         }
147                     }
148                     if(getUrl.lastIndexOf("&") == getUrl.length()-1) {
149                         httpMethod = new GetMethod(getUrl.substring(0, getUrl.length()-1));
150                     } else {
151                         httpMethod = new GetMethod(getUrl.toString());
152                     }
153                 } else {
154                     httpMethod = new GetMethod(url);
155                 }
156             }
157             client.executeMethod(httpMethod);
158 //            result = httpMethod.getResponseBodyAsString();
159             BufferedReader reader = new BufferedReader(new InputStreamReader(
160                     httpMethod.getResponseBodyAsStream(),"ISO-8859-1"));
161             String line = null;
162             String html = null;
163             while((line = reader.readLine()) != null){
164                 if(html == null) {
165                     html = "";
166                 } else {
167                     html += "\r\n";
168                 }
169                 html += line;
170             }
171             if(html != null) {
172                 result = new String(html.getBytes("ISO-8859-1"), charset);
173             }
174         } catch (SocketTimeoutException e) {
175             logger.error("连接超时[" + url + "]");
176             throw e;
177         } catch (java.net.ConnectException e) {
178             logger.error("连接失败[" + url + "]");
179             throw e;
180         } catch (Exception e) {
181             logger.error("连接时出现异常[" + url + "]");
182             throw e;
183         } finally {
184             if (httpMethod != null) {
185                 try {
186                     httpMethod.releaseConnection();
187                 } catch (Exception e) {
188                     logger.error("释放网络连接失败[" + url + "]");
189                     throw e;
190                 }
191             }
192         }
193 
194         return result;
195     }
196 
197     public void setCharset(String charset) {
198         this.charset = charset;
199     }
200 
201     public void setTimeout(int timeout) {
202         this.timeout = timeout;
203     }
204 
205     public void setProxyHost(String proxyHost) {
206         this.proxyHost = proxyHost;
207     }
208 
209     public void setProxyPort(int proxyPort) {
210         this.proxyPort = proxyPort;
211     }
212 
213     public void setProxyUsername(String proxyUsername) {
214         this.proxyUsername = proxyUsername;
215     }
216 
217     public void setProxyPassword(String proxyPassword) {
218         this.proxyPassword = proxyPassword;
219     }
220 
221     public void setUseProxy(boolean useProxy) {
222         this.useProxy = useProxy;
223     }
224 
225     public synchronized boolean isInitialized() {
226         return initialized;
227     }
228 }
```
http访问网络的代理ip和端口，还有使用用户及密码都可以在Spring容器中注入进来:
```
1 <bean id="httpInvoker" class="HttpInvoker">
  2         <constructor-arg type="java.lang.String" value="gbk" /><!--useProxy-->
  3         <constructor-arg type="int" value="10000" /><!--useProxy-->
  4         <constructor-arg type="boolean" value="true" /><!--useProxy-->
  5         <!--代理地址 -->
  6         <constructor-arg type="java.lang.String" value="192.168.1.1" />
  7         <constructor-arg type="int" value="8080" />
  8         <constructor-arg type="java.lang.String" value="" /><!--用户名-->
  9         <constructor-arg type="java.lang.String" value="" /><!--密码-->
 10 </bean>
```
使用方式：post
Map<String,String> params = new HashMap<String,String>();
params.put("check", check);
String result = httpInvoker.invoke( "someURL", params, true);
使用方式：get
String content  = httpInvoker.invoke(url);
## 参考资料：
httpclient首页： [http://jakarta.apache.org/commons/httpclient/](http://jakarta.apache.org/commons/httpclient/)
关于NTLM是如何工作：  [http://davenport.sourceforge.net/ntlm.html](http://davenport.sourceforge.net/ntlm.html)
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
HttpClient入门
[http://blog.csdn.net/ambitiontan/archive/2006/01/07/572644.aspx](http://blog.csdn.net/ambitiontan/archive/2006/01/07/572644.aspx)
Jakarta Commons HttpClient 学习笔记
[http://blog.csdn.net/cxl34/archive/2005/01/19/259051.aspx](http://blog.csdn.net/cxl34/archive/2005/01/19/259051.aspx)
Cookies,SSL，httpclient的多线程处理，HTTP方法
[http://blog.csdn.net/bjbs_270/archive/2004/11/05/168233.aspx](http://blog.csdn.net/bjbs_270/archive/2004/11/05/168233.aspx)
[HttpClient 学习整理](http://www.blogjava.net/Alpha/archive/2007/01/22/95216.html)
内容来自：[cnblogs：牛奶、不加糖](http://www.cnblogs.com/ITtangtang/archive/2012/04/22/2465382.html)
