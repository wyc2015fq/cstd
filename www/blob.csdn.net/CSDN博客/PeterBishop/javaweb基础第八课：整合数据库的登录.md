# javaweb基础第八课：整合数据库的登录 - PeterBishop - CSDN博客





2018年12月23日 20:52:38[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：33
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们在上节课的基础上整合一下数据库。

  既然要整合数据库，首先得有一个数据库，我们就起名叫javaweb_teach，字符编码设置为utf8

  然后建一个user表



  然后开始整合，首先要添加包，说下往javaweb项目里添加包的步骤:
- 在/WEB-INF下建一个文件夹叫lib(必须是这个名)
- 把需要的jar包拷贝到lib中
- 如果使用的是idea，还需要在Project Structure里添加依赖(eclipse无视此步骤)



  以前我们写过一个JdbcTool，我已经把这个项目打包成jar包了，等会上传大家可以下下来直接用,注意别忘可mysql-connector



  然后建个bean包，里面放和数据库对应的实体类



  下一步，写Mapper用于处理数据库相关操作



LoginServlet.java:
package com.servlet;



import com.bean.User;

import com.dao.UserMapper;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;

import java.lang.reflect.InvocationTargetException;

import java.sql.SQLException;

import java.util.HashMap;

import java.util.List;

import java.util.Map;



//这个servlet用来处理登录请求

public class LoginServlet extends HttpServlet {



    private UserMapper userMapper = UserMapper.*getInstance*();



    @Override

    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        String username = req.getParameter("username");

        String password = req.getParameter("password");



        Map<String,Object> map = new HashMap<>();

        map.put("username",username);

        map.put("password",password);



        try {

            List<User> users = userMapper.selectByExample(map);

            if (users != null && users.size() == 1)

            {

                User user = users.get(0);

                if (user != null)

                {

                    //跳转到登录成功的页面

                    */**                     * **跳转页面使用request.getRequestDispatcher(要跳转的页面的路径).forward(request,response);                     */                    *req.getRequestDispatcher("/WEB-INF/login_success.jsp").forward(req,resp);

                }

                else

                {

                    //如果登录失败，我们给点提示信息

                    req.setAttribute("errorMsg","登录失败");   //向request域里设置属性和值

                    req.getRequestDispatcher("/WEB-INF/login.jsp").forward(req,resp);

                }

            }

            else

            {

                //如果登录失败，我们给点提示信息

                req.setAttribute("errorMsg","登录失败");   //向request域里设置属性和值

                req.getRequestDispatcher("/WEB-INF/login.jsp").forward(req,resp);

            }

        } catch (SQLException e) {

            e.printStackTrace();

        } catch (IllegalAccessException e) {

            e.printStackTrace();

        } catch (InstantiationException e) {

            e.printStackTrace();

        } catch (NoSuchMethodException e) {

            e.printStackTrace();

        } catch (NoSuchFieldException e) {

            e.printStackTrace();

        } catch (InvocationTargetException e) {

            e.printStackTrace();

        }

    }

}


DataSource.java:
package com.dao;



import com.tool.Dbc;



import java.sql.Connection;

import java.sql.SQLException;



public class DataSource {



    private static String

        *driver *= "com.mysql.jdbc.Driver",

        *url *= "jdbc:mysql://localhost:3306/javaweb_teach",

        *username *= "root",

        *password *= "root";



    public static Connection getConnection() throws SQLException, ClassNotFoundException {

        Connection connection = Dbc.*getConnection*(*driver*, *url*, *username*, *password*);

        return connection;

    }

}


UserMapper.java
package com.dao;



import com.bean.User;

import com.tool.Mapper;



import java.io.IOException;

import java.sql.Connection;

import java.sql.SQLException;



//注意搞成单例

public class UserMapper extends Mapper<User> {



    private static UserMapper *instance *= null;



    private UserMapper(Connection connection) {

        super(connection);

    }



    private UserMapper(String dbcPropertiesPath) throws SQLException, IOException, ClassNotFoundException {

        super(dbcPropertiesPath);

    }



    public static UserMapper getInstance() {

        if (*instance *== null)

        {

            try {

                *instance *= new UserMapper(DataSource.*getConnection*());

            } catch (SQLException e) {

                e.printStackTrace();

            } catch (ClassNotFoundException e) {

                e.printStackTrace();

            }

        }

        return *instance*;

    }

}


User.java:
package com.bean;



public class User {

    private String username;

    private String password;



    public User() {

    }



    public User(String username, String password) {

        this.username = username;

        this.password = password;

    }



    public String getUsername() {

        return username;

    }



    public void setUsername(String username) {

        this.username = username;

    }



    public String getPassword() {

        return password;

    }



    public void setPassword(String password) {

        this.password = password;

    }



    @Override

    public String toString() {

        return "User{" +

                "username='" + username + '\'' +

                ", password='" + password + '\'' +

                '}';

    }

}


login.jsp
<%@ **page **contentType="text/html;charset=UTF-8" language="java" %><html>

<head>

    <title>登录</title>

</head>

<body style="text-align: center">

    <form style="margin-top: 100px;border: 1px solid black;text-align: center" action="/login" method="post">

        <input type="text" name="username" placeholder="用户名"><br><br>

        <input type="password" name="password" placeholder="密码"><br><br>

        <!-- 这里开始涉及jsp的知识了，下节课开始讲，先看看效果 -->

        <scan>**<%=**request.getAttribute("errorMsg")**%>**</scan><br>

        <button type="submit">登录</button>

    </form>

</body>

</html>




