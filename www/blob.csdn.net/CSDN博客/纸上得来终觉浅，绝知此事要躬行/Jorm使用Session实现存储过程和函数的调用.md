# Jorm使用Session实现存储过程和函数的调用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月01日 22:54:44[boonya](https://me.csdn.net/boonya)阅读数：1495标签：[存储过程																[session																[开源中国																[单元测试																[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[JORM](https://blog.csdn.net/boonya/article/category/1720951)





        在Jorm中使用最多的操作就是使用Session来实现对数据的CRUD操作，因为使用session才更符合JORM的设计编程的标准。在Hibernate中也是使用session来管理数据库操作的，这一点Jorm和Hibernate极为相似。我前几天就因为这个session的存储过程调用和函数调用而卡壳了，所以今天特别在这里写出，姑且记之于此，待来者阅览。

**1、编写存储过程和函数**



```
//存储过程
DROP PROCEDURE   IF      EXISTS  count_has_same_name_proc;

CREATE PROCEDURE count_has_same_name_proc (xname VARCHAR(50 ) )
  BEGIN
    SELECT  COUNT(* )  FROM t_user WHERE name  like xname;
  END;

//函数
DROP   FUNCTION   IF      EXISTS  count_has_same_name_func;

CREATE    FUNCTION count_has_same_name_func ( xname  VARCHAR(50 ) ) 
RETURNS  INT
    BEGIN     
          DECLARE  s  INT ;
	  SELECT   COUNT(id ) into s  FROM t_user WHERE name  like xname;
	  RETURN s;
    END;
```



**2、编写需要的SessionManager**

```java
package com.boonya.jorm.service.utils;

import org.javaclub.jorm.Jorm;
import org.javaclub.jorm.Session;

public class SessionManager
{

	private static final Session session = Jorm.getSession();

	public static Session getInstance()
	{
		return SessionManager.session;
	}

}
```
**3、Session的存储过程和函数调用的单元测试用例**
```java
package com.boonya.jorm.test;

import java.sql.SQLException;
import java.util.List;
import junit.framework.Assert;
import org.javaclub.jorm.Session;
import org.javaclub.jorm.jdbc.JdbcException;
import org.javaclub.jorm.jdbc.callable.ProcedureCaller;
import org.junit.Test;
import com.boonya.jorm.entity.User;
import com.boonya.jorm.service.utils.LogManager;
import com.boonya.jorm.service.utils.SessionManager;
import com.mysql.jdbc.CallableStatement;
import com.mysql.jdbc.ResultSet;

public class SessionTest
{
	@Test
	public void session(){
		Session session=SessionManager.getInstance();
		Assert.assertNotNull("Session:会话不为空",session);
	}
	
	@Test
	public void getFirtUser()
	{
		int minId = 50;
		String sql = " SELECT * FROM t_user WHERE id > ?";
		Session session = SessionManager.getInstance();
		User user = session.loadFirst(User.class, sql, minId);
		Assert.assertNotNull("符合条件的第一个用户:", user);
	}

	@Test
	public void deleteBetweenIds()
	{
		int startId = 97, endId = 100;
		Session session = SessionManager.getInstance();
		boolean flag = false;
		session.beginTransaction();
		try
		{
			session.delete(User.class, " id > " + startId + " and id < " + endId);
			flag = true;
			session.commit();
		} catch (JdbcException e)
		{
			LogManager.setLog("区间ID删除：失败!", e);
		} finally
		{
			session.endTransaction();
		}
		Assert.assertTrue("区间ID删除：成功", flag);
	}

	@Test
	public void getNameList()
	{
		String sql = "SELECT name FROM t_user WHERE id > ? ";
		Session session = SessionManager.getInstance();
		List<String> names = session.list(String.class, sql, 10);
		System.out.println(names == null ? "列表为空" : "列表长度：" + names.size());
	}

	@Test
	public void sessionTransaction()
	{
		Session session = SessionManager.getInstance();
		session.beginTransaction();
		try
		{
			User user = new User("peng", "male", 24, "UI designer");
			session.save(user);
			session.commit();
		} catch (Exception e)
		{
			LogManager.setLog("事务添加用户：失败!", e);
			session.rollback();
		} finally
		{
			session.endTransaction();
		}
	}

	@Test
	public void procedure()
	{

		final String pro = "call count_has_same_name_proc(?)";

		final Session session = SessionManager.getInstance();

		String res = session.call(new ProcedureCaller()
		{
			@Override
			public CallableStatement prepare() throws SQLException
			{
				CallableStatement cs = (CallableStatement) getSession().getConnection().prepareCall(pro);
				cs.setString(1, "boonya");
				return cs;
			}

			@Override
			public Object callback(java.sql.CallableStatement cs) throws SQLException
			{
				ResultSet rs = (ResultSet) cs.executeQuery();
				Assert.assertNotNull("procedure：resultset", rs);
				while (rs.next())
				{
					int count = rs.getInt(1);
					System.out.println("procedure:统计同名用户结果：" + count);

				}
				return rs;
			}
		});
		System.out.println(res);
	}

	@Test
	public void function()
	{

		final String pro = "select count_has_same_name_func(?)";

		final Session session = SessionManager.getInstance();

		String res = session.call(new ProcedureCaller()
		{
			@Override
			public CallableStatement prepare() throws SQLException
			{
				CallableStatement cs = (CallableStatement) getSession().getConnection().prepareCall(pro);
				cs.setString(1, "boonya");
				return cs;
			}

			@Override
			public Object callback(java.sql.CallableStatement cs) throws SQLException
			{
				ResultSet rs = (ResultSet) cs.executeQuery();
				Assert.assertNotNull("function：resultset", rs);
				while (rs.next())
				{
					int count = rs.getInt(1);
					System.out.println("function:统计同名用户结果：" + count);

				}
				return rs.toString();
			}
		});
		System.out.println(res);
	}

}
```
注：被误导的调用写法：pro="{? = call hello_proc(?)}";可以理解该coder是将此当做对象来查，然后将查询结果赋值给一个缺省的数据类型来存储，但是这样是调用不成功的。可以在sql语句中添加"{}" ，应该这样写："{ call hello_proc(?)}"。 这个在开源中国上可以找得到的，地址：[http://www.oschina.net/p/gerald-jorm](http://www.oschina.net/p/gerald-jorm)](https://so.csdn.net/so/search/s.do?q=单元测试&t=blog)](https://so.csdn.net/so/search/s.do?q=开源中国&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=存储过程&t=blog)




