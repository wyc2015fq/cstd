# Maven构建mybatis的Junit测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月07日 14:44:37[boonya](https://me.csdn.net/boonya)阅读数：15999








使用Maven构建项目非常快速，只需要在pom.xml添加相应的依赖就可以实现程序中jar的下载工作，节省了开发者到处寻找开发包所需的时间。mybatis的Junit测试，导入Junit依赖包即可。在数据库方面我这里采用是HSQLDB的内存数据库模式，不需要持久化数据到磁盘造成大堆的垃圾数据，所以HSQLDB非常好用，而且大多数Junit测试都是用它来作为数据库操作的实例。

**1、构建项目测试目录src/main/test**

![](https://img-blog.csdn.net/20131107142330718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**2、添加pom.xml的JUNIT和HSQLDB依赖配置**



```
<dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.10</version>
      <scope>compile</scope>
    </dependency>
    <dependency>
      <groupId>hsqldb</groupId>
      <artifactId>hsqldb</artifactId>
      <version>1.8.0.10</version>
      <scope>compile</scope>
    </dependency>
```
**3、创建com.boonya.mybatis.testJava放置文件并添加测试示例的实体和接口**


实体对象User



```java
package com.boonya.mybatis.test;

public class User {

  private Integer id;
  private String name;

  public Integer getId() {
    return id;
  }

  public void setId(Integer id) {
    this.id = id;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }
}
```
mybatis风格的接口对象UserMapper




```java
package com.boonya.mybatis.test;

public interface UserMapper
{

	User getUser(Integer id);

}
```



**4、创建配置文件目录config.mybatis.files编写HSQLDB的数据库操作语句脚本hqldb.sql**



```
drop table users if exists;

create table users (
  id int,
  name varchar(20)
);

insert into users (id, name) values(1, 'User1');
```


**5、在config.mybatis.files编写mybatis接口对象的映射SQL UserMapper.xml文件**


```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper
    PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
    "http://mybatis.org/dtd/mybatis-3-mapper.dtd">

<mapper namespace="com.boonya.mybatis.test.UserMapper">

	<select id="getUser" resultType="com.boonya.mybatis.test.User">
		select * from users
	</select>

</mapper>
```
**6、在config.mybatis.files添加mybatis的核心配置文件mybatis-config.xml**




```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
    PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
    "http://mybatis.org/dtd/mybatis-3-config.dtd">

<configuration>

	<environments default="development">
		<environment id="development">
			<transactionManager type="JDBC">
				<property name="" value="" />
			</transactionManager>
			<dataSource type="UNPOOLED">
				<property name="driver" value="org.hsqldb.jdbcDriver" />
				<property name="url" value="jdbc:hsqldb:mem:basetest" />
				<property name="username" value="sa" />
			</dataSource>
		</environment>
	</environments>

	<mappers>
		<mapper resource="config/mybatis/files/UserMapper.xml" />
	</mappers>

</configuration>
```
**7、在com.boonya.mybatis.test包下添加单元测试**




```java
package com.boonya.mybatis.test;

import java.io.Reader;
import java.sql.Connection;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.jdbc.ScriptRunner;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

public class UserTest
{

	private static SqlSessionFactory sqlSessionFactory;

	@BeforeClass
	public static void setUp() throws Exception
	{
		// create a SqlSessionFactory
		Reader reader = Resources.getResourceAsReader("config/mybatis/files/mybatis-config.xml");
		sqlSessionFactory = new SqlSessionFactoryBuilder().build(reader);
		reader.close();

		// populate in-memory database
		SqlSession session = sqlSessionFactory.openSession();
		Connection conn = session.getConnection();
		reader = Resources.getResourceAsReader("config/mybatis/files/hqlbd.sql");
		ScriptRunner runner = new ScriptRunner(conn);
		runner.setLogWriter(null);
		runner.runScript(reader);
		reader.close();
		session.close();
	}

	@Test
	public void shouldGetAUser()
	{
		SqlSession sqlSession = sqlSessionFactory.openSession();
		try
		{
			UserMapper mapper = sqlSession.getMapper(UserMapper.class);
			User user = mapper.getUser(1);
			Assert.assertEquals("User1", user.getName());
		} finally
		{
			sqlSession.close();
		}
	}

}
```
**8、设置完成后的项目文件目录结构**


![](https://img-blog.csdn.net/20131107144133937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**9、Junit测试验证结果**

![](https://img-blog.csdn.net/20131107144201687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



通过Junit测试，测试Ok!



