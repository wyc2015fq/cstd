# 【MyBatis学习02】走进MyBatis的世界 - z69183787的专栏 - CSDN博客
2017年07月18日 10:38:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：200
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)
mybatis是个持久层的框架，用来执行[数据库](http://lib.csdn.net/base/mysql)操作的，无外乎增删改查，上一节对mybatis有了宏观上的了解后，这一篇博客主要通过一个小示例来入门mybatis，先看一下要写的示例需求：
> 
根据用户id查询用户信息 
根据用户名称模糊查询用户信息 
添加用户、删除用户、更新用户
其实也就是增删改查，掌握这些，基本上就掌握了mybatis的基本操作了，下面一个个模块来完成。
## 1. mybatis环境准备
毫无疑问，砍柴得有把刀才行，环境的搭建是开发中必需的一个流程，mybatis的环境我分成以下几个部分来总结：
### 1.1 mybatis运行环境
mybatis运行环境主要是指mybatis的jar包，mybatis已经将它的各个版本托管到github上了，大家可以去[github](https://github.com/mybatis/mybatis-3)上下载，（如果觉得不方便）也可以去CSDN下载频道下载，我已经上传上去了，我用的是mybatis-3.3.0版本，下载地址==>[传送门](http://download.csdn.net/detail/eson_15/9542081)。 
从mybatis的jar包中就可以看出，mybatis和[hibernate](http://lib.csdn.net/base/javaee)有着强烈的对比，mybatis的jar包很简洁，不像hibernate有很多jar包，mybatis的jar包包括一个核心包和几个依赖包，我们全部导入到工程中即可。然后就是[MySQL](http://lib.csdn.net/base/mysql)的驱动包，也要导入到工程中。下面看一下总共需要导入的jar包： 
![jar包](https://img-blog.csdn.net/20160606002411163)
### 1.2 log4j配置文件
从上面的jar包中可以看出，mybatis的依赖包中有很多日志包，既然mybatis依赖log4j，那我们首先要建立一个log4j.properties文件，这个可以在官方的文件中拷贝下，如下：
```
# Global logging configuration
# developer-->DEBUG  productor-->INFO or ERROR
log4j.rootLogger=DEBUG, stdout
# MyBatis logging configuration...
log4j.logger.org.mybatis.example.BlogMapper=TRACE
# Console output...
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%5p [%t] - %m%n
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
在开发环境下日志级别要设置成DEBUG，产品模式下可以设置成INFO或者ERROR
### 1.3 全局配置文件
mybatis需要配置一个全局配置文件SqlMapConfig.xml，这个文件是用来配置mybatis的运行环境，即数据源、事务等。我们也可以从官方的例子中拷贝一份，然后做一下修改：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!-- 和Spring整合后environment配置都会被干掉 -->
    <environments default="development">
        <environment id="development">
            <!-- 使用jdbc事务管理，目前由mybatis来管理 -->
            <transactionManager type="JDBC" />
            <!-- 数据库连接池，目前由mybatis来管理 -->
            <dataSource type="POOLED">
                <property name="driver" value="com.mysql.jdbc.Driver" />
                <property name="url" value="jdbc:mysql://localhost:3306/mybatis" />
                <property name="username" value="root" />
                <property name="password" value="root" />
            </dataSource>
        </environment>
    </environments>
    <mappers>
        <mapper resource="sqlmap/User.xml" />
    </mappers>
</configuration>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
以上这些环境配置（`<environments>`标签中的内容在以后和[spring](http://lib.csdn.net/base/javaee)整合后，都会交给Spring来管理，现在暂时交给
 mybatis来管理）中，修改成自己数据库相对应的情况即可，`<mapper>`标签用来配置映射文件的，这些映射文件是针对不同的pojo的，这个示例中只操作一个User对象，所以只有一个配置文件，在sqlmap目录下的User.xml，在下文中可以看到。最后来看一下整个环境的结构： 
![整个环境目录](https://img-blog.csdn.net/20160606004010778)
接下来就开始开发示例程序了。
## 2. 程序的编写
首先我们得有一个pojo，在mybatis.po包中新建一个User.[Java](http://lib.csdn.net/base/java)类： 
![po](https://img-blog.csdn.net/20160606004522437)
对应数据库的建表语句：
```
use mybatis
drop table if exists user;
create table user
(
    id int primary key not null auto_increment,
    username varchar(40),
    birthday date,
    sex char(1),
    address varchar(255)
);
insert into user(username,birthday,sex,address) values("张三1","1990-09-19","男","同济大学");
insert into user(username,birthday,sex,address) values("张三2","1990-09-19","男","同济大学");
/*自己多加几个*/
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
下面开始写代码，完成示例程序。
### 2.1 根据用户id查询用户信息
首先得创建配置文件User.xml，并在配置文件中创建sql语句，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd"> 
<mapper namespace="test">
    <!-- 需求：通过id查询用户 -->
    <select id="findUserById" parameterType="int" resultType="mybatis.po.User">
        select * from user where id = #{id}
    </select>
</mapper>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
来简单解释下该配置文件中一些参数的作用：
> - `<select>`标签：用于执行数据库查询的，所有关于查询的都使用该标签。
- id属性：标识映射文件中的sql，将sql语句封装到mappedStatement对象中，即statement的id，下面执行数据库的时候需要用到这个id。
- #{}：表示一个占位符，用来接收输入参数的。
- #{id}：id标识接收输入的参数，参数名称就是id，如果输入参数是简单类型，那么#{}中的参数名可以任意，可以value或者其他名称。
- parameterType：指定输入参数的类型，这个要和数据库中的保持一致。
- resultType：指定输出结果的类型，即查询结果所映射的java对象。
有了User.xml配置文件后，需要在全局配置文件SqlMapConfig.xml中添加这个映射，上面已经添加过了，即：
```xml
<mappers>
    <mapper resource="sqlmap/User.xml" />
</mappers>
```
- 1
- 2
- 3
- 1
- 2
- 3
接下来就是写[测试](http://lib.csdn.net/base/softwaretest)类了：
```java
public class MybatisFirst {
    //因为接下来的测试代码中，获取sqlSession这部分都相同，所以抽取成一个方法
    public SqlSession getSession() throws IOException {
        String resource = "SqlMapConfig.xml"; //mybatis配置文件
        //得到配置文件的流
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //创建会话工厂SqlSessionFactory,要传入mybaits的配置文件的流
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        //通过工厂得到SqlSession
        SqlSession sqlSession = sqlSessionFactory.openSession();
        return sqlSession;
    }
    //根据id查询用户的信息，得到一条记录的结果
    @Test
    public void findUserById() throws IOException {
        SqlSession sqlSession = getSession(); //调用上面的方法获取sqlSession
        //通过SqlSession操作数据库
        //第一个参数：映射文件中statement的id，= namespace + statement的id
        //第二个参数：指定和映射文件中所匹配的parameterType类型的参数
        //selectOne表示查询出一条记录进行映射
        User user = sqlSession.selectOne("test.findUserById", 1);
        System.out.println(user);
        //释放资源，最好放在finally中，这里只是测试程序，就不弄了
        sqlSession.close();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
从java程序中可以看出，这个流程很明确，就是上一节我画的那个mybatis的流程图，而且mybatis有个特点，就是要执行什么语句，都写在配置文件中，需要传入或者输出什么参数类型也写在配置文件中，在java中只要对应那个配置传入我们想要的参数或者接受输出参数即可，很方便。
### 2.2 根据用户名称模糊查询用户信息
有了上面的过程，接下来就比较简单了，使用mybatis开发，流程都一样，先在User.xml中添加配置：
```xml
<mapper namespace="test">   
    <!-- 省略其他 -->
    <!-- 根据用户名称模糊查询用户信息，可能返回多条 -->
    <select id="findUserByName" parameterType="java.lang.String" resultType="mybatis.po.User">
        select * from user where username like '%${value}%'
    </select>
</mapper>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
来解释下部分参数的作用：
> - resultType：这里虽然是查询多条记录，但是resultType指定的就是单条记录所映射的java对象类型。
- `${}`：表示拼接sql串，将接收到的参数的内容不加任何的修饰拼接在sql中,`${}`中只能使用value，但是使用${}来拼接sql，可能会引起sql注入，所以不建议使用这种方法。
这里使用${}来拼接sql是为了后面java程序中直接输入方便（比如我模糊查询“张三”，就不用输入“%张三%”了），但是有sql注入的隐患。如果安全一点，还是使用#{}来接收参数，不过此时的话，java程序中输入参数就得是“%张三%”了。 
下面看下java程序：
```java
public class MybatisFirst {
    //省略不相关代码
    //根据用户名称模糊查询用户列表
    @Test
    public void findUserByName() throws IOException {
        SqlSession sqlSession = getSession();
        //selectList表示查询出一个列表（多条记录）进行映射
        List<User> list = sqlSession.selectList("test.findUserByName", "张三");
        System.out.println(list);
        //释放资源，最好放在finally中，这里只是测试程序，就不弄了
        sqlSession.close();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
处理流程和上面一模一样，不在赘述。
### 2.3 添加用户
首先完成添加用户的配置文件：
```xml
<mapper namespace="test">
    <!-- 省略不相关配置 -->
    <!-- 添加用户 -->
    <insert id="insertUser" parameterType="mybatis.po.User">
        insert into user(username,birthday,sex,address) values(#{username},#{birthday},#{sex},#{address})
        <!-- 将插入数据的主键返回，返回到user对象中 -->
        <selectKey keyProperty="id" order="AFTER" resultType="java.lang.Integer">
            select last_insert_id()
        </selectKey>
        <!-- 
        <selectKey keyProperty="id" order="BEFORE" resultType="java.lang.String">
            select uuid()
        </selectKey>
         -->
    </insert>
</mapper>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
下面解释下部分属性的作用：
> - `<insert>`标签：用于执行数据库查询的，所有关于查询的都使用该标签。
- parameterType：要传入一个具体的pojo（包括用户信息）
- #{}中指定pojo的属性名，接收到pojo对象的属性值，mybatis通过OGNL获取对象的属性值。
- `<selectKey>`标签：用来返回插入数据的主键的，实际中如果有获得主键的需要就可以使用它。
- select last_insert_id()：是sql函数，表示得到刚刚insert进去记录的主键值，只适用于自增主键。
- keyProperty：表示将查询到主键值设置到上面parameterType指定的对象的哪个属性。
- order：表示select last_insert_id()的执行顺序，是相对于insert语句来说的。
- resultType：表示select last_insert_id()的结果类型。
下面看下java代码：
```java
public class MybatisFirst {
    //省略不相关代码
    //添加用户信息
    @Test
    public void insertUser() throws IOException {
        SqlSession sqlSession = getSession();
        User user = new User("倪升武", new Date(), "男", "同济大学");
        sqlSession.insert("test.insertUser", user); //添加一项
        //提交事务
        sqlSession.commit();
        System.out.println(user.getId()); //获取刚刚添加的id
        //释放资源，最好放在finally中，这里只是测试程序，就不弄了
        sqlSession.close();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
### 2.4 更新用户和删除用户
接下来都比较简单了，基本参数的使用在上面都有涉及，我放在一起写了：
```xml
<mapper namespace="test">
    <!-- 省略不相关配置 -->
    <!-- 删除用户 -->
    <delete id="deleteUser" parameterType="java.lang.Integer">
        delete from user where id=#{id}
    </delete>
    <!-- 更新用户 -->
    <update id="updateUser" parameterType="mybatis.po.User">
        update user set username=#{username},birthday=#{birthday},sex=#{sex},address=#{address} where id=#{id}
    </update>
</mapper>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
下面看一下java代码：
```java
public class MybatisFirst {
    //删除用户信息
    @Test
    public void deleteUser() throws IOException {
        SqlSession sqlSession = getSession();
        //传入id，删除用户
        sqlSession.delete("test.deleteUser", 16);
        //提交事务
        sqlSession.commit();
        //释放资源，最好放在finally中，这里只是测试程序，就不弄了
        sqlSession.close();
    }
    //更新用户信息
    @Test
    public void updateUser() throws IOException {
        SqlSession sqlSession = getSession();
        User user = new User("倪升武", new Date(), "男", "同济大学");
        user.setId(9);
        //更新用户
        sqlSession.update("test.updateUser", user);
        //提交事务
        sqlSession.commit();
        //释放资源，最好放在finally中，这里只是测试程序，就不弄了
        sqlSession.close();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
好了，到这里mybatis的入门程序就写完了，从上面的程序来看，mybatis的基本操作还是挺简便的。主要要搞清楚mybatis的一个执行流程就比较好理解了
