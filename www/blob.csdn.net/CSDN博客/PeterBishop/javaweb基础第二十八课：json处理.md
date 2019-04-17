# javaweb基础第二十八课：json处理 - PeterBishop - CSDN博客





2019年01月19日 18:04:30[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：24








  这节课讲怎么处理json数据以及怎么给前台传数据



  要处理json数据，需要一个第三方json处理的jar包，目前主流的有gson,fastjson,jackson，我们用fastjson，jar包已经准备好了。



  然后我的idea好像出了bug，需要在ProjectStructure里也引一下才能用...不知道你们有没有这个bug



JsonTest.java
package com.test;



import com.alibaba.fastjson.JSON;

import com.alibaba.fastjson.JSONObject;

import com.bean.User;

import org.junit.Test;



public class JsonTest {



    @Test

    public void testJson()

    {

        //可以看到fastjson是阿里出品的

        //处理json无非两种，字符串转对象，对象转字符串



        //用单元测试测吧

        User user = new User("xiaoye","123456");



        //对象转字符串

        String jsonStr = JSON.*toJSONString*(user);

        System.*out*.println(jsonStr);



        //字符串转对象

        //JSON.parse()得到的对象的类型是JSONObject

        Object jsonObj = JSON.*parse*(jsonStr);

        System.*out*.println("jsonObj: ");

        System.*out*.println(jsonObj.toString());

        //这就是json对象和字符串之间的互转

    }

}


AjaxServlet.java:
package com.servlet;



import com.alibaba.fastjson.JSON;

import com.alibaba.fastjson.JSONObject;

import com.bean.User;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;

import java.io.PrintWriter;



public class AjaxServlet extends HttpServlet {



    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        //这里写怎么给前台返回json数据

        User user = new User("xiaoye","123456");

        String jsonString = JSON.*toJSONString*(user);

        PrintWriter writer = resp.getWriter();

        writer.write(jsonString);

        //通过response.getWriter()后writer.write()即可给前台返回数据

    }

}


web.xml:
<servlet>

    <servlet-name>AjaxServlet</servlet-name>

    <servlet-class>com.servlet.AjaxServlet</servlet-class>

</servlet>

<servlet-mapping>

    <servlet-name>AjaxServlet</servlet-name>

    <url-pattern>/ajax</url-pattern>

</servlet-mapping>


ajax.jsp:
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>Title</title>

</head>

<body>

    <script src="/jquery.js"></script>

    <script>

        $.ajax({

            url:"/ajax",

            type:"get",

            success:**function **(data) {

                console.log(data);

            }

        });

    </script>

</body>

</html>




