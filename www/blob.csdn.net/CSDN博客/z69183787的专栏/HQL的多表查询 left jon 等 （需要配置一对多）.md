# HQL的多表查询 left jon 等 （需要配置一对多） - z69183787的专栏 - CSDN博客
2014年01月15日 19:09:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：17910
对象之间总是有各种各样的关系，关联关系是类之间最常见的关系。多表查询是HQL中的强大功能之一，包括内连接、左连接和右连接等。多表查询的设置及运行都比较麻烦，在运行本节中的示例时，务必保证每一步都没有错误。 
6.4.1  表之间的关联关系 
在数据库joblog中用到了3个表：student（学生表）、course（课程表）和sc（选课表）。这些表的详细信息见6.1.1节“示例中用到的默认数据库表和数据”。在现实模型中，一个学生可以选择多门课程，一个课程可以被多个学生选择，student和course是多对多的关联关系。为了便于演示HQL的多表查询，本节中假设student和course之间是单向关联关系。 
在多对多的关联关系中，一般来说有个中间表，这个表描述了多对多关系，这就是选课表sc，sc每一行数据代表一个学生的选课和成绩。 
各个表的主键、外键设置如下。 
student表的主键是id字段。 
course表的主键是id字段。 
sc表的主键是id字段。 
sc表中的Sno字段是student表id字段的外键。 
sc表中的Cno字段是course表id字段的外键。 
图6-8是3个表之间关系的直观表示。 
图6-8  3个表之间的关系 
在MySQL Query Browser中设置好上述关系。如果此处设置不正确，可能会影响多表连接查询。其中sc表的建表信息如下（其中包含了外键关系）。 
```
CREATE TABLE  'joblog'. 'sc' ( 
'id' int(10) unsigned NOT NULL auto_increment COMMENT 'id', 
'Sno' int(10) unsigned NOT NULL default '0' COMMENT '学号', 
'Cno' int(10) unsigned NOT NULL default '0' COMMENT '课程号', 
'Grade' int(10) unsigned default NULL COMMENT '成绩', 
PRIMARY KEY  ('id'), 
  KEY 'FK_sc_1' ('Sno'), 
  KEY 'FK_sc_2' ('Cno'), 
  CONSTRAINT 'FK_sc_1' FOREIGN KEY ('Sno') REFERENCES 'student' ('id'),  /* 外键信息 */ 
  CONSTRAINT 'FK_sc_2' FOREIGN KEY ('Cno') REFERENCES 'course' ('id')   /* 外键信息 */ 
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;
```
6.4.2  表中的数据 
这一节中用到了3个表的数据，student表和course表的数据如6.1节中图6-2和6-4所示，但是sc表的内容变为图6-9所示的数据，其中Sno和Cno存储的分别是student表和course表中对应的主键值。 
图6-9  本节中所用的表sc中的内容 
6.4.3  修改持久化类 
Student对象和Course对象之间是多对多的关系。此处使用的是单向关联，仅仅建立从Student到Course的单向关联。如图6-10所示，仅有Student到Course的单向关联。 
图6-10  Student到Course类的单向关联 
为了建立Student到Course的单向关联关系，在Student.java中新加一个属性course。course属性是Set型的，可以在这个属性中加入多个Course对象，建立起关联关系。下面是加入course属性后的源代码，粗体部分为加入的代码。 
```java
package hibernate.ch06; 
import java.util.HashSet; 
import java.util.Set; 
public class Student  implements java.io.Serializable { 
       private Integer id;            //ID 
       private Integer sno;           //学号 
       private String sname;          //姓名 
       private String ssex;           //性别 
       private String sdept;          //系部 
       private Integer sage;          //年龄 
       private String saddress;      //住址 
       private Set course=new HashSet();    //所选课程 
      public Student() { 
} 
//此处省略其他的构造方法 
//此处省略getter/setter访问器 
//course属性的get访问器 
   public Set getCourse() { 
         return course; 
   } 
   //course属性的set访问器 
   public void setCourse(Set course) { 
         this.course = course; 
   } 
}
```
持久化类Course.java和SC.java无需修改。 
6.4.4 在映射文件中加入关联信息 
在Student.hbm.xml映射配置文件中，加入Student到Course的映射信息。关于如何映射关联关系，将在第8章讲解，读者可暂时按照下面的设置，具体含义等阅读完第八章便可理解。具体代码如下。 
```
<set name="course" table="sc" lazy="false" cascade="save-update"> 
          <key column="sno" /> 
          <many-to-many class="hibernate.ch06.Course" column="cno" /> 
        </set>
```
说明如下。 
     <set>元素是和<class>元素平行的元素。<set>元素表明将要映射的字段对应着一个集合。<set>元素包含多个属性，其中：name属性用于设置映射的持久化类的属性名称，在本例中为Student表的course属性；table属性表示多对多关联关系的中间表名称，此处为sc表；cascade表示当保存或者更新Student实例时，是否保存或更新Course
 对象。 
     <set>元素的子元素<key column="sno" />设定与student表关联的中间表sc的外键sno。 
     <set>元素的子元素<many-to-many>用于设定多对多关系。在该元素中，class属性用于设定多对多关系中，与Student类关联的类Course类；column属性设定中间表与course表连接的外键cno。 
