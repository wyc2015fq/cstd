# Spring整合JUnit实现测试用例 - 零度的博客专栏 - CSDN博客
2016年04月06日 14:11:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：5898
通过Spring进行对象管理之后，做测试变得复杂了。因为所有的Bean都需要在applicationContext.xml中加载好，之后再通过@Resource去取得。如果每次都要整个业务流做的差不多了再去测试，这样效率很低，也很麻烦。如果单独去写一个测试用类，这样太繁琐了。于是想起Spring有一个测试框架，能够整合JUnit进行测试。
## 1. 加入依赖包
　　使用Spring的测试框架需要加入以下依赖包：
- JUnit 4 （官方下载：[http://www.junit.org/](http://www.junit.org/)）
- Spring Test （Spring框架中的test包）
- Spring 相关其他依赖包（不再赘述了，就是context等包）
## 2. 创建测试源目录和包
在此，推荐创建一个和src平级的源文件目录，因为src内的类都是为日后产品准备的，而此处的类仅仅用于测试。而包的名称可以和src中的目录同名，这样由于在test源目录中，所以不会有冲突，而且名称又一模一样，更方便检索。
## 3. 创建测试类
创建一个测试用的类，推荐名称为 “被测试类名称 + Test”。
测试类应该继承与 AbstractJUnit4SpringContextTests 或 AbstractTransactionalJUnit4SpringContextTests，注：高版本已经不需要继承了
对于 AbstractJUnit4springcontextTests 和 AbstractTransactionalJUnit4SpringContextTests 类的选择：
　　如果再你的测试类中，需要用到事务管理（比如要在测试结果出来之后回滚测试内容），就可以使用AbstractTransactionalJUnit4SpringTests类。事务管理的使用方法和正常使用Spring事务管理是一样的。再此需要注意的是，如果想要使用声明式事务管理，即使用AbstractTransactionalJUnitSpringContextTests类，请在applicationContext.xml文件中加入transactionManager
 bean：
```
<bean id="transactionManager"
class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
    <property name="dataSource" ref="dataSource" />
</bean>
```
　如果没有添加上述bean，将会抛出NoSuchBeanDefinitionException，指明
 No bean named 'transactionManager' is definded.
## 4. 配置测试类
添加如下内容在class前，用于配置applicationContext.xml文件的位置。
```
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
```
## 5. 创建测试方法
创建测试用方法，推荐名称为 “被测方法名称+ Test”。
测试方法上方加入 @Test
## 6. 通过JUnit 4 执行
右键方法名，选择则“Run As”→“JUnit Test”即可
示例代码如下：
```java
package org.nercita.bcp.test;
import java.util.ArrayList;
import java.util.List;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.nercita.bcp.system.dao.UserDao;
import org.nercita.bcp.system.domain.User;
import org.nercita.bcp.system.domain.UserGroup;
import org.nercita.bcp.system.domain.vo.ReturnResult;
import org.nercita.bcp.system.service.UserGroupService;
import org.nercita.bcp.system.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.transaction.TransactionConfiguration;
import org.springframework.transaction.annotation.Transactional;
import com.alibaba.fastjson.JSON;
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:applicationContext.xml","classpath:applicationContext-security.xml","classpath:config/webmvc-config.xml"})
@TransactionConfiguration(transactionManager="transactionManager",defaultRollback=false)
@Transactional
public class UserServiceTest {
	@Autowired
	private UserService userService;
	
	@Autowired
	private UserDao userDao;	
		
	@Autowired
	private UserGroupService userGroupService;
	
	@Test
	public void testGetUser(){
		
		User user = userService.getById("40288e9350a2f9780150a2fa0b930002");
	    System.out.println(user);
	}
	
	@Test
	public void testupdateUser(){
		
		User user = userService.getById("4028f5815156dc4b01515734c8630023");
		user.setActivationCode(null);
		userService.update(user);
	    
	}
	
	
	
	@Test
	public void testApiLogin(){
		User user = userService.getById("40288e9350a2f9780150a2fa0b930001");
		ReturnResult returnResult = userService.apiLogin(user.getName(),user.getPassword());		
		System.out.println(JSON.toJSONString(returnResult));  
		
	}
	
	@Test
	public void testDeleteUserbyId(){
		
		User user = userService.getById("40288e93519e382001519e57ee350001");
		UserGroup userGroup = user.getUserGroup();
		if(userGroup !=null){
			userGroup.setTeamAdmin(null);
			userGroupService.update(userGroup);
		}		
		userService.delete(user);	  
		
	}
	
	
	@Test
	public void testDeleteUserbyActivationCode(){
		String activationCode = "b8567d5416c74fc6bb60fedf411af6a6";
		User user = userService.findUniqueByProperty("activationCode", activationCode);	
		userService.delete(user);			  
	}
	
	@Test
	public void testdeleteNoActivatedUsers(){
		    userService.deleteNoActivatedUsers();
	}
	
	
	
	
	
	@Test
	public void testQueryUsers(){
		
		List<User> users = userService.findAllEq("name", "xxxxxxxxxxx");
		System.out.println(users);
		Assert.assertEquals(users, new ArrayList<User>());
		  
		
	}
	
	/**
	 * 统计一个团队 每个作物对应的用户数量
	 * @param teamAdmin
	 */
	@SuppressWarnings("unchecked")
	@Test
	public void testQuery2Users(){
		//String sql =  "select count(*) FROM User u  inner join  u.cropSet as crop where u.userGroup.teamAdmin.id = '4028f5815156dc4b01515734c8630023' and  crop.id='1001'";
		String sql =  "select crop.id, count(*) FROM User u  inner join  u.cropSet as crop where u.userGroup.teamAdmin.id = '4028f5815156dc4b01515734c8630023' group by  crop.id";
		List<Object[]>  ls = userDao.getSession().createQuery(sql).list();		
		System.out.println(ls);
	
	}
	
	
	/**
	 * 统计一个团队 每个作物对应的用户数量
	 * @param teamAdmin
	 */
	@SuppressWarnings("unchecked")
	@Test
	public void testQuery3Users(){
		String hql =  "select crop.id, count(*) FROM User u  inner join  u.cropSet as crop where crop.id is not in ('1001','1002') and u.userGroup.teamAdmin.id = '4028f5815156dc4b01515734c8630023' group by  crop.id ";
		List<Object[]>  ls = userDao.getSession().createQuery(hql).list();		
		System.out.println(ls);
	
	}
	
	
	
	/**
	 * 统计一个团队 每个作物对应的用户数量
	 * @param teamAdmin
	 */
	@Test
	public void testQuery4UserGroups(){
		List<UserGroup> list = new ArrayList<UserGroup>();
		System.out.println(iterQueryUserGroups(list,"40288e8c52149d09015214b824c60056"));
		/*
		String hql =  "FROM  UserGroup u where u.parentUserGroup.id = '40288e8c52149d09015214b824c60056'";
		List<UserGroup>  ls = userDao.getSession().createQuery(hql).list();	
		System.out.println(ls);
		*/
	    
	}
	
	
	
	
	/**
	 * 根据父用户组Id获取所有子用户组
	 * @param teamAdmin
	 */
	@SuppressWarnings("unchecked")
	public List<UserGroup> iterQueryUserGroups(List<UserGroup> list, String fatherGroupId){
		String hql =  "FROM  UserGroup u where u.parentUserGroup.id = '"+fatherGroupId+"'";
		List<UserGroup>  ls = userDao.getSession().createQuery(hql).list();	
		if(ls !=null && ls.size()>0){
			list.addAll(ls);
			for(UserGroup ug : ls){
				iterQueryUserGroups(list,ug.getId());
			}
		}
		return list;
	
	}
	
	
	
	
	
	
	
	
	
}
```
