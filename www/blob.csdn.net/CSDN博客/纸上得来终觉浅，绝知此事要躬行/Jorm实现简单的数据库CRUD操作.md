# Jorm实现简单的数据库CRUD操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月01日 22:26:10[boonya](https://me.csdn.net/boonya)阅读数：1824








Jorm 跟Hibernate的相似度非常高，它也是一个封装了JDBC的数据库对象持久化的框架。Jorm本身提供了annotation的组件引用，可以通过简单地标记实体对象的字段和类就可以实现Java对象与数据库对象的一一对应关系。通过Jorm的工具方法来实现数据库的持久化或查询操作，最重要的工具类文件是Jorm.java，其次是Session.java。眼见为实，下面来看一下程序实例。

**1、数据库及测试表创建**

   打开mysql数据库管理界面，创建一个名为test的数据库，并创建一个t_user的数据库表，数据库建表语句如下：



```
CREATE TABLE `t_user` (                
        `id` int(11) NOT NULL,               
        `name` varchar(50) DEFAULT NULL,     
        `sex` char(4) DEFAULT NULL,          
        `age` int(11) DEFAULT NULL,          
        `career` varchar(100) DEFAULT NULL,  
        PRIMARY KEY (`id`)                   
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```
**2、创建User实体对象（此对象与数据库表字段一一对应）**




```java
package com.boonya.jorm.entity;

import org.javaclub.jorm.annotation.Column;
import org.javaclub.jorm.annotation.Entity;
import org.javaclub.jorm.annotation.Id;
import org.javaclub.jorm.annotation.NoColumn;
import org.javaclub.jorm.annotation.PK;
/**
 * 
 * <li>文件名称: User</li>
 * <li>文件描述: $</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-10-27</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午04:51:57,修改内容描述</li>
 *
 */
@PK(value = "id")
@Entity(table="t_user")
public class User {
    
    @Id
    @Column("id")
    private int id;

    @Column("name")
    private String name;
        
    // 与数据库字段命名一致，可以不指定@Column
    private String sex;

    @Column("age")
    private Integer age;

    @Column("career")
    private String career;
    
    @NoColumn
    private int kvalue;
    
    public User() {
        super();
    }

    public User(String name, String sex, Integer age, String career) {
        super();
        this.name = name;
        this.sex = sex;
        this.age = age;
        this.career = career;
    }

    public User(Integer id, String name, String sex, Integer age, String career) {
        super();
        this.id = id;
        this.name = name;
        this.sex = sex;
        this.age = age;
        this.career = career;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public String getCareer() {
        return career;
    }

    public void setCareer(String career) {
        this.career = career;
    }

    public int getKvalue() {
        return kvalue;
    }

    public void setKvalue(int kvalue) {
        this.kvalue = kvalue;
    }

    @Override
    public String toString() {
        StringBuffer sb = new StringBuffer();
        sb.append("[" + id + ", " + name + ", " + sex + ", " + age + ", " + career + "]");
        return sb.toString();
    }

}
```
注：@PK表示主键，@NoColumn表示不在数据库持久化该字段（即不进行该字段的映射）。


**3、编写IUserService接口，定义用户动作**



```java
package com.boonya.jorm.service;

import java.util.List;

import com.boonya.jorm.entity.User;
/**
 * 
 * <li>文件名称: IUserService</li>
 * <li>文件描述: $</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-10-27</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午04:55:36,修改内容描述</li>
 *
 */
public interface IUserService {
	
	public boolean add(User user);
	
	public boolean batchAdd(List<User> userList);
	
	public boolean delete(int id);
	
	public User getUser(int id);
	
	public boolean update(User user);
	
	public List<User> getPagingList(Object [] params,int page, int pageSize,String sortType,String sortName);

}
```



**4、编写SessionManager来实现数据库访问**



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
**5、编写UserDao的CRUD方法**




