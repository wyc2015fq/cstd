# hibernate注解方式实现一对多映射 - z69183787的专栏 - CSDN博客
2012年11月18日 19:55:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1035

首先是在hibernate.cfg.xml中引入要映射的class
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE hibernate-configuration PUBLIC
"-//Hibernate/Hibernate Configuration DTD 3.0//EN"
"http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">
<hibernate-configuration>
    <session-factory>
        <property name="hibernate.connection.driver_class">com.mysql.jdbc.Driver</property>
        <property name="hibernate.connection.url">jdbc:mysql://localhost:3306/test?useUnicode=true&characterEncoding=GBK</property>
        <property name="hibernate.connection.username">root</property>
        <property name="hibernate.connection.password">sa</property>
        <property name="hibernate.dialect">org.hibernate.dialect.MySQLDialect</property>
        <property name="show_sql">true</property>
        <property name="hibernate.hbm2ddl.auto">update</property>
        <mapping class="com.sszd.hibernate.Reader"/>
        <mapping class="com.sszd.hibernate.Book"/> 
    </session-factory>
</hibernate-configuration>
我们分别看看reader.java和book.java文件
**Reader.java**
package com.sszd.hibernate;
import java.util.HashSet;
import java.util.Set;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;
@Entity
@Table(name="reader")
public class Reader {
 @Id
 @Column(name="ID")
 @GeneratedValue(strategy=GenerationType.AUTO)
 private int id;
 @Column(name="MENO")
 private String meno;
 @Column(name="NAME")  
 private String name;
 @OneToMany(mappedBy = "reader", cascade = CascadeType.ALL, fetch = FetchType.LAZY)
 private Set<Book> books = new HashSet<Book>();
 public Reader()
 {
 }
 public Set getBooks() {
  return books;
 }
 public void setBooks(Set books) {
  this.books = books;
 }
 public int getId() {
  return id;
 }
 public void setId(int id) {
  this.id = id;
 }
 public String getMeno() {
  return meno;
 }
 public void setMeno(String meno) {
  this.meno = meno;
 }
 public String getName() {
  return name;
 }
 public void setName(String name) {
  this.name = name;
 }
}
**book.java**
package com.sszd.hibernate;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
@Entity
@Table(name="book")
public class Book {
 @Id
 @Column(name="ID")
 @GeneratedValue(strategy=GenerationType.AUTO) 
 private int id;
 @Column(name="TITLE")
 private String title;
 @ManyToOne(cascade = CascadeType.ALL, optional = false)
 @JoinColumn(name="rea_id", referencedColumnName="id")//外键为sut_id，与student中的id关联
 private Reader reader;
 public int getId() {
  return id;
 }
 public Reader getReader() {
  return reader;
 }
 public void setReader(Reader reader) {
  this.reader = reader;
 }
 public void setId(int id) {
  this.id = id;
 }
 public String getTitle() {
  return title;
 }
 public void setTitle(String title) {
  this.title = title;
 }
}
接下来是我们的本地测试用例
package com.sszd.main;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.AnnotationConfiguration;
import com.sszd.hibernate.Book;
import com.sszd.hibernate.Reader;
public class ReaderToBooks {
 private static  SessionFactory sessionFactory;
      public static void main(String[] args) {
   sessionFactory = new AnnotationConfiguration().configure().buildSessionFactory();
Session session = sessionFactory.openSession();
Reader r = new Reader();
r.setName("Reader zhang");
Book b1 = new Book();
b1.setTitle("title1");
Book b2 = new Book();
b2.setTitle("title2");
b1.setReader(r);
b2.setReader(r);
r.getBooks().add(b1);
r.getBooks().add(b2);
Transaction trx = session.beginTransaction();
session.save(r);
trx.commit();
session.close();
}
}
