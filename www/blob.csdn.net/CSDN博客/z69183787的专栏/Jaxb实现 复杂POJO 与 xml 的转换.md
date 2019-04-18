# Jaxb实现 复杂POJO 与 xml 的转换 - z69183787的专栏 - CSDN博客
2013年10月31日 09:52:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8650
jaxml各参数请看：[http://blog.csdn.net/z69183787/article/details/12970523](http://blog.csdn.net/z69183787/article/details/12970523)
Person：
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
	
	@XmlElement(name = "BasicData")
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
Child.java
```java
package com.test;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlAccessType;
@XmlRootElement(name="BasicData")
@XmlAccessorType(XmlAccessType.FIELD)
public class Child {
	private String sex;
	public String getSex() {
		return sex;
	}
	public void setSex(String sex) {
		this.sex = sex;
	}
	
	
}
```
JaxbDateSerializer.java 处理Date
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
Main
```java
package com.test;
import java.io.FileWriter;
import java.io.IOException;
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
	public static void main(String[] args) throws JAXBException, IOException{
		System.out.println(java.net.URLEncoder.encode("的","utf-8"));
		//java 类 转 xml 文件
		List<Child> l = new ArrayList<Child>();
		Person p = new Person();
		p.setAge("123");
		p.setName("周顺");
		p.setD(new Date());
		for(int i=0;i<5;i++){
			Child c = new Child();
			c.setSex(i+"");
			l.add(c);
		}
		p.setChildren(l);
		StringWriter writer = new StringWriter();
		URL read = ClassLoader.getSystemResource("com/test/person.xml");
		FileWriter fw = new FileWriter(read.getPath());
		JAXBContext context = JAXBContext.newInstance(Person.class);
		Marshaller m = context.createMarshaller();
		m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true); 
		m.setProperty(Marshaller.JAXB_ENCODING, "GBK"); //防止文件中文乱码
		m.marshal(p, writer);
		m.marshal(p, fw);
		fw.flush();
		fw.close();
		String xml = writer.toString();
		System.out.println(xml);
		
		
	//	xml文件转java类
		 JAXBContext context2 = JAXBContext.newInstance(Person.class);
	        Unmarshaller unmarshaller = context2.createUnmarshaller();
	        //class目录 jaxb/bin
	        //URL xmlFileUrl = ClassLoader.getSystemResource("");
	        URL xmlFileUrl = ClassLoader.getSystemResource("com/test/person.xml");
	        System.out.println(xmlFileUrl.getPath());
	        Person p2 = (Person) unmarshaller.unmarshal(xmlFileUrl);
               //  SAXReader reader = new SAXReader();   
          //  Document document = reader.read(new File(
           //Test.class.getClassLoader().getResource("com/test/person.xml").getPath()));
       // Person p2 = (Person) unmarshaller.unmarshal(new StringReader(document.asXML()));
	        System.out.println(p2.getName());
	        System.out.println(p2.getChildren().get(4).getSex());
	}
}
```
xml
```
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<person><name>zhoushun</name><age>123</age><BasicData><sex>0</sex></BasicData><BasicData><sex>1</sex></BasicData><BasicData><sex>2</sex></BasicData><BasicData><sex>3</sex></BasicData><BasicData><sex>4</sex></BasicData><time>2013-10-31</time>
</person>
```