完整的配置文件Student.hbm.xml如下所示。 
```
<?xml version="1.0"?> 
<!DOCTYPE hibernate-mapping PUBLIC "-//Hibernate/Hibernate Mapping DTD 3.0//EN" 
"http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd"> 
<hibernate-mapping> 
     <class name="hibernate.ch06.Student" table="student" catalog="joblog"> 
          <id name="id" type="integer"> 
                <column name="id" /> 
                <generator class="identity"></generator> 
          </id> 
          <!--映射学号--> 
          <property name="sno" type="integer"> 
                <column name="Sno" not-null="true" /> 
          </property> 
          <!--映射姓名--> 
          <property name="sname" type="string"> 
               <column name="Sname" length="45" /> 
          </property> 
          <!--映射系部--> 
          <property name="sdept" type="string"> 
               <column name="Sdept" length="10" /> 
          </property> 
          <!--映射年龄--> 
          <property name="sage" type="integer"> 
               <column name="Sage" /> 
          </property> 
          <!--映射性别--> 
          <property name="ssex" type="string"> 
               <column name="Ssex" length="2" /> 
          </property> 
          <!--映射住址--> 
          <property name="saddress" type="string"> 
               <column name="Saddress" length="45" /> 
          </property> 
          <!--联接--> 
          <set name="course" table="sc" lazy="false" cascade="save-update"> 
              <key column="sno" /> 
              <many-to-many class="hibernate.ch06.Course" column="cno" />    <!--多对多--> 
          </set> 
      </class> 
</hibernate-mapping>
```
6.4.5  左外连接 
左外连接(Left Outer Join)查询出左表对应的复合条件的所有记录，如查询李晓梅同学的选课信息。下面是类HQLLeftOuterJoinQuery的源代码。 
```java
package hibernate.ch06; 
import hibernate.HibernateSessionFactory; 
import java.util.Iterator; 
import java.util.List; 
import org.hibernate.Query; 
import org.hibernate.Session; 
public class HQLLeftOuterJoinQuery { 
      public static void main(String[] args) { 
            Session session=HibernateSessionFactory.currentSession(); 
            //HQL查询语句 
            String hql="from Student s left join s.course c where s.sname='李晓梅'"; 
            Query query=session.createQuery(hql);         //创建查询 
            List list=query.list();                          //执行查询 
            Iterator it=list.iterator(); 
            while(it.hasNext()){ 
                  Object[] obj=(Object[])it.next(); 
                  Student stu=(Student)obj[0]; 
                  Course course=(Course)obj[1]; 
                  System.out.println("*********学生信息及其选课信息******************"); 
                  if(course!=null){ 
                   System.out.println(stu.getSno()+"/t"+stu.getSname()+"/t"+ 
                                "课程:"+course.getCname()); 
                   }else{ 
                         System.out.println(stu.getSno()+"/t"+stu.getSname()+"/t"); 
                   }; 
            } 
      } 
}
```
如果只用单表查询，只能从student表中查询出李晓梅的个人信息，而无法知道她的选课信息，因为选课信息存储在中间表sc中。HQL语句from Student s left join s.course c where s.sname='李晓梅'检索出了李晓梅的选课信息。 
     在HQL中使用left outer join关键字进行左外连接，outer关键字可以省略。 
     s.course是Student对象中的一个属性，用来存储Student对象的选课信息。在执行查询时，将根据Student.hbm.xml中的配置生成SQL语句，并检索信息。 
     查询的结果返回一个Object[]数组，数组的第0个元素是Student对象，第1个元素是与Object[0]中对应的学生所选课的Course对象。 