```java
package com.boonya.jorm.dao;

import java.util.List;
import org.javaclub.jorm.Page;
import org.javaclub.jorm.Session;
import org.javaclub.jorm.jdbc.JdbcException;
import org.javaclub.jorm.jdbc.batch.JdbcBatcher;
import org.javaclub.jorm.jdbc.criterion.Order;
import org.javaclub.jorm.jdbc.sql.SqlParams;
import com.boonya.jorm.entity.User;
import com.boonya.jorm.service.utils.LogManager;
import com.boonya.jorm.service.utils.SessionManager;
/**
 * 
 * <li>文件名称: UserDao</li>
 * <li>文件描述: $</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-10-27</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午04:52:16,修改内容描述</li>
 *
 */
public class UserDao
{

	/**
	 * 根据ID查询用户信息
	 * 
	 * @param id
	 *            主键ID
	 * @return User 用户信息对象
	 */
	public User getUserById(int id)
	{
		Session session = SessionManager.getInstance();
		User user = session.read(User.class, id);
		return user;
	}

	/**
	 * 用户信息入库
	 * 
	 * @param user
	 *            用户信息对象
	 * @return boolean 成功(true)/失败(false)
	 */
	public boolean saveUser(User user)
	{
		Session session = SessionManager.getInstance();
		// 清空User实体对应的数据库表的记录
		session.clean(User.class);
		try
		{
			session.save(user);
			return true;
		} catch (JdbcException e)
		{
			LogManager.setLog("用户信息入库失败", e);
		}
		return false;
	}

	/**
	 * 根据用户ID删除用户
	 * 
	 * @param id
	 *            用户ID
	 */
	public boolean deleteUser(int id)
	{
		Session session = SessionManager.getInstance();
		User user = session.read(User.class, id);
		try
		{
			if (user != null)
			{
				session.delete(user);
				return true;
			}
		} catch (JdbcException e)
		{
			LogManager.setLog("根据用户ID删除用户失败", e);
		}
		return false;
	}

	/**
	 * 用户信息更新
	 * 
	 * @param user
	 *            用户信息对象
	 * @return boolean 成功(true)/失败(false)
	 */
	public boolean updateUser(User user)
	{
		Session session = SessionManager.getInstance();
		try
		{
			session.update(user);
			return true;
		} catch (JdbcException e)
		{
			LogManager.setLog("用户信息更新失败", e);
		}
		return false;
	}

	/**
	 * [批量入库]用户信息
	 * 
	 * @param userList
	 *            用户列表
	 * @return boolean 成功(true)/失败(false)
	 */
	public boolean batchSaveUser(List<User> userList)
	{
		Session session = SessionManager.getInstance();
		// 清空User实体对应的数据库表的记录
		session.clean(User.class);
		JdbcBatcher batcher = session.createBatcher();
		try
		{
			User user = null;
			for (int i = 0, j = userList.size(); i < j; i++)
			{
				user = userList.get(i);
				if (user != null)
				{
					batcher.save(user);
				}
			}
			batcher.execute();
			return true;
		} catch (JdbcException e)
		{
			LogManager.setLog("[批量入库]用户信息失败", e);
		}
		return false;
	}

	/**
	 * 用户分页查询(条件查询)
	 * 
	 * @param page
	 *            分页索引
	 * @param pageSize
	 *            每页条数
	 * @param sortType
	 *            排序类型
	 * @param sortName
	 *            排序字段
	 * @return List<User> 用户分页列表
	 */
	@SuppressWarnings(
	{ "rawtypes", "unchecked" })
	public List<User> getUserPage(Object[] params, int page, int pageSize, String sortType, String sortName)
	{
		Session session = SessionManager.getInstance();
		String sql = " select * from t_user where name = ? and id > ? ";
		SqlParams sqlParams = new SqlParams(sql, params);
		sqlParams.setObjectClass(User.class);
		// 设置查询条件
		// sqlParams.setParams(params);
		// sqlParams.setSql(sql);
		// 设置分页的查询索引
		int firstPageIndex = (page - 1) * pageSize;
		int maxPageIndex = page * pageSize;
		sqlParams.setFirstResult(firstPageIndex);
		sqlParams.setMaxResults(maxPageIndex);
		sqlParams.addOrder(sortType.equals("asc") ? Order.asc(sortName) : Order.desc(sortName));

		List<User> users = session.list(sqlParams);
		return users;
	}

	/**
	 * 用户分页查询
	 * 
	 * @param page
	 *            分页索引
	 * @param pageSize
	 *            每页条数
	 * @param sortType
	 *            排序类型
	 * @param sortName
	 *            排序字段
	 * @return List<User> 用户分页列表
	 */
	public List<User> getUserPage(int page, int pageSize, String sortType, String sortName)
	{
		Session session = SessionManager.getInstance();
		SqlParams<User> sqlParams = new SqlParams<User>();
		sqlParams.setObjectClass(User.class);
		// 设置分页的查询索引
		int firstPageIndex = (page - 1) * pageSize;
		int maxPageIndex = page * pageSize;
		sqlParams.setFirstResult(firstPageIndex);
		sqlParams.setMaxResults(maxPageIndex);

		// 添加排序类型和字段
		sqlParams.addOrder(sortType.equals("asc") ? Order.asc(sortName) : Order.desc(sortName));
		// 添加了排序类型和字段必须设置SQL语句，否则异常提示不能获取到Query对象
		sqlParams.setSql(" select * from t_user ");

		List<User> users = session.list(sqlParams);
		return users;
	}

	/**
	 * 内置分页数据对象
	 * 
	 * @param page
	 *            分页索引
	 * @param pageSize
	 *            显示条数
	 * @param totalCount
	 *            查询总数
	 * @param users
	 *            查询结果集
	 * @return Page<User> 分页数据对象
	 */
	public Page<User> getPage(int page, int pageSize, int totalCount, List<User> users)
	{
		Page<User> pageData = new Page<User>();
		pageData.setPageNo(page);
		pageData.setPageSize(pageSize);
		pageData.setTotalCount(totalCount);
		pageData.setResult(users);
		return pageData;
	}

}
```
**6、创建UserService实现IUserService**




