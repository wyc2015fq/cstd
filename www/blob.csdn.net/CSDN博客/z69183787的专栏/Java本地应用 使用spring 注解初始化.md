# Java本地应用 使用spring 注解初始化 - z69183787的专栏 - CSDN博客
2012年12月12日 10:53:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7681
service 类 及 接口 
ISample.java
```java
/**
 * 
 */
package test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
/** 
 * @ClassName: Test 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2012-12-12 上午10:13:51 
 *  
 */
public class Test {
	public static void main(String[] args){
		ApplicationContext applicationContext = null;  
		String[] fileUrl = new String[]{"classpath*:*Context*.xml"};  
		applicationContext = new ClassPathXmlApplicationContext(fileUrl);  
		//applicationContext = new FileSystemXmlApplicationContext(fileUrl); 
		ISample s = (ISample)SpringBeanUtil.getBean("sampleService");
		s.test();
	}
}
```
SampleService.java
```java
/**
 * 
 */
package test;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Scope;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
/** 
 * @ClassName: SampleService 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2012-12-12 上午10:10:38 
 *  
 */
@Transactional(propagation=Propagation.REQUIRED,rollbackFor=Exception.class)
@Service("sampleService")
@Scope("prototype")
public class SampleService implements ISample{
	
	private HibernateTemplate h;
	
	public void test() {
		List list = this.h.getSessionFactory().getCurrentSession().createSQLQuery("select * from cs_user").list();
		System.out.println("----------------------------------------");
		System.out.println(list.size());
	}
	public HibernateTemplate getH() {
		return h;
	}
	@Autowired(required=false)
	public void setH(@Qualifier(value="hibernateTemplate")HibernateTemplate h) {
		this.h = h;
	}
	
	
}
```
SpringBeanUtil.java 工具类
```java
package test;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import javax.sql.DataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.SessionFactory;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.core.io.Resource;
import org.springframework.stereotype.Component;
/** 
* @ClassName: SpringBeanUtil 
* @Description: TODO(spring功能类，用于获取bean) 
* @author zhoushun
* @date 2012-11-27 下午04:22:36 
*  
*/ 
@Component("springBeanUtil")//使用注解
public class SpringBeanUtil implements ApplicationContextAware {
	protected final static Log logger = LogFactory.getLog(SpringBeanUtil.class);
	
	private static ApplicationContext ctx = null;
	
	private static Map<String, Properties> propMap = new HashMap<String, Properties>(0);
	
	public void setApplicationContext(ApplicationContext ctx)
			throws BeansException {
		SpringBeanUtil.ctx = ctx;
	}
	
	public static Object getBean(String prop) {
		Object obj = ctx.getBean(prop);
		if (logger.isDebugEnabled()) {
			logger.debug("property=[" + prop + "],object=[" + obj + "]");
		}
		return obj;
	}
	public static Properties getProperties(String filepath) {
		if (propMap.containsKey(filepath)) return propMap.get(filepath);
		
		Resource resource = ctx.getResource(filepath);
		Properties prop = new Properties();
		try {
			prop.load(resource.getInputStream());
			propMap.put(filepath, prop);
			return prop;
		} catch (IOException e) {
			logger.error("can not find the resource file:[" + filepath + "]", e);
			return null;
		}
	}
	public static DataSource getDataSource(String source) {
		return (DataSource) getBean(source);
	}
	
	public static SessionFactory getSessionFactory() {
		return (SessionFactory) getBean("sessionFactory");
	}
}
```
Test.java Main类
```java
/**
 * 
 */
package test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
/** 
 * @ClassName: Test 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2012-12-12 上午10:13:51 
 *  
 */
public class Test {
	public static void main(String[] args){
		ApplicationContext applicationContext = null;  
		String[] fileUrl = new String[]{"classpath*:*Context*.xml"};  
		applicationContext = new ClassPathXmlApplicationContext(fileUrl);  //初始化 applicationContext
		//applicationContext = new FileSystemXmlApplicationContext(fileUrl); 
		ISample s = (ISample)SpringBeanUtil.getBean("sampleService");
		s.test();
	}
}
```
applicationContext.xml
```java
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:context="http://www.springframework.org/schema/context" 
	xmlns:aop="http://www.springframework.org/schema/aop"
	xmlns:tx="http://www.springframework.org/schema/tx"
	xsi:schemaLocation="
	http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
	http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
	http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd">
	
	<!-- =================================================================== -->
	<!-- Context Define                                                      -->
	<!-- =================================================================== -->
	<bean id="propertyConfigurer"
		class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
		<property name="locations">
			<list>
				<value>classpath:database.properties</value>
			</list>
		</property>
	</bean>
		
	<context:component-scan base-package="test" />
	
	<bean 
		id="dataSource" 
		class="org.springframework.jdbc.datasource.DriverManagerDataSource">
		<property name="driverClassName" value="${hibernate.connection.driver_class}"/>
		<property name="url" value="${hibernate.connection.url}"/>
		<property name="username" value="${hibernate.connection.username}"/>
		<property name="password" value="${hibernate.connection.password}"/>
	</bean>
	<!-- =================================================================== -->
	<!-- SessionFactory(For Hibernate)/ManagerFactory(For JPA) Define        -->
	<!--  ONLY ONE OF THEM SHOULD BE USED                                    -->
	<!-- =================================================================== -->
	<!--  class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean" spring 2.5之后继承的工厂类 -->
	
	<bean id="sessionFactory"
		class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
		<property name="dataSource" ref="dataSource" />
		<property name="configLocation"
			value="classpath:hibernate.cfg.xml" />
			<property name="packagesToScan"><!-- 自动扫描注解 -->
		    <list>
		        <value>com.wonders.*</value>
		    </list>
			</property>
		<property name="configurationClass"  value="org.hibernate.cfg.AnnotationConfiguration"></property>
		<property name="lobHandler" ref="${jdbc.handler}" />
	</bean>
	<bean id="hibernateTemplate" class="org.springframework.orm.hibernate3.HibernateTemplate">
		<property name="sessionFactory" ref="sessionFactory"></property>
	</bean>
	
	<bean id = "jdbcTemplate" class = "org.springframework.jdbc.core.JdbcTemplate">
	    <property name = "dataSource" ref="dataSource"/>
	</bean>
	
	<bean id="oracleLobHandler" class="org.springframework.jdbc.support.lob.OracleLobHandler">
		<property name="nativeJdbcExtractor" ref="nativeJdbcExtractor" />
	</bean>
	<bean id="defaultLobHandler" class="org.springframework.jdbc.support.lob.DefaultLobHandler">
	</bean>
	<bean id="nativeJdbcExtractor" class="org.springframework.jdbc.support.nativejdbc.SimpleNativeJdbcExtractor" lazy-init="true" />
	
	
	<bean id="txManager" class="org.springframework.orm.hibernate3.HibernateTransactionManager">
       	<property name="sessionFactory" ref="sessionFactory"/>
	</bean>
	
	
	
	<tx:annotation-driven transaction-manager="txManager" proxy-target-class="true"/>
	<aop:aspectj-autoproxy/>
</beans>
```
其余配置文件不一一给出，经测试 成功！
