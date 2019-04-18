# 用JAXB从对象生成XML - z69183787的专栏 - CSDN博客
2013年08月22日 15:24:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3758

Java代码  ![收藏代码](http://extjs2.iteye.com/images/icon_star.png)
- import java.io.FileOutputStream;  
- 
- import javax.xml.bind.JAXBContext;  
- import javax.xml.bind.Marshaller;  
- import javax.xml.bind.annotation.XmlRootElement;  
- 
- publicclass JavaToXMLDemo {  
- publicstaticvoid main(String[] args) throws Exception {  
-     JAXBContext context = JAXBContext.newInstance(Employee.class);  
- 
-     Marshaller m = context.createMarshaller();  
-     m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);  
- 
-     Employee object = new Employee();  
-     object.setCode("CA");  
-     object.setName("Cath");  
-     object.setSalary(300);  
- 
-     m.marshal(object, System.out);  
- 
-   }  
- }  
Java代码  ![收藏代码](http://extjs2.iteye.com/images/icon_star.png)
- @XmlRootElement
- class Employee {  
- private String code;  
- 
- private String name;  
- 
- privateint salary;  
- 
- public String getCode() {  
- return code;  
-   }  
- 
- publicvoid setCode(String code) {  
- this.code = code;  
-   }  
- 
- public String getName() {  
- return name;  
-   }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-   }  
- 
- publicint getSalary() {  
- return salary;  
-   }  
- 
- publicvoid setSalary(int population) {  
- this.salary = population;  
-   }  
- }  
运行结果:
Xml代码  ![收藏代码](http://extjs2.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"standalone="yes"?>
- <employee>
- <code>CA</code>
- <name>Cath</name>
- <salary>300</salary>
- </employee>
