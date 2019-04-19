# Hibernate - LC900730的博客 - CSDN博客
2017年08月07日 09:43:16[lc900730](https://me.csdn.net/LC900730)阅读数：156标签：[hibernate](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## 搭建开发环境
1.下载源码 
2.引入jar文件 
    hibernate3.jar+required必须引入的(6个)+jpa目录+数据库驱动包 
3.写对象以及对象的映射 
Employee.java       对象 
Employee.hbm.xml    对象的映射 
4.src/hibernate.cfg.xml 
    数据库连接配置 
    加载所用的映射(*.hbm.xml)
### Hibernate的API
#### Configuration
```
//创建配置管理类对象
    org.hibernate.cfg.Configuration
    Configuration config=new Configuration()
//加载配置文件
    src/hibernate.cfg.xml
config.configure();//因此一般情况省略不写
如果放在其他包下
config.configure(cn/itcast/a_hello/xxx.xml)
```
#### SessionFactory
```
根据加载的配置管理类对象,创建SessionFactory对象
SessionFactory sf = config.buildSessionFactory();
//根据session工厂，或者说代表了整个配置文件
```
一个项目只需要一个session对象，并且只需要加载一次 
那么应该放在static代码块中。
```
public class App1{
        private static SessionFactory sf;
        static{
        //1.创建配置管理类对象
        //org.hibernate.cfg.Configuration
        Configuration config=new Configuration();
        //2.加载配置
        config.configure();
        //3.创建SessionFactory对象
        sf=config.buildSessionFactory();
        }
    }
sf.openSession();    //创建一个session对象
sf.getCurrentSession();  //创建session或取出session对象
session对象维护了一个连接(connection),代表了与数据库连接的会话。也是Hibernate最重要的对象，只要使用hibernate与数据库操作，都需要这个。
//org.hibernate.Session session=sf.openSession();
session
//开启事务
hibernate要求所有的与数据库操作必须使用事务，否则报错
Transaction tx=session.beginTransaction();  
session.save(emp);
tx.commit();
session.close();
```
#### 更新
```
public void testUpdate() throws Exception{
    //对象
    Employee emp=new Employee();
    emp.setEmpId(1);
    emp.setEmpName("张三");
    //创建session
    Session session=sf.openSession();
    //开启事务
    Transaction tx=session.beginTransaction();
    //执行更新的时候必须要有主键
    //一般是先查询在更新
    //主键查询
    Employee emp=(Employee)session.get(Employ.class,1);
//主键查询还可以用load,一般用的是get
    //执行操作
    session.update(emp);
    tx.commit();
    session.close();
```
#### 保存或更新
```
saveOrUpdate(emp);
```
没有设置主键，执行保存；设置了则执行更新；如果设置主键不存在，则报错。
### 其他方法
HQL查询与SQL查询区别：
```
SQL：查询的是表与字段
    HQL:hibernate query language，hibernate提供的查面向对象查询语言，查询的是对象以及对象的属性，   区分大小写。
```
//查询全部
```
//import org.hibernate.query.Query;
Query q=session.createQuery("from Employee");   
List<Employee> list =q.list();
```
//查询的是对象的属性，不是字段，因此用empId
```
Query q=session.createQuery("from Employee where empId=1 or empId=2");
```
#### QBC
query by criteria:完全面向对象的查询
Criteria criteria=session.createCriteria(Employee.class);
```
//查询全部
List<Employee> list =criteria.list()
//条件查询
criteria.add(Restrictions.eq("empId",1));
```
#### 本地SQL查询
确定：不能跨数据库 
    使用原生SQL查询 
//把每一行记录封装为对象数组，添加到list集合 
SQLQuery sqlquery=session.createSQLQuery(“select * from employee”); 
SQLQuery sqlquery=session.createSQLQuery(“select * from employee”).addEntity(Employee.class);
### hibernate.cfg.xml
```
主配置文件
通常 session-factory节点代表一个数据库
配置分为3个部分
    1.数据库连接配置
    2.其他相关配置参数
    3.映射信息(即所有对象与字段的对应关系)
```
- 数据库连接配置，hibernate在运行的时候，会根据不同 的dialect来生成符合当前数据库语法的 sql 
org.hibernate.dialect.MySQLDialect
2.其他相关配置
```
//2.1显示执行的sql语句
    <property name="hibernate.show_sql">true</property>
//2.2格式化sql
    <property name="hibernate.format_sql">true</property>
//2.3自动建表
    <property name="hibernate.hbm2ddl.auto">create</property>
    //如果属性值是create，那么每次都删除重新建立
    //如果属性值是update,那么表不存在就创建，存在则修改
    //如果create-drop每次在创建sessionFactory时候执行创建表
    //validate(生成环境)执行验证，当映射文件的内容与数据库表结构不一致的时候报错
```
### 映射文件
映射一个实体类对象，描述一个对象最终实现可以直接保存对象数据到数据库中。
```
<hibernate-mapping  package="XXX" auto-import="true">
其中package：要映射的实体类(即对象)所在的包。
//如果不写package的话，那么就需要在class写上类全名
auto-import默认是true，在写hql的时候会自动导包，如果为false那么在写hql的时候必须写上类全名
//class映射某一个对象(一般情况下一个对象写一个映射文件，即一个class节点）
name指定要映射的对象类型，table对象对应的表。
如果没有指定表名，那么默认与对象名称一致
<class name="XXX" table="XXX">
//主键映射
    <id name="" column="">
        //主键的生成策略
        <generator   class="native"
        //native是自增
    </id>
//普通字段映射
    //name是对象属性名
    //length不指定默认是255
    //type:指定表的字段的类型，如果不指定会匹配属性的类型
    java类型：必须写全名
    hibernate类型：直接写类型，都是小写(string)
    如果列名称为数据库关键字，需要用 `desc`
    <property name="XXX" column="XXX" length="20" type="java.lang.String"></property>(生成varchar(20))
```
#### 主键选择
```
identity:自增长(mysql、db2、sqlserver、sybase等内置标识字段提供支持。返回标识符是long、int、short)
increment:用于long、int或者short类型生成唯一标识，只有在没有其他进程往同一张表中插入数据时才能使用。(集群下不使用
，会有并发访问的问题)
sequence:自增长序列，oracle自增长是以序列方法实现。
native:自增长(会根据底层数据库的自增长方式选择identity或sequence)，如果是mysql采用的是identity，如果是oracle数据库，使用sequence序列的方式实现自增长。
assigned:指定主键，不指定主键时候插入数据报错
uuid:指定uuid随机生成的唯一值。
foreign:外键
```
#### 映射多列作为主键
```
<composite-id name="keys">
        <key-property name="userName" type="string"></key-property>
        <key-property name="address" type="string"></key-property>
    </composite-id>
//复合主键查询
```
public class CompositeKeys implements Serializable{ 
    private String userName； 
    private Set address； 
    private String  
}
```
//get查询的时候使用的是主键
//那么需要构建主键在查询
CompositeKeys keys=new CompositeKeys();
keys.setAddress("广州");
keys.setUserName("jack")；
(User)session.get(User.class,keys);
```
#### 集合数据的获取
```
t_user表
```
|uid|userName|
|----|----|
|1|Jack|
```
t_address表
```
|u_id|address|
|----|----|
|1|beijing|
|1|shanghai|
```
@Test
    public void testGet() throws Exception{
        Session session=sf.openSession();
        session.beginTransaction();
        User user=session.get(User.class, 3);
        System.out.println(user.getUserId());
        System.out.println(user.getUserName());
    //当查询用户，同时可以获取用户关联的list集合数据(因为有正确映射)
    //当使用到集合时候，才向数据库发送执行的sql语句(懒加载)
        System.out.println(user.getAddressList());
        session.getTransaction().commit();
        session.close();
        }
```
