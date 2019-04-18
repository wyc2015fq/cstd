# HQL实现单一 属性与多个属性的查询 - z69183787的专栏 - CSDN博客
2013年11月07日 11:27:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3147
一、在hibernate_hql项目中，继续上一个博文
二、创建测试类
```java
package com.bjsxt.hibernate;
import java.util.Iterator;
import java.util.List;
import org.hibernate.Query;
import org.hibernate.Session;
import junit.framework.TestCase;
public class SimplePropertyQueryTest extends TestCase {
 
 
 public void testQuery1(){
  Session session=null;
  try{
   session=HibernateUtils.getSession();
   session.beginTransaction();
   
   
   List student=session.createQuery("select name from Student").list();
   
   
   Iterator iterator=student.iterator();
   while(iterator.hasNext()){
    //查询方法返回结果集属性列表，元素的类型和实体类中的相应属性类型一致
     String name=(String)iterator.next();
     System.out.println(name);
   }
   
   session.getTransaction().commit();
  }catch(Exception e){
   e.printStackTrace();
  }finally{
   session.getTransaction().rollback();
  }
 }
}
```
三、第二个测试方法
```java
public void testQuery2(){
  Session session=null;
  try{
   session=HibernateUtils.getSession();
   session.beginTransaction();
   
   
   
   //查询多个属性，其集合类型是数组
   //数组元素的类型和对应的属性在实体类中的类型是一致
   //数组的长度取决与select中属性的个数
   List student=session.createQuery("select id,name from Student").list();
   
   
   Iterator iterator=student.iterator();
   while(iterator.hasNext()){
    //
     Object[] obj=(Object[])iterator.next();
    
     System.out.println(obj[0]+" ,"+obj[1]);
   }
   
   session.getTransaction().commit();
  }catch(Exception e){
   e.printStackTrace();
  }finally{
   session.getTransaction().rollback();
  }
 }
```
四、第4个测试方式：以上的两个测试方式，单一 属性返回值为实体类属性的类型
多个属性查询返回值为Object[]也就是对象数组类型的。可是这样的类型，还要根据实体属性的类型来一一对照，显然很麻烦。可是有没有方法，使得返回值为Studnet实体类的对象类型呢？
 这个测试方式就可以实现。只是要在实体类上设置一个构造函数。由于实体类的构造 原则，还要有一个空的构造函数
1、Student.class类如下:
```java
package com.bjsxt.hibernate;
import java.util.Date;
public class Student {
 
 //根据实体类的设计原则，还必须提供一个空的构造 函数
 public Student(){
  
 }
 
 //还要设计一个带参数，一个为id, 另一个为name
 
 public  Student(int id,String name){
  this.id=id;
  this.name=name;HQL实现单一 <wbr>属性与多个属性的查询
 }
 
 private int id;
 
 private String name;
 
 private Date createTime;
 
 private Classes classes;
 
 
 
 
 
 public int getId() {
  return id;
 }
 public void setId(int id) {
  this.id = id;
 }
 public String getName() {
  return name;
 }
 public void setName(String name) {
  this.name = name;
 }
 public Classes getClasses() {
  return classes;
 }
 public void setClasses(Classes classes) {
  this.classes = classes;
 }
 public Date getCreateTime() {
  return createTime;
 }
 public void setCreateTime(Date createTime) {
  this.createTime = createTime;
 }
}
2、测试方法为：
public void testLoad3(){
  Session session=null;
  try{
   session=HibernateUtils.getSession();
   session.beginTransaction();
   
   
   // 如果认为返回对象数组不够对象化，可以采用HQL动态实例化Studnt对象
   //此时list为student对象集合
   List HQL实现单一 <wbr>属性与多个属性的查询students=session.createQuery("select new StudentHQL实现单一 <wbr>属性与多个属性的查询(id,name) from Student").list();
   Iterator iterator=students.iterator();
   while(iterator.hasNext()){HQL实现单一 <wbr>属性与多个属性的查询
    Student student=HQL实现单一 <wbr>属性与多个属性的查询(Student)iterator.next();
    System.out.println(student.getId()+","+student.getName());
    
   }
   
   session.getTransaction().commit();
  }catch(Exception e){
   e.printStackTrace();
   session.getTransaction().rollback();
  }finally{
   HibernateUtils.closeSession(session);
  }
 }
```
四、使用别名的测试类
```java
/*
  * 在HQL 中也可以使用别名的
  */
 public void testQuery4(){
  Session session=null;
  try{
   session=HibernateUtils.getSession();
   session.beginTransaction();
   
   
   //查询多个属性，其集合类型是数组
   //数组元素的类型和对应的属性在实体类中的类型是一致
   //数组的长度取决与select中属性的个数
   //s为Studnet的别名哦！
   List student=session.createQuery("select s.id,s.name from Student s").list();
   
   
   Iterator iterator=student.iterator();
   while(iterator.hasNext()){
    //
     Object[] obj=(Object[])iterator.next();
    
     System.out.println(obj[0]+" ,"+obj[1]);
   }
   
   session.getTransaction().commit();
  }catch(Exception e){
   e.printStackTrace();
  }finally{
   session.getTransaction().rollback();
  }
 }
```
五、
```java
public void testQuery5(){
  Session session=null;
  try{
   session=HibernateUtils.getSession();
   session.beginTransaction();
   
   
   //查询多个属性，其集合类型是数组
   //数组元素的类型和对应的属性在实体类中的类型是一致
   //数组的长度取决与select中属性的个数
   //s为Studnet的别名哦！可以使用as命名别名
   List student=session.createQuery("select s.id,s.name from Student as s").list();
   
   
   Iterator iterator=student.iterator();
   while(iterator.hasNext()){
    //
     Object[] obj=(Object[])iterator.next();
    
     System.out.println(obj[0]+" ,"+obj[1]);
   }
   
   session.getTransaction().commit();
  }catch(Exception e){
   e.printStackTrace();
  }finally{
   session.getTransaction().rollback();
  }
 }
```
