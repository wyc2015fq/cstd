# EJB+JPA演示实体类基本操作CRUD的实现 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月24日 16:16:50[boonya](https://me.csdn.net/boonya)阅读数：870标签：[j2ee																[ejb3.0																[jpa																[jboss																[entity](https://so.csdn.net/so/search/s.do?q=entity&t=blog)
个人分类：[JPA&EJB](https://blog.csdn.net/boonya/article/category/2355153)





作者: [Michael](http://www.micmiu.com/author/michael/) 日期:
 2013 年 9 月 25       原文地址：[http://www.micmiu.com/j2ee/ejb/ejb-jpa-entity-crud-demo/](http://www.micmiu.com/j2ee/ejb/ejb-jpa-entity-crud-demo/)





本文详细介绍EJB+JPA实现Entity的CRUD基本操作。

目录
- 
创建POJO：BlogVo
- 
创建Entity：Blog
- 
配置persistence.xml
- 
定义Remote接口
- 
创建SessionBean实现类
- 
配置datasource数据源
- 
部署EJB工程
- 
创建客户端测试类
- 
运行测试类

[一]、创建POJO：BlogVo.java



|`1`|`package``com.micmiu.ejb.vo;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``java.io.Serializable;`|
|----|----|


|`4`||
|----|----|


|`5`|`/**`|
|----|----|


|`6`|` ````* Blog vo对象```|
|----|----|


|`7`|` ``*`|
|----|----|


|`8`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`9`|` ````* @time Create on 2013-9-25 下午3:02:17```|
|----|----|


|`10`|` ````* @version 1.0```|
|----|----|


|`11`|` ``*/`|
|----|----|


|`12`|`public``class``BlogVo ``implements````Serializable {```|
|----|----|


|`13`||
|----|----|


|`14`|`    ``private``static``final``long````serialVersionUID = -6384496304647072095L;```|
|----|----|


|`15`||
|----|----|


|`16`|`    ``private````Integer id;```|
|----|----|


|`17`||
|----|----|


|`18`|`    ``private````String title;```|
|----|----|


|`19`||
|----|----|


|`20`|`    ``private````String url;```|
|----|----|


|`21`||
|----|----|


|`22`|`    ``private````String author;```|
|----|----|


|`23`||
|----|----|


|`24`|`    ``public````Integer getId() {```|
|----|----|


|`25`|`        ``return``id;`|
|----|----|


|`26`|`    ``}`|
|----|----|


|`27`||
|----|----|


|`28`|`    ``public````String getTitle() {```|
|----|----|


|`29`|`        ``return``title;`|
|----|----|


|`30`|`    ``}`|
|----|----|


|`31`||
|----|----|


|`32`|`    ``public````String getUrl() {```|
|----|----|


|`33`|`        ``return``url;`|
|----|----|


|`34`|`    ``}`|
|----|----|


|`35`||
|----|----|


|`36`|`    ``public````String getAuthor() {```|
|----|----|


|`37`|`        ``return``author;`|
|----|----|


|`38`|`    ``}`|
|----|----|


|`39`||
|----|----|


|`40`|`    ``public``void````setId(Integer id) {```|
|----|----|


|`41`|`        ``this````.id = id;```|
|----|----|


|`42`|`    ``}`|
|----|----|


|`43`||
|----|----|


|`44`|`    ``public``void````setTitle(String title) {```|
|----|----|


|`45`|`        ``this````.title = title;```|
|----|----|


|`46`|`    ``}`|
|----|----|


|`47`||
|----|----|


|`48`|`    ``public``void````setUrl(String url) {```|
|----|----|


|`49`|`        ``this````.url = url;```|
|----|----|


|`50`|`    ``}`|
|----|----|


|`51`||
|----|----|


|`52`|`    ``public``void````setAuthor(String author) {```|
|----|----|


|`53`|`        ``this````.author = author;```|
|----|----|


|`54`|`    ``}`|
|----|----|


|`55`||
|----|----|


|`56`|`    ``@Override`|
|----|----|


|`57`|`    ``public````String toString() {```|
|----|----|


|`58`|`        ``return````"BlogVo [id="``````+ id +``````", title="``````+ title +``````", url="``````+ url```|
|----|----|


|`59`|`                ``+ ````", author="``````+ author +````"]"``;`|
|----|----|


|`60`|`    ``}`|
|----|----|


|`61`|`}`|
|----|----|




[二]、创建Entity：Blog.java



|`1`|`package``com.micmiu.ejb.entity;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``java.io.Serializable;`|
|----|----|


|`4`||
|----|----|


|`5`|`import``javax.persistence.Column;`|
|----|----|


|`6`|`import``javax.persistence.Entity;`|
|----|----|


|`7`|`import``javax.persistence.GeneratedValue;`|
|----|----|


|`8`|`import``javax.persistence.Id;`|
|----|----|


|`9`|`import``javax.persistence.NamedQueries;`|
|----|----|


|`10`|`import``javax.persistence.NamedQuery;`|
|----|----|


|`11`|`import``javax.persistence.Table;`|
|----|----|


|`12`||
|----|----|


|`13`|`/**`|
|----|----|


|`14`|` ````* 实体类blog```|
|----|----|


|`15`|` ``*`|
|----|----|


|`16`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`17`|` ````* @time Create on 2013-9-24 下午1:57:47```|
|----|----|


|`18`|` ````* @version 1.0```|
|----|----|


|`19`|` ``*/`|
|----|----|


|`20`|`@Entity`|
|----|----|


|`21`|`@Table````(name =````"DEMO_T_BLOG"``)`|
|----|----|


|`22`|`@NamedQueries``({ ``@NamedQuery````(name =````"queryAll"````, query =``````"select t from Blog t "``````) })```|
|----|----|


|`23`|`public``class``Blog ``implements````Serializable {```|
|----|----|


|`24`||
|----|----|


|`25`|`    ``private``static``final``long````serialVersionUID = -1371929956020543775L;```|
|----|----|


|`26`||
|----|----|


|`27`|`    ``@Id`|
|----|----|


|`28`|`    ``@GeneratedValue`|
|----|----|


|`29`|`    ``@Column````(name =````"ID"``)`|
|----|----|


|`30`|`    ``private````Integer id;```|
|----|----|


|`31`||
|----|----|


|`32`|`    ``@Column````(name =````"TITLE"````, length =````128``)`|
|----|----|


|`33`|`    ``private````String title;```|
|----|----|


|`34`||
|----|----|


|`35`|`    ``@Column````(name =````"URL"````, length =````1024``)`|
|----|----|


|`36`|`    ``private````String url;```|
|----|----|


|`37`||
|----|----|


|`38`|`    ``@Column````(name =````"AUTHOR"````, length =````20``)`|
|----|----|


|`39`|`    ``private````String author;```|
|----|----|


|`40`||
|----|----|


|`41`|`    ``public````Integer getId() {```|
|----|----|


|`42`|`        ``return``id;`|
|----|----|


|`43`|`    ``}`|
|----|----|


|`44`||
|----|----|


|`45`|`    ``public````String getTitle() {```|
|----|----|


|`46`|`        ``return``title;`|
|----|----|


|`47`|`    ``}`|
|----|----|


|`48`||
|----|----|


|`49`|`    ``public````String getUrl() {```|
|----|----|


|`50`|`        ``return``url;`|
|----|----|


|`51`|`    ``}`|
|----|----|


|`52`||
|----|----|


|`53`|`    ``public````String getAuthor() {```|
|----|----|


|`54`|`        ``return``author;`|
|----|----|


|`55`|`    ``}`|
|----|----|


|`56`||
|----|----|


|`57`|`    ``public``void````setId(Integer id) {```|
|----|----|


|`58`|`        ``this````.id = id;```|
|----|----|


|`59`|`    ``}`|
|----|----|


|`60`||
|----|----|


|`61`|`    ``public``void````setTitle(String title) {```|
|----|----|


|`62`|`        ``this````.title = title;```|
|----|----|


|`63`|`    ``}`|
|----|----|


|`64`||
|----|----|


|`65`|`    ``public``void````setUrl(String url) {```|
|----|----|


|`66`|`        ``this````.url = url;```|
|----|----|


|`67`|`    ``}`|
|----|----|


|`68`||
|----|----|


|`69`|`    ``public``void````setAuthor(String author) {```|
|----|----|


|`70`|`        ``this````.author = author;```|
|----|----|


|`71`|`    ``}`|
|----|----|


|`72`||
|----|----|


|`73`|`    ``@Override`|
|----|----|


|`74`|`    ``public````String toString() {```|
|----|----|


|`75`|`        ``return````"Blog [id="``````+ id +``````", title="``````+ title +``````", url="``````+ url```|
|----|----|


|`76`|`                ``+ ````", author="``````+ author +````"]"``;`|
|----|----|


|`77`|`    ``}`|
|----|----|


|`78`||
|----|----|


|`79`|`}`|
|----|----|




[三]、配置persistence.xml


修改配置文件 src: META-INF/persistence.xml 



|`1`|`<?``xml``version``=``"1.0"``encoding``=``"UTF-8"``?>`|
|----|----|


|`2`|`<``persistence``version``=``"2.0"`|
|----|----|


|`3`|`    ``xmlns``=``"[http://java.sun.com/xml/ns/persistence](http://java.sun.com/xml/ns/persistence)"``xmlns:xsi``=``"[http://www.w3.org/2001/XMLSchema-instance](http://www.w3.org/2001/XMLSchema-instance)"`|
|----|----|


|`4`|`    ``xsi:schemaLocation="[http://java.sun.com/xml/ns/persistence](http://java.sun.com/xml/ns/persistence)`|
|----|----|


|`5`|`             ``[http://java.sun.com/xml/ns/persistence/persistence_2_0.xsd](http://java.sun.com/xml/ns/persistence/persistence_2_0.xsd)">`|
|----|----|


|`6`|`    ````<!-- 为持久化单元取名=micmiuJPA,transaction-type=JTA 利用容器的事务管理 -->```|
|----|----|


|`7`|`    ``<``persistence-unit``name``=``"micmiuJPA"``transaction-type``=``"JTA"``>`|
|----|----|


|`8`|`        ``<``provider``>org.hibernate.ejb.HibernatePersistence</``provider``>`|
|----|----|


|`9`||
|----|----|


|`10`|`        ``<``jta-data-source``>java:/micmiuOracleDS</``jta-data-source``>`|
|----|----|


|`11`|`        ````<!-- <class>com.micmiu.ejb.entity.Blog</class> -->```|
|----|----|


|`12`|`        ````<!-- 声明是否扫描jar文件中标注了@Enity类.若不扫描,值为true 默认也是true -->```|
|----|----|


|`13`|`        ``<``exclude-unlisted-classes``>true</``exclude-unlisted-classes``>`|
|----|----|


|`14`|`        ``<``properties``>`|
|----|----|


|`15`|`            ``<``property``name``=``"hibernate.archive.autodetection"``value``=``"class,hbm"``/>`|
|----|----|


|`16`|`            ````<!--配置Hibernate方言 -->```|
|----|----|


|`17`|`            ``<``property``name``=``"hibernate.dialect"``value``=``"org.hibernate.dialect.Oracle10gDialect"``/>`|
|----|----|


|`18`|`            ````<!--自动输出schema创建DDL语句 -->```|
|----|----|


|`19`|`            ``<``property``name``=``"hibernate.hbm2ddl.auto"``value``=``"update"``/>`|
|----|----|


|`20`|`            ````<!-- 输出SQL语句 -->```|
|----|----|


|`21`|`            ``<``property``name``=``"hibernate.show_sql"``value``=``"true"``/>`|
|----|----|


|`22`|`            ````<!-- 格式化SQL语句 -->```|
|----|----|


|`23`|`            ``<``property``name``=``"hibernate.format_sql"``value``=``"true"``/>`|
|----|----|


|`24`|`        ``</``properties``>`|
|----|----|


|`25`|`    ``</``persistence-unit``>`|
|----|----|


|`26`|`</``persistence``>`|
|----|----|





ps：<jta-data-source></jta-data-source>配置的值“java:/micmiuOracleDS”需要和JBOSS_HOME/server/default/conf/tandardjbosscmp-jdbc.xml 中配置的<datasource></datasource>一致，详见
 下面 第[六]部分 配置datasource数据源

[四]、定义Remote接口


BlogBeanRemote.java



|`1`|`package``com.micmiu.ejb;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``java.util.List;`|
|----|----|


|`4`||
|----|----|


|`5`|`import``javax.ejb.Remote;`|
|----|----|


|`6`||
|----|----|


|`7`|`import``com.micmiu.ejb.vo.BlogVo;`|
|----|----|


|`8`||
|----|----|


|`9`|`/**`|
|----|----|


|`10`|` ````* BlogBean remote```|
|----|----|


|`11`|` ``*`|
|----|----|


|`12`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`13`|` ````* @time Create on 2013-9-25 下午3:05:42```|
|----|----|


|`14`|` ````* @version 1.0```|
|----|----|


|`15`|` ``*/`|
|----|----|


|`16`|`@Remote`|
|----|----|


|`17`|`public``interface````BlogBeanRemote {```|
|----|----|


|`18`||
|----|----|


|`19`|`    ````BlogVo create(BlogVo blog);```|
|----|----|


|`20`||
|----|----|


|`21`|`    ````BlogVo read(Integer id);```|
|----|----|


|`22`||
|----|----|


|`23`|`    ``void````update(BlogVo blog);```|
|----|----|


|`24`||
|----|----|


|`25`|`    ``void````delete(Integer id);```|
|----|----|


|`26`||
|----|----|


|`27`|`    ````List<BlogVo> queryAll();```|
|----|----|


|`28`||
|----|----|


|`29`|`    ````List<BlogVo> queryByAuthor(String author);```|
|----|----|


|`30`|`}`|
|----|----|




[五]、创建SessionBean实现类


BlogBean.java



|`1`|`package``com.micmiu.ejb;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``java.util.ArrayList;`|
|----|----|


|`4`|`import``java.util.List;`|
|----|----|


|`5`||
|----|----|


|`6`|`import``javax.ejb.Stateless;`|
|----|----|


|`7`|`import``javax.persistence.EntityManager;`|
|----|----|


|`8`|`import``javax.persistence.PersistenceContext;`|
|----|----|


|`9`|`import``javax.persistence.Query;`|
|----|----|


|`10`||
|----|----|


|`11`|`import``com.micmiu.ejb.entity.Blog;`|
|----|----|


|`12`|`import``com.micmiu.ejb.vo.BlogVo;`|
|----|----|


|`13`||
|----|----|


|`14`|`/**`|
|----|----|


|`15`|` ````* Session Bean implementation class BlogBean```|
|----|----|


|`16`|` ``*`|
|----|----|


|`17`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`18`|` ````* @time Create on 2013-9-25 上午9:20:36```|
|----|----|


|`19`|` ````* @version 1.0```|
|----|----|


|`20`|` ``*/`|
|----|----|


|`21`|`@Stateless`|
|----|----|


|`22`|`public``class``BlogBean ``implements````BlogBeanRemote {```|
|----|----|


|`23`||
|----|----|


|`24`|`    ````@PersistenceContext(unitName =````"micmiuJPA"``)`|
|----|----|


|`25`|`    ``private````EntityManager em;```|
|----|----|


|`26`||
|----|----|


|`27`|`    ``/**`|
|----|----|


|`28`|`     ````* Default constructor.```|
|----|----|


|`29`|`     ``*/`|
|----|----|


|`30`|`    ``public````BlogBean() {```|
|----|----|


|`31`|`    ``}`|
|----|----|


|`32`||
|----|----|


|`33`|`    ``@Override`|
|----|----|


|`34`|`    ``public````BlogVo create(BlogVo vo) {```|
|----|----|


|`35`|`        ````Blog po =````this``.parseVo2Po(vo);`|
|----|----|


|`36`|`        ``em.persist(po);`|
|----|----|


|`37`|`        ``em.flush();`|
|----|----|


|`38`|`        ``return``this``.parsePo2Vo(po);`|
|----|----|


|`39`|`    ``}`|
|----|----|


|`40`||
|----|----|


|`41`|`    ``@Override`|
|----|----|


|`42`|`    ``public````BlogVo read(Integer id) {```|
|----|----|


|`43`|`        ````Blog po = em.find(Blog.````class````, id);```|
|----|----|


|`44`|`        ``return``this``.parsePo2Vo(po);`|
|----|----|


|`45`||
|----|----|


|`46`|`    ``}`|
|----|----|


|`47`||
|----|----|


|`48`|`    ``@Override`|
|----|----|


|`49`|`    ``public``void````update(BlogVo vo) {```|
|----|----|


|`50`|`        ````Blog po =````this``.parseVo2Po(vo);`|
|----|----|


|`51`|`        ``em.merge(po);`|
|----|----|


|`52`|`    ``}`|
|----|----|


|`53`||
|----|----|


|`54`|`    ``@Override`|
|----|----|


|`55`|`    ``public``void``delete````(Integer id) {```|
|----|----|


|`56`|`        ``em.remove(em.getReference(Blog.``class````, id));```|
|----|----|


|`57`|`    ``}`|
|----|----|


|`58`||
|----|----|


|`59`|`    ``@SuppressWarnings(``"unchecked"``)`|
|----|----|


|`60`|`    ``@Override`|
|----|----|


|`61`|`    ``public````List<BlogVo> queryAll() {```|
|----|----|


|`62`|`        ````List<Blog> list = em.createNamedQuery(````"queryAll"``).getResultList();`|
|----|----|


|`63`|`        ````List<BlogVo> volist =````new``ArrayList<BlogVo>();`|
|----|----|


|`64`|`        ``for````(Blog po : list) {```|
|----|----|


|`65`|`            ``volist.add(``this``.parsePo2Vo(po));`|
|----|----|


|`66`|`        ``}`|
|----|----|


|`67`|`        ``return``volist;`|
|----|----|


|`68`|`    ``}`|
|----|----|


|`69`||
|----|----|


|`70`|`    ``@SuppressWarnings(``"unchecked"``)`|
|----|----|


|`71`|`    ``@Override`|
|----|----|


|`72`|`    ``public````List<BlogVo> queryByAuthor(````String````author) {```|
|----|----|


|`73`|`        ````Query query = em.createQuery(```|
|----|----|


|`74`|`                ````"select t from Blog t where t.author =:author"````).setParameter(`|
|----|----|


|`75`|`                ``"author"````, author);```|
|----|----|


|`76`|`        ````List<Blog> list = query.getResultList();```|
|----|----|


|`77`|`        ````List<BlogVo> volist =````new``ArrayList<BlogVo>();`|
|----|----|


|`78`|`        ``for````(Blog po : list) {```|
|----|----|


|`79`|`            ``volist.add(``this``.parsePo2Vo(po));`|
|----|----|


|`80`|`        ``}`|
|----|----|


|`81`|`        ``return``volist;`|
|----|----|


|`82`|`    ``}`|
|----|----|


|`83`||
|----|----|


|`84`|`    ``private````Blog parseVo2Po(BlogVo vo) {```|
|----|----|


|`85`|`        ````Blog blog =````new``Blog();`|
|----|----|


|`86`|`        ``blog.setAuthor(vo.getAuthor());`|
|----|----|


|`87`|`        ``blog.setId(vo.getId());`|
|----|----|


|`88`|`        ``blog.setTitle(vo.getTitle());`|
|----|----|


|`89`|`        ``blog.setUrl(vo.getUrl());`|
|----|----|


|`90`|`        ``return``blog;`|
|----|----|


|`91`|`    ``}`|
|----|----|


|`92`||
|----|----|


|`93`|`    ``private````BlogVo parsePo2Vo(Blog po) {```|
|----|----|


|`94`|`        ````BlogVo vo =````new``BlogVo();`|
|----|----|


|`95`|`        ``vo.setAuthor(po.getAuthor());`|
|----|----|


|`96`|`        ``vo.setId(po.getId());`|
|----|----|


|`97`|`        ``vo.setTitle(po.getTitle());`|
|----|----|


|`98`|`        ``vo.setUrl(po.getUrl());`|
|----|----|


|`99`|`        ``return``vo;`|
|----|----|


|`100`|`    ``}`|
|----|----|


|`101`|`}`|
|----|----|




ps:
- @PersistenceContext 用来注入 EntityManager

- unitName 的值”micmiuJPA” 和配置文件persistence.xml 中定义的名称一致

[六]、配置datasource数据源


数据源配置详见：[http://www.micmiu.com/j2ee/ejb/ejb-deploy-jboss-ds-config/](http://www.micmiu.com/j2ee/ejb/ejb-deploy-jboss-ds-config/)

[七]、部署EJB工程


导出EJB工程打包，copy 到 JBOSS_HOME/server/default/deploy 默认发布配置下,启动JBOSS即可，能看的类似如下的日志信息：

> 

09:39:43,291 INFO [SessionSpecContainer] Starting jboss.j2ee:jar=EJB-HelloWorld.jar,name=BlogBean,service=EJB3

09:39:43,298 INFO [EJBContainer] STARTED EJB: com.micmiu.ejb.BlogBean ejbName: BlogBean

09:39:43,306 INFO [JndiSessionRegistrarBase] Binding the following Entries in Global JNDI:

BlogBean/remote – EJB3.x Default Remote Business Interface

BlogBean/remote-com.micmiu.ejb.BlogBeanRemote – EJB3.x Remote Business Interface

[八]、创建客户端测试类


把EJB工程中的Remote接口以及引用到的Vo类导出JAR包，copy到客户端工程中，然后创建客户端调用类：BlogBeanClientTest.java



|`1`|`package``com.micmiu.ejb.demo;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``java.util.List;`|
|----|----|


|`4`|`import``java.util.Properties;`|
|----|----|


|`5`||
|----|----|


|`6`|`import``javax.naming.InitialContext;`|
|----|----|


|`7`|`import``javax.naming.NamingException;`|
|----|----|


|`8`||
|----|----|


|`9`|`import``com.micmiu.ejb.BlogBeanRemote;`|
|----|----|


|`10`|`import``com.micmiu.ejb.vo.BlogVo;`|
|----|----|


|`11`||
|----|----|


|`12`|`/**`|
|----|----|


|`13`|` ````* 客户端调用测试类```|
|----|----|


|`14`|` ``*`|
|----|----|


|`15`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`16`|` ````* @time Create on 2013-9-21 下午2:51:59```|
|----|----|


|`17`|` ````* @version 1.0```|
|----|----|


|`18`|` ``*/`|
|----|----|


|`19`|`public``class````BlogBeanClientTest {```|
|----|----|


|`20`||
|----|----|


|`21`|`    ``/**`|
|----|----|


|`22`|`     ````* @param args```|
|----|----|


|`23`|`     ``*/`|
|----|----|


|`24`|`    ``public``static``void````main(String[] args) {```|
|----|----|


|`25`|`        ``try``{`|
|----|----|


|`26`|`            ````Properties props =````new``Properties();`|
|----|----|


|`27`|`            ``props.setProperty(``"java.naming.factory.initial"``,`|
|----|----|


|`28`|`                    ``"org.jnp.interfaces.NamingContextFactory"``);`|
|----|----|


|`29`|`            ``props.setProperty(``"java.naming.provider.url"``, ``"localhost:1099"``);`|
|----|----|


|`30`|`            ``props.setProperty(``"java.naming.factory.url.pkgs"``,`|
|----|----|


|`31`|`                    ``"org.jboss.naming"``);`|
|----|----|


|`32`||
|----|----|


|`33`|`            ``System.out.println(````">>>> InitialContext "````);`|
|----|----|


|`34`|`            ````InitialContext ctx =````new``InitialContext(props);`|
|----|----|


|`35`||
|----|----|


|`36`|`            ``System.out.println(````">>>> lookup  "````);`|
|----|----|


|`37`|`            ````String serviceName =````"BlogBean/remote"``;`|
|----|----|


|`38`|`            ````BlogBeanRemote remote = (BlogBeanRemote) ctx.lookup(serviceName);```|
|----|----|


|`39`||
|----|----|


|`40`|`            ``System.out.println(````">>>>  操作之前 query michael's Blog"````);`|
|----|----|


|`41`|`            ````List<BlogVo> list = remote.queryByAuthor(````"michael"``);`|
|----|----|


|`42`|`            ``System.out.println(````"query result size = "``````+ list.size());```|
|----|----|


|`43`|`            ``for````(BlogVo blog : list) {```|
|----|----|


|`44`|`                ``System.out.println(blog);`|
|----|----|


|`45`|`            ``}`|
|----|----|


|`46`||
|----|----|


|`47`|`            ``System.out.println(````">>>> create new blog"````);`|
|----|----|


|`48`|`            ````BlogVo blog =````new``BlogVo();`|
|----|----|


|`49`|`            ``blog.setAuthor(``"michael"``);`|
|----|----|


|`50`|`            ``blog.setTitle(``"图文演示Eclipse+JBoss创建第一个EJB项目"``);`|
|----|----|


|`51`|`            ``blog.setUrl(``"[http://www.micmiu.com/j2ee/ejb/eclipse-jboss-ejb-demo/](http://www.micmiu.com/j2ee/ejb/eclipse-jboss-ejb-demo/)"``);`|
|----|----|


|`52`|`            ````BlogVo createVo = remote.create(blog);```|
|----|----|


|`53`|`            ``System.out.println(createVo);`|
|----|----|


|`54`||
|----|----|


|`55`|`            ``System.out.println(````">>>> read exit blog"````);`|
|----|----|


|`56`|`            ````BlogVo readVo = remote.read(````1``);`|
|----|----|


|`57`|`            ``System.out.println(readVo);`|
|----|----|


|`58`||
|----|----|


|`59`|`            ``System.out.println(````">>>> update blog"````);`|
|----|----|


|`60`|`            ``System.out.println(````"update title "``````+ readVo.getTitle()```|
|----|----|


|`61`|`                    ``+ ````" to 大大&&小小 "````);`|
|----|----|


|`62`|`            ``readVo.setTitle(``"大大&&小小"``);`|
|----|----|


|`63`|`            ``remote.update(readVo);`|
|----|----|


|`64`||
|----|----|


|`65`|`            ``System.out.println(````">>>>  操作之后 query michael's Blog"````);`|
|----|----|


|`66`|`            ````list = remote.queryByAuthor(````"michael"``);`|
|----|----|


|`67`|`            ``System.out.println(````">>>> 结果 = "``````+ list.size());```|
|----|----|


|`68`|`            ``for````(BlogVo vo : list) {```|
|----|----|


|`69`|`                ``System.out.println(vo);`|
|----|----|


|`70`|`            ``}`|
|----|----|


|`71`||
|----|----|


|`72`|`        ``} ``catch````(NamingException e) {```|
|----|----|


|`73`|`            ``e.printStackTrace();`|
|----|----|


|`74`|`        ``}`|
|----|----|


|`75`|`    ``}`|
|----|----|


|`76`|`}`|
|----|----|




[九]、运行测试类


运行后日志信息如下：

> 

>>>> InitialContext

>>>> lookup

>>>> 操作之前 query michael’s Blog

query result size = 1

BlogVo [id=1, title=Michael的博客, url=http://www.micmiu.com, author=michael]

>>>> create new blog

BlogVo [id=114, title=图文演示Eclipse+JBoss创建第一个EJB项目, url=http://www.micmiu.com/j2ee/ejb/eclipse-jboss-ejb-demo/, author=michael]

>>>> read exit blog

BlogVo [id=1, title=Michael的博客, url=http://www.micmiu.com, author=michael]

>>>> update blog

update title Michael的博客 to 大大&&小小

>>>> 操作之后 query michael’s Blog

>>>> 结果 = 2

BlogVo [id=1, title=大大&&小小, url=http://www.micmiu.com, author=michael]

BlogVo [id=114, title=图文演示Eclipse+JBoss创建第一个EJB项目, url=http://www.micmiu.com/j2ee/ejb/eclipse-jboss-ejb-demo/, author=michael]

本文介绍到此结束@[Michael Sun](http://www.micmiu.com/).](https://so.csdn.net/so/search/s.do?q=jboss&t=blog)](https://so.csdn.net/so/search/s.do?q=jpa&t=blog)](https://so.csdn.net/so/search/s.do?q=ejb3.0&t=blog)](https://so.csdn.net/so/search/s.do?q=j2ee&t=blog)




