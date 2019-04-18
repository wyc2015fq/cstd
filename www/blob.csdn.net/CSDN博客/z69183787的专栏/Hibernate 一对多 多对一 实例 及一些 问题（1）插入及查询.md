# Hibernate 一对多 多对一 实例 及一些 问题（1）插入及查询 - z69183787的专栏 - CSDN博客
2014年04月03日 20:11:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2724
 Clazz.java
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
import javax.persistence.OneToOne;
import javax.persistence.Table;
import javax.persistence.Version;
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
	
	private int version;
	private String id;
	private String name;
	private Set<Student> set;
	private Teacher teacher;
	
	 @Version
	    @Column(name = "version")
	public int getVersion() {
		return version;
	}
	public void setVersion(int version) {
		this.version = version;
	}
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
	//@OneToMany(cascade={CascadeType.PERSIST,CascadeType.MERGE,CascadeType.REFRESH},mappedBy="clazz") 
	public Set<Student> getSet() {
		return set;
	}
	public void setSet(Set<Student> set) {
		this.set = set;
	}
	
	@OneToOne(cascade=CascadeType.ALL,mappedBy="clazz") 
	public Teacher getTeacher() {
		return teacher;
	}
	public void setTeacher(Teacher teacher) {
		this.teacher = teacher;
	}
	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	
	
	
}
```
Student.java
```java
/**
 * 
 */
package com.wonders.task.sample.bo;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import javax.persistence.Transient;
import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.GenericGenerator;
import org.hibernate.annotations.Where;
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
	private String classId;
	
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
	
	//@ManyToOne(cascade={CascadeType.PERSIST,CascadeType.MERGE}) 
	@ManyToOne(cascade={CascadeType.ALL},fetch=FetchType.LAZY) 
	@JoinColumn(name="class_id")
  	//@ManyToOne
  	//@Cascade(value=org.hibernate.annotations.CascadeType.SAVE_UPDATE)
	//@JoinColumn(name="class_id" ,nullable = true,insertable=false,updatable=false)     //student类中对应外键的属性：classid 
	//@Where(clause = "removed=0")
	public Clazz getClazz() {
		return clazz;
	}
	public void setClazz(Clazz clazz) {
		this.clazz = clazz;
	}
	
	@Transient
	@Column(name = "CLASS_ID", length = 50)
	public String getClassId() {
		return classId;
	}
	public void setClassId(String classId) {
		this.classId = classId;
	}
	
	
}
```
测试类：
```java
/**
 * 
 */
package com.wonders.task.sample.main;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.annotation.Resource;
import org.hibernate.Hibernate;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
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
import com.wonders.task.sample.bo.Teacher;
/** 
 * @ClassName: TestMain 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月27日 下午12:18:44 
 *  
 */
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
/* 功能模块入口点，beanid即数据库配置中的name */
@Service("execMain")
@Scope("prototype")
public class ExecMain {
	private HibernateTemplate hibernateTemplate;
	
	public HibernateTemplate getHibernateTemplate() {
		return hibernateTemplate;
	}
	
	//注入hibernateTemplate
	@Resource(name="hibernateTemplate")
	public void setHibernateTemplate(HibernateTemplate hibernateTemplate) {
		this.hibernateTemplate = hibernateTemplate;
	}
	
	private static SessionFactory sf;
	
	
	
	public static SessionFactory getSf() {
		return sf;
	}
	@Resource(name="sessionFactory")
	public void setSf(SessionFactory sf) {
		this.sf = sf;
	}
	public static void main(String[] args){
		ApplicationContext applicationContext = null;  
		String[] fileUrl = new String[]{"classpath*:applicationContext*.xml"};  
		applicationContext = new ClassPathXmlApplicationContext(fileUrl);  		
		ExecMain main = (ExecMain)SpringBeanUtil.getBean("execMain");
		
		//1  多方插入
//		Clazz c = new Clazz();
//		c.setName("class");
//		Student s = new Student();
//		s.setName("student");
//		s.setClazz(c);
//		main.getHibernateTemplate().save(s);
		
		//2一方插入
//		Clazz c = new Clazz();
//		c.setName("class2");
//		Student s1 = new Student();
//		s1.setName("student1");
//		Student s2 = new Student();
//		s2.setName("student2");
//		s1.setClazz(c);s2.setClazz(c);
//		Set<Student> set = new HashSet<Student>();
//		set.add(s1);set.add(s2);
//		c.setSet(set);
//		main.getHibernateTemplate().save(c);
		
		//3一方查找 lazy 就报错 因为 延迟加载 一方 fetchType 默认lazy
		//解决方法：lazy改成eager  或者hql left join fetch
		//Clazz c = main.getHibernateTemplate().get(Clazz.class, "8a81a97c452741d701452741de480002");
//		String hql = "from Clazz c left join fetch c.set";
//		Session session = main.getHibernateTemplate().getSessionFactory().
//				openSession();
//		Clazz c = (Clazz) session.createQuery(hql).uniqueResult();
//		//Clazz c = (Clazz) session.get(Clazz.class, "8a81a97c452741d701452741de480002");
//		session.close();
//		System.out.println(c.getName());
//		System.out.println(c.getSet().size());
		
		//4多方查找 lazy 就报错 因为 延迟加载 多方 fetchType 默认 eager
		//解决方法：lazy改成eager  或者hql left join fetch
		//Student s = main.getHibernateTemplate().get(Student.class, "8a81a97c452741d701452741df130003");
//		String hql = "from Student s left join fetch s.clazz where s.id = '8a81a97c452741d701452741df130003'";
//		Session session = main.getHibernateTemplate().getSessionFactory().
//				openSession();
//		Student s = (Student) session.createQuery(hql).uniqueResult();
//		System.out.println(s.getClazz().getName());
	
		
		
		
		
	}
}
```
