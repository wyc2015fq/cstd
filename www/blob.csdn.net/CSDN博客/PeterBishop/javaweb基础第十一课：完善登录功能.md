# javaweb基础第十一课：完善登录功能 - PeterBishop - CSDN博客





2018年12月25日 15:25:00[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：22
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来完善登录功能:



Login.jsp:
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>登录</title>

</head>

<body style="text-align: center">

    <form style="margin-top: 100px;border: 1px solid black;text-align: center" action="/login" method="post">

        <input type="text" name="username" placeholder="用户名"><br><br>

        <input type="password" name="password" placeholder="密码"><br><br>

        <!-- 这里开始涉及jsp的知识了，下节课开始讲，先看看效果 -->

        **<%            **if (request.getAttribute("errorMsg") != null)

            {

        **%>**<scan style="color: red">**<%=**request.getAttribute("errorMsg")**%>**</scan><br>

        **<%            **}

        **%>**<button type="submit">登录</button>

    </form>

</body>

</html>




