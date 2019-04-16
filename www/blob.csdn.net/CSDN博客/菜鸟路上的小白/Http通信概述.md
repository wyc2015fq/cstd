# Http通信概述 - 菜鸟路上的小白 - CSDN博客





2016年09月14日 13:58:40[somnus_小凯](https://me.csdn.net/u012486840)阅读数：424









在进行编码之前，我们先创建一个`Servlet`，该`Servlet`接收客户端的参数(name和age)，并响应客户端。

```java
@WebServlet(urlPatterns={"/demo.do"})
public class DemoServlet extends HttpServlet {

    private static final long serialVersionUID = 1L;

    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        request.setCharacterEncoding("utf-8");
        response.setContentType("text/html;charset=utf-8");
        String name = request.getParameter("name");
        String age = request.getParameter("age");
        PrintWriter pw = response.getWriter();
        pw.print("您使用GET方式请求该Servlet。<br />" + "name = " + name + ",age = " + age);
        pw.flush();
        pw.close();
    }

    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        request.setCharacterEncoding("utf-8");
        response.setContentType("text/html;charset=utf-8");
        String name = request.getParameter("name");
        String age = request.getParameter("age");
        PrintWriter pw = response.getWriter();
        pw.print("您使用POST方式请求该Servlet。<br />" + "name = " + name + ",age = " + age);
        pw.flush();
        pw.close();
    }

}
```

## 使用JDK实现http通信

### 使用`URLConnection`实现GET请求
- 实例化一个`java.net.URL`对象；
- 通过`URL`对象的`openConnection()`方法得到一个`java.net.URLConnection`;
- 通过`URLConnection`对象的`getInputStream()`方法获得输入流；
- 读取输入流；
- 关闭资源。

```java
public void get() throws Exception{

    URL url = new URL("http://127.0.0.1/http/demo.do?name=Jack&age=10");
    URLConnection urlConnection = url.openConnection();                                                    // 打开连接
    BufferedReader br = new BufferedReader(new InputStreamReader(urlConnection.getInputStream(),"utf-8")); // 获取输入流
    String line = null;
    StringBuilder sb = new StringBuilder();
    while ((line = br.readLine()) != null) {
        sb.append(line + "\n");
    }

    System.out.println(sb.toString());
}
```

![](https://img-blog.csdn.net/20160921153112242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 使用`HttpURLConnection`实现POST请求

`java.net.HttpURLConnection`是`java.net.URL`的子类，提供了更多的关于http的操作（getXXX
 和 setXXX方法）。该类中定义了一系列的HTTP状态码：

![](https://img-blog.csdn.net/20160921153128461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```java
public void post() throws IOException{

    URL url = new URL("http://127.0.0.1/http/demo.do");
    HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();

    httpURLConnection.setDoInput(true);
    httpURLConnection.setDoOutput(true);        // 设置该连接是可以输出的
    httpURLConnection.setRequestMethod("POST"); // 设置请求方式
    httpURLConnection.setRequestProperty("charset", "utf-8");

    PrintWriter pw = new PrintWriter(new BufferedOutputStream(httpURLConnection.getOutputStream()));
    pw.write("name=welcome");                   // 向连接中输出数据（相当于发送数据给服务器）
    pw.write("&age=14");
    pw.flush();
    pw.close();

    BufferedReader br = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream(),"utf-8"));
    String line = null;
    StringBuilder sb = new StringBuilder();
    while ((line = br.readLine()) != null) {    // 读取数据
        sb.append(line + "\n");
    }

    System.out.println(sb.toString());
}
```

![](https://img-blog.csdn.net/20160921153149633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 使用`httpclient`进行http通信


httpclient大大简化了JDK中http通信的实现。


maven依赖：

```xml
<dependency>
    <groupId>org.apache.httpcomponents</groupId>
    <artifactId>httpclient</artifactId>
    <version>4.3.6</version>
</dependency>
```

### GET请求

```java
public void httpclientGet() throws Exception{

    // 创建HttpClient对象
    HttpClient client = HttpClients.createDefault();

    // 创建GET请求（在构造器中传入URL字符串即可）
    HttpGet get = new HttpGet("http://127.0.0.1/http/demo.do?name=admin&age=40");

    // 调用HttpClient对象的execute方法获得响应
    HttpResponse response = client.execute(get);

    // 调用HttpResponse对象的getEntity方法得到响应实体
    HttpEntity httpEntity = response.getEntity();

    // 使用EntityUtils工具类得到响应的字符串表示
    String result = EntityUtils.toString(httpEntity,"utf-8");
    System.out.println(result);
}
```

![](https://segmentfault.com/img/bVm8od)

### POST请求

```java
public void httpclientPost() throws Exception{

    // 创建HttpClient对象
    HttpClient client = HttpClients.createDefault();

    // 创建POST请求
    HttpPost post = new HttpPost("http://127.0.0.1/http/demo.do");

    // 创建一个List容器，用于存放基本键值对（基本键值对即：参数名-参数值）
    List<BasicNameValuePair> parameters = new ArrayList<>();
    parameters.add(new BasicNameValuePair("name", "张三"));
    parameters.add(new BasicNameValuePair("age", "25"));

    // 向POST请求中添加消息实体
    post.setEntity(new UrlEncodedFormEntity(parameters, "utf-8"));

    // 得到响应并转化成字符串
    HttpResponse response = client.execute(post);
    HttpEntity httpEntity = response.getEntity();
    String result = EntityUtils.toString(httpEntity,"utf-8");
    System.out.println(result);
}
```

![](https://img-blog.csdn.net/20160921153212653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



