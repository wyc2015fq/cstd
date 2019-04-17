# Maven中使用mybatis - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年12月18日 17:57:12[boonya](https://me.csdn.net/boonya)阅读数：34060








**1、Mybatis优缺点**

优点： Mybatis实现了对Dao层的封装，隔离了SQL语句，便于管理，避免了像JDBC那样操作数据集，便于扩展等等。

缺点： Mybatis属于?半自动“ORM”，比Hibernate的工作做得要多很多，对象参数传递比较麻烦，没有Hibernate对象操作的概念。

**2、Mybatis的实现方式**

Mybatis提供两种应用实现：XML配置和注解。

2.1配置主要依赖实体对象的xml文件，将对象以<resultMap>形式注入，并提供给<insert > <delete > <select> <update> 语句引用。

2.2使用注解来的比配置XML文件要简单得多。只需要在接口上添加相应的注解并附上SQL语句就行了，如：

**插入语句：@insert(" insert into table_user ..")**

**修改语句： @update(" update table_user set ....")**

**删除语句：@delete(" delete from table_user .....")**

**查询语句：@select(" select * from table_user .....")**

**3、下载mybatis的jar包：**mybatis-3.1.1-bundle.zip?，网址：[http://code.google.com/p/mybatis/](http://code.google.com/p/mybatis/)

Maven的pom.xml依赖配置如下：



```
<dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis</artifactId>
    <version>3.1.1</version>
  </dependency>
```



依赖仓库配置：



```
<repository>
    <id>mybatis-snapshot</id>
    <name>MyBatis Snapshot Repository</name>
    <url>https://oss.sonatype.org/content/repositories/snapshots</url>
</repository>
```



**4、构建自己的mybatis项目,如图所示：**

**![](http://images.cnitblog.com/blog/311199/201212/18222102-00e44c144f9f408ca25196e8ee67905f.png)**

**展开文件展示：**

**![](https://img-blog.csdn.net/20131102143639359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**![](https://img-blog.csdn.net/20131102143659468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**5、mybatis-config.xml:配置数据源和隐射文件**



```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//ibatis.apache.org//DTD Config 3.0//EN" "http://ibatis.apache.org/dtd/ibatis-3-config.dtd">
<configuration>
	<environments default="environment">
		<environment id="environment">
			<transactionManager type="JDBC" />
			<dataSource type="POOLED">
				<property name="driver" value="com.mysql.jdbc.Driver" />
				<property name="url"
					value="jdbc:mysql://localhost:3306/mybatis?characterEncoding =UTF-8" />
				<property name="username" value="root" />
				<property name="password" value="root" />
			</dataSource>
		</environment>
	</environments>
       <!-- <mappers> <mapper resource="mybatis/User.xml"/> </mappers> -->
</configuration>
```



注：<!-- <mappers> <mapper resource="mybatis/User.xml"/> </mappers> -->是使用实体配置的实现，如果使用注解则不需要添加。

**6、使用注解实现CRUD操作-此处以实现用户管理为例 **

6.1编写接口（interface）：实际上就是官方文档所述的Mapper



```java
public interface UserService {
     @Insert(" insert into users (id,username,password) values (#{id},#{username},#{password})")
     void add(@Param("id")String id,@Param("username") String username,@Param("password")String password);

     @Delete(" delete from users where id=#{id}") 
     void delete(String id);

     @Update(" update users set username=#{username},password=#{password} where id=#{id}")
     int update(@Param("username") String username,@Param("password")String password,@Param("id")String id);

     @Select(" select * from users where id=#{id}")
     User getUser(String id);

     @Select(" select * from users order by id asc ")
     List<User> getUsers(); 

     @Select(" select * from users order by id asc limit #{pageSize} offset #{offset} ")
     List<User> getUsersByPage(@Param("offset")int offset,@Param("pageSize") int pageSize);//offset=pageSize*(page-1)}?
}
```



注：使用注解时接口的方法不能重载，否者会产生Mapped Statements collection does not contain value for 异常,另外还应尽量避免配置和注解混合使用的情况。

6.2获取数据源



```java
public class GetSqlSessionFactory
{
	private static SqlSessionFactory sqlSessionFactory = null;
	
	private static GetSqlSessionFactory getSqlSessionFactory = null;

	private GetSqlSessionFactory()
	{
		String rs = "mybatis-config.xml";
		Reader reader = null;
		try
		{
			reader = Resources.getResourceAsReader(rs);
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		sqlSessionFactory = new SqlSessionFactoryBuilder().build(reader);
		// 注解方式查询时需要注册mapper
		sqlSessionFactory.getConfiguration().addMapper(UserService.class);
	}

	public static GetSqlSessionFactory getInstance()
	{
		if (getSqlSessionFactory == null)
			getSqlSessionFactory = new GetSqlSessionFactory();
		return getSqlSessionFactory;
	}

	public static SqlSessionFactory getSqlSessionFactory()
	{
		return sqlSessionFactory;
	}

}
```



6.3DAO层：数据访问



```java
public class UserDao
{
	@SuppressWarnings("static-access")
	public void add(String id, String username, String password)
	{
		SqlSession session = GetSqlSessionFactory.getInstance().getSqlSessionFactory().openSession();
		try
		{
			session.getMapper(UserService.class).add(id, username, password);
			session.commit();// 提交事务
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			session.close();
		}
	}

	@SuppressWarnings("static-access")
	public void delete(String id)
	{
		SqlSession session = GetSqlSessionFactory.getInstance().getSqlSessionFactory().openSession();
		try
		{
			session.getMapper(UserService.class).delete(id);
			session.commit();// 提交事务
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			session.close();
		}
	}

	@SuppressWarnings("static-access")
	public int update(String username, String password, String id)
	{
		int count = 0;
		SqlSession session = GetSqlSessionFactory.getInstance().getSqlSessionFactory().openSession();
		try
		{
			// Map<String, Object> map=new HashMap<String, Object>();
			// map.put("username", user.getUsername());
			// map.put("password", user.getPassword());
			// session.update("updateUser", map);
			count = session.getMapper(UserService.class).update(username, password, id);
			session.commit();// 提交事务
		} catch (Exception e)
		{
			count = 0;
			e.printStackTrace();
		} finally
		{
			session.close();
		}
		return count;
	}

	@SuppressWarnings("static-access")
	public List<User> getUsers(int offset, int pageSize)
	{
		List<User> users = new ArrayList<User>();
		SqlSession session = GetSqlSessionFactory.getInstance().getSqlSessionFactory().openSession();
		try
		{
			// users=session.selectList("user_list_page", new User(),new
			// RowBounds(offset,pageSize));//未测试过
			// 注解方式查询
			users = session.getMapper(UserService.class).getUsersByPage(offset, pageSize);
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			session.close();
		}
		return users;
	}
}
```



6.4servlet实现数据跳转访问



```java
protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
	{
		response.setContentType("text/html;charset=utf-8");
		UserService userService = new UserServiceImpl();
		PrintWriter out = response.getWriter();
		String method = request.getParameter("method");
		String returnString = "";
		// 方法入口
		if (method.equals("users"))
		{
			returnString = new JsonUtil<User>().getJsonByListObj(userService.getUsers());
		} else if (method.equals("user"))
		{
			String id = request.getParameter("id");
			returnString = new JsonUtil<User>().getJsonByEntity(userService.getUser(id));
		} else if (method.equals("delete"))
		{
			String id = request.getParameter("id");
			Map<String, Object> map = new HashMap<String, Object>();
			map.put("event", method);
			map.put("param", id);
			returnString = new JsonUtil<User>().getJsonByObject(map);
		} else if (method.equals("update"))
		{
			String id = request.getParameter("id");
			String username = request.getParameter("username");
			String password = request.getParameter("password");
			Map<String, Object> map = new HashMap<String, Object>();
			int count = userService.update(username, password, id);
			map.put("status", count == 0 ? false : true);
			map.put("event", method);
			map.put("param", id);
			returnString = new JsonUtil<User>().getJsonByObject(map);
		} else if (method.equals("paginate"))
		{
			int limit = Integer.valueOf(request.getParameter("pageSize"));
			int offset = Integer.valueOf(request.getParameter("offset"));
			returnString = new JsonUtil<User>().getJsonByListObj(userService.getUsersByPage(offset, limit));
		}
		// 输出JSON数据
		out.print(returnString);
		out.flush();
		out.close();
	}
```



至此，注解方式的实现就全部完成了。

**7、User.xml实体对象配置**



```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//ibatis.apache.org//DTD Mapper 3.0//EN" "http://ibatis.apache.org/dtd/ibatis-3-mapper.dtd">
<mapper namespace="com.boonya.mybatis.entity.User">
	<resultMap id="UserResultMap" type="com.boonya.mybatis.entity.User">
		<result property="id" column="id" jdbcType="VARCHAR" javaType="string" />
		<result property="name" column="name" javaType="string"
			jdbcType="VARCHAR" />
		<result property="createtime" column="createtime" javaType="string"
			jdbcType="VARCHAR" />
		<result property="tilepath" column="tilepath" javaType="string"
			jdbcType="VARCHAR" />
		<result property="lastlogintime" column="lastlogintime"
			javaType="string" jdbcType="VARCHAR" />
		<result property="nickname" column="nickname" javaType="string"
			jdbcType="VARCHAR" />
		<result property="password" column="password" javaType="string"
			jdbcType="VARCHAR" />
		<result property="picture" column="picture" javaType="string"
			jdbcType="VARCHAR" />
		<result property="sex" column="sex" javaType="string" jdbcType="VARCHAR" />
		<result property="username" column="username" javaType="string"
			jdbcType="VARCHAR" />
	</resultMap>
	<!-- 一般的SQL查询或操作语句配置 -->
	<select id="user" parameterType="int" resultType="com.boonya.mybatis.entity.User"
		resultMap="UserResultMap">select * from users where id=#{id} </select>
	<select id="users" resultMap="UserResultMap">select * from users order by id asc
	</select>
	<insert id="addUser" parameterType="com.boonya.mybatis.entity.User"
		parameterMap="UserResultMap">insert into users
		(id,name,createtime,email,tilepath,lastloginip,lastlogintime,nickname,password,picture,sessionid,sex,tel,username)
		values
		(#{id},#{name},#{createtime},#{email},#{tilepath},#{lastloginip},#{lastlogintime},#{nickname},#{password},#{picture},#{sessionid},#{sex},#{tel},#{username})
	</insert>
	<!-- Dynamic SQL 这里的方法未经测试 -->
	<!-- IF -->
	<select id="dynamic_user" resultMap="UserResultMap">
		select * from users where username=#{username}
		<if test="name!=null">and name=#{name}</if>
	</select>
	<!-- FOREACH -->
	<select id="dynamic_users" resultMap="UserResultMap">
		select * from users name=#{name} and createtime in
		<foreach collection="list" item="classTimeList" open="("
			separator="," close=")"> #{classTimeList} </foreach>
	</select>
	<!-- choose when otherwise -->
	<select id="dynamic_findNameLike" resultMap="UserResultMap">
		SELECT * FROM USERS WHERE NAME IS NOT NULL
		<choose>
			<when test="nickname!=null">AND nickname like #{nickname}</when>
			<when test="picture!=null ">AND picture like #{picture}</when>
			<otherwise>AND lastlogintime > #{lastlogintime}</otherwise>
		</choose>
	</select>
	<!-- WHERE -->
	<select id="dynamic_findUSERSLike" resultMap="UserResultMap">
		select * from users
		<where>
			<if test="name!=null">WHERE name LIKE #{name}</if>
		</where>
	</select>
</mapper>
```



注：如果使用配置需将mybatis-config.xml文件的mapper去掉注释： 

<!-- <mappers> 

         <mapper resource="mybatis/User.xml"/> 

</mappers> -->

并且注释掉使用注解的mapper隐射： 

//注解方式查询时需要注册mapper 

sqlSessionFactory.getConfiguration().addMapper(UserService.class);

配置方式是根据SQL语句的隐射去访问的 如UserDao 中的getUsers方法： 

//xml方式查询 

//users=session.selectList("users");

**8、测试数据是否能够正常访问**

8-1、数据库mybatis创建users表并添加数据



```
/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50513
Source Host           : localhost:3306
Source Database       : mybatis

Target Server Type    : MYSQL
Target Server Version : 50513
File Encoding         : 65001

Date: 2012-12-17 18:25:15
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `users`
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` varchar(50) NOT NULL,
  `username` varchar(30) DEFAULT NULL,
  `password` varchar(20) DEFAULT NULL,
  `name` varchar(30) DEFAULT NULL,
  `nickname` varchar(30) DEFAULT NULL,
  `sex` varchar(10) DEFAULT NULL,
  `picture` varchar(255) DEFAULT NULL,
  `createtime` varchar(20) DEFAULT NULL,
  `lastlogintime` varchar(50) DEFAULT NULL,
  `tilepath` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of users
-- ----------------------------
INSERT INTO `users` VALUES ('HUSDHSHFSD_00001', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '男', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_000010', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '男', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00002', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '女', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00003', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '男', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00004', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '男', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00005', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '女', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00006', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '男', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00007', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '男', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00008', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '男', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
INSERT INTO `users` VALUES ('HUSDHSHFSD_00009', 'BOONYA', '123456', 'BOONYA_U', 'BOONYA_*', '女', 'images/inde.png', '2012-12-17 15:30:23', '1832928398932', 'this/home/index.tile');
```



8-2、修改web.xml的servlet的配置



```
<!DOCTYPE web-app PUBLIC
 "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
 "http://java.sun.com/dtd/web-app_2_3.dtd" >

<web-app>
  <display-name>Archetype Created Web Application</display-name>
  <servlet>
  	<servlet-name>DefaultUserListHandler</servlet-name>
  	<display-name>DefaultUserListHandler</display-name>
  	<description></description>
  	<servlet-class>com.boonya.mybatis.servlet.DefaultUserListHandler</servlet-class>
  </servlet>
  <servlet-mapping>
  	<servlet-name>DefaultUserListHandler</servlet-name>
  	<url-pattern>/userservice</url-pattern>
  </servlet-mapping>
  
  <welcome-file-list>
         <welcome-file>index.html</welcome-file>
 </welcome-file-list>
 
</web-app>
```

将mybatis项目部署到Tomcat服务器中，启动服务器，在浏览器输入：[http://localhost:8080/mybatis/userservice?method=users](http://localhost:8080/mybatis/userservice?method=users)会看到如下图所示的结果：



![](https://img-blog.csdn.net/20131102214405796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：刚开始学习mybatis的使用很多地方或细节不是很在意，如某些地方存在问题希望指点一二！

示例项目下载地址：[http://download.csdn.net/detail/boonya/6491753](http://download.csdn.net/detail/boonya/6491753)





