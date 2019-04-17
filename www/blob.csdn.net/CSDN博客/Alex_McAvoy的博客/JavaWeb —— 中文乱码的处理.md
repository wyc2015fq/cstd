# JavaWeb —— 中文乱码的处理 - Alex_McAvoy的博客 - CSDN博客





2018年11月01日 23:43:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：74








# 【中文乱码的情况】
- 在 JSP 页面上输入中文，在编译后，页面出现乱码
- 发送请求时，请求的参数值为中文

# 【解决方法】

## 1.方法一

对于第一种情况，只需要保证保证 page 指令中的 contentType 属性的 charset 与 pageEncoding 属性一致即可，通常建议取值 UTF-8，此外，还需保证浏览器的显示字符编码与请求 JSP 页面的编码一致
`<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>`
## 2.方法二

对于第二种情况，在请求传输过程中，默认使用的编码为 ISO-8859-1，该编码不支持中文

1）对于 post 请求，在获取请求信息之前，可调用 request.setCharactorEncoding("UTF-8")，然后再获取请求信息

```
<% request.setCharacterEncoding("UTF-8"); %>
<%= request.getParameter("username"); %>
```

2）对于 get 请求，可将获取到的数据用 ISO-8859-1 解码后再用 UTF-8 重新构造一个 String 对象
`姓名参数值：<%= new String(request.getParameter("name").getBytes("ISO-8859-1"),"UTF-8") %>`
## 3.方法三

可修改 Tomcat 的 index.html 文件的 <Connector> 结点，添加 useBodyEncodingForURI 属性并设为 true，使得请求参数的编码方式采用请求体的编码方式。





