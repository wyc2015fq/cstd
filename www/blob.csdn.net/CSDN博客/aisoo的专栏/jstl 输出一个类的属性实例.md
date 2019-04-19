# jstl 输出一个类的属性实例 - aisoo的专栏 - CSDN博客
2009年11月09日 19:00:00[aisoo](https://me.csdn.net/aisoo)阅读数：3633

我在数据库中检索出一个list集合，list中存的是user类的对象集合，下面我用jstl把user中的各项属性逐个输出出来
1 创建web工程Jstl,首先写个User对象：
     package com.bld.jstl;
public class User {
    public String userName;
    public String password;
    public String getUserName() {
        return userName;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
    public String getPassword() {
        return password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
}
2  创建相应的数据库 我用的是mysql数据库， 我就用我以前的 懒的创建了
3 用到的两个jar包，jstl.jar， standard.jar ,将它们放到web工程WEB-INF/lib下
  补充一下 别忘了mysql的驱动包放在TOMCAT_HOME/common/lib下
4 现在写jsp页面 index.jsp
 <%@ page contentType="text/html; charset=UTF-8" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page import="com.bld.jstl.User"%>
<%@ page import="java.util.*"%>
<%@ page import="java.sql.*"%>
<%     List<User> resultList = new ArrayList<User>();
        String driver = "com.mysql.jdbc.Driver";
        String url = "jdbc:mysql://localhost/hibernate_first";
        String user = "root";
        String password = "#############";
        Connection con = null;
        Statement stmt = null;
        ResultSet rst = null;
        try {
            Class.forName(driver);
            con = DriverManager.getConnection(url, user, password);
            stmt = con.createStatement();
            String sql = " SELECT * FROM  user";
            rst = stmt.executeQuery(sql);
            while (rst.next()) {
                User userBean = new User();
                userBean.setUserName(rst.getString(2));
                userBean.setPassword(rst.getString(3));
                resultList.add(userBean);
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
       pageContext.setAttribute("list", resultList); 
%>
<html>
<head>
        <title>Condition Flow</title>
</head>
<body>
        <c:forEach items= "${list}" var= "user"> 
            <c:out value= "${user.userName}"/>
            <c:out value="${user.password}"/><br>
        </c:forEach> 
</body>
</html>
5 好了 启动tomcat 输入http://localhost:8080/Jstl/index.jsp 你就会看到你从数据库中检索出的User对象的属性值了
