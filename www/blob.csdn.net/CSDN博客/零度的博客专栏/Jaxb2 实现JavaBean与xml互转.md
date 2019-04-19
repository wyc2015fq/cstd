# Jaxb2 实现JavaBean与xml互转 - 零度的博客专栏 - CSDN博客
2016年03月25日 15:02:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：759

一、简介
      JAXB（Java Architecture for XML Binding) 是一个业界的标准，是一项可以根据XML Schema产生Java类的技术。该过程中，JAXB也提供了将XML实例文档反向生成Java对象树的方法，并能将Java对象树的内容重新写到 XML实例文档。
      Jaxb 2.0是JDK 1.6的组成部分。我们不需要下载第三方jar包 即可做到轻松转换。Jaxb2使用了JDK的新特性，如：Annotation、GenericType等，需要在即将转换的JavaBean中添加annotation注解。
二、重要概念
- 
JAXBContext类，是应用的入口，用于管理XML/Java绑定信息。 
- 
Marshaller接口，将Java对象序列化为XML数据。 
- 
Unmarshaller接口，将XML数据反序列化为Java对象。
- 
@XmlType，将Java类或枚举类型映射到XML模式类型 
- 
@XmlAccessorType(XmlAccessType.FIELD) ，控制字段或属性的序列化。FIELD表示JAXB将自动绑定Java类中的每个非静态的（static）、非瞬态的（由@XmlTransient标 注）字段到XML。其他值还有XmlAccessType.PROPERTY和XmlAccessType.NONE。
- 
@XmlAccessorOrder，控制JAXB 绑定类中属性和字段的排序。 
- 
@XmlJavaTypeAdapter，使用定制的适配器（即扩展抽象类XmlAdapter并覆盖marshal()和unmarshal()方法），以序列化Java类为XML。
- 
@XmlElementWrapper ，对于数组或集合（即包含多个元素的成员变量），生成一个包装该数组或集合的XML元素（称为包装器）。 
- 
@XmlRootElement，将Java类或枚举类型映射到XML元素。 
- 
@XmlElement，将Java类的一个属性映射到与属性同名的一个XML元素。 
- 
@XmlAttribute，将Java类的一个属性映射到与属性同名的一个XML属性。
 三、示例
