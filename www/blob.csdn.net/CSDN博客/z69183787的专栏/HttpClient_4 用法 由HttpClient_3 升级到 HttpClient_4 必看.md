# HttpClient_4 用法 由HttpClient_3 升级到 HttpClient_4 必看 - z69183787的专栏 - CSDN博客
2015年07月17日 11:55:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：645
个人分类：[Httpclient-简介](https://blog.csdn.net/z69183787/article/category/2185975)
HttpClient程序包是一个实现了 HTTP 协议的客户端编程工具包，要想熟练的掌握它，必须熟悉 HTTP协议。一个最简单的调用如下：
import java.io.IOException;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.client.DefaultHttpClient;
public class Test {
    public static void main(String[] args) {
       // 核心应用类
       HttpClient httpClient = new DefaultHttpClient();
        // HTTP请求
        HttpUriRequest request =
                new HttpGet("http://localhost/index.html");
        // 打印请求信息
        System.out.println(request.getRequestLine());
        try {
            // 发送请求，返回响应
            HttpResponse response = httpClient.execute(request);
            // 打印响应信息
            System.out.println(response.getStatusLine());
        } catch (ClientProtocolException e) {
            // 协议错误
            e.printStackTrace();
        } catch (IOException e) {
            // 网络异常
            e.printStackTrace();
        }
    }
}
如果HTTP服务器正常并且存在相应的服务，则上例会打印出两行结果：
    GET http://localhost/index.html HTTP/1.1
    HTTP/1.1 200 OK 
核心对象httpClient的调用非常直观，其execute方法传入一个request对象，返回一个response对象。使用 httpClient发出HTTP请求时，系统可能抛出两种异常，分别是ClientProtocolException和IOException。第一种异常的发生通常是协议错误导致，如在构造HttpGet对象时传入的协议不对（例如不小心将”http”写成”htp”），或者服务器端返回的内容不符合HTTP协议要求等；第二种异常一般是由于网络原因引起的异常，如HTTP服务器未启动等。
从实际应用的角度看，HTTP协议由两大部分组成：HTTP请求和HTTP响应。那么HttpClient程序包是如何实现HTTP客户端应用的呢？实现过程中需要注意哪些问题呢？
HTTP请求
HTTP 1.1由以下几种请求组成：GET, HEAD, POST, PUT, DELETE, TRACE and OPTIONS， 程序包中分别用HttpGet, HttpHead, HttpPost, HttpPut, HttpDelete, HttpTrace, and HttpOptions 这几个类创建请求。所有的这些类均实现了HttpUriRequest接口，故可以作为execute的执行参数使用。
所有请求中最常用的是GET与POST两种请求，与创建GET请求的方法相同，可以用如下方法创建一个POST请求：
HttpUriRequest request = new HttpPost(
        "http://localhost/index.html");
HTTP请求格式告诉我们，有两个位置或者说两种方式可以为request提供参数：request-line方式与request-body方式。
request-line
request-line方式是指在请求行上通过URI直接提供参数。
（1）
我们可以在生成request对象时提供带参数的URI，如：
HttpUriRequest request = new HttpGet(
        "http://localhost/index.html?param1=value1¶m2=value2");
（2）
另外，HttpClient程序包为我们提供了URIUtils工具类，可以通过它生成带参数的URI，如：
URI uri = URIUtils.createURI("http", "localhost", -1, "/index.html",
    "param1=value1¶m2=value2", null);
HttpUriRequest request = new HttpGet(uri);
System.out.println(request.getURI());
上例的打印结果如下：
    http://localhost/index.html?param1=value1¶m2=value2
（3）
需要注意的是，如果参数中含有中文，需将参数进行URLEncoding处理，如：
String param = "param1=" + URLEncoder.encode("中国", "UTF-8") + "¶m2=value2";
URI uri = URIUtils.createURI("http", "localhost", 8080,
"/sshsky/index.html", param, null);
System.out.println(uri);
上例的打印结果如下：
    http://localhost/index.html?param1=%E4%B8%AD%E5%9B%BD¶m2=value2
（4）
对于参数的URLEncoding处理，HttpClient程序包为我们准备了另一个工具类：URLEncodedUtils。通过它，我们可以直观的（但是比较复杂）生成URI，如：
List params = new ArrayList();
params.add(new BasicNameValuePair("param1", "中国"));
params.add(new BasicNameValuePair("param2", "value2"));
String param = URLEncodedUtils.format(params, "UTF-8");
URI uri = URIUtils.createURI("http", "localhost", 8080,
"/sshsky/index.html", param, null);
System.out.println(uri);
上例的打印结果如下：
    http://localhost/index.html?param1=%E4%B8%AD%E5%9B%BD¶m2=value2
request-body
与request-line方式不同，request-body方式是在request-body中提供参数，此方式只能用于POST请求。在 HttpClient程序包中有两个类可以完成此项工作，它们分别是UrlEncodedFormEntity类与MultipartEntity类。这两个类均实现了HttpEntity接口。
（1）
使用最多的是UrlEncodedFormEntity类。通过该类创建的对象可以模拟传统的HTML表单传送POST请求中的参数。如下面的表单：
<form action="http://localhost/index.html" method="POST">
    <input type="text" name="param1" value="中国"/>
    <input type="text" name="param2" value="value2"/>
    <inupt type="submit" value="submit"/>
</form>
我们可以用下面的代码实现：
List formParams = new ArrayList();
formParams.add(new BasicNameValuePair("param1", "中国"));
formParams.add(new BasicNameValuePair("param2", "value2"));
HttpEntity entity = new UrlEncodedFormEntity(formParams, "UTF-8");
HttpPost request = new HttpPost(“http://localhost/index.html”);
request.setEntity(entity);
当然，如果想查看HTTP数据格式，可以通过HttpEntity对象的各种方法取得。如：
List formParams = new ArrayList();
formParams.add(new BasicNameValuePair("param1", "中国"));
formParams.add(new BasicNameValuePair("param2", "value2"));
UrlEncodedFormEntity entity = new UrlEncodedFormEntity(formParams, "UTF-8");
System.out.println(entity.getContentType());
System.out.println(entity.getContentLength());
System.out.println(EntityUtils.getContentCharSet(entity));
System.out.println(EntityUtils.toString(entity));
上例的打印结果如下：
    Content-Type: application/x-www-form-urlencoded; charset=UTF-8
    39
    UTF-8
    param1=%E4%B8%AD%E5%9B%BD¶m2=value2 
（2）
除了传统的application/x-www-form-urlencoded表单，我们另一个经常用到的是上传文件用的表单，这种表单的类型为 multipart/form-data。在HttpClient程序扩展包（HttpMime）中专门有一个类与之对应，那就是 MultipartEntity类。此类同样实现了HttpEntity接口。如下面的表单：
<form action="http://localhost/index.html" method="POST"
        enctype="multipart/form-data">
    <input type="text" name="param1" value="中国"/>
    <input type="text" name="param2" value="value2"/>
    <input type="file" name="param3"/>
    <inupt type="submit" value="submit"/>
</form>
我们可以用下面的代码实现：
MultipartEntity entity = new MultipartEntity();
entity.addPart("param1", new StringBody("中国", Charset.forName("UTF-8")));
entity.addPart("param2", new StringBody("value2", Charset.forName("UTF-8")));
entity.addPart("param3", new FileBody(new File("C:\\1.txt")));
HttpPost request = new HttpPost(“http://localhost/index.html”);
request.setEntity(entity);
HTTP响应
HttpClient程序包对于HTTP响应的处理较之HTTP请求来说是简单多了，其过程同样使用了HttpEntity接口。我们可以从 HttpEntity对象中取出数据流（InputStream），该数据流就是服务器返回的响应数据。需要注意的是，HttpClient程序包不负责解析数据流中的内容。如：
HttpUriRequest request = ...;
HttpResponse response = httpClient.execute(request);
// 从response中取出HttpEntity对象
HttpEntity entity = response.getEntity();
// 查看entity的各种指标
System.out.println(entity.getContentType());
System.out.println(entity.getContentLength());
System.out.println(EntityUtils.getContentCharSet(entity));
// 取出服务器返回的数据流
InputStream stream = entity.getContent();
// 以任意方式操作数据流stream
// 调用方式 略
附注：
本文说明的是HttpClient 4.0.1，该程序包（包括依赖的程序包）由以下几个JAR包组成：
commons-logging-1.1.1.jar
commons-codec-1.4.jar
httpcore-4.0.1.jar
httpclient-4.0.1.jar
apache-mime4j-0.6.jar
httpmime-4.0.1.jar
可以在此处下载完整的JAR包。

现在Apache已经发布了：HttpCore 4.0-beta3、HttpClient 4.0-beta1。
到此处可以去下载这些源代码：http://hc.apache.org/downloads.cgi
另外，还需要apache-mime4j-0.5.jar 包。
在这里先写个简单的POST方法，中文资料不多，英文不太好。
package test;
import java.util.ArrayList;
import java.util.List;
import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.params.CookiePolicy;
import org.apache.http.client.params.ClientPNames;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.EntityUtils;
public class Test2 {
    public static void main(String[] args) throws Exception {
        DefaultHttpClient httpclient = new DefaultHttpClient();      //实例化一个HttpClient
        HttpResponse response = null;
        HttpEntity entity = null;
        httpclient.getParams().setParameter(
                ClientPNames.COOKIE_POLICY, CookiePolicy.BROWSER_COMPATIBILITY);  //设置cookie的兼容性
        HttpPost httpost = new HttpPost("http://127.0.0.1:8080/pub/jsp/getInfo");           //引号中的参数是：servlet的地址
        List <NameValuePair> nvps = new ArrayList <NameValuePair>();                     
        nvps.add(new BasicNameValuePair("jqm", "fb1f7cbdaf2bf0a9cb5d43736492640e0c4c0cd0232da9de"));  
        //   BasicNameValuePair("name", "value"), name是post方法里的属性, value是传入的参数值
        nvps.add(new BasicNameValuePair("sqm", "1bb5b5b45915c8"));
        httpost.setEntity(new UrlEncodedFormEntity(nvps, HTTP.UTF_8));            //将参数传入post方法中
        response = httpclient.execute(httpost);                                               //执行
        entity = response.getEntity();                                                             //返回服务器响应
        try{
            System.out.println("----------------------------------------");
            System.out.println(response.getStatusLine());                           //服务器返回状态
            Header[] headers = response.getAllHeaders();                    //返回的HTTP头信息
            for (int i=0; i<headers.length; i++) {                              
            System.out.println(headers[i]);
            }
            System.out.println("----------------------------------------");
            String responseString = null;
            if (response.getEntity() != null) {
            responseString = EntityUtils.toString(response.getEntity());      / /返回服务器响应的HTML代码
            System.out.println(responseString);                                   //打印出服务器响应的HTML代码
            }
        } finally {
            if (entity != null)                          
            entity.consumeContent();                                                   // release connection gracefully
        }
        System.out.println("Login form get: " + response.getStatusLine());
        if (entity != null) {
        entity.consumeContent();
        }
    }
}

HttpClient4.0 学习实例 - 页面获取
HttpClient 4.0出来不久，所以网络上面相关的实例教程不多，搜httpclient得到的大部分都是基于原 Commons HttpClient 3.1 (legacy) 包的，官网下载页面：http://hc.apache.org/downloads.cgi，如果大家看了官网说明就明白httpclient4.0是从原包分支出来独立成包的，以后原来那个包中的httpclient不会再升级，所以以后我们是用httpclient新分支，由于4.0与之前的3.1包结构以及接口等都有较大变化，所以网上搜到的实例大部分都是不适合4.0的，当然，我们可以通过那些实例去琢磨4.0的用法，我也是新手，记录下学习过程方便以后检索
本实例我们来获取抓取网页编码，内容等信息
默认情况下，服务器端会根据客户端的请求头信息来返回服务器支持的编码，像google.cn他本身支持utf-8,gb2312等编码，所以如果你在头部中不指定任何头部信息的话他默认会返回gb2312编码，而如果我们在浏览器中直接访问google.cn，通过httplook，或者firefox 的firebug插件查看返回头部信息的话会发现他返回的是UTF-8编码
下面我们还是看实例来解说吧，注释等我也放代码里面解释，放完整代码，方便新手理解
本实例将
使用的httpclient相关包
httpclient-4.0.jar
httpcore-4.0.1.jar
httpmime-4.0.jar
commons-logging-1.0.4.jar等其它相关包
// HttpClientTest.java
package com.baihuo.crawler.test;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
class HttpClientTest {
    public final static void main(String[] args) throws Exception {
        // 初始化，此处构造函数就与3.1中不同
        HttpClient httpclient = new DefaultHttpClient();
        HttpHost targetHost = new HttpHost("www.google.cn");
        //HttpGet httpget = new HttpGet("http://www.apache.org/");
        HttpGet httpget = new HttpGet("/");
        // 查看默认request头部信息
        System.out.println("Accept-Charset:" + httpget.getFirstHeader("Accept-Charset"));
        // 以下这条如果不加会发现无论你设置Accept-Charset为gbk还是utf-8，他都会默认返回gb2312（本例针对google.cn来说）
        httpget.setHeader("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.1.2)");
        // 用逗号分隔显示可以同时接受多种编码
        httpget.setHeader("Accept-Language", "zh-cn,zh;q=0.5");
        httpget.setHeader("Accept-Charset", "GB2312,utf-8;q=0.7,*;q=0.7");
        // 验证头部信息设置生效
        System.out.println("Accept-Charset:" + httpget.getFirstHeader("Accept-Charset").getValue());
        // Execute HTTP request
        System.out.println("executing request " + httpget.getURI());
        HttpResponse response = httpclient.execute(targetHost, httpget);
        //HttpResponse response = httpclient.execute(httpget);
        System.out.println("----------------------------------------");
        System.out.println("Location: " + response.getLastHeader("Location"));
        System.out.println(response.getStatusLine().getStatusCode());
        System.out.println(response.getLastHeader("Content-Type"));
        System.out.println(response.getLastHeader("Content-Length"));
        System.out.println("----------------------------------------");
        // 判断页面返回状态判断是否进行转向抓取新链接
        int statusCode = response.getStatusLine().getStatusCode();
        if ((statusCode == HttpStatus.SC_MOVED_PERMANENTLY) ||
                (statusCode == HttpStatus.SC_MOVED_TEMPORARILY) ||
                (statusCode == HttpStatus.SC_SEE_OTHER) ||
                (statusCode == HttpStatus.SC_TEMPORARY_REDIRECT)) {
            // 此处重定向处理  此处还未验证
            String newUri = response.getLastHeader("Location").getValue();
            httpclient = new DefaultHttpClient();
            httpget = new HttpGet(newUri);
            response = httpclient.execute(httpget);
        }
        // Get hold of the response entity
        HttpEntity entity = response.getEntity();
        // 查看所有返回头部信息
        Header headers[] = response.getAllHeaders();
        int ii = 0;
        while (ii < headers.length) {
            System.out.println(headers[ii].getName() + ": " + headers[ii].getValue());
            ++ii;
        }
        // If the response does not enclose an entity, there is no need
        // to bother about connection release
        if (entity != null) {
            // 将源码流保存在一个byte数组当中，因为可能需要两次用到该流，
            byte[] bytes = EntityUtils.toByteArray(entity);
            String charSet = "";
            // 如果头部Content-Type中包含了编码信息，那么我们可以直接在此处获取
            charSet = EntityUtils.getContentCharSet(entity);
            System.out.println("In header: " + charSet);
            // 如果头部中没有，那么我们需要 查看页面源码，这个方法虽然不能说完全正确，因为有些粗糙的网页编码者没有在页面中写头部编码信息
            if (charSet == "") {
                regEx="(?=<meta).*?(?<=charset=[\\'|\\\"]?)([[a-z]|[A-Z]|[0-9]|-]*)";
                p=Pattern.compile(regEx, Pattern.CASE_INSENSITIVE);
                m=p.matcher(new String(bytes));  // 默认编码转成字符串，因为我们的匹配中无中文，所以串中可能的乱码对我们没有影响
                result=m.find();
                if (m.groupCount() == 1) {
                    charSet = m.group(1);
                } else {
                    charSet = "";
                }
            }
            System.out.println("Last get: " + charSet);
            // 至此，我们可以将原byte数组按照正常编码专成字符串输出（如果找到了编码的话）
            System.out.println("Encoding string is: " + new String(bytes, charSet));
        }
        httpclient.getConnectionManager().shutdown();        
    }
}
