# jdbc篇第1课：jdbc的介绍和使用 - PeterBishop - CSDN博客





2018年11月24日 10:10:26[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：16








  我们已经学过mysql了，接下来我们要学学如何用java来操作mysql。



  Java操作数据库所使用的技术叫JDBC，看百度百科:



JDBC（Java DataBase Connectivity,java数据库连接）是一种用于执行SQL语句的Java API，可以为多种关系数据库提供统一访问，它由一组用[Java语言](https://baike.baidu.com/item/Java%E8%AF%AD%E8%A8%80)编写的类和接口组成。JDBC提供了一种基准，据此可以构建更高级的工具和接口，使数据库开发人员能够编写数据库应用程序



简单来说，jdbc就是一座桥梁，连接着java和数据库



我们要学习的就是如何用java来操作mysql



mysql服务是否运行的查看方式:

![](https://img-blog.csdnimg.cn/20181124100627547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





而我们连接mysql服务用的就是各种客户端，比如：
- Mysql Command Line Client
- Navicat for Mysql
- mysql-connector.jar(我们今天要学的)



下面来说下什么是jar包，算了，自己解释:

 Jar包或者说.jar文件，是一种压缩包，这种压缩包里放着.class文件(这个是我们要用的)或者.java文件(放.java的叫源码包)，通过引用jar包就可以使用其他人写好的类



Jdk里也有很多jar包，比如我们配置java环境的时候

Path里有一项是: %JAVA_HOME%\bin;%JAVA_HOME%\jre\bin;

JAVA_HOEM是: F:\Java\jdk





F:\Java\jdk\bin下放着各种.exe文件(可执行文件)

F:\Java\jdk\jre\bin下放着各种.dll文件(动态链接库，dynamic link library)

再来看看lib文件夹:

F:\Java\jdk\lib下放着各种jar包

F:\Java\jdk\jre\lib下放着各种jar包(java程序运行需要用的jar包)



我们用的String类就在rt.jar下



我们想要连接mysql，需要用到mysql-connector这个jar包，这个我已经准备好了

用idea的同学可以看下我是怎么引用jar包的，用eclipse的同学去百度怎么在项目里引入其他jar包

![](https://img-blog.csdnimg.cn/20181124100647682.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)







可以看到在引入mysql-connector这个jar包后就可以使用里面的类了



接下来讲怎么用mysql-connector连接数据库



Demo 01：
package jdbc;





import java.sql.Connection;

import java.sql.DriverManager;

import java.sql.SQLException;



public class JdbcTeach {



    public static void main(String[] args) {

        //首先，mysql其实是分为服务端和客户端，服务端在后台启动着，可以来看看



        */**         * **用mysql-connector连接数据库步骤:         * 1. 加载com.mysql.jdbc.Driver类         * 2. 通过DriverManager.getConnection()来获取连接(一个Connection对象)         * 3. 获取了Connection对象后就已经自动连接上mysql数据库了         * 4. 用完了别忘记关闭连接         */        *try {

            Class.*forName*("com.mysql.jdbc.Driver");



            */**             * DriverManager.getConnection(url,user,password)             * **@Param **url: **要连接到数据库，固定写法: jdbc:mysql://你的ip地址:mysql的端口号/数据库名             * **@Param **user: 连接数据库的用户名             * **@Param **password: 连接数据库的密码             *             * **@return **返回一个Connection对象             */            *//如果连的是本地数据库，ip地址可以是localhost或者127.0.0.1

            //mysql端口号默认是3306

            String url = "jdbc:mysql://localhost:3306/teach";



            String user = "root";

            String password = "root";



            //这个Connection是一个接口，我们来看看它引用的对象是那个类型

            //引用的是com.mysql.jdbc.JDBC4Connection类型的对象

            //当然mysql-connector都帮我们封装好了，我们不用在意这些细节

            //这也体现了接口的作用，接口只是定义一组方法，可以交由实现类去具体实现

            //而我们使用接口是不必关心它的实现类的具体实现细节的

            Connection connection = DriverManager.*getConnection*(url, user, password);



//            System.out.println("Connection的类型为: " + connection.getClass().getName());

            connection.close();



        } catch (ClassNotFoundException e) {

            e.printStackTrace();

        } catch (SQLException e) {

            e.printStackTrace();

        }

    }

}




