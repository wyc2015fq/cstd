# hibernate中的一对一双向主键关联的配置以及注意事项 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月13日 20:14:00[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：43标签：[hibernate																[一对一双向关联](https://so.csdn.net/so/search/s.do?q=一对一双向关联&t=blog)](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)
个人分类：[框架](https://blog.csdn.net/weixin_40247263/article/category/7511186)


```java
package hibernate.pojo;

import java.io.Serializable;

public class EmpVo{

	private int empid;
	private String empname;
	private String cardno;
	private String sex;
	private int age;
	private String nation;
	private String nationName;
	
	//一对一
	private AccountVo account;
	
	
	
	public EmpVo() {
		super();
		// TODO Auto-generated constructor stub
	}
	public EmpVo(int empid, String empname, String cardno, String sex, int age, String nation, String nationName,
			AccountVo account) {
		super();
		this.empid = empid;
		this.empname = empname;
		this.cardno = cardno;
		this.sex = sex;
		this.age = age;
		this.nation = nation;
		this.nationName = nationName;
		this.account = account;
	}
	public AccountVo getAccount() {
		return account;
	}
	public void setAccount(AccountVo account) {
		this.account = account;
	}
	public String getNationName() {
		return nationName;
	}
	public void setNationName(String nationName) {
		this.nationName = nationName;
	}
	public String getNation() {
		return nation;
	}
	public void setNation(String nation) {
		this.nation = nation;
	}
	public int getEmpid() {
		return empid;
	}
	public void setEmpid(int empid) {
		this.empid = empid;
	}
	public String getEmpname() {
		return empname;
	}
	public void setEmpname(String empname) {
		this.empname = empname;
	}

	public String getCardno() {
		return cardno;
	}
	public void setCardno(String cardno) {
		this.cardno = cardno;
	}
	public String getSex() {
		return sex;
	}
	public void setSex(String sex) {
		this.sex = sex;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	@Override
	public String toString() {
		return "EmpVo [empid=" + empid + ", empname=" + empname + ", cardno=" + cardno + ", sex=" + sex + ", age=" + age
				+ ", nation=" + nation + ", nationName=" + nationName + ", account=" + account + "]";
	}

	
	
}
```

```java
package hibernate.pojo;

import java.io.Serializable;

public class AccountVo implements Serializable{

	private int empid;
	private String username;
	private String password;
	private int status;
	
	//一对一	
	private EmpVo emp;
	
	
	public AccountVo() {
		super();
		// TODO Auto-generated constructor stub
	}
	public AccountVo(int empid, String username, String password, int status, EmpVo emp) {
		super();
		this.empid = empid;
		this.username = username;
		this.password = password;
		this.status = status;
		this.emp = emp;
	}
	public EmpVo getEmp() {
		return emp;
	}
	public void setEmp(EmpVo emp) {
		this.emp = emp;
	}
	public int getEmpid() {
		return empid;
	}
	public void setEmpid(int empid) {
		this.empid = empid;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	@Override
	public String toString() {
		return "AccountVo [empid=" + empid + ", username=" + username + ", password=" + password + ", status=" + status
				+ ", emp=" + emp + "]";
	}

	
	
}
```

```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE hibernate-mapping PUBLIC 
    "-//Hibernate/Hibernate Mapping DTD 3.0//EN"
    "http://www.hibernate.org/dtd/hibernate-mapping-3.0.dtd">
<hibernate-mapping>

    <class name="hibernate.pojo.AccountVo" table="account" >
		<id name="empid" type="int">
			<!-- assigned制定主键的值，native数据库自动赋值 -->
			<generator class="foreign">
				<!-- accout表的主键id是emp这个表的主键id的外键 -->
				<param name="property">emp</param>
			</generator>
		</id>
		<property name="username" type="string">
			<column name="username" length="10">
				<comment>用户名称</comment>
			</column>
		</property>
		<property name="password" type="string">
			<column name="password" length="18">
				<comment>密码</comment>
			</column>		
		</property>
		<property name="status" type="int"></property>
		<!-- 一对一配置 -->
		<one-to-one name="emp" class="hibernate.pojo.EmpVo"></one-to-one>		
    </class>
</hibernate-mapping>
```

```java
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE hibernate-mapping PUBLIC 
    "-//Hibernate/Hibernate Mapping DTD 3.0//EN"
    "http://www.hibernate.org/dtd/hibernate-mapping-3.0.dtd">
<hibernate-mapping>
    
    <!-- 
        name：即实体类的全名
        table：映射到数据库里面的那个表的名称
        catalog：数据库的名称
     -->
    <class name="hibernate.pojo.EmpVo" table="employee">
        
        <!-- 
        	 name:类中的成员变量名（对应数据库中的主键）
        	 column:数据库中的主键的字段
        	 type : 可以用hibernate自己的，也可以用java的
        -->
        <id name="empid"  type="int"> <!-- java数据类型 -->
	        <!-- 
	        	 name:类中的成员变量名（对应数据库中的主键）
	        -->
             <column name="empid">
				 <!-- 对该字段的中文描述，就是给别人看的，对程序无影响 -->
				 <comment>员工名称</comment>             
             </column>
            <!-- 主键生成策略
            	 assigned自己给主键赋值
            	 native 自动生成
            	 foreign外键机制生成主键的值
             -->
            <generator class="native"></generator>
        </id>
        <!-- 
		            使用property来描述属性与字段的对应关系
		            如果length忽略不写，且你的表是自动创建这种方案，那么length的默认长度是255
		         如果javabean中的成员变量名和表中的字段名称一致，则colum可以省略    
        -->
        <property name="empname" column="empname" length="20" type="string" not-null="false"></property> 
        <property name="cardno">
            <column name="cardno" length="18" sql-type="varchar(18)" unique="true"></column> <!-- SQL的数据类型 -->
        </property>
        <property name="sex" length="2" type="string" not-null="false"></property>
    	<property name="age" length="11" type="int" ></property>
        <property name="nation" length="20" type="string"></property>
  
  		<!-- 一对一配置 -->
    	<one-to-one name="account" class="hibernate.pojo.AccountVo"></one-to-one>
    
    </class>
</hibernate-mapping>
```

```java
package hibernate.db;

import java.io.Serializable;

import org.hibernate.Session;
import org.hibernate.Transaction;

import hibernate.HibernateSessionFactory;

public class HBaseDao {

	public void save(Object bean) {
		
		Session session = HibernateSessionFactory.getSession();
		Transaction tx = session.getTransaction();
		try {
			tx.begin();
			session.save(bean);
			tx.commit();
		} catch (Exception e) {
			tx.rollback();
			e.printStackTrace();
		}
		
	}
	

	
	
	public void update(Object bean) {
		
		Session session = HibernateSessionFactory.getSession();
		Transaction tx = session.getTransaction();
		try {
			tx.begin();
			session.update(bean);
			tx.commit();
		} catch (Exception e) {
			tx.rollback();
			e.printStackTrace();
		}
		
	}
	
	
	public void del(Object bean) {
		
		Session session = HibernateSessionFactory.getSession();
		Transaction tx = session.getTransaction();
		try {
			tx.begin();
			session.delete(bean);
			tx.commit();
		} catch (Exception e) {
			tx.rollback();
			e.printStackTrace();
		}
		
	}
	
	// 根据主键查询函数
	public<T> T get(Class<T> cls,Serializable id) {
		T result = null;
		Session session = HibernateSessionFactory.getSession();
		Transaction tx = session.getTransaction();
		try {
			tx.begin();
			result = session.get(cls, id);
			tx.commit();
		} catch (Exception e) {
			tx.rollback();
			e.printStackTrace();
		}
		return result;
		
	}
	
	
}
```

```java
package hibernate;

import org.hibernate.Session;
import org.hibernate.Transaction;

import hibernate.db.HBaseDao;
import hibernate.pojo.AccountVo;
import hibernate.pojo.EmpVo;

public class Tgenerate {

	public static void main(String[] args) {

		HBaseDao base = new HBaseDao();
		EmpVo emp = new EmpVo();
		AccountVo acc = new AccountVo();
		acc.setEmpid(27);
/*		emp.setEmpname("张三");
		emp.setCardno("123456789123456789");
		emp.setSex("男");
		emp.setNation("1");
		AccountVo acc = new AccountVo();
		acc.setUsername("张三");
		acc.setPassword("1234");
		acc.setStatus(0);*/
		emp.setEmpid(27);
		//一对一关联的实体设置--要把emp设置到acc对象中
		//acc.setEmp(emp);
		//这条可省略
		//emp.setAccount(acc);
		
		EmpVo emplist = (EmpVo)base.get(emp.getClass(), emp.getEmpid());
		System.err.println(emplist);
		
		AccountVo acclist = (AccountVo)base.get(acc.getClass(), acc.getEmpid());
		System.err.println(acclist);
		
	}
}
```

解释：EmpVo对应emp表、AccountVo对应account表，其中account表的主键是empid，emp表的主键也是empid，account表的empid是emp表的empid的外键。

测试结论：查询emp表将account表结果带出来  和 查询account表 把emp表带出来的区别 就是 查谁谁的数据会被显示全。言语不好说清楚，直接看hibernate执行的sql语句。

查 account表 将emp带出来的语句

```
Hibernate: 
    select
        accountvo0_.empid as empid1_0_0_,
        accountvo0_.username as username2_0_0_,
        accountvo0_.password as password3_0_0_,
        accountvo0_.status as status4_0_0_,
        empvo1_.empid as empid1_1_1_,
        empvo1_.empname as empname2_1_1_,
        empvo1_.cardno as cardno3_1_1_,
        empvo1_.sex as sex4_1_1_,
        empvo1_.age as age5_1_1_,
        empvo1_.nation as nation6_1_1_ 
    from
        account accountvo0_ 
    left outer join
        employee empvo1_ 
            on accountvo0_.empid=empvo1_.empid 
    where
        accountvo0_.empid=?
```

查emp表将 account表带出来的语句

```
Hibernate: 
    select
        empvo0_.empid as empid1_1_0_,
        empvo0_.empname as empname2_1_0_,
        empvo0_.cardno as cardno3_1_0_,
        empvo0_.sex as sex4_1_0_,
        empvo0_.age as age5_1_0_,
        empvo0_.nation as nation6_1_0_,
        accountvo1_.empid as empid1_0_1_,
        accountvo1_.username as username2_0_1_,
        accountvo1_.password as password3_0_1_,
        accountvo1_.status as status4_0_1_ 
    from
        employee empvo0_ 
    left outer join
        account accountvo1_ 
            on empvo0_.empid=accountvo1_.empid 
    where
        empvo0_.empid=?
```

注意：我的pojo写的有问题，我的两个pojo为了打印看结果，都写了toString 方法，这就造成了输出 emp的时候会调用toString方法，toString方法里面将account输出，输出的时候又会调用account的toString方法，最终造成StackOverflowError。如果想做这个测试需要对自动生成的toString方法做修改，调用pojo的get方法来获得变量值，然后拼接输出。