```java
package com.boonya.jorm.service;

import java.util.List;

import com.boonya.jorm.dao.UserDao;
import com.boonya.jorm.entity.User;
/**
 * 
 * <li>文件名称: UserService</li>
 * <li>文件描述: $</li>
 * <li>内容摘要: 包括模块、函数及其功能的说明</li>
 * <li>完成日期：2013-10-27</li>
 * <li>所属作者：boonyachengdu@gmail.com</li>
 * <li>修改记录1：下午04:55:55,修改内容描述</li>
 *
 */
public class UserService implements IUserService {
	
	public static UserDao userDao=new UserDao();

	@Override
	public boolean add(User user) {
		return userDao.saveUser(user); 
	}
	
	@Override
	public boolean batchAdd(List<User> userList)
	{
		return userDao.batchSaveUser(userList);
	}

	@Override
	public boolean delete(int id) {
		return userDao.deleteUser(id);
	}

	@Override
	public User getUser(int id) {
		return userDao.getUserById(id);
	}

	@Override
	public boolean update(User user) {
		return userDao.updateUser(user);
	}

	@Override
	public List<User> getPagingList(Object [] params,int page, int pageSize, String sortType, String sortName)
	{
		return userDao.getUserPage(params,page, pageSize, sortType, sortName);
	}

}
```



**7、编写单元测试用例**



```java
package com.boonya.jorm.test;

import java.util.ArrayList;
import java.util.List;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import com.boonya.jorm.entity.User;
import com.boonya.jorm.service.IUserService;
import com.boonya.jorm.service.UserService;

public class UserTest
{
	private IUserService userService;

	@Before
	public void initInterface()
	{
		userService = new UserService();
	}

	@After
	public void freeSources()
	{
		userService = null;
	}

	@Test
	public void add()
	{
		User user = new User("boonya", "male", 23, "Java programmer");
		boolean success = userService.add(user);
		Assert.assertTrue("添加新的用户", success);
	}

	@Test
	public void bgetUser()
	{
		User user = userService.getUser(1);
		Assert.assertNull("用户ID=1查询", user);
	}

	@Test
	public void cupdate()
	{
		User user = userService.getUser(1);
		Assert.assertNotNull("存在ID=1用户", user);
		user.setCareer("Test case update " + System.currentTimeMillis());
		boolean success = userService.update(user);
		Assert.assertTrue("更新ID=1的用户", success);
	}

	@Test
	public void delete()
	{
		User user = userService.getUser(1);
		Assert.assertNotNull("存在ID=1用户", user);
		boolean success = userService.delete(1);
		Assert.assertTrue("删除ID=1的用户", success);
	}

	@Test
	public void fbatchAdd()
	{
		// 批量添加
		List<User> userList = new ArrayList<User>();
		User user;
		for (int i = 0; i < 100; i++)
		{
			user = new User("boonya", "male", 23, "Java programmer" + i);
			userList.add(user);
		}
		Assert.assertNotNull("fbatchAdd集合不为空", userList);
		boolean success = userService.batchAdd(userList);
		Assert.assertTrue("批量添加", success);
	}

	@Test
	public void gpaingQuery()
	{
		// 分页数据查询
		Object[] params = new Object[]
		{ "boonya100", "90" };
		List<User> users = userService.getPagingList(params, 1, 8, "asc", "id");
		Assert.assertNotNull("gpaingQuery集合不为空", users);
		User user;
		for (int i = 0, j = users.size(); i < j; i++)
		{
			user = users.get(i);
			System.out.println(user == null ? "" : user.toString());
		}
	}

}
```
注：此处的Page<T>分页只是最简单的一种封装方式，后面需要详细地了解，另外整个数据访问只包含一个数据库session访问，所以不用关闭该session，若是单个的session需要用完即对其进行关闭，释放资源占用。






