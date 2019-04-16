# Servlet3.0学习总结(三)——基于Servlet3.0的文件上传 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Servlet3.0学习总结(三)——基于Servlet3.0的文件上传](https://www.cnblogs.com/xdp-gacl/p/4224960.html)



　　在Servlet2.5中，我们要实现文件上传功能时，一般都需要借助第三方开源组件，例如Apache的commons-fileupload组件，在Servlet3.0中提供了对文件上传的原生支持，我们不需要借助任何第三方上传组件，直接使用Servlet3.0提供的API就能够实现文件上传功能了。

## 一、使用Servlet3.0提供的API实现文件上传

### 1.1、编写上传页面

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>Servlet3.0实现文件上传</title>
 6   </head>
 7   
 8   <body>
 9         <fieldset>
10             <legend>
11                 上传单个文件
12             </legend>
13             <!-- 文件上传时必须要设置表单的enctype="multipart/form-data"-->
14             <form action="${pageContext.request.contextPath}/UploadServlet"
15                 method="post" enctype="multipart/form-data">
16                 上传文件：
17                 <input type="file" name="file">
18                 <br>
19                 <input type="submit" value="上传">
20             </form>
21         </fieldset>
22         <hr />
23         <fieldset>
24             <legend>
25                 上传多个文件
26             </legend>
27             <!-- 文件上传时必须要设置表单的enctype="multipart/form-data"-->
28             <form action="${pageContext.request.contextPath}/UploadServlet"
29                 method="post" enctype="multipart/form-data">
30                 上传文件：
31                 <input type="file" name="file1">
32                 <br>
33                 上传文件：
34                 <input type="file" name="file2">
35                 <br>
36                 <input type="submit" value="上传">
37             </form>
38         </fieldset>
39     </body>
40 </html>
```

### 1.2、开发处理文件上传的Servlet

**1、使用注解@MultipartConfig将一个Servlet标识为支持文件上传。**

**　　2、Servlet3.0将multipart/form-data的POST请求封装成Part，通过Part对上传的文件进行操作**。

　　UploadServlet 代码如下：


```
1 package me.gacl.web.controller;
 2 
 3 import java.io.File;
 4 import java.io.IOException;
 5 import java.io.PrintWriter;
 6 import java.util.Collection;
 7 
 8 import javax.servlet.ServletException;
 9 import javax.servlet.annotation.MultipartConfig;
10 import javax.servlet.annotation.WebServlet;
11 import javax.servlet.http.HttpServlet;
12 import javax.servlet.http.HttpServletRequest;
13 import javax.servlet.http.HttpServletResponse;
14 import javax.servlet.http.Part;
15 
16 //使用@WebServlet配置UploadServlet的访问路径
17 @WebServlet(name="UploadServlet",urlPatterns="/UploadServlet")
18 //使用注解@MultipartConfig将一个Servlet标识为支持文件上传
19 @MultipartConfig//标识Servlet支持文件上传
20 public class UploadServlet extends HttpServlet {
21 
22     public void doGet(HttpServletRequest request, HttpServletResponse response)
23             throws ServletException, IOException {
24              request.setCharacterEncoding("utf-8");
25             response.setCharacterEncoding("utf-8");
26             response.setContentType("text/html;charset=utf-8");
27             //存储路径
28             String savePath = request.getServletContext().getRealPath("/WEB-INF/uploadFile");
29             //获取上传的文件集合
30             Collection<Part> parts = request.getParts();
31             //上传单个文件
32             if (parts.size()==1) {
33                  //Servlet3.0将multipart/form-data的POST请求封装成Part，通过Part对上传的文件进行操作。
34                 //Part part = parts[0];//从上传的文件集合中获取Part对象
35                 Part part = request.getPart("file");//通过表单file控件(<input type="file" name="file">)的名字直接获取Part对象
36                 //Servlet3没有提供直接获取文件名的方法,需要从请求头中解析出来
37                 //获取请求头，请求头的格式：form-data; name="file"; filename="snmp4j--api.zip"
38                 String header = part.getHeader("content-disposition");
39                 //获取文件名
40                 String fileName = getFileName(header);
41                 //把文件写到指定路径
42                 part.write(savePath+File.separator+fileName);
43             }else {
44                 //一次性上传多个文件
45                 for (Part part : parts) {//循环处理上传的文件
46                     //获取请求头，请求头的格式：form-data; name="file"; filename="snmp4j--api.zip"
47                     String header = part.getHeader("content-disposition");
48                     //获取文件名
49                     String fileName = getFileName(header);
50                     //把文件写到指定路径
51                     part.write(savePath+File.separator+fileName);
52                 }
53             }
54             PrintWriter out = response.getWriter();
55             out.println("上传成功");
56             out.flush();
57             out.close();
58     }
59 
60      /**
61      * 根据请求头解析出文件名
62      * 请求头的格式：火狐和google浏览器下：form-data; name="file"; filename="snmp4j--api.zip"
63      *                 IE浏览器下：form-data; name="file"; filename="E:\snmp4j--api.zip"
64      * @param header 请求头
65      * @return 文件名
66      */
67     public String getFileName(String header) {
68         /**
69          * String[] tempArr1 = header.split(";");代码执行完之后，在不同的浏览器下，tempArr1数组里面的内容稍有区别
70          * 火狐或者google浏览器下：tempArr1={form-data,name="file",filename="snmp4j--api.zip"}
71          * IE浏览器下：tempArr1={form-data,name="file",filename="E:\snmp4j--api.zip"}
72          */
73         String[] tempArr1 = header.split(";");
74         /**
75          *火狐或者google浏览器下：tempArr2={filename,"snmp4j--api.zip"}
76          *IE浏览器下：tempArr2={filename,"E:\snmp4j--api.zip"}
77          */
78         String[] tempArr2 = tempArr1[2].split("=");
79         //获取文件名，兼容各种浏览器的写法
80         String fileName = tempArr2[1].substring(tempArr2[1].lastIndexOf("\\")+1).replaceAll("\"", "");
81         return fileName;
82     }
83     
84     public void doPost(HttpServletRequest request, HttpServletResponse response)
85             throws ServletException, IOException {
86         this.doGet(request, response);
87     }
88 }
```

运行结果如下：

![](https://images0.cnblogs.com/blog/289233/201501/151044067142764.gif)

　　可以看到，使用Servlet3.0提供的API实现文件上传功能是非常方便的。









