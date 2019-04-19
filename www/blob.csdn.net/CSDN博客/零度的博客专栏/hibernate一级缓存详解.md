# hibernate一级缓存详解 - 零度的博客专栏 - CSDN博客
2016年04月29日 11:27:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：2533
一级缓存的生命周期和session的生命周期一致，当前sessioin一旦关闭，一级缓存就消失，session间不能共享一级缓存的数据，因此一级缓存也叫session级的缓存或事务级缓存。一级缓存只存实体对象的 ，它不会缓存一般的对象属性（查询缓存可以），即当获得对象后，就将该对象的缓存起来，如果在同一session中如果再去获取这个对象时，它会先判断缓存中有没有该对象的id，如果有就直接从缓存中取出，反之则去数据库中取，取的同时将该对象的缓存起来，有以下方法可以支持一级缓存：
•get()
•save()
•load()
•iterate（查询实体对象）
Query和Criteria的list()和uniqueResult()只会写缓存，但不会从缓存中读取（除非结合查询缓存）。一级缓存无法取消，但可以管理，可以使用session.clear()、session.evict()清除或驱逐。
以下对hibernate的一级缓存进行代码测试：hibernate.cfg.xml配置：
<!DOCTYPE  hibernate-configuration PUBLIC  "-//Hibernate/Hibernate Configuration DTD 3.0//EN" "[http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd](http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd)">
<hibernate-configuration> 
  <session-factory> 
    <property name="hibernate.connection.url"> jdbc:mysql://localhost/hibernate_testcache </property>
    <property name="hibernate.connection.driver_class"> com.mysql.jdbc.Driver</property> 
    <property name="hibernate.connection.username">root</property>
    <property name="hibernate.connection.password">root</property> 
    <property name="hibernate.dialect"> org.hibernate.dialect.MySQLDialect </property> 
    <property name="hibernate.show_sql">true</property> 
    <mapping resource="com/sunyzc/hibernate/testcache/User.hbm.xml"/>
  </session-factory> 
</hibernate-configuration> 
实体Bean：User.java
package
com.sunyzc.hibernate.testcache; 
public class User { 
  private int id; 
  private String name; 
  //getters、setters 
} 
User.hbm.xml
<?xml version="1.0"?> 
<!DOCTYPE hibernate-mapping PUBLIC "-//Hibernate/Hibernate Mapping DTD 3.0//EN"  "[http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd](http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd)">
<hibernate-mapping> 
  <class name="com.sunyzc.hibernate.testcache.User" table="t_user">
    <id name="id"> 
      <generator class="native"/> 
    </id> 
    <property name="name" column="name" type="java.lang.String"/>
  </class> 
</hibernate-mapping> 
创建表的工具：ExportDB.java
package com.sunyzc.hibernate.testcache; 
import org.hibernate.cfg.Configuration; 
import org.hibernate.tool.hbm2ddl.SchemaExport; 
 
public class ExportDB { 
  public static  void main(String[] args) { 
    //读取hibernate.cfg.xml文件 
    Configuration cfg = new Configuration().configure();
    SchemaExportexport = new SchemaExport(cfg); 
    export.create(true,true);  
  } 
} 
HibernateUtils.java
package com.sunyzc.hibernate.testcache; 
import org.hibernate.Session; 
import org.hibernate.SessionFactory; 
import org.hibernate.cfg.Configuration; 
public class HibernateUtils { 
  private static SessionFactory factory; 
  static{ 
    try{ 
      factory = new Configuration().configure().buildSessionFactory(); 
    }catch(Exception e) { 
      e.printStackTrace();  
    }  
  } 
  public static Session getSession() { 
    return factory.openSession();  
  } 
  public static void closeSession(Session session) { 
    if(session != null&& session.isOpen()) {  
      session.close();  
    } 
  } 
} 
测试1：在同一个session中发出两次load查询
public void testCache1() { 
  Session session = null; 
  try{ 
    session= HibernateUtils.getSession(); 
    session.beginTransaction(); 
    User user = (User) session.load(User.class,1); 
    System.out.println("user.name="+ user.getName()); 
    //不会发出sql，因为load使用缓存  
    user= (User) session.load(User.class,1); 
    System.out.println("user.name="+ user.getName()); 
    session.getTransaction().commit();  
  }catch(Exception e) { 
    e.printStackTrace();  
    session.getTransaction().rollback();  
  }
finally
{  
    HibernateUtils.closeSession(session); 
  } 
} 
输出结果：
Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
 user.name=user0
 user.name=user0
