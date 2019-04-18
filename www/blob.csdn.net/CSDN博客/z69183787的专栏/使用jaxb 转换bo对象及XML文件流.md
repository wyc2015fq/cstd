# 使用jaxb 转换bo对象及XML文件流 - z69183787的专栏 - CSDN博客
2012年10月22日 10:51:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2451
bo对象：
注：
jaxb只会序列化 public 的方法 ，若属性声明为public，且声明 @XmlElement(name = "name") ，则需要在该属性get方法上声明@XmlTransient，表示不将get方法序列化成xml元素，否则会报重复属性序列化。或者将属性改为private ，将@XmlElement(name = "name")放置在get方法上。
其二：若寻在 属性中有 Date类型，需要继承XmlAdapter<String, Date>类，并重载2个方法后，在bo对象处使用@XmlJavaTypeAdapter(JaxbDateSerializer.class) 进行序列化。
```java
package com.test;
import java.util.Date;
import java.util.List;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;
import javax.xml.bind.annotation.adapters.XmlJavaTypeAdapter;
@XmlRootElement(name = "person")
public class Person {
	@XmlElement(name = "name")
	private String name ;
	
	@XmlElement(name = "age")
	private String age;
	
	@XmlElement(name = "child")
	private List<Child> children;
	
	
	private Date d;
	
	@XmlElement(name = "time")
	@XmlJavaTypeAdapter(JaxbDateSerializer.class) 
	public Date getD() {
		return d;
	}
	public void setD(Date d) {
		this.d = d;
	}
	
	@XmlTransient
	public List<Child> getChildren() {
		return children;
	}
	public void setChildren(List<Child> children) {
		this.children = children;
	}
	@XmlTransient
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	@XmlTransient
	public String getAge() {
		return age;
	}
	public void setAge(String age) {
		this.age = age;
	}
	
}
```
```java
package com.test;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.xml.bind.annotation.adapters.XmlAdapter;
public class JaxbDateSerializer extends XmlAdapter<String, Date>{
    private SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
    @Override 
    public String marshal(Date date) throws Exception { 
        return dateFormat.format(date); 
    }
    @Override 
    public Date unmarshal(String date) throws Exception { 
        return dateFormat.parse(date); 
    } 
}
```
main：
```java
package com.test;
import java.io.StringWriter;
import java.net.URL;
import java.util.Date;
import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
public class Test {
	public static void main(String[] args) throws JAXBException{
		//java 类 转 xml 文件
		List<Child> l = new ArrayList<Child>();
		Person p = new Person();
		p.setAge("123");
		p.setName("zhoushun");
		p.setD(new Date());
		for(int i=0;i<5;i++){
			Child c = new Child();
			c.setSex(i+"");
			l.add(c);
		}
		p.setChildren(l);
		StringWriter writer = new StringWriter();
		JAXBContext context = JAXBContext.newInstance(Person.class,Child.class);
		Marshaller m = context.createMarshaller();
		m.marshal(p, writer);
		String xml = writer.toString();
		System.out.println(xml);
		
		
		//xml文件转java类
		 JAXBContext context2 = JAXBContext.newInstance(Person.class);
	        Unmarshaller unmarshaller = context2.createUnmarshaller();
	        //class目录 jaxb/bin
	        //URL xmlFileUrl = ClassLoader.getSystemResource("");
	        URL xmlFileUrl = ClassLoader.getSystemResource("person.xml");
	        System.out.println(xmlFileUrl.getPath());
	        Person p2 = (Person) unmarshaller.unmarshal(xmlFileUrl);
	        System.out.println(p2.getName());
	        System.out.println(p2.getChildren().get(4).getSex());
	}
}
```
xml文件：
```
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<person><name>zhoushun123</name><age>123</age><child><sex>0</sex></child><child><sex>1</sex></child><child><sex>2</sex></child><child><sex>3</sex></child><child><sex>4</sex></child><d>2012-06-01</d>
</person>
<person><name>zhoushun125</name><age>123</age><child><sex>0</sex></child><child><sex>1</sex></child><child><sex>2</sex></child><child><sex>3</sex></child><child><sex>4</sex></child><d>2012-06-01</d>
</person>
```
