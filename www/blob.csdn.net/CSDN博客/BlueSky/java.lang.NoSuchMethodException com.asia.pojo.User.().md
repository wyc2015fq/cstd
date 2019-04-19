# java.lang.NoSuchMethodException: com.asia.pojo.User.() - BlueSky - CSDN博客
2015年12月29日 10:44:34[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：3218
Spring mvc运行出现错误，如下：
```
SEVERE: Servlet.service() for servlet osAppServlet threw exception
java.lang.NoSuchMethodException: com.asia.pojo.User.<init>()
```
``
The message `java.lang.NoSuchMethodException: `
`com.asia.pojo.User.<init>()` means that someone tried to call a constructor without any parameters. Adding a default constructor should solve this problem:
```
public class User {
    public User() {
    }
    ..
}
```
**${pageContext.request.contextPath}**等价于**<%=request.getContextPath()%>** 或者可以说是**<%=request.getContextPath()%**>的EL版 意思就是取出部署的应用程序名或者是当前的项目名称
比如我的项目名称是ajax01 在浏览器中输入为**http://localhost:8080/ajax01/login.jsp ${pageContext.request.contextPath**}或**<%=request.getContextPath()%>**取出来的就是**/ajax01**,而"/"代表的含义就是**http://localhost:8080**
所以我们项目中应该这样写**${pageContext.request.contextPath}/login.jsp**
