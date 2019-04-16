# javaweb基础第二十课：cookie - PeterBishop - CSDN博客





2019年01月06日 18:36:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：68








大纲:
- 简介
- 使用Cookie
	- 设置Cookie
- 读取Cookie
- Cookie有效时间
- Cookie路径




  这节课我们来讲Cookie，大纲写好了


- 简介

Cookie 是存储在客户端计算机上的文本文件，并保留了各种跟踪信息。

更准确的说，是浏览器软件在本地保存的信息.

比如说，chrome浏览器是一个软件，大家要使用这个软件必须先下载安装，cookie就是这个软件保存的一些信息。


- 使用Cookie
	- 设置Cookie
		- 构造cookie对象
- 添加到response中

- 获取Cookie


通过request获取
- - Cookie有效时间


就是Cookie的过期时间，当过期时间过了cookie就会被删掉

可以通过cookie.setMaxAge(int expiry)方法设置过期时间,单位是秒

参数说明:

    若参数为0，表示立即失效

                       若参数为正，表示expiry秒后失效

若参数为负，表示一次会话结束后失效(浏览器关闭或浏览器清空缓存)
- - Cookie路径


表示那些请求被发起时要携带该cookie

也就是那些请求被发起时可以通过req获取到该Cookie

可通过cookie.setPath()来设置

默认是项目根路径



CookieTeachServlet.java
package com.servlet;



import javax.servlet.ServletException;

import javax.servlet.http.Cookie;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;



//Cookie教学Servlet

public class CookieTeachServlet extends HttpServlet {

    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {



        String url = req.getRequestURL().toString();

        if (url.endsWith("/cookie/set"))

        {

            setCookie(resp);

        }

        if (url.indexOf("/cookie/get") > 0)

            getCookie(req);

    }



    private void getCookie(HttpServletRequest req) {

        //通过request获取cookie

        Cookie[] cookies = req.getCookies();

        for (Cookie cookie1 : cookies) {

            System.*out*.println(cookie1.getName() + ": " + cookie1.getValue());

        }

        /*

            id: 1001

            Webstorm-7d175dab: 3d609cdc-d57b-4c10-8eb3-c728ffb5425b

            Hm_lvt_080836300300be57b7f34f4b3e97d911: 1544144252

            id: 1001

            JSESSIONID: F2CEF3BC6E7BA69465DC00EEF2379A0B



            上面这些都是Cookie里的信息，也就是说/cookie/get这个请求里呆了5条cookie

        */

    }



    private void setCookie(HttpServletResponse resp) {

        //构造cookie对象

        */**         * Cookie**的构造器如下         *  public Cookie(String name, String value);         */        *Cookie cookie =  new Cookie("id","1001");

//        cookie.setMaxAge(10);   //10秒后过期

        cookie.setPath("/cookie/get/1");

        /*

        Webstorm-7d175dab: 3d609cdc-d57b-4c10-8eb3-c728ffb5425b

        Hm_lvt_080836300300be57b7f34f4b3e97d911: 1544144252

        id: 1001

        JSESSIONID: 3A33CF0BC08416BBE26D75D8C42ED04C



        5条变4条，显然少了一条

         */

        //添加到response中

        resp.addCookie(cookie);

        //添加完后会在响应头信息里加一行 Set-Cookie: id=1001

    }



/*

/cookie/get/1的cookie

id: 1001

Webstorm-7d175dab: 3d609cdc-d57b-4c10-8eb3-c728ffb5425b

Hm_lvt_080836300300be57b7f34f4b3e97d911: 1544144252

id: 1001

JSESSIONID: 6143489039E0A06E778F47DA85827854



/cookie/get/2的cookie

Webstorm-7d175dab: 3d609cdc-d57b-4c10-8eb3-c728ffb5425b

Hm_lvt_080836300300be57b7f34f4b3e97d911: 1544144252

id: 1001

JSESSIONID: 6143489039E0A06E778F47DA85827854



显然2比1少1条，就是因为/cookie/set里的cookie设置了路径

cookie.setPath("/cookie/get/1");

 */

}


web.xml
<servlet>

    <servlet-name>CookieTeachServlet</servlet-name>

    <servlet-class>com.servlet.CookieTeachServlet</servlet-class>

</servlet>



<servlet-mapping>

    <servlet-name>CookieTeachServlet</servlet-name>

    <url-pattern>/cookie</url-pattern>

</servlet-mapping>



<servlet-mapping>

    <servlet-name>CookieTeachServlet</servlet-name>

    <url-pattern>/cookie/set</url-pattern>

</servlet-mapping>



<servlet-mapping>

    <servlet-name>CookieTeachServlet</servlet-name>

    <url-pattern>/cookie/get</url-pattern>

</servlet-mapping>



<servlet-mapping>

    <servlet-name>CookieTeachServlet</servlet-name>

    <url-pattern>/cookie/get/1</url-pattern>

</servlet-mapping>



<servlet-mapping>

    <servlet-name>CookieTeachServlet</servlet-name>

    <url-pattern>/cookie/get/2</url-pattern>

</servlet-mapping>




