# Hibernate 一对多注解 mappedby 作用 - z69183787的专栏 - CSDN博客
2012年11月18日 20:12:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：32072
package oneToMany; 
import java.util.Set; 
import javax.persistence.*; 
/* 
注意导入时，是导入：import javax.persistence.*;    
非导入org.hibernate的相关类：import org.hibernate.annotations.Entity; 
*/
@Entity 
@Table(name="classes") 
publicclass Classes implements Serializable { 
  @Id 
  @GeneratedValue(strategy=GenerationType.AUTO) 
privateint id; 
private String name; 
  @OneToMany(cascade=CascadeType.ALL,mappedBy="classes")    
private Set<Student> students; 
//getter,setter省略 
} 
package oneToMany; 
import javax.persistence.*; 
@Entity 
@Table(name="student") 
publicclass Student implements Serializable  { 
  @Id 
  @GeneratedValue(strategy=GenerationType.AUTO) 
privateint sid; 
private String sname; 
//若有多个cascade，可以是：{CascadeType.PERSIST,CascadeType.MERGE} 
  @ManyToOne(cascade={CascadeType.ALL})         
  @JoinColumn(name="classid")     //student类中对应外键的属性：classid 
private Classes classes; 
//getter,setter省略 
} 
publicclass TestOneToMany { 
/* 
CREATE TABLE    student (    --要定义外键!!!!!!! 
    `sid` double NOT NULL auto_increment, 
    `classid` double NULL, 
    `sname` varchar(255) NOT NULL, 
    PRIMARY KEY    (sid), 
    INDEX par_ind (classid), 
    FOREIGN KEY (classid) REFERENCES classes(id) ON DELETE CASCADE ON UPDATE CASCADE 
) ENGINE=InnoDB 
*/
publicstaticvoid main(String[] args) throws SQLException    
  { 
try
    { 
      SessionFactory sf = new AnnotationConfiguration().configure().buildSessionFactory(); 
      Session session=sf.openSession(); 
      Transaction tx=session.beginTransaction();         
/*
因为mappedBy是定义在classes中,即classes类不负责维护级联关系.即维护者是student.所以,
1.要将clsses的数据,赋给student,即用student的setClasses()方法去捆定class数据;
2.在进行数据插入/更新session.save()/session.update()时,最后操作的是student.
*/
      Classes classes=new Classes(); 
      classes.setName("access"); 
      Student st1=new Student(); 
      st1.setSname("jason"); 
      st1.setClasses(classes); 
      session.save(st1); 
      Student st2=new Student(); 
      st2.setSname("hwj"); 
      st2.setClasses(classes); 
      session.save(st2); 
      tx.commit();
/* 
输出如下:
Hibernate: insert into classes (name) values (?)
Hibernate: insert into student (classid, sname) values (?, ?)
Hibernate: insert into student (classid, sname) values (?, ?)
*/
/*
因为一端维护关系另一端不维护关系的原因，我们必须注意避免在应用中用不维护关系的类(class)建立关系，因为这样建立的关系是不会在数据库中存储的。
如上的代码倒过来,则插入时,student的外键值为空.如下:
*/
//      Student st1=new Student(); 
//      st1.setSname("jason"); 
//      session.save(st1); 
//        
//      Student st2=new Student(); 
//      st2.setSname("hwj"); 
//      session.save(st2); 
//        
//      Set<Student> students=new HashSet<Student>(); 
//      students.add(st1); 
//      students.add(st2); 
//        
//      Classes classes=new Classes(); 
//      classes.setName("access"); 
//      classes.setStudents(students); 
//      session.save(classes); 
/*
输出如下:
Hibernate: insert into student (classid, sname) values (?, ?)
Hibernate: insert into student (classid, sname) values (?, ?)
Hibernate: insert into classes (name) values (?)
*/
    } 
catch(HibernateException e) 
    { 
      e.printStackTrace();        
    } 
  } 
}
