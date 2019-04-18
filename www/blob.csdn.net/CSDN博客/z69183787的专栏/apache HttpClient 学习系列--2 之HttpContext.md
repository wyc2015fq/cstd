# apache HttpClient 学习系列--2 之HttpContext - z69183787的专栏 - CSDN博客
2017年04月26日 17:24:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2478
                
摘要: 本文主要讲解HttpContext的使用
首先是一个Servlet用于本次试验。
```java
package com.lu.controller;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
@SuppressWarnings("serial")
public class RirectServlet extends HttpServlet {
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        this.doPost(request, response);
    }
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setContentType("text/html;charset=utf-8");
        OutputStream out = response.getOutputStream();
        PrintWriter pw = new PrintWriter(out);
        // 记录返回的内容
        String returnContent = null;
        HttpSession session = request.getSession();
        String user = (String) session.getAttribute("username");
        // 如果不为空,说明已登录
        if (user != null) {
            returnContent = "you have been logined, " + user;
        } else {
            // 为空说明未登录,设置username到session中
            String username = request.getParameter("username");
            session.setAttribute("username", username);
            returnContent = "login success";
        }
        try {
            pw.println(returnContent);
        } finally {
            pw.close();
            out.close();
        }
    }
}
```
在本机中访问URI为：http://localhost:8080/spiderweb/RirectServlet
下面看请求
```java
package com.lu.test;
import java.util.ArrayList;
import java.util.List;
import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.protocol.HttpClientContext;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;
import org.apache.http.util.EntityUtils;
public class HttpClientTest {
    private static HttpContext localContext = new BasicHttpContext();
    private static HttpClientContext context = HttpClientContext
            .adapt(localContext);
    public static void main(String[] args) throws Exception {
        CloseableHttpClient httpClient = HttpClients.createDefault();
        try {
            // 模拟表单
            List<NameValuePair> params = new ArrayList<NameValuePair>();
            params.add(new BasicNameValuePair("username", "**"));
            UrlEncodedFormEntity entity = new UrlEncodedFormEntity(params,
                    "UTF-8");
            HttpPost httpPost = new HttpPost(
                    "http://localhost:8080/spiderweb/RirectServlet");
            httpPost.setEntity(entity);
            // 将HttpClientContext传入execute()中
            CloseableHttpResponse response = httpClient.execute(httpPost,
                    context);
            try {
                HttpEntity responseEntity = response.getEntity();
                System.out.println(EntityUtils.toString(responseEntity));
            } finally {
                response.close();
            }
        } finally {
            httpClient.close();
        }
        CloseableHttpClient httpClient2 = HttpClients.createDefault();
        try {
            HttpGet httpGet = new HttpGet(
                    "http://localhost:8080/spiderweb/RirectServlet");
            // 设置相同的HttpClientContext
            CloseableHttpResponse response = httpClient2.execute(httpGet,
                    context);
            try {
                HttpEntity entity = response.getEntity();
                System.out.println(EntityUtils.toString(entity));
            } finally {
                response.close();
            }
        } finally {
            httpClient2.close();
        }
    }
}
```
输出结果为：
login success
you have been logined, **
可能大家认为这个结果很正常。第一次登陆了，第二次肯定会输出you have been logined, **。但是大家别忘了，现在不是在浏览器环境下，浏览器会帮你提取服务器传回的sessionId并在你下次请求的时候传给服务器，那么两次请求用的就是同一个session对象，那么自然的就会有上面的输出。
但是现在是在自己写代码访问。我们没有获取sessionId，也没有向服务器传送sessionId，那么两次请求服务器就会为你创建两个sessionId不同的session对象。
那为什么输出结果却是跟浏览器得到的结果一样呢？肯定是什么东西为我们自动完成了获取sessionId传送sessionId的功能。毫无疑问，就是context。 BasicHttpContext里有个Map<Object,Object>的对象用来记录一次请求响应的信息，当响应信息返回时，就会被set到context里，当然响应的cookie信息也就被存储在context里,包括传回的sessionId。
当第二次请求的时候传入相同的context，那么请求的过程中会将context里的sessionId提取出来传给服务器，sessionId一样，自然而然的就是同一个session对象。
那么大家明白context的作用了吗？
按照官方教程的说法来说
代表一个逻辑相关的会话的多个请求序列应该具有相同的HttpContext实例以确保请求之间的信息状态的自动传播
由于初学，不对的地方还望大家指出。