1、准备工作
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package utils;  
- 
- import java.io.StringReader;  
- import java.io.StringWriter;  
- 
- import javax.xml.bind.JAXBContext;  
- import javax.xml.bind.Marshaller;  
- import javax.xml.bind.Unmarshaller;  
- 
- /**
-  * Jaxb2工具类
-  * @author      zhuc
-  * @create      2013-3-29 下午2:40:14
-  */
- publicclass JaxbUtil {  
- 
- /**
-      * JavaBean转换成xml
-      * 默认编码UTF-8
-      * @param obj
-      * @param writer
-      * @return 
-      */
- publicstatic String convertToXml(Object obj) {  
- return convertToXml(obj, "UTF-8");  
-     }  
- 
- /**
-      * JavaBean转换成xml
-      * @param obj
-      * @param encoding 
-      * @return 
-      */
- publicstatic String convertToXml(Object obj, String encoding) {  
-         String result = null;  
- try {  
-             JAXBContext context = JAXBContext.newInstance(obj.getClass());  
-             Marshaller marshaller = context.createMarshaller();  
-             marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);  
-             marshaller.setProperty(Marshaller.JAXB_ENCODING, encoding);  
- 
-             StringWriter writer = new StringWriter();  
-             marshaller.marshal(obj, writer);  
-             result = writer.toString();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
- 
- return result;  
-     }  
- 
- /**
-      * xml转换成JavaBean
-      * @param xml
-      * @param c
-      * @return
-      */
- @SuppressWarnings("unchecked")  
- publicstatic <T> T converyToJavaBean(String xml, Class<T> c) {  
-         T t = null;  
- try {  
-             JAXBContext context = JAXBContext.newInstance(c);  
-             Unmarshaller unmarshaller = context.createUnmarshaller();  
-             t = (T) unmarshaller.unmarshal(new StringReader(xml));  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
- 
- return t;  
-     }  
- }  
      非常简单易懂，需要注意的是
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);  
- marshaller.setProperty(Marshaller.JAXB_ENCODING, encoding);  
Marshaller.JAXB_FORMATTED_OUTPUT 决定是否在转换成xml时同时进行格式化（即按标签自动换行，否则即是一行的xml）
Marshaller.JAXB_ENCODING xml的编码方式
另外，Marshaller 还有其他Property可以设置，可以去查阅api。
2、最简单转换
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t1;  
- 
- import java.util.Date;  
- 
- import javax.xml.bind.annotation.XmlAccessType;  
- import javax.xml.bind.annotation.XmlAccessorType;  
- import javax.xml.bind.annotation.XmlAttribute;  
- import javax.xml.bind.annotation.XmlElement;  
- import javax.xml.bind.annotation.XmlRootElement;  
- import javax.xml.bind.annotation.XmlType;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:49:48
-  */
- @XmlAccessorType(XmlAccessType.FIELD)  
- @XmlRootElement
- @XmlType(name = "book", propOrder = { "author", "calendar", "price", "id" })  
- publicclass Book {  
- 
- @XmlElement(required = true)  
- private String author;  
- 
- @XmlElement(name = "price_1", required = true)  
- privatefloat price;  
- 
- @XmlElement
- private Date calendar;  
- 
- @XmlAttribute
- private Integer id;  
- 
- /**
-      * @return the author
-      */
- public String getAuthor() {  
- return author;  
-     }  
- 
- /**
-      * @return the price
-      */
- publicfloat getPrice() {  
- return price;  
-     }  
- 
- /**
-      * @return the calendar
-      */
- public Date getCalendar() {  
- return calendar;  
-     }  
- 
- /**
-      * @return the id
-      */
- public Integer getId() {  
- return id;  
-     }  
- 
- /**
-      * @param author the author to set
-      */
- publicvoid setAuthor(String author) {  
- this.author = author;  
-     }  
- 
- /**
-      * @param price the price to set
-      */
- publicvoid setPrice(float price) {  
- this.price = price;  
-     }  
- 
- /**
-      * @param calendar the calendar to set
-      */
- publicvoid setCalendar(Date calendar) {  
- this.calendar = calendar;  
-     }  
- 
- /**
-      * @param id the id to set
-      */
- publicvoid setId(Integer id) {  
- this.id = id;  
-     }  
- 
- /* (non-Javadoc)
-      * @see java.lang.Object#toString()
-      */
- @Override
- public String toString() {  
- return"Book [author=" + author + ", price=" + price + ", calendar=" + calendar + ", id=" + id + "]";  
-     }  
- 
- }  
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t1;  
- 
- import java.util.Date;  
- 
- import javax.xml.bind.JAXBException;  
- 
- import org.junit.Test;  
- 
- import utils.JaxbUtil;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:50:00
-  */
- publicclass JaxbTest1 {  
- 
- /**
-      * @throws JAXBException
-      */
- @Test
- publicvoid showMarshaller()  {  
-         Book book = new Book();  
-         book.setId(100);  
-         book.setAuthor("James");  
-         book.setCalendar(new Date());  
-         book.setPrice(23.45f);   //默认是0.0
- 
-         String str = JaxbUtil.convertToXml(book);  
-         System.out.println(str);  
-     }  
- 
- /**
-      * @throws JAXBException
-      */
- @Test
- publicvoid showUnMarshaller() {  
-         String str = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" +  
- "<book id=\"100\">" +  
- "    <author>James</author>" +  
- "   <calendar>2013-03-29T09:25:56.004+08:00</calendar>" +  
- "  <price_1>23.45</price_1>" +  
- "</book>";  
- 
-         Book book = JaxbUtil.converyToJavaBean(str, Book.class);  
-         System.out.println(book);  
-     }  
- }  
输出结果分别为：
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<book id="100">
    <author>James</author>
    <calendar>2013-03-29T14:50:58.974+08:00</calendar>
    <price_1>23.45</price_1>
