# javaweb基础第十八课：EL表达式 - PeterBishop - CSDN博客





2019年01月04日 19:15:18[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：26








大纲:
- 简介
- 基本使用
- 隐含对象
	- - pageContext
- 请求域
- 请求参数
- 其他


- header；
- headerValues；
- initParam；
- cookie；
- EL逻辑运算



  这节课我们来讲el表达式，讲什么看大纲


- 简介

JSP表达式语言（EL）使得访问存储在JavaBean中的数据变得非常简单。JSP EL既可以用来创建算术表达式也可以用来创建逻辑表达式。在JSP EL表达式内可以使用整型数，浮点数，字符串，常量true、false，还有null。



简单点说就是取数据用的，相当于<%=%>


- 基本使用

     所有域对象都有setAttribute和getAttribute这两个方法，el表达式就是取的attribute
- - 语法: ${xxx}
- 取属性值: ${name}
- 取对象里的属性值: ${obj.name}或${obj[“name”]}


注意这个name不是属性名，而是getXxx的Xxx然后首字母小写
- - 取map里的值: ${obj.key}，key就是map里存的键，可以取到键对应的值
- 取特定域对象的属性值:
		- ${pageScope.xxx}
- ${requestScope.xxx}
- ${sessionScope.xxx}
- ${applicationScope.xxx}



一般情况下只有当不同的域里有同属性名的属性值时才需要用这个
- - 逻辑表达式


后面讲


- 隐含对象
	- - pageContext: 相当于jsp里的pageContext，用的不多
- 请求域
			- pageScope
- requestScope
- sessionScope
- applicationScope

- 请求参数



获取请求里带有的参数
- - - - 如果参数不是数组是单个值,${param.name}，name就是参数名
- 如果参数是数组,${paramValues.name[index]}, name是数组名，index是下标

- 其他


- header；
- headerValues；
- initParam；
- cookie；

用不到


- el逻辑运算
- ![](https://img-blog.csdnimg.cn/20190104191501671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



用法举例:

${1+2}

${empty obj}







ElTeachServlet:
package com.servlet;



import com.bea.User;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;

import java.util.HashMap;

import java.util.Map;



//el表达式教学Servlet

public class ElTeachServlet extends HttpServlet {

    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {



        User user = new User();

        user.setId(1001);

        user.setName("xiaoye");

        user.setGender("男");



        Map<String,Object> map = new HashMap<>();

        map.put("xiaoye",user);

        req.setAttribute("map",map);

        //演示下对象里存对象怎么取

        req.getRequestDispatcher("/WEB-INF/el.jsp").forward(req,resp);

    }

}


web.xml:
<servlet>

    <servlet-name>ElTeachServlet</servlet-name>

    <servlet-class>com.servlet.ElTeachServlet</servlet-class>

</servlet>

<servlet-mapping>

    <servlet-name>ElTeachServlet</servlet-name>

    <url-pattern>/el</url-pattern>

</servlet-mapping>


/index.jsp:
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><%--jsp:forward本质就是发个请求--%>

<**jsp****:forward **page="/el"></**jsp****:forward**><%--

  jsp:forword发了/el请求

  然后服务器拦截后分发给ElTeachServlet执行service方法

  然后跳转到/WEBB-INF/el.jsp

--%>


/WEB-INF/el.jsp
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>Title</title>

</head>

<body>

    小叶信息如下: <br>

        id: **${**map.xiaoye.id**}**<br>

        name: **${**map.xiaoye.name**}**<br>

        gender: **${**map.xiaoye.gender**}**<br>

</body>

</html>