测试2：在同一个session中发出两次get查询
public void testCache2() { 
  Session session = null;  
  try{ 
    session= HibernateUtils.getSession(); 
    session.beginTransaction(); 
    User user = (User) session.get(User.class,1); 
    System.out.println("user.name="+ user.getName()); 
    //不会发出sql，因为get使用缓存 
    user = (User) session.get(User.class,1); 
    System.out.println("user.name="+ user.getName()); 
    session.getTransaction().commit(); 
  }catch(Exception e) { 
    e.printStackTrace(); 
    session.getTransaction().rollback();  
  }finally{ 
    HibernateUtils.closeSession(session); 
  } 
} 
输出结果：
Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
 user.name=user0
 user.name=user0
测试3：在同一个session中发出两次iterate查询实体对象
public void testCache3() { 
  Session session = null;  
  try{ 
    session= HibernateUtils.getSession(); 
    session.beginTransaction();  
    String hql = "from User u where u.id=1";  
    User user = (User) session.createQuery(hql).iterate().next();  
    System.out.println("user.name="+ user.getName()); 
    //会发出查询id的sql，不会发出查询实体对象的sql，因为iterate使用缓存 
    user= (User) session.createQuery(hql).iterate().next();
    System.out.println("user.name="+ user.getName());
    session.getTransaction().commit();  
  }catch(Exception e) { 
    e.printStackTrace(); 
    session.getTransaction().rollback();  
  }finally{ 
    HibernateUtils.closeSession(session); 
  } 
} 
输出结果：
 Hibernate: select user0_.id as col_0_0_ from t_user user0_ where user0_.id=1
 Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
 user.name=user0
 Hibernate: select user0_.id as col_0_0_ from t_user user0_ where user0_.id=1
 user.name=user0
测试4：在通一个session中发出两次uniqueResult查询实体对象不会缓存，如果第一次用uniqueResult 第二次用iterate查询是会缓存的
public void testCache4() { 
  Session session = null; 
  try{ 
    session = HibernateUtils.getSession(); 
    session.beginTransaction(); 
    String hql = "from User u where u.id=1"; 
    User user = (User) session.createQuery(hql).uniqueResult(); 
    System.out.println("user.name="+ user.getName()); 
    user= (User) session.createQuery(hql).uniqueResult(); 
    System.out.println("user.name="+ user.getName()); 
    session.getTransaction().commit(); 
  }catch(Exception e) { 
    e.printStackTrace(); 
    session.getTransaction().rollback();  
  }finally{ 
    HibernateUtils.closeSession(session); 
  } 
18 
} 
输出结果：
 Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_ where user0_.id=1
 user.name=user0
 Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_ where user0_.id=1
 user.name=user0
测试5：在同一个session中发出两次list查询实体对象不会缓存
public void testCache4() { 
  Session session = null;  
  try{  
    session= HibernateUtils.getSession();  
    session.beginTransaction(); 
    List list = session.createQuery("from User u").list(); 
    System.out.println("user.name="+ list.get(0).getName()); 
    list= session.createQuery("from User u").list(); 
    System.out.println("user.name="+ list.get(0).getName());  
    session.getTransaction().commit(); 
  }catch(Exception e) { 
    e.printStackTrace();  
    session.getTransaction().rollback();  
  }finally{ 
    HibernateUtils.closeSession(session); 
  } 
} 
输出结果：
 Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_
 user.name=user0
 Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_
 user.name=user0