</book>
Book [author=James, price=23.45, calendar=Fri Mar 29 09:25:56 CST 2013, id=100]
3、类中包含复杂对象的转换
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t2;  
- 
- import javax.xml.bind.annotation.XmlAccessType;  
- import javax.xml.bind.annotation.XmlAccessorType;  
- import javax.xml.bind.annotation.XmlAttribute;  
- import javax.xml.bind.annotation.XmlElement;  
- import javax.xml.bind.annotation.XmlRootElement;  
- import javax.xml.bind.annotation.XmlType;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:51:44
-  */
- @XmlAccessorType(XmlAccessType.FIELD)  
- @XmlRootElement(name = "student")  
- @XmlType(propOrder = {})  
- publicclass Student {  
- 
- @XmlAttribute
- private Integer id;  
- 
- @XmlElement
- private String name;  
- 
- @XmlElement(name = "role")  
- private Role role;  
- 
- /**
-      * @return the id
-      */
- public Integer getId() {  
- return id;  
-     }  
- 
- /**
-      * @return the name
-      */
- public String getName() {  
- return name;  
-     }  
- 
- /**
-      * @return the role
-      */
- public Role getRole() {  
- return role;  
-     }  
- 
- /**
-      * @param id the id to set
-      */
- publicvoid setId(Integer id) {  
- this.id = id;  
-     }  
- 
- /**
-      * @param name the name to set
-      */
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- /**
-      * @param role the role to set
-      */
- publicvoid setRole(Role role) {  
- this.role = role;  
-     }  
- 
- /* (non-Javadoc)
-      * @see java.lang.Object#toString()
-      */
- @Override
- public String toString() {  
- return"Student [id=" + id + ", name=" + name + ", role=" + role + "]";  
-     }  
- 
- }  
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t2;  
- 
- import javax.xml.bind.annotation.XmlAccessType;  
- import javax.xml.bind.annotation.XmlAccessorType;  
- import javax.xml.bind.annotation.XmlElement;  
- import javax.xml.bind.annotation.XmlType;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:51:52
-  */
- @XmlAccessorType(XmlAccessType.FIELD)  
- @XmlType(propOrder = { "name", "desc" })  
- publicclass Role {  
- 
- @XmlElement
- private String name;  
- 
- @XmlElement
- private String desc;  
- 
- /**
-      * @return the name
-      */
- public String getName() {  
- return name;  
-     }  
- 
- /**
-      * @return the desc
-      */
- public String getDesc() {  
- return desc;  
-     }  
- 
- /**
-      * @param name the name to set
-      */
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- /**
-      * @param desc the desc to set
-      */
- publicvoid setDesc(String desc) {  
- this.desc = desc;  
-     }  
- 
- /* (non-Javadoc)
-      * @see java.lang.Object#toString()
-      */
- @Override
- public String toString() {  
- return"Role [name=" + name + ", desc=" + desc + "]";  
-     }  
- 
- }  
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t2;  
- 
- import org.junit.Test;  
- 
- import utils.JaxbUtil;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:52:00
-  */
- publicclass JaxbTest2 {  
- 
- @Test
- publicvoid showMarshaller() {  
- 
-         Student student = new Student();  
-         student.setId(12);  
-         student.setName("test");  
- 
-         Role role = new Role();  
-         role.setDesc("管理");  
-         role.setName("班长");  
- 
-         student.setRole(role);  
- 
-         String str = JaxbUtil.convertToXml(student);  
-         System.out.println(str);  
-     }  
- 
- @Test
- publicvoid showUnMarshaller() {  
-         String str = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"+  
- "<student id=\"12\">"+  
- "    <name>test</name>"+  
- "   <role>"+  
- "      <name>班长</name>"+  
- "     <desc>管理</desc>"+  
- "</role>"+  
- "</student>";  
-         Student student = JaxbUtil.converyToJavaBean(str, Student.class);  
-         System.out.println(student);  
-     }  
- 
- }  
输出结果分别为：
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<student id="12">
    <name>test</name>
    <role>
        <name>班长</name>
        <desc>管理</desc>
    </role>
