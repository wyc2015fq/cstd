# Jorm使用JDBC式的Connection实现存储过程和函数的调用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月01日 22:37:04[boonya](https://me.csdn.net/boonya)阅读数：1492








Jrom使用JDBC式的Connection实现存储过程和函数的调用，这个问题是峰回路转的，前几天看了别人写的例子被误导了，一直纠结处理，今天通过Jorm原始的JDBC方式的存储过程调用实现了该功能，殊途同归，这就是许多持久化框架集成JDBC的最主要原因之一。

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



**2、ConnectionManger的编写（获取Connection对象）**

```java
package com.boonya.jorm.service.utils;

import org.javaclub.jorm.Jorm;
import com.mysql.jdbc.Connection;

public class ConnectionManager
{

	private static final Connection connection = (Connection) Jorm.getConnection();

	public static Connection getInstance()
	{
		return ConnectionManager.connection;
	}

}
```

**3、普通JDBC的写法调用存储过程和函数测试用例**

```java
package com.boonya.jorm.test;

import java.sql.SQLException;
import org.junit.Assert;
import org.junit.Test;
import com.boonya.jorm.service.utils.ConnectionManager;
import com.boonya.jorm.service.utils.LogManager;
import com.mysql.jdbc.Connection;
import com.mysql.jdbc.PreparedStatement;
import com.mysql.jdbc.ResultSet;

public class ConnectionTest
{
	@Test
	public void connection(){
		Connection conn=ConnectionManager.getInstance();
		Assert.assertNotNull("Connection:数据库连接不为空",conn);
	}
	
	@Test
	public  void procedure(){
		String sql = "call count_has_same_name_proc(?)";
		Connection conn=ConnectionManager.getInstance();
		try
		{
			PreparedStatement pstmt=conn.clientPrepareStatement(sql);
			pstmt.setString(1, "boonya");
			ResultSet rs=(ResultSet) pstmt.executeQuery();
			Assert.assertNotNull("procedure:数据集不为空",rs);
	        while (rs.next())
			{
	        	int count=rs.getInt(1);
				System.out.println("procedure:统计同名用户结果："+count);
				
			}
		} catch (SQLException e)
		{
			LogManager.setLog("调用数据库procedure",e);
		}
	}
	
	@Test
	public  void function(){
		String sql = "select count_has_same_name_func(?)";
		Connection conn=ConnectionManager.getInstance();
		try
		{
			PreparedStatement pstmt=conn.clientPrepareStatement(sql);
			pstmt.setString(1, "boonya");
			ResultSet rs=(ResultSet) pstmt.executeQuery();
			Assert.assertNotNull("function:数据集不为空",rs);
	        while (rs.next())
			{
	        	int count=rs.getInt(1);
				System.out.println("function:统计同名用户结果："+count);
				
			}
		} catch (SQLException e)
		{
			LogManager.setLog("调用数据库function",e);
		}
	}


}
```
注：此实现是JDBC实现的数据库存储过程和函数的调用方式，此处也未关闭Connection，因为系统中只需要创建一个Connection的实例就行了，若单独创建一个Connection的使用需要关闭该Connection，进行资源释放。