HQLLeftOuterJoinQuery类在执行过程中生成的左外连接的SQL语句如下。 
```
Hibernate: 
    select 
         student0_.id as id1_0_, 
         course2_.id as id4_1_, 
         student0_.Sno as Sno1_0_, 
         student0_.Sname as Sname1_0_, 
         student0_.Sdept as Sdept1_0_, 
         student0_.Sage as Sage1_0_, 
         student0_.Ssex as Ssex1_0_, 
         student0_.Saddress as Saddress1_0_, 
         course2_.Cno as Cno4_1_, 
         course2_.Cname as Cname4_1_, 
         course2_.Ccredit as Ccredit4_1_ 
    from 
         joblog.student student0_ 
    left outer join 
         sc course1_ 
              on student0_.id=course1_.sno 
    left outer join 
         joblog.course course2_ 
              on course1_.cno=course2_.id 
    where 
         student0_.Sname='李晓梅' 
Hibernate: 
    select 
         course0_.sno as sno1_, 
         course0_.cno as cno1_, 
         course1_.id as id4_0_, 
         course1_.Cno as Cno4_0_, 
         course1_.Cname as Cname4_0_, 
         course1_.Ccredit as Ccredit4_0_ 
    from 
         sc course0_ 
  left outer join 
         joblog.course course1_ 
               on course0_.cno=course1_.id 
    where 
         course0_.sno=?
```
程序的查询结果如下。 
*********学生信息及其选课信息****************** 
20040001    李晓梅    课程:数据库 
*********学生信息及其选课信息****************** 
20040001    李晓梅    课程:操作系统 
使用如下语句将只返回Student对象。 
select s from Student s left join s.course c where s.sname='李晓梅' 
如下是只返回Student对象的部分代码。 
```java
Session session=HibernateSessionFactory.currentSession(); 
        //HQL查询语句 
        String hql="select s from Student s left join s.course c where s.sname='李晓梅'"; 
        Query query=session.createQuery(hql);             //创建查询 
        List list=query.list();                             //执行查询 
        Iterator it=list.iterator(); 
        while(it.hasNext()){
              Student stu=(Student)it.next(); 
              System.out.println("*********学生信息及其选课信息******************"); 
              System.out.println(stu.getSno()+"/t"+stu.getSname()+"/t"); 
        }
```
6.4.6  左外抓取连接 
左外抓取连接指定在Hibernate检索数据时，采用抓取的方式，直接将数据加载到与Student对象关联的course属性中。下面是左外抓取连接的程序。 
        //HQL查询语句 
```java
String hql="select s from Student s left join fetch s.course c where s.sname='李晓梅'"; 
        Query query=session.createQuery(hql);             //创建查询 
        List list=query.list();                             //执行查询 
        Iterator it=list.iterator(); 
        while(it.hasNext()){ 
               Student stu=(Student)it.next(); 
               System.out.println("*********学生信息及其选课信息******************"); 
               System.out.println(stu.getSno()+"/t"+stu.getSname()+"/t"); 
        }
```
     左外抓取连接使用left join fetch关键字。 
     与左外连接不同的是：左外抓取连接query.list()返回的集合中存放Student对象的引用，与之相关联的选课信息存放在course属性中。 
6.4.7  右外连接 
HQL中使用关键字right outer join右外连接，outer关键字可以省略。右外连接与左外连接类似，不再赘述。 
6.4.8  内连接 
内连接（Inner Join）是指两个表中指定的关键字相等的值才会出现在结果集中的一种查询方式。HQL中使用关键字inner join进行内连接，下面是使用内连接的程序。 
```java
Session session=HibernateSessionFactory.currentSession()             //创建Session 
        String hql="from Student s inner join s.course c";                    //HQL查询语句 
        Query query=session.createQuery(hql);                                    //创建查询 
        List list=query.list();                                                    //执行查询 
        Iterator it=list.iterator(); 
        while(it.hasNext()){ 
               Object[] obj=(Object[])it.next(); 
               Student stu=(Student)obj[0]; 
               Course course=(Course)obj[1]; 
               System.out.println("*********学生信息及其选课信息******************"); 
               System.out.println(stu.getSno()+"/t"+stu.getSname()+"/t"+"课程:"+course. getCname()); 
        }
```
     HQL中使用inner join进行内连接，内连接只关联并检索那些选了课的学生信息及其选课信息，没有选课的学生不在检索结果中。 
     可以使用select s from Student s inner join s.course c只返回Student对象。 
6.4.9  抓取内连接 
抓取内连接与内连接不同之处在于其对象的内存状态不一样。HQL中使用inner join fetch进行抓取内连接，如下程序所示。 
```java
Session session=HibernateSessionFactory.currentSession();            //创建Session 
        String hql="select s from Student s inner join fetch s.course c";                //HQL语句 
        Query query=session.createQuery(hql);                                    //创建查询 
        List list=query.list();                                                    //执行查询 
        Iterator it=list.iterator(); 
        while(it.hasNext()){ 
        Student stu=(Student)it.next(); 
        System.out.println("*********学生信息及其选课信息******************"); 
        System.out.println(stu.getSno()+"/t"+stu.getSname()+"/t"); 
    }
```
     内抓取连接使用inner join fech关键字。 
     它与内连接的区别是返回检索的list中存放的是Student对象的引用，与之相关联的选课信息存放在course属性中。