</student>
Student [id=12, name=test, role=Role [name=班长, desc=管理]]
4、集合对象的转换（同样适用于Set）
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t3;  
- 
- import java.util.List;  
- 
- import javax.xml.bind.annotation.XmlAccessType;  
- import javax.xml.bind.annotation.XmlAccessorType;  
- import javax.xml.bind.annotation.XmlElement;  
- import javax.xml.bind.annotation.XmlElementWrapper;  
- import javax.xml.bind.annotation.XmlRootElement;  
- import javax.xml.bind.annotation.XmlType;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:55:56
-  */
- @XmlAccessorType(XmlAccessType.FIELD)  
- @XmlRootElement(name = "country")  
- @XmlType(propOrder = { "name", "provinceList" })  
- publicclass Country {  
- 
- @XmlElement(name = "country_name")  
- private String name;  
- 
- @XmlElementWrapper(name = "provinces")  
- @XmlElement(name = "province")  
- private List<Province> provinceList;  
- 
- /**
-      * @return the name
-      */
- public String getName() {  
- return name;  
-     }  
- 
- /**
-      * @return the provinceList
-      */
- public List<Province> getProvinceList() {  
- return provinceList;  
-     }  
- 
- /**
-      * @param name the name to set
-      */
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- /**
-      * @param provinceList the provinceList to set
-      */
- publicvoid setProvinceList(List<Province> provinceList) {  
- this.provinceList = provinceList;  
-     }  
- 
- /* (non-Javadoc)
-      * @see java.lang.Object#toString()
-      */
- @Override
- public String toString() {  
- return"Country [name=" + name + ", provinceList=" + provinceList + "]";  
-     }  
- 
- }  
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t3;  
- 
- import javax.xml.bind.annotation.XmlAccessType;  
- import javax.xml.bind.annotation.XmlAccessorType;  
- import javax.xml.bind.annotation.XmlElement;  
- import javax.xml.bind.annotation.XmlType;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:56:03
-  */
- @XmlAccessorType(XmlAccessType.FIELD)  
- @XmlType(propOrder = { "name", "provCity" })  
- publicclass Province {  
- 
- @XmlElement(name = "province_name")  
- private String name;  
- 
- @XmlElement(name = "prov_city")  
- private String provCity;  
- 
- /**
-      * @return the provCity
-      */
- public String getProvCity() {  
- return provCity;  
-     }  
- 
- /**
-      * @param provCity the provCity to set
-      */
- publicvoid setProvCity(String provCity) {  
- this.provCity = provCity;  
-     }  
- 
- /**
-      * @return the name
-      */
- public String getName() {  
- return name;  
-     }  
- 
- /**
-      * @param name the name to set
-      */
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- /* (non-Javadoc)
-      * @see java.lang.Object#toString()
-      */
- @Override
- public String toString() {  
- return"Province [name=" + name + ", provCity=" + provCity + "]";  
-     }  
- 
- }  
Java代码  ![收藏代码](http://zhuchengzzcc.iteye.com/images/icon_star.png)
- package t3;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.junit.Test;  
- 
- import utils.JaxbUtil;  
- 
- /**
-  * @author      zhuc
-  * @create      2013-3-29 下午2:56:11
-  */
- publicclass JaxbTest3 {  
- 
- /**
-      * @throws JAXBException
-      */
- @Test
- publicvoid showMarshaller() {  
-         Country country = new Country();  
-         country.setName("中国");  
- 
-         List<Province> list = new ArrayList<Province>();  
-         Province province = new Province();  
-         province.setName("江苏省");  
-         province.setProvCity("南京市");  
-         Province province2 = new Province();  
-         province2.setName("浙江省");  
-         province2.setProvCity("杭州市");  
-         list.add(province);  
-         list.add(province2);  
- 
-         country.setProvinceList(list);  
- 
-         String str = JaxbUtil.convertToXml(country);  
-         System.out.println(str);  
-     }  
- 
- /**
-      * 
-      */
- @Test
- publicvoid showUnMarshaller() {  
-         String str = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"+  
- "<country>"+  
- "    <country_name>中国</country_name>"+  
- "    <provinces>"+  
- "        <province>"+  
- "            <province_name>江苏省</province_name>"+  
- "           <prov_city>南京市</prov_city>"+  
- "        </province>"+  
- "       <province>"+  
- "           <province_name>浙江省</province_name>"+  
- "           <prov_city>杭州市</prov_city>"+  
- "       </province>"+  
- "    </provinces>"+  
- "</country>";  
-         Country country = JaxbUtil.converyToJavaBean(str, Country.class);  
-         System.out.println(country);  
-     }  
- 
- }  
输出结果分别为：
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<country>
    <country_name>中国</country_name>
    <provinces>
        <province>
            <province_name>江苏省</province_name>
            <prov_city>南京市</prov_city>
        </province>
        <province>
            <province_name>浙江省</province_name>
            <prov_city>杭州市</prov_city>
        </province>
    </provinces>
</country>
Country [name=中国, provinceList=[Province [name=江苏省, provCity=南京市], Province [name=浙江省, provCity=杭州市]]]
