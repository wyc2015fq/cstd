# jdbc篇第6课：使用properties当做Dbc的配置文件 - PeterBishop - CSDN博客





2018年11月27日 19:27:16[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：32








  这节课我们实现一个功能，我们把连接mysql要用到的信息放在properties配置文件里，让Dbc加载这个文件读取配置信息给我们返回一个Connection对象



实现:
package com.tool;



import java.io.*;

import java.sql.Connection;

import java.sql.DriverManager;

import java.sql.SQLException;

import java.util.Properties;



//Database Connection，负责连接数据库，返回一个Connection对象

public class Dbc {



    //不允许调用构造器

    private Dbc(){}



    private static Properties *properties*;



    private static InputStream *inputStream*;



    public static Connection getConnection(String driver,String url,String user, String password) throws ClassNotFoundException, SQLException {

        Class.*forName*(driver);

        return DriverManager.*getConnection*(url,user,password);

    }



    public static Connection getConnection(String propertiesPath) throws IOException, SQLException, ClassNotFoundException {

        File file = new File(propertiesPath);

        if (file.getName().endsWith(".properties") && file.exists())

        {

            *inputStream *= new FileInputStream(file);

            *properties *= new Properties();

            *properties*.load(*inputStream*);



            String driver = *properties*.getProperty("driver");

            String url = *properties*.getProperty("url");

            String user = *properties*.getProperty("user");

            String password = *properties*.getProperty("password");



            return *getConnection*(driver,url,user,password);

        }

        else

        {

            throw new FileNotFoundException("cannot find file " + file.getAbsolutePath());

        }

    }

}


测试:
public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException {

    Connection conn = Dbc.*getConnection*("src/com/resource/dbc.properties");



    //测试下连接是否真的可用

    Statement statement = conn.createStatement();

    String sql = "select * from tbl_employee where id = 1";

    ResultSet resultSet = statement.executeQuery(sql);

    resultSet.next();

    System.*out*.println(resultSet.getInt(1));

    System.*out*.println(resultSet.getString(2));

    //连接确实可用

}


结果:

1

xiaoye



