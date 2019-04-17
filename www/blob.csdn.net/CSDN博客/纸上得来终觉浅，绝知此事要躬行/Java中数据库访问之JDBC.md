# Java中数据库访问之JDBC - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年08月31日 18:16:25[boonya](https://me.csdn.net/boonya)阅读数：771








**1、实现多种数据库访问机制**

原理：采用属性文件（xxx.properties）来修改不同数据库的访问。

例如：db.properties文件下(访问Oracle数据库)：



```
driver=oracle.jdbc.driver.OracleDriver
    url=jdbc:oracle:thin:@localhost:1521:orcl
    user=scot
    pwd=tiger
```



**2、编写数据库连接类：ConnectionUtils**



```java
public final class ConnectionUtils
{
	private static final String DRIVER = PropertiesUtil.getValue("driver");
	private static final String URL = PropertiesUtil.getValue("url");
	private static final String USER = PropertiesUtil.getValue("user");
	private static final String PWD = PropertiesUtil.getValue("pwd");
	/**
	 * 加载驱动,通常放在静态代码块中，因为加载驱动是一个非常重量级的操作
	 * 因此我们应保证在整个程序运行过程中，它只被加载一次，但要随时可用
	 * 静态代码块刚好满足此要求，它在类被加载时执行一次，以后就再不会执行了
	 */
	static
	{
		try
		{
			Class.forName(DRIVER);
		} catch (ClassNotFoundException e)
		{
			e.printStackTrace();
		}
	}


	/**
	 * 避免用户实例化此类(因此类是一个工具类，应尽量使用静态的方式来访问)
	 */
	private ConnectionUtils()
	{
	}


	/**
	 * 取得连接的工具方法
	 * 
	 * @return
	 */
	public static Connection getConnection()
	{
		try
		{
			return DriverManager.getConnection(URL, USER, PWD);
		} catch (SQLException e)
		{
			e.printStackTrace();
		}
		return null;
	}


	public static void close(ResultSet rs, Statement st, Connection conn)
	{
		if (rs != null)
		{
			try
			{
				rs.close();
			} catch (SQLException e)
			{
				e.printStackTrace();
			}
		}
		if (st != null)
		{
			try
			{
				st.close();
			} catch (SQLException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if (conn != null)
		{
			try
			{
				conn.close();
			} catch (SQLException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}


	public static void close(Statement st, Connection conn)
	{
		ConnectionUtils.close(null, st, conn);
	}
}
```



**3、初始化属性文件的PropertiesUtil 类（出于对性能的考虑，避免多次访问）**



```java
public class PropertiesUtil
{
	private static Properties prop = new Properties();
	static
	{
		InputStream ins = PropertiesUtil.class.getClassLoader().getResourceAsStream("db.properties");
		try
		{
			prop.load(ins);
		} catch (IOException e)
		{
			e.printStackTrace();
		}
	}


	public static String getValue(String key)
	{
		return prop.getProperty(key);
	}

}
```



**4、编写常用方法的实现类BaseDaoImpl**



```java
public class BaseDaoImpl<T>
{
	private Connection conn = null;
	private PreparedStatement pst = null;
	private ResultSet rs = null;


	/**
	 * 更新(增加到、删除、修改)数据的方法
	 * 
	 * @param sql
	 * @param paras
	 * @return
	 */
	public int updateData(String sql, Object[] paras)
	{
		this.createPreparedStatement(sql, paras);
		int result = 0;
		try
		{
			result = this.pst.executeUpdate();
		} catch (SQLException e)
		{
			e.printStackTrace();
		} finally
		{
			ConnectionUtils.close(rs, pst, conn);
		}
		return result;
	}


	public int updateData(String sql)
	{
		return this.updateData(sql, null);
	}


	public List<T> query(String sql, ResultSetter rssetter)
	{
		return this.query(sql, null, rssetter);
	}


	public List<T> query(String sql, Object[] paras, ResultSetter rssetter)
	{
		this.createPreparedStatement(sql, paras);
		try
		{
			ResultSet rs = this.pst.executeQuery();
			// 将ResultSet中的数据封装到集合中[]
			return rssetter.setResultSetData(rs);
		} catch (SQLException e)
		{
			e.printStackTrace();
		} finally
		{
			ConnectionUtils.close(rs, pst, conn);
		}
		return null;
	}


	/**
	 * 创建数据操作对象
	 * 
	 * @param sql
	 * @param paras
	 */
	private void createPreparedStatement(String sql, Object[] paras)
	{
		this.conn = ConnectionUtils.getConnection();
		try
		{
			this.pst = this.conn.prepareStatement(sql);
			// 绑定动态参数
			if (paras != null && paras.length > 0)
			{
				for (int i = 0; i < paras.length; i++)
				{
					this.pst.setObject(i + 1, paras[i]);
				}
			}
		} catch (SQLException e)
		{
			e.printStackTrace();
		}
	}
}
```

注：一般对象实现类继承BaseDaoImpl；方法调用，实现具体业务。



