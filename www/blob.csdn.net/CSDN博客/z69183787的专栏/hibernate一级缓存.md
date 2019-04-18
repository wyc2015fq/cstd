# hibernate一级缓存 - z69183787的专栏 - CSDN博客
2015年01月23日 14:44:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：563
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)

一级缓存的生命周期和session的生命周期一致，当前sessioin一旦关闭，一级缓存就消失，session间不能共享一级缓存的数据，因此一级缓存也叫session级的缓存或事务级缓存。一级缓存只存实体对象的 ，它不会缓存一般的对象属性（查询缓存可以），即当获得对象后，就将该对象的缓存起来，如果在同一session中如果再去获取这个对象时，它会先判断缓存中有没有该对象的id，如果有就直接从缓存中取出，反之则去数据库中取，取的同时将该对象的缓存起来，有以下方法可以支持一级缓存：
- get()
- save()
- load()
- iterate（查询实体对象）
Query和Criteria的list()和uniqueResult()只会写缓存，但不会从缓存中读取（除非结合查询缓存）。一级缓存无法取消，但可以管理，可以使用session.clear()、session.evict()清除或驱逐。
**以下对hibernate的一级缓存进行代码测试：**
hibernate.cfg.xml配置：
|`1`|```<!DOCTYPE hibernate-configuration PUBLIC```|
|`2`|`  ````"-//Hibernate/Hibernate Configuration DTD 3.0//EN"```|
|`3`|`  ``"http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">`|
|`4`||
|`5`|`<``hibernate-configuration``>`|
|`6`|`  ``<``session-factory``>`|
|`7`|`    ``<``property````name````=``"hibernate.connection.url"``>`|
|`8`|`      ``jdbc:mysql://localhost/hibernate_testcache`|
|`9`|`    ``</``property``>`|
|`10`|`    ``<``property````name````=``"hibernate.connection.driver_class"``>`|
|`11`|`      ``com.mysql.jdbc.Driver`|
|`12`|`    ``</``property``>`|
|`13`|`    ``<``property````name````=``"hibernate.connection.username"``>root</``property``>`|
|`14`|`    ``<``property````name````=``"hibernate.connection.password"``>root</``property``>`|
|`15`|`    ``<``property````name````=``"hibernate.dialect"``>`|
|`16`|`      ``org.hibernate.dialect.MySQLDialect`|
|`17`|`    ``</``property``>`|
|`18`|`    ``<``property````name````=``"hibernate.show_sql"``>true</``property``>`|
|`19`||
|`20`|`    ``<``mapping````resource````=``"com/sunyzc/hibernate/testcache/User.hbm.xml"````/>```|
|`21`|`  ``</``session-factory``>`|
|`22`|`</``hibernate-configuration``>`|
实体Bean：User.java
|`1`|`package````com.sunyzc.hibernate.testcache;```|
|`2`||
|`3`|`public````class``````User {```|
|`4`|`  ``private````int``````id;```|
|`5`|`  ``private````String name;```|
|`6`||
|`7`|`  ````// getters、setters```|
|`8`||
|`9`|`}`|
User.hbm.xml
|`1`|`<?``xml````version````=``"1.0"``?>`|
|`2`|```<!DOCTYPE hibernate-mapping PUBLIC```|
|`3`|`  ````"-//Hibernate/Hibernate Mapping DTD 3.0//EN"```|
|`4`|`  ``"http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">`|
|`5`|`<``hibernate-mapping``>`|
|`6`|`  ``<``class````name````=``"com.sunyzc.hibernate.testcache.User"````table````=``"t_user"``>`|
|`7`|`    ``<``id````name````=``"id"``>`|
|`8`|`      ``<``generator````class````=``"native"````/>```|
|`9`|`    ``</``id``>`|
|`10`|`    ``<``property````name````=``"name"````column````=``"name"````type````=``"java.lang.String"````/>```|
|`11`|`  ``</``class``>`|
|`12`|`</``hibernate-mapping``>`|
创建表的工具：ExportDB.java
|`1`|`package````com.sunyzc.hibernate.testcache;```|
|`2`||
|`3`|`import````org.hibernate.cfg.Configuration;```|
|`4`|`import````org.hibernate.tool.hbm2ddl.SchemaExport;```|
|`5`||
|`6`|`public````class``````ExportDB {```|
|`7`|`  ``public````static``````void``````main(String[] args) {```|
|`8`|`    ````// 读取hibernate.cfg.xml文件```|
|`9`|`    ````Configuration cfg =````new````Configuration().configure();```|
|`10`|`    ````SchemaExport export =````new````SchemaExport(cfg);```|
|`11`|`    ``export.create(``true````,````true``);`|
|`12`|`  ``}`|
|`13`|`}`|
HibernateUtils.java
|`1`|`package````com.sunyzc.hibernate.testcache;```|
|`2`||
|`3`|`import````org.hibernate.Session;```|
|`4`|`import````org.hibernate.SessionFactory;```|
|`5`|`import````org.hibernate.cfg.Configuration;```|
|`6`||
|`7`|`public````class``````HibernateUtils {```|
|`8`|`  ``private````static``````SessionFactory factory;```|
|`9`|`  ``static````{```|
|`10`|`    ``try````{```|
|`11`|`      ````factory =````new````Configuration().configure().buildSessionFactory();```|
|`12`|`    ````}````catch````(Exception e) {```|
|`13`|`      ``e.printStackTrace();`|
|`14`|`    ``}`|
|`15`|`  ``}`|
|`16`||
|`17`|`  ``public````static``````Session getSession() {```|
|`18`|`    ``return````factory.openSession();```|
|`19`|`  ``}`|
|`20`||
|`21`|`  ``public````static``````void``````closeSession(Session session) {```|
|`22`|`    ``if````(session !=````null````&& session.isOpen()) {```|
|`23`|`      ``session.close();`|
|`24`|`    ``}`|
|`25`|`  ``}`|
|`26`|`}`|
**测试1：在同一个session中发出两次load查询**
|`1`|`public````void``````testCache1() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ````User user = (User) session.load(User.````class````,````1``);`|
|`7`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`8`|`    ``//不会发出sql，因为load使用缓存`|
|`9`|`    ````user = (User) session.load(User.````class````,````1``);`|
|`10`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`11`|`    ``session.getTransaction().commit();`|
|`12`|`  ````}````catch````(Exception e) {```|
|`13`|`    ``e.printStackTrace();`|
|`14`|`    ``session.getTransaction().rollback();`|
|`15`|`  ````}````finally````{```|
|`16`|`    ``HibernateUtils.closeSession(session);`|
|`17`|`  ``}`|
|`18`|`}`|
**输出结果：**
> 
Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
user.name=user0
user.name=user0
**测试2：在同一个session中发出两次get查询**
|`1`|`public````void``````testCache2() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ````User user = (User) session.get(User.````class````,````1``);`|
|`7`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`8`|`    ````// 不会发出sql，因为get使用缓存```|
|`9`|`    ````user = (User) session.get(User.````class````,````1``);`|
|`10`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`11`|`    ``session.getTransaction().commit();`|
|`12`|`  ````}````catch````(Exception e) {```|
|`13`|`    ``e.printStackTrace();`|
|`14`|`    ``session.getTransaction().rollback();`|
|`15`|`  ````}````finally````{```|
|`16`|`    ``HibernateUtils.closeSession(session);`|
|`17`|`  ``}`|
|`18`|`}`|
**输出结果：**
> 
Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
user.name=user0
user.name=user0
**测试3：在同一个session中发出两次iterate查询实体对象**
|`1`|`public````void``````testCache3() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ````String hql =``````"from User u where u.id=1"````;`|
|`7`|`    ````User user = (User) session.createQuery(hql).iterate().next();```|
|`8`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`9`|`    ````// 会发出查询id的sql，不会发出查询实体对象的sql，因为iterate使用缓存```|
|`10`|`    ````user = (User) session.createQuery(hql).iterate().next();```|
|`11`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`12`|`    ``session.getTransaction().commit();`|
|`13`|`  ````}````catch````(Exception e) {```|
|`14`|`    ``e.printStackTrace();`|
|`15`|`    ``session.getTransaction().rollback();`|
|`16`|`  ````}````finally````{```|
|`17`|`    ``HibernateUtils.closeSession(session);`|
|`18`|`  ``}`|
|`19`|`}`|
**输出结果：**
> 
Hibernate: select user0_.id as col_0_0_ from t_user user0_ where user0_.id=1
Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
user.name=user0
Hibernate: select user0_.id as col_0_0_ from t_user user0_ where user0_.id=1
user.name=user0
**测试4：在通一个session中发出两次uniqueResult查询实体对象不会缓存，如果第一次用uniqueResult第二次用iterate查询是会缓存的**
|`1`|`public````void``````testCache4() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ````String hql =``````"from User u where u.id=1"````;`|
|`7`|`    ````User user = (User) session.createQuery(hql).uniqueResult();```|
|`8`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`9`|`    ````user = (User) session.createQuery(hql).uniqueResult();```|
|`10`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`11`|`    ``session.getTransaction().commit();`|
|`12`|`  ````}````catch````(Exception e) {```|
|`13`|`    ``e.printStackTrace();`|
|`14`|`    ``session.getTransaction().rollback();`|
|`15`|`  ````}````finally````{```|
|`16`|`    ``HibernateUtils.closeSession(session);`|
|`17`|`  ``}`|
|`18`|`}`|
**输出结果：**
> 
Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_ where user0_.id=1
user.name=user0
Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_ where user0_.id=1
user.name=user0
**测试5：在通一个session中发出两次list查询实体对象不会缓存**
|`1`|`public````void``````testCache4() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ````List list = session.createQuery(``````"from User u"````).list();`|
|`7`|`    ``System.out.println(``"user.name="````+ list.get(````0``).getName());`|
|`8`|`    ````list = session.createQuery(``````"from User u"````).list();`|
|`9`|`    ``System.out.println(``"user.name="````+ list.get(````0``).getName());`|
|`10`|`    ``session.getTransaction().commit();`|
|`11`|`  ````}````catch````(Exception e) {```|
|`12`|`    ``e.printStackTrace();`|
|`13`|`    ``session.getTransaction().rollback();`|
|`14`|`  ````}````finally````{```|
|`15`|`    ``HibernateUtils.closeSession(session);`|
|`16`|`  ``}`|
|`17`|`}`|
**输出结果：**
> 
Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_
user.name=user0
Hibernate: select user0_.id as id0_, user0_.name as name0_ from t_user user0_
user.name=user0
**测试6：在同一个session中发出两次iterate查询普通属性不会缓存**
|`1`|`public````void``````testCache4() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ````String hql =``````"select u.name from User u where u.id=1"````;`|
|`7`|`    ````String name = (String) session.createQuery(hql).iterate().next();```|
|`8`|`    ``System.out.println(``"user.name="````+ name);```|
|`9`|`    ````// iterate查询普通属性，一级缓存不会缓存，所以发出sql```|
|`10`|`    ````// 一级缓存是缓存实体对象的```|
|`11`|`    ````name = (String) session.createQuery(hql).iterate().next();```|
|`12`|`    ``System.out.println(``"user.name="````+ name);```|
|`13`|`    ``session.getTransaction().commit();`|
|`14`|`  ````}````catch````(Exception e) {```|
|`15`|`    ``e.printStackTrace();`|
|`16`|`    ``session.getTransaction().rollback();`|
|`17`|`  ````}````finally````{```|
|`18`|`    ``HibernateUtils.closeSession(session);`|
|`19`|`  ``}`|
|`20`|`}`|
**输出结果：**
> 
Hibernate: select user0_.name as col_0_0_ from t_user user0_ where user0_.id=1
user.name=user0
Hibernate: select user0_.name as col_0_0_ from t_user user0_ where user0_.id=1
user.name=user0
**测试7：开启两个session中发出load查询**
|`1`|`public````void``````testCache7() {```|
|`2`|`  ````Session session1 =````null``;`|
|`3`|`  ````Session session2 =````null``;`|
|`4`|`  ``try````{```|
|`5`|`    ````session1 = HibernateUtils.getSession();```|
|`6`|`    ````session2 = HibernateUtils.getSession();```|
|`7`|`    ``session1.beginTransaction();`|
|`8`|`    ``session2.beginTransaction();`|
|`9`||
|`10`|`    ````User user1 = (User) session1.load(User.````class````,````1``);`|
|`11`|`    ``System.out.println(``"user1.name="````+ user1.getName());```|
|`12`|`    ````// 会发出查询语句，session间不能共享一级缓存的数据```|
|`13`|`    ````// 因为它会伴随session的生命周期存在和消亡```|
|`14`|`    ````User user2 = (User) session2.load(User.````class````,````1``);`|
|`15`|`    ``System.out.println(``"user2.name="````+ user2.getName());```|
|`16`||
|`17`|`    ``session1.getTransaction().commit();`|
|`18`|`    ``session2.getTransaction().commit();`|
|`19`|`  ````}````catch````(Exception e) {```|
|`20`|`    ``e.printStackTrace();`|
|`21`|`    ``session1.getTransaction().rollback();`|
|`22`|`    ``session2.getTransaction().rollback();`|
|`23`|`  ````}````finally````{```|
|`24`|`    ``HibernateUtils.closeSession(session1);`|
|`25`|`    ``HibernateUtils.closeSession(session2);`|
|`26`|`  ``}`|
|`27`|`}`|
**输出结果：**
> 
Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
user1.name=user0
Hibernate: select user0_.id as id0_0_, user0_.name as name0_0_ from t_user user0_ where user0_.id=?
user2.name=user0
**测试8：在同一个session中先save，再发出load查询save过的数据**
|`1`|`public````void``````testCache8() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ````User u =````new````User();```|
|`7`|`    ``u.setName(``"张三"``);`|
|`8`|`    ````Serializable id = session.save(u);```|
|`9`|`    ````// 不会发出sql，因为save是使用缓存的```|
|`10`|`    ````User user = (User) session.load(User.````class````, id);```|
|`11`|`    ``System.out.println(``"user.name="````+ user.getName());```|
|`12`|`    ``session.getTransaction().commit();`|
|`13`|`  ````}````catch````(Exception e) {```|
|`14`|`    ``e.printStackTrace();`|
|`15`|`    ``session.getTransaction().rollback();`|
|`16`|`  ````}````finally````{```|
|`17`|`    ``HibernateUtils.closeSession(session);`|
|`18`|`  ``}`|
|`19`|`}`|
**输出结果：**
> 
Hibernate: insert into t_user (name) values (?)
user.name=张三
**如何避免一次性大量的实体数据入库导致内存溢出？**
先flush()，再clear()。 save方法会将save的对象放入一级缓存中，如果要save大批对象，则应该要及时清空一级缓存，可以采用Session的clear()方法。若不清理缓存，可发现javaw进程占用内存持续增长，到一定程度会内存溢出的。如果数据量特别大，考虑采用jdbc实现，如果jdbc也不能满足要求可以考虑采用数据本身的特定导入工具。
**测试9：向数据库中批量加入1000条数据**
|`1`|`public````void``````testCache9() {```|
|`2`|`  ````Session session =````null``;`|
|`3`|`  ``try````{```|
|`4`|`    ````session = HibernateUtils.getSession();```|
|`5`|`    ``session.beginTransaction();`|
|`6`|`    ``for````(````int````i =````0````; i <````1000````; i++) {```|
|`7`|`      ````User user =````new````User();```|
|`8`|`      ``user.setName(``"user"````+ i);```|
|`9`|`      ``session.save(user);`|
|`10`|`      ````// 每20条数据就强制session将数据持久化```|
|`11`|`      ````// 同时清除缓存，避免大量数据造成内存溢出```|
|`12`|`      ``if````(i %````20````==````0````) {```|
|`13`|`        ``session.flush();`|
|`14`|`        ``session.clear();`|
|`15`|`      ``}`|
|`16`|`    ``}`|
|`17`|`    ``session.getTransaction().commit();`|
|`18`|`  ````}````catch````(Exception e) {```|
|`19`|`    ``e.printStackTrace();`|
|`20`|`    ``session.getTransaction().rollback();`|
|`21`|`  ````}````finally````{```|
|`22`|`    ``HibernateUtils.closeSession(session);`|
|`23`|`  ``}`|
|`24`|`}`|
