
# 使用DOM4J读取xml - 李昆鹏的博客 - CSDN博客


2018年07月04日 07:54:33[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：87标签：[xml																](https://so.csdn.net/so/search/s.do?q=xml&t=blog)个人分类：[XML																](https://blog.csdn.net/weixin_41547486/article/category/7771052)


----------------------------------使用DOM4J读取xml-------------------------------------
### DOM4J概述

#### 1　DOM4J是什么
DOM4J是针对Java开发人员专门提供的XML文档解析规范，它不同与DOM，但与DOM相似。DOM4J针对Java开发人员而设计，所以对于Java开发人员来说，使用DOM4J要比使用DOM更加方便。
DOM4J对DOM和SAX提供了支持，使用DOM4J可以把org.dom4j.document转换成org.w3c.Document，DOM4J也支持基于SAX的事件驱动处理模式。
使用者需要注意，DOM4J解析的结果是org.dom4j.Document，而不是org.w3c.Document。DOM4J与DOM一样，只是一组规范（接口与抽象类组成），底层必须要有DOM4J解析器的实现来支持。
DOM4J使用JAXP来查找SAX解析器，然后把XML文档解析为org.dom4j.Document对象。它还支持使用org.w3c.Document来转换为org.dom4j.Docment对象。

#### 2　DOM4J中的类结构
在DOM4J中，也有Node、Document、Element等接口，结构上与DOM中的接口比较相似。但还是有很多的区别：
![](https://img-blog.csdn.net/20180704075348266?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在DOM4J中，所有XML组成部分都是一个Node，其中Branch表示可以包含子节点的节点，例如Document和Element都是可以有子节点的，它们都是Branch的子接口。
Attribute是属性节点，CharacterData是文本节点，文本节点有三个子接口，分别是CDATA、Text、Comment。

#### 3　DOM4J获取Document对象
使用DOM4J来加载XML文档，需要先获取SAXReader对象，然后通过SAXReader对象的read()方法来加载XML文档：
|SAXReader reader =|new|SAXReader();
|//     reader.setValidation(true);
|Document  doc = reader.read(|"src/students.xml"|);
|

#### 4　DOM4J保存Document对象
保存Document对象需要使用XMLWriter对象的write()方法来完成，在创建XMLWriter时还可以为其指定XML文档的格式（缩进字符串以及是否换行），这需要使用OutputFormat来指定。
|doc.addDocType(|"students"|,|""|,|"students.dtd"|);
|OutputFormat format =|new|OutputFormat(|"\t"|,|true|);
|format.setEncoding(|"UTF-8"|);
|XMLWriter writer =|new|XMLWriter(|new|FileWriter(xmlName), format);
|writer.write(doc);
|writer.close();
|

#### 5　DOM4J创建Document对象
DocumentHelper类有很多的createXXX()方法，用来创建各种Node对象。
|Document doc = DocumentHelper.|createDocument|();
|
代码示例：
读取person.xml文件的信息
Person.xml
|<?|xml|version|=|"1.0"|encoding|=|"UTF-8"|?>
|<|persons|>
|<|person|id|=|"p1"|gender|=|"|男|"|>
|<|name|>|刘备|</|name|>
|<|age|>|27|</|age|>
|<|address|>|蜀国|</|address|>
|</|person|>
|<|person|id|=|"p2"|gender|=|"|男|"|>
|<|name|>|张飞|</|name|>
|<|age|>|22|</|age|>
|<|address|>|蜀国|</|address|>
|</|person|>
|<|person|id|=|"p3"|gender|=|"|男|"|>
|<|name|>|关羽|</|name|>
|<|age|>|25|</|age|>
|<|address|>|蜀国|</|address|>
|</|person|>
|</|persons|>
|
DOM4J代码示例：
|package cn.tx.dom4j;
|import java.io.FileInputStream;
|import java.io.IOException;
|import java.util.List;
|import org.dom4j.Attribute;
|import org.dom4j.Document;
|import org.dom4j.Element;
|import org.dom4j.io.SAXReader;
|public class ReadXML {

|/**
|* 读取xml文件的方法
|*/
|public  static void readXML() {
|//创建xml文档读取dom4j的对象
|SAXReader  reader = new SAXReader();
|//创建文档流
|FileInputStream  in = null;
|try  {
|in  = new FileInputStream("src/person.xml");
|//读取文档，返回文档对象
|Document  doc = reader.read(in);
|//获得文档对象的根元素
|Element  root = doc.getRootElement();
|System.out.println(root);
|//获得元素名称
|String  elementName = root.getName();
|System.out.println(elementName);
|//获得根元素下的孩子元素
|List<Element>  eList = root.elements();
|//遍历子元素
|for(Element  element:eList) {
|//System.out.println(element);
|//获取元素的名字
|String  eleName = element.getName();
|System.out.println(eleName);
|//通过属性名，直接从元素中取得属性的值
|String  idVal = element.attributeValue("id");
|String  genderVal = element.attributeValue("gender");
|System.out.println("ID="+idVal+"   性别="+genderVal);
|/*
|//在知道属性名字的情况下获得元素的属性
|Attribute  idAttr = element.attribute("id");
|Attribute  genderAttr = element.attribute("gender");
|//获得元素的所有属性,在不知道属性名字的情况下
|List<Attribute>  aList = element.attributes();
|//遍历每一个元素的属性
|for(Attribute  attr:aList) {
|//获得属性名字
|String  attrName = attr.getName();
|//获得属性值
|//String  attVal = attr.getText()
|String  attVal = attr.getValue();
|System.out.println(attrName+"="+attVal);
|}*/
|}
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(in  != null) {
|in.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|}
|/**
|* 读取xml文件的方法,获取子元素
|*/
|public  static void readXML1() {
|//创建xml文档读取dom4j的对象
|SAXReader  reader = new SAXReader();
|//创建文档流
|FileInputStream  in = null;
|try  {
|in  = new FileInputStream("src/person.xml");
|//读取文档，返回文档对象
|Document  doc = reader.read(in);
|//获得文档对象的根元素
|Element  root = doc.getRootElement();
|System.out.println(root);
|//获得元素名称
|String  elementName = root.getName();
|System.out.println(elementName);
|//获得根元素下的孩子元素
|List<Element>  eList = root.elements();
|for  (Element element : eList) {
|//获得每一个person的子元素
|//Element ename =  element.element("name");
|//System.out.println(ename.getText());
|List<Element>  eleList = element.elements();
|//遍历每一个person下的子元素
|for  (Element ele : eleList) {
|String  eleName = ele.getName();
|//获得子元素的文本内容
|String  eleText = ele.getText();
|System.out.println(eleName+":"+eleText);
|}
|}
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(in  != null) {
|in.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|}
|}
|
代码示例2
将person.xml中的属性写入到person类中
先建立一个Person类
|package cn.tx.dom4j;
|public class Person {
|private  Integer id;
|private  String name;
|private  Integer age;
|private  String gender;
|private  String address;
|public  Integer getId() {
|return  id;
|}
|public  void setId(Integer id) {
|this.id  = id;
|}
|public  String getName() {
|return  name;
|}
|public  void setName(String name) {
|this.name  = name;
|}
|public  Integer getAge() {
|return  age;
|}
|public  void setAge(Integer age) {
|this.age  = age;
|}
|public  String getGender() {
|return  gender;
|}
|public  void setGender(String gender) {
|this.gender  = gender;
|}
|public  String getAddress() {
|return  address;
|}
|public  void setAddress(String address) {
|this.address  = address;
|}
|@Override
|public  String toString() {
|return  "Person [id=" + id + ", name=" + name + ",  age=" + age + ", gender=" + gender + ", address=" +  address
|+  "]";
|}

|}
|
带使用DOM4J写入信息
|package cn.tx.dom4j;
|import java.io.FileInputStream;
|import java.io.IOException;
|import java.util.ArrayList;
|import java.util.List;
|import org.dom4j.Document;
|import org.dom4j.Element;
|import org.dom4j.io.SAXReader;
|public class ReadXML2 {
|public  static void main(String[] args) {
|readXML2();
|}
|/**
|* 读取xml文件,并且写入到person集合中,返回person集合
|*/
|public  static List<Person> readXML2() {
|//创建一个存储Person对象的集合
|List<Person>  pList = new ArrayList<Person>();
|//创建xml文档读取dom4j的对象
|SAXReader  reader = new SAXReader();
|//创建文档流
|FileInputStream  in = null;
|try  {
|in  = new FileInputStream("src/person.xml");
|//读取文档，返回文档对象
|Document  doc = reader.read(in);
|//获得文档对象的根元素
|Element  root = doc.getRootElement();
|//获得元素名称
|String  elementName = root.getName();
|//获得根元素下的孩子元素
|List<Element>  eList = root.elements();
|//遍历子元素
|for(Element  element:eList) {
|//获得person元素的属性值
|String  idVal = element.attributeValue("id");
|String  genderVal = element.attributeValue("gender");
|//创建person对象并且赋值
|System.out.println("创建person对象");
|Person  p = new Person();
|p.setId(idVal);
|p.setGender(genderVal);
|//获得person下子元素的内容
|/*String  nameText = element.element("name").getText();
|String  ageText = element.element("age").getText();
|String  addressText = element.element("address").getText();*/
|//根据父元素获得指定子元素的内容
|String  nameText = element.elementText("name");
|String  ageText = element.elementText("age");
|String  addressText = element.elementText("address");
|//将Person下子元素的内容写入到person对象中
|p.setName(nameText);
|p.setAge(new  Integer(ageText));
|p.setAddress(addressText);
|//将Person对象加入集合中
|pList.add(p);
|System.out.println("person加入集合");
|}
|//打印person集合
|for  (Person p : pList) {
|System.out.println(p);
|}
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(in  != null) {
|in.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|//返回Person集合
|return  pList;
|}
|}
|