测试6：在同一个session中发出两次iterate查询普通属性不会缓存
public void testCache4() { 
  Session session = null; 
  try{  
    session= HibernateUtils.getSession(); 
    session.beginTransaction(); 
    String hql = "select u.name from User u where u.id=1"; 
    String name = (String) session.createQuery(hql).iterate().next(); 
    System.out.println("user.name="+ name); 
    //iterate查询普通属性，一级缓存不会缓存，所以发出sql 
    //一级缓存是缓存实体对象的 
    name = (String) session.createQuery(hql).iterate().next(); 
    System.out.println("user.name="+ name); 
    session.getTransaction().commit();  
  }catch(Exception e) { 
    e.printStackTrace(); 
    session.getTransaction().rollback(); 
  }finally{ 
    HibernateUtils.closeSession(session); 
  } 
} 
输出结果：
 Hibernate: select user0_.name as col_0_0_ from t_user user0_ where user0_.id=1
 user.name=user0
 Hibernate: select user0_.name as col_0_0_ from t_user user0_ where user0_.id=1
 user.name=user0
测试7：开启两个session中发出load查询
public void testCache7() { 
  Session session1 = null;  
  Session session2 = null;  
  try{ 
    session1 = HibernateUtils.getSession(); 
    session2 = HibernateUtils.getSession(); 
    session1.beginTransaction(); 
    session2.beginTransaction(); 
    User user1 = (User) session1.load(User.class,1); 
    System.out.println("user1.name="+ user1.getName()); 
    //会发出查询语句，session间不能共享一级缓存的数据 
    //因为它会伴随session的生命周期存在和消亡 
    User user2 = (User) session2.load(User.class,1); 
    System.out.println("user2.name="+ user2.getName());
    session1.getTransaction().commit(); 
    session2.getTransaction().commit();  
  }catch(Exception e) { 
    e.printStackTrace(); 
    session1.getTransaction().rollback(); 
    session2.getTransaction().rollback(); 
  }finally{ 
    HibernateUtils.closeSession(session1); 
    HibernateUtils.closeSession(session2); 
  } 
} 
输出结果：
 Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
 user1.name=user0
 Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
 user2.name=user0
测试8：在同一个session中先save，再发出load查询save过的数据
public void testCache8() { 
  Session session = null;  
  try{  
    session= HibernateUtils.getSession(); 
    session.beginTransaction();  
    User u = new User();  
    u.setName("张三");  
    Serializable id = session.save(u);  
    //不会发出sql，因为save是使用缓存的  
    User user = (User) session.load(User.class,id); 
    System.out.println("user.name="+ user.getName()); 
    session.getTransaction().commit();  
  }catch(Exception e) {  
    e.printStackTrace();  
    session.getTransaction().rollback();  
  }finally{ 
    HibernateUtils.closeSession(session); 
  }  
} 
输出结果：
 Hibernate: insert into t_user (name) values (?)
 user.name=张三
如何避免一次性大量的实体数据入库导致内存溢出？
先flush()，再clear()。 save方法会将save的对象放入一级缓存中，如果要save大批对象，则应该要及时清空一级缓存，可以采用Session的clear()方法。若不清理缓存，可发现javaw进程占用内存持续增长，到一定程度会内存溢出的。如果数据量特别大，考虑采用jdbc实现，如果jdbc也不能满足要求可以考虑采用数据本身的特定导入工具。
测试9：向数据库中批量加入1000条数据
public void testCache9() { 
Session session = null; 
  try{  
    session = HibernateUtils.getSession();  
    session.beginTransaction(); 
    for(int i = 0;i < 1000; i++) { 
      User user = new User(); 
      user.setName("user"+ i); 
      session.save(user); 
      //每20条数据就强制session将数据持久化 
      //同时清除缓存，避免大量数据造成内存溢出 
      if(i % 20== 0){ 
        session.flush(); 
        session.clear(); 
      } 
    }  
    session.getTransaction().commit();  
  }catch(Exception e) { 
    e.printStackTrace();  
    session.getTransaction().rollback();  
  }finally{ 
    HibernateUtils.closeSession(session); 
  } 
} 
