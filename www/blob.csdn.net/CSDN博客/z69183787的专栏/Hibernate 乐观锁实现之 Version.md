# Hibernate 乐观锁实现之 Version - z69183787的专栏 - CSDN博客
2014年04月03日 06:50:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8575
通过在表中及POJO中增加一个Timestamp字段来表示记录的最后更新时间，来达到多用户同时更改一条数据的冲突，这个timestamp由数据库自动添加，无需人工干预
数据库结构：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)package com.ematchina.test;
import java.sql.Timestamp;
import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
@Entity
@Table(name = "book")
@Cache(usage=CacheConcurrencyStrategy.READ_WRITE)
public class Book {
    private int id;    
    //private Timestamp lastDate;    
    private String name;    
    private int price;
    private int version;
    @Id
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    @Column(name = "id", length = 11)
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    @Version
    @Column(name = "version",length = 11)
    public int getVersion() {
        return version;
    }
    public void setVersion(int version) {
        this.version = version;
    }
    @Column(name = "name",length = 10)
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    @Column(name = "price",length = 11)
    public int getPrice() {
        return price;
    }
    public void setPrice(int price) {
        this.price = price;
    }
    /*@Version
    @Temporal(TemporalType.TIMESTAMP)
    @Column(name = "lastDate")
    public Date getLastDate() {
        return lastDate;
    }
    public void setLastDate(Timestamp lastDate) {
        this.lastDate = lastDate;
    }*/
}
Hibernate.cfg.xml
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<?xml version='1.0' encoding='UTF-8'?>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<!DOCTYPE hibernate-configuration PUBLIC
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)          "-//Hibernate/Hibernate Configuration DTD 3.0//EN"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)          "http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<!-- Generated by MyEclipse Hibernate Tools.                   -->
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<hibernate-configuration>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<session-factory>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="connection.username">root</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="connection.url">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        jdbc:mysql://localhost:3306/schoolproject?characterEncoding=gb2312&useUnicode=true
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="dialect">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        org.hibernate.dialect.MySQLDialect
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="myeclipse.connection.profile">mysql</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="connection.password">1234</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="connection.driver_class">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        com.mysql.jdbc.Driver
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="hibernate.dialect">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        org.hibernate.dialect.MySQLDialect
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="hibernate.show_sql">true</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="current_session_context_class">thread</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<property name="jdbc.batch_size">15</property>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</session-factory>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</hibernate-configuration>
测试代码：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)package Timestamp;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)import java.io.File;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)import java.util.Iterator;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)import java.util.Set;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)import org.hibernate.Session;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)import org.hibernate.SessionFactory;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)import org.hibernate.Transaction;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)import org.hibernate.cfg.Configuration;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)publicclass Test {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)   @org.junit.Test
    public void main() {
        /*
         * String filePath = System.getProperty("user.dir") + File.separator +
         * "src/Timestamp" + File.separator + "hibernate.cfg.xml"; File file =
         * new File(filePath);
         */
        // System.out.println(filePath);
        Session session = sessionFactory.openSession();
        Transaction t = session.beginTransaction();
        Book stu = new Book();
        stu.setName("tom11");
        session.save(stu);
        t.commit();
        /*
         * 模拟多个session操作student数据表
         */
        Session session1 = sessionFactory.openSession();
        Session session2 = sessionFactory.openSession();
        Book stu1 = (Book) session1.createQuery("from Book b where b.name='liy'").uniqueResult();
        Book stu2 = (Book) session2.createQuery("from Book b where b.name='liy'").uniqueResult();
        System.out.println(stu1.getVersion());
        System.out.println(stu2.getVersion());
        // 这时候，两个版本号是相同的
        /*
         * System.out.println("v1=" + stu1.getLastDate() + "--v2=" +
         * stu2.getLastDate());
         */
        Transaction tx1 = session1.beginTransaction();
        stu1.setName("session1");
        tx1.commit();
        // 这时候，两个版本号是不同的，其中一个的版本号递增了
        /*
         * System.out.println("v1=" + stu1.getLastDate() + "--v2=" +
         * stu2.getLastDate());
         */
        System.out.println(stu1.getVersion());
        System.out.println(stu1.getName());
        System.out.println(stu2.getVersion());
        Transaction tx2 = session2.beginTransaction();
        stu2.setName("session2");
        tx2.commit();
    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
