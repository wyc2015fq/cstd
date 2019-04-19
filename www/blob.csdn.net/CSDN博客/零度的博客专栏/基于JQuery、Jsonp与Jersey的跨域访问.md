# 基于JQuery、Jsonp与Jersey的跨域访问 - 零度的博客专栏 - CSDN博客
2016年05月03日 11:01:41[零度anngle](https://me.csdn.net/zmx729618)阅读数：715
个人分类：[Java																[JSON/JSONP](https://blog.csdn.net/zmx729618/article/category/6197661)](https://blog.csdn.net/zmx729618/article/category/3108443)
基于JQuery、Jsonp与Jersey的跨域访问
# 1. 背景
在北京地铁数据中心的项目中，有一个中心的数据提供模块，为其他模块提供数据查询服务。
由于项目中模块比较多，所以需要使用多机进行分组部署，由此就带来了跨域访问的问题。
在本项目中，采用了前后端分离的技术，前端基于JQuery框架，通过自定义的组件，实现界面的开发；后端采用SpringFramework与Jersey结合的方式，由SpringFramework的MVC提供页面导航功能，有Jersey提供REST服务。
## 1.1 Jsonp
Jsonp(JSON with Padding)是资料格式 json 的一种“使用模式”，可以让网页从别的网域获取资料。
由于同源策略，一般来说位于 server1.example.com 的网页与不是 server1.example.com 的服务器沟通，而 HTML 的 <script> 元素是一个例外。利用 <script> 元素的这个开放策略，网页可以得到从其他来源动态产生的 JSON 资料，而这种使用模式就是所谓的 JSONP。用 JSONP 抓到的资料并不是 JSON，而是任意的JavaScript，用 JavaScript 直译器执行而不是用 JSON 解析器解析。
## 1.2 JQuery的Jsonp实现
大多数的js框架都提供了Jsonp的实现，由于本次项目选择使用了JQuery，因此只介绍一下JQuery对Jsonp的实现。
从 1.2 版本开始，JQuery就提供了对 JSONP 回调的本地支持。通过指定JSONP回调，就可以加载位于另一个域的 JSON 数据。回调的语法为：url?callback=?。
jQuery会自动将 ? 替换为要调用的生成函数名。如下面的例子：
jQuery.getJSON("http://www.example2.com:8080/rest/json?callback=?", 
function(data) {
    alert("Symbol: " + data.symbol + ", Price: " + data.price);
});
JQuery会自动的将？替换为一个js函数名，并在服务器返回数据时，执行此函数，以获取服务器返回的数据。
另外一种方式，就是使用JQuery的$.ajax方法：
    $.ajax({  
        type : "get",  
        async:false,  
        url : "http://example2.com:8080/app2/rest/json",  
        dataType : "jsonp",//数据类型为jsonp  
        jsonp: " callback",//服务端用于接收callback调用的function名的参数  
        success : function(data){  
            $("#showcontent").text("Result:"+data.result)  
        },  
        error:function(){  
            alert('fail');
        }  
    });
JQuery会根据jsonp的数据，动态的在url上附加相应的参数，构建出如上一个例子中的url。
## 1.3 Jersey
Jersey是一个REST框架，Jersey项目是Oracle项目支持的一个开源JAX-RS实现，用于构建RESTfull服务。
Glassfish、Weblogic Server都使用Jersey作为JAX-RS实现。
关于REST相关的内容，可以在互联网上找到很多，在此不做说明。
Jersey提供了相应的模块，可以方便的与SpringFramework进行集成。
# 2. 实现
## 2.1 环境搭建
关于客户端的内容，由于过于简单，因此不做过多说明，本次测试使用的例子如下：
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>跨域访问示例</title>
<script type="text/javascript" src="jquery-1.8.1.js"></script>
<script type="text/javascript">
$(function(){     
    $.ajax({  
        type : "get",  
        async:false,  
        url : "http://www.example2.com:8080/app2/rest/json?sid=1494&busiId=101",  
        dataType : "jsonp",//数据类型为jsonp  
        jsonp: "jsonpCallback",//服务端用于接收callback调用的function名的参数  
        success : function(data){  
            $("#showcontent").text("Result:"+data.result)  
        },  
        error:function(){  
            alert('fail');
        }  
    });   
});  
</script>
</head>
<body>
<div id="showcontent">Result:</div>
</body>
</html>
服务器端使用标准的Jersey标记实现服务：
package com.neusoft.education.web.rest.resources;
import java.util.HashMap;
import java.util.Map;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import org.springframework.stereotype.Component;
@Component
@Path("/json")
public class Json {
@GET
@Produces("application/json")
public Map<String,String> test(@QueryParam("sid")String sid , @QueryParam("busiId")String busiId) {
Map<String,String> map = new HashMap<String,String>();   
map.put("result", "sid=" +sid + "&busiId=" + busiId);
return map;
}
}
由于本次测试只能在一台笔记本上进行，为了模拟出跨域的效果，需要做一些配置。具体的做法是，修改系统的hosts文件，在Windows系统上，hosts文件位于%windir%/System32/Drivers/etc/目录下。本次测试中，定义了[www.example1.com](http://www.example1.com/)和[www.example2.com](http://www.example2.com/)两个域名，配置如下：
127.0.0.1 www.example1.com
127.0.0.1 [www.example2.com](http://www.example2.com/)
## 2.2 功能实现
按照Jsonp的要求，服务器端返回的结果，需要通过由jsonpCallback参数指定的字符串为名称的js函数进行调用。
因此如果要实现Jersey对Jsonp的支持，就需要对Jersey执行结果前后进行封装，即:
1. 在Jersey结果返回前，返回jsonpCallback参数指定的字符串及一个”(”字符
2. 在Jersey结果返回后，额外添加一个”)”字符
根据上面的分析，对于熟悉J2EE架构的人来说，一个直接的念头，就是通过添加一个filter，拦截Jersey服务的调用，并在调用前后，分别输出相应的内容。
沿着这条思路，功能的实现将会非常的简单：
package com.neusoft.education.web.filter;
import java.io.IOException;
import java.io.OutputStream;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebFilter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
@WebFilter("/rest/*")
public class JsonpFilter implements Filter {
public void destroy() {
}
public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain) throws IOException, ServletException {
String jsonCallback = ((HttpServletRequest)req).getParameter("jsonpCallback");
if (jsonCallback != null) {
HttpServletResponse response = (HttpServletResponse)res;
OutputStream os = response.getOutputStream();
os.write((jsonCallback + "(").getBytes());
chain.doFilter(req, res);
os.write(")".getBytes());
} else {
chain.doFilter(req, res);
}
}
public void init(FilterConfig fConfig) throws ServletException {
}
}
# 3. 限制
由于Jsonp本质上是通过动态<script> tag实现的，因此Jsonp只能支持GET方式的请求。
