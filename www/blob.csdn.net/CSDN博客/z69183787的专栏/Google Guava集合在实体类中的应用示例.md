# Google Guava集合在实体类中的应用示例 - z69183787的专栏 - CSDN博客
2016年11月25日 16:38:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1442
将实体list，转换成map，拿到所有id的集合方法。减少访问数据库。
实体基类BaseEntity
```java
package com.mytest;
import org.apache.commons.lang3.builder.ToStringBuilder;
import org.apache.commons.lang3.builder.ToStringStyle;
public class BaseEntity {
	private Long id;
	public Long getId() {
		return id;
	}
	public void setId(Long id) {
		this.id = id;
	}
	/**
	 * 覆盖toString方法，目的显示所有JavaBean的属性值，省略写很多的getXxx的方法
	 */
	public String toString() {
		return ToStringBuilder.reflectionToString(this,
				ToStringStyle.MULTI_LINE_STYLE);
	}
}
```
实体类Person
```java
package com.mytest;
public class Person extends BaseEntity{
	
	private String name;
	private String gender;
	private int age;
	private double weight;
	
	public Person(Long id, String name, String gender, int age, double weight) {
		super();
		this.setId(id);
		this.name = name;
		this.gender = gender;
		this.age = age;
		this.weight = weight;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getGender() {
		return gender;
	}
	public void setGender(String gender) {
		this.gender = gender;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	public double getWeight() {
		return weight;
	}
	public void setWeight(double weight) {
		this.weight = weight;
	}
}
```
Example
```java
package com.mytest;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import com.google.common.base.Function;
import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.google.common.collect.Ordering;
public class Example {
    
	public static Logger logger = LoggerFactory.getLogger(Example.class);       
	
	public static void main(String[] args) {
        List<Person> list = Lists.newArrayList();
        list.add(new Person(4L,"jason", "male", 16, 120.50));
        list.add(new Person(1L,"tom", "male", 48, 163.50));
        list.add(new Person(3L,"john", "male", 33, 105));
        list.add(new Person(2L,"rose", "female", 26, 90));
        
        
        logger.debug(list.toString());
        //获取ids
        List<Long> ids  = entityIds(list);  //entityIds方法
        logger.debug("---------------ids-------------------");
        logger.debug(ids.toString());
        
        logger.debug("---------------maps-------------------");
        //获取map
        Map<Long, Person> maps = entityMaps(list); //entityMaps方法
        Set<Long> keys = maps.keySet();
        for (Long id : keys) {
			logger.debug("key: " + id + " value: " + maps.get(id));
		}
	}
	
	/**
	 * 获取表记录集合的Id集合
	 * @param list
	 * @return
	 */
	public static<T extends BaseEntity> List<Long> entityIds(List<T> list){
		List<Long> ids = Lists.transform(list, new Function<T, Long>(){
        	    public Long apply(T entity){
        		return entity.getId();
        	    }
                }); 
		return ids;
	}
	
	/**
	 * 根据实体记录，输出Map<Long id,K extend BaseEntity>集合
	 * @param list
	 * @return
	 */
	public static<V extends BaseEntity> Map<Long,V> entityMaps(List<V> list){
	        Map<Long,V> maps = Maps.uniqueIndex(list.iterator(),  new Function<V, Long>() {
		    @Override
		    public Long apply(V entity) {
		        return entity.getId();
		    }
		});
		return maps;
	}
	
}
```
entityIds方法
```java
/**
	 * 获取表记录集合的Id集合
	 * @param list
	 * @return
	 */
	public static<T extends BaseEntity> List<Long> entityIds(List<T> list){
		List<Long> ids = Lists.transform(list, new Function<T, Long>(){
        	    public Long apply(T entity){
        		return entity.getId();
        	    }
                }); 
		return ids;
	}
```
entityMaps方法
```java
/**
	 * 根据实体记录，输出Map<Long id,K extend BaseEntity>集合
	 * @param list
	 * @return
	 */
	public static<V extends BaseEntity> Map<Long,V> entityMaps(List<V> list){
	        Map<Long,V> maps = Maps.uniqueIndex(list.iterator(),  new Function<V, Long>() {
		    @Override
		    public Long apply(V entity) {
		        return entity.getId();
		    }
		});
		return maps;
	}
```
引入包，可参考pom.xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>com.gzdfbz</groupId>
	<artifactId>mytest</artifactId>
	<version>0.0.1-SNAPSHOT</version>
	<dependencies>
		<dependency>
			<groupId>com.google.guava</groupId>
			<artifactId>guava</artifactId>
			<version>18.0</version>
		</dependency>
		<dependency>
			<groupId>org.apache.commons</groupId>
			<artifactId>commons-lang3</artifactId>
			<version>3.3.2</version>
		</dependency>
		<dependency>
			<groupId>org.slf4j</groupId>
			<artifactId>slf4j-api</artifactId>
			<version>1.7.7</version>
		</dependency>
		<dependency>
			<groupId>org.slf4j</groupId>
			<artifactId>slf4j-log4j12</artifactId>
			<version>1.7.7</version>
		</dependency>
		<dependency>
			<groupId>joda-time</groupId>
			<artifactId>joda-time</artifactId>
			<version>2.3</version>
		</dependency>
	</dependencies>
</project>
log4j参考：
#
log4j.rootLogger=DEBUG,console
log4j.additivity.org.apache=true
#
log4j.appender.console=org.apache.log4j.ConsoleAppender
log4j.appender.console.Threshold=DEBUG
log4j.appender.console.ImmediateFlush=true
log4j.appender.console.Target=System.err
log4j.appender.console.layout=org.apache.log4j.PatternLayout
#log4j.appender.console.layout.ConversionPattern=[%-5p] %d(%r) --> [%t] %l: %m %x %n
log4j.appender.console.layout.ConversionPattern=%l: %m %x %n
log4j.appender.com.mytest =  debug
 来自：http://my.oschina.net/u/221951/blog/333286
