
# 使用DOM4J向xml中写入文档和删除指定的元素、在指定位置插入元素 - 李昆鹏的博客 - CSDN博客


2018年07月04日 07:57:09[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：391


-----------------------------------------使用DOM4J向xml中写入文档和删除指定的元素、在指定位置插入元素------------------------------------
将student对象写入xml文档中
Student类
|package cn.tx.dom4j;
|public class Student {
|private  Integer studNo;
|private  String name;
|private  String address;
|private  Integer gender;
|private  Integer score;
|public  Integer getStudNo() {
|return  studNo;
|}
|public  void setStudNo(Integer studNo) {
|this.studNo  = studNo;
|}
|public  String getName() {
|return  name;
|}
|public  void setName(String name) {
|this.name  = name;
|}
|public  String getAddress() {
|return  address;
|}
|public  void setAddress(String address) {
|this.address  = address;
|}
|public  Integer getGender() {
|return  gender;
|}
|public  void setGender(Integer gender) {
|this.gender  = gender;
|}
|public  Integer getScore() {
|return  score;
|}
|public  void setScore(Integer score) {
|this.score  = score;
|}
|public  Student(Integer studNo, String name, String address, Integer gender, Integer  score) {
|super();
|this.studNo  = studNo;
|this.name  = name;
|this.address  = address;
|this.gender  = gender;
|this.score  = score;
|}
|}
|
DOM4J向xml中写入student示例
|package cn.tx.dom4j;
|import java.io.FileNotFoundException;
|import java.io.FileOutputStream;
|import java.io.IOException;
|import java.util.ArrayList;
|import java.util.List;
|import org.dom4j.Document;
|import org.dom4j.DocumentHelper;
|import org.dom4j.Element;
|import org.dom4j.io.OutputFormat;
|import org.dom4j.io.XMLWriter;
|public class WriterXML {
|public  static void writerXML() {
|List<Student>  stuList = new ArrayList<Student>();
|for  (int i = 1; i < 10; i++) {
|Student  s = new Student(i, "学生"+i, "中国", 1, 90+i);
|stuList.add(s);
|}
|//创建xml的文档对象
|Document  doc = DocumentHelper.createDocument();
|//添加根节点
|Element  root = doc.addElement("students");
|//遍历集合写入student节点
|for  (Student s : stuList) {
|//在根节点上添加student元素，并且给student元素添加属性
|Element  stuElement = root.addElement("student")
|.addAttribute("stuNo",  s.getStudNo()+"")
|.addAttribute("gender",  s.getGender()+"");
|//在student元素中加入name,address,score元素
|stuElement.addElement("name").setText(s.getName());
|stuElement.addElement("address").setText(s.getAddress());
|stuElement.addElement("score").setText(s.getScore()+"");
|}
|FileOutputStream  out = null;
|XMLWriter  xw = null;
|try  {
|//创建文件输出流
|out  = new FileOutputStream("src/student.xml");
|//创建漂亮的格式化对象
|OutputFormat  of = OutputFormat.createPrettyPrint();
|//创建xml输出对象
|xw  = new XMLWriter(out, of);
|//把文档对象写入到xml中
|xw.write(doc);
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(xw  != null) {
|xw.close();
|}
|if(out  != null) {
|out.close();
|}
|}  catch (IOException e) {
|//  TODO Auto-generated catch block
|e.printStackTrace();
|}
|}
|}
|}
|
通过DOM4J删除xml中的元素
|package cn.tx.dom4j;
|import java.io.FileInputStream;
|import java.io.FileNotFoundException;
|import java.io.FileOutputStream;
|import java.io.IOException;
|import java.util.ArrayList;
|import java.util.List;
|import org.dom4j.Document;
|import org.dom4j.DocumentException;
|import org.dom4j.DocumentHelper;
|import org.dom4j.Element;
|import org.dom4j.io.OutputFormat;
|import org.dom4j.io.SAXReader;
|import org.dom4j.io.XMLWriter;
|public class WriterXML {
|/**
|* 删除xml中的元素
|* @param stuNO 指定删除sutdent元素的ID
|*/
|public  static void deleteStudent(int stuNo) {
|XMLWriter  xw = null;
|//创建SAXReader对象
|SAXReader  reader = new SAXReader();
|try  {
|//通过reader对象获得xml文档对象
|Document  doc = reader.read(new FileInputStream("src/student.xml"));
|//通过doc获得xml的根元素
|Element  root = doc.getRootElement();
|//通过根元素获得所有的子元素集合
|List<Element>  eList = root.elements();
|//遍历子元素集合
|for  (Element e : eList) {
|//通过子元素获得student的id属性的值
|Integer  stuNoVal = new Integer(e.attributeValue("stuNo"));
|if(stuNo  == stuNoVal) {
|//需要通过父节点来删除
|//获得父节点
|Element  pElement = e.getParent();
|//通过父节点删除元素
|pElement.remove(e);
|break;
|}
|}
|//创建写入XML的对象
|xw  = new XMLWriter(new FileOutputStream("src/student.xml"),  OutputFormat.createPrettyPrint());
|xw.write(doc);
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(xw  != null) {
|xw.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|}
|}
|
通过DOM4J向xml文档指定位置插入元素
|package cn.tx.dom4j;
|import java.io.FileInputStream;
|import java.io.FileNotFoundException;
|import java.io.FileOutputStream;
|import java.io.IOException;
|import java.util.ArrayList;
|import java.util.List;
|import org.dom4j.Document;
|import org.dom4j.DocumentException;
|import org.dom4j.DocumentHelper;
|import org.dom4j.Element;
|import org.dom4j.io.OutputFormat;
|import org.dom4j.io.SAXReader;
|import org.dom4j.io.XMLWriter;
|import org.dom4j.tree.DefaultElement;
|public class WriterXML {
|/**
|* 向xml中插入新的元素
|* @param stuNO 在sutdent元素的ID的后面插入新的元素
|*/
|public  static void insertStudent(int stuNo) {
|XMLWriter  xw = null;
|//创建SAXReader对象
|SAXReader reader = new SAXReader();
|try  {
|//通过reader对象获得xml文档对象
|Document  doc = reader.read(new FileInputStream("src/student.xml"));
|//通过doc获得xml的根元素
|Element  root = doc.getRootElement();
|//查看root的具体类,获得root是属于DefaultElement的实例
|System.out.println(root.getClass());
|//通过根元素获得所有的子元素集合
|List<Element>  eList = root.elements();
|//新建一个student元素
|Element  student7 = new DefaultElement("student");
|//向student7中添加属性
|student7.addAttribute("stuNo",  "7");
|student7.addAttribute("gender",  "1");
|//向student7元素中添加子元素，并且写入内容
|student7.addElement("name").setText("李四");
|student7.addElement("address").setText("中国");
|student7.addElement("score").setText("90");
|//遍历子元素集合
|for(int  i =0 ; i < eList.size(); i++) {
|//获得集合中的元素
|Element  e = eList.get(i);
|//通过子元素获得student的id属性的值
|Integer  stuNoVal = new Integer(e.attributeValue("stuNo"));
|System.out.println(eList.indexOf(e));
|if(stuNo  == stuNoVal) {
|//获得元素的索引
|int  index = eList.indexOf(e);
|System.out.println(index);
|//将student7元素加入 到eList集合中,集合中在指定索引处添加元素会把原来位置的索引忘后面移动
|//注意向xml文档中添加元素时，一个元素对象只能添加一次，不能重复添加
|eList.add(index+1,  student7);
|}
|}
|//创建写入XML的对象
|xw  = new XMLWriter(new FileOutputStream("src/student.xml"), OutputFormat.createPrettyPrint());
|xw.write(doc);
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(xw  != null) {
|xw.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|}
|}
|
Student.xml文档
|<?xml version="1.0"  encoding="UTF-8"?>
|<students>
|<student stuNo="1" gender="1">
|<name>学生1</name>
|<address>中国</address>
|<score>91</score>
|</student>
|<student stuNo="2" gender="1">
|<name>学生2</name>
|<address>中国</address>
|<score>92</score>
|</student>
|<student stuNo="3" gender="1">
|<name>学生3</name>
|<address>中国</address>
|<score>93</score>
|</student>
|<student stuNo="4" gender="1">
|<name>学生4</name>
|<address>中国</address>
|<score>94</score>
|</student>
|<student stuNo="5" gender="1">
|<name>学生5</name>
|<address>中国</address>
|<score>95</score>
|</student>
|<student stuNo="6" gender="1">
|<name>学生6</name>
|<address>中国</address>
|<score>96</score>
|</student>
|<student stuNo="7" gender="1">
|<name>李四</name>
|<address>中国</address>
|<score>90</score>
|</student>
|<student stuNo="8" gender="1">
|<name>学生8</name>
|<address>中国</address>
|<score>98</score>
|</student>
|<student stuNo="9" gender="1">
|<name>学生9</name>
|<address>中国</address>
|<score>99</score>
|</student>
|</students>
|

