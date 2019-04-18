# Hibernate 一对多注解 实例 - z69183787的专栏 - CSDN博客
2014年02月27日 15:21:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：27062
student：
```java
/**
 * 
 */
package com.wonders.task.sample.bo;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import org.hibernate.annotations.GenericGenerator;
/** 
 * @ClassName: Student 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月27日 上午11:48:30 
 *  
 */
@Entity 
@Table(name="Z_STUDENT") 
public class Student {
	private String id;
	private String name;
	private Clazz clazz;
	
	
	@Id
	@GeneratedValue(generator="system-uuid")
	@GenericGenerator(name="system-uuid", strategy = "uuid")
	@Column(name = "ID")
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	
	@Column(name = "NAME", length = 500)
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	@ManyToOne(cascade={CascadeType.ALL})         
  	@JoinColumn(name="class_id")     //student类中对应外键的属性：classid 
	public Clazz getClazz() {
		return clazz;
	}
	public void setClazz(Clazz clazz) {
		this.clazz = clazz;
	}
	
	
}
```
classes:
```java
/**
 * 
 */
package com.wonders.task.sample.bo;
import java.util.Set;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import org.hibernate.annotations.GenericGenerator;
/** 
 * @ClassName: Clazz 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月27日 上午11:48:47 
 *  
 */
@Entity 
@Table(name="Z_CLASS") 
public class Clazz implements java.io.Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = -6224738252966513441L;
	private String id;
	private String name;
	private Set<Student> set;
	
	@Id
	@GeneratedValue(generator="system-uuid")
	@GenericGenerator(name="system-uuid", strategy = "uuid")
	@Column(name = "ID")
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	
	@Column(name = "NAME", length = 500)
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	@OneToMany(cascade=CascadeType.ALL,mappedBy="clazz") 
       //@OneToMany(cascade=CascadeType.ALL,mappedBy="clazz",fetch=FetchType.EAGER) 关闭延迟加载。        
	public Set<Student> getSet() {
		return set;
	}
	public void setSet(Set<Student> set) {
		this.set = set;
	}
	
	
	
}
```
main:
```java
/**
 * 
 */
package com.wonders.task.sample.main;
import java.util.HashSet;
import java.util.Set;
import javax.annotation.Resource;
import org.hibernate.Hibernate;
import org.hibernate.Session;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.Scope;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import com.wonders.schedule.util.SpringBeanUtil;
import com.wonders.task.sample.ITaskService;
import com.wonders.task.sample.bo.Clazz;
import com.wonders.task.sample.bo.Student;
/** 
 * @ClassName: TestMain 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月27日 下午12:18:44 
 *  
 */
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
/* 功能模块入口点，beanid即数据库配置中的name */
@Service("testMain")
@Scope("prototype")
public class TestMain {
	private HibernateTemplate hibernateTemplate;
	
	public HibernateTemplate getHibernateTemplate() {
		return hibernateTemplate;
	}
	
	//注入hibernateTemplate
	@Resource(name="hibernateTemplate")
	public void setHibernateTemplate(HibernateTemplate hibernateTemplate) {
		this.hibernateTemplate = hibernateTemplate;
	}
	
	public void test(){
		Session session = this.getHibernateTemplate().getSessionFactory().getCurrentSession();
		Clazz c2 = (Clazz) session.get(Clazz.class, "8a81a97c4471c661014471c669360002");
		//Clazz c2 = (Clazz) main.getHibernateTemplate().get(Clazz.class, "8a81a97c4471c661014471c669360002");
		Hibernate.initialize(c2.getSet());
		
		System.out.println("11111111111111111111111");
		//session.close();
		Set<Student> sss =  c2.getSet();
		System.out.println("22222222222222222");
		for(Student s: sss){
			
			System.out.println(s.getName());
		}
	}
	
	public static void main(String[] args){
		ApplicationContext applicationContext = null;  
		String[] fileUrl = new String[]{"classpath*:applicationContext*.xml"};  
		applicationContext = new ClassPathXmlApplicationContext(fileUrl);  
		
		Student s1 = new Student();
		Student s2 = new Student();
		Clazz c1 = new Clazz();
		c1.setName("1班");
		s1.setName("z1");
		s1.setClazz(c1);
		s2.setName("z2");
		s2.setClazz(c1);
		Set<Student> set = new HashSet<Student>();
		set.add(s1);set.add(s2);
		c1.setSet(set);
		TestMain main = (TestMain)SpringBeanUtil.getBean("testMain");
		//main.getHibernateTemplate().save(c1);
		
//		Session session = main.getHibernateTemplate().getSessionFactory().openSession();
//		Clazz c2 = (Clazz) session.get(Clazz.class, "8a81a97c4471c661014471c669360002");
//		//Clazz c2 = (Clazz) main.getHibernateTemplate().get(Clazz.class, "8a81a97c4471c661014471c669360002");
//		Hibernate.initialize(c2.getSet());
//		
//		System.out.println("11111111111111111111111");
//		session.close();
//		Set<Student> sss =  c2.getSet();
//		System.out.println("22222222222222222");
//		for(Student s: sss){
//			
//			System.out.println(s.getName());
//		}
		main.test();
		//main.getHibernateTemplate().delete(c2);
	}
}
```
级联保存
方法1：自动维护两表关系，mappedby 为被拥维护方，即对应的student为维护方
```java
Student s1 = new Student();
		Student s2 = new Student();
		Clazz c1 = new Clazz();
		c1.setName("1班");
		s1.setName("z1");
		s1.setClazz(c1);
		s2.setName("z2");
		s2.setClazz(c1);
		TestMain main = (TestMain)SpringBeanUtil.getBean("testMain");
		main.getHibernateTemplate().save(s1);
		main.getHibernateTemplate().save(s2);
```
方法2：手动维护，两方都需要set对应对方的变量
```java
Student s1 = new Student();
		Student s2 = new Student();
		Clazz c1 = new Clazz();
		c1.setName("1班");
		s1.setName("z1");
		s1.setClazz(c1);
		s2.setName("z2");
		s2.setClazz(c1);
		Set<Student> set = new HashSet<Student>();	
		set.add(s1);set.add(s2);
		c1.setSet(set);
		TestMain main = (TestMain)SpringBeanUtil.getBean("testMain");
		main.getHibernateTemplate().save(c1);
```
关于延迟加载：
[http://blog.csdn.net/z69183787/article/details/20049409](http://blog.csdn.net/z69183787/article/details/20049409)
[http://blog.csdn.net/z69183787/article/details/17136627](http://blog.csdn.net/z69183787/article/details/17136627)
关于mappedby关键字 及一对多 更多的解释：
[http://blog.csdn.net/z69183787/article/details/8197106](http://blog.csdn.net/z69183787/article/details/8197106)
[http://blog.csdn.net/z69183787/article/details/18615577](http://blog.csdn.net/z69183787/article/details/18615577)
