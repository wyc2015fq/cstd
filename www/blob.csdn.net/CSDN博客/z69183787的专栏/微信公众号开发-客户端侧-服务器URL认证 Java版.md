# 微信公众号开发-客户端侧-服务器URL认证 Java版 - z69183787的专栏 - CSDN博客
2016年12月15日 16:31:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：666
个人分类：[微信/支付宝开发](https://blog.csdn.net/z69183787/article/category/3277967)
[https://mp.weixin.qq.com/wiki](https://mp.weixin.qq.com/wiki)
```java
<%@ page import="java.util.List" %>
<%@ page import="java.util.ArrayList" %>
<%@ page import="java.util.Collections" %>
<%@ page import="java.util.Comparator" %>
<%@ page import="java.security.MessageDigest" %>
<%@ page import="com.xxxxx.xxxxx.brand.wx.util.SHA1" %>
<%@ page import="java.io.IOException" %>
<%@ page import="com.xxxxx.xxxxx.brand.wx.util.HttpClientUtil" %>
<%@ page import="com.xxxxx.xxxxx.brand.wx.util.WxUtil" %>
<%@ page import="com.xxxxx.xxxxx.brand.wx.bean.AccessTokenResult" %>
<%@ page import="com.xxxxx.xxxxx.brand.wx.bean.UserInfoResult" %>
<%@page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<%!
    private String access(HttpServletRequest request, HttpServletResponse response) {
        // 验证URL真实性
        System.out.println("进入验证access");
        String signature = request.getParameter("signature");// 微信加密签名
        String timestamp = request.getParameter("timestamp");// 时间戳
        String nonce = request.getParameter("nonce");// 随机数
        String echostr = request.getParameter("echostr");// 随机字符串
        List<String> params = new ArrayList<String>();
        params.add("zhoushun");
        params.add(timestamp);
        params.add(nonce);
        // 1. 将token、timestamp、nonce三个参数进行字典序排序
        Collections.sort(params, new Comparator<String>() {
            @Override
            public int compare(String o1, String o2) {
                return o1.compareTo(o2);
            }
        });
        // 2. 将三个参数字符串拼接成一个字符串进行sha1加密
        String temp = SHA1.encode(params.get(0) + params.get(1) + params.get(2));
        if (temp.equals(signature)) {
                //response.getWriter().write(echostr);
                System.out.println("成功返回 echostr：" + echostr);
                return echostr;
        }
        System.out.println("失败 认证");
        return null;
    }
%>
<%
    System.out.println("进入chat");
    boolean isGet = request.getMethod().toLowerCase().equals("get");
    if (isGet) {
        String signature = request.getParameter("signature");
        String timestamp = request.getParameter("timestamp");
        String nonce = request.getParameter("nonce");
        String echostr = request.getParameter("echostr");
        System.out.println(signature);
        System.out.println(timestamp);
        System.out.println(nonce);
        System.out.println(echostr);
        out.println(access(request, response));
    }else {
        // 进入POST聊天处理
        System.out.println("enter post");
    }
%>
<html>
<body>
<h2>Hello World!</h2>
</body>
</html>
```
OAUTH2.0 接入详见：
[http://blog.csdn.net/z69183787/article/details/50674902](http://blog.csdn.net/z69183787/article/details/50674902)
