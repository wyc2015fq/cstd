# JPA - weixin_33985507的博客 - CSDN博客
2017年01月30日 20:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### 一、JPA简介
JPA全称为Java Persistence API。JPA提供了一种简单高效的方式来管理Java对象(POJO)到关系型数据库的映射，此类Java对象成为JPA实体或简称实体。JPA和Hibernate之间的关系，可以简单的理解为**JPA是标准接口，Hibernate是实现**。从功能上来说，**JPA现在就是Hibernate功能的一个子集**。Hibernate 从3.2开始，就开始兼容JPA。Hibernate3.2获得了Sun TCK的 JPA(Java  Persistence API) 兼容认证。
  那么Hibernate是如何实现与JPA的这种关系的呢？Hibernate主要是通过三个组件来实现的，及hibernate-annotation、hibernate-entitymanager和hibernate-core。
- hibernate-annotation是Hibernate支持annotation方式配置的基础，它包括了标准的JPA annotation以及Hibernate自身特殊功能的annotation。
- hibernate-core是Hibernate的核心实现，提供了Hibernate所有的核心功能。
- hibernate-entitymanager实现了标准的JPA，可以把它看成hibernate-core和JPA之间的适配器，它并不直接提供ORM的功能，而是对hibernate-core进行封装，使得Hibernate符合JPA的规范。
总的来说，JPA是规范，Hibernate是框架，JPA是持久化规范，而Hibernate实现了JPA。
### 二、JPA的实体状态
实体对象的状态，在Hibernate有瞬时、持久、离线三种，而JPA里有new，managed，detached，removed，两者的这些状态都是一一对应的。
![4444657-29307892bfb86814.png](https://upload-images.jianshu.io/upload_images/4444657-29307892bfb86814.png)
### 三、JPA配置
我们看看基于Hibernate提供的一个比较完整的JPA2.1的配置文件。
pom.xml：
```
<properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
</properties>    
<dependencies>
    <dependency>
       <groupId>org.hibernate</groupId>
       <artifactId>hibernate-core</artifactId>
       <version>5.1.0.Final</version>
       <scope>compile</scope>
     </dependency>
     <dependency>
        <groupId>org.hibernate</groupId>
        <artifactId>hibernate-entitymanager</artifactId>
        <version>5.1.0.Final</version>
        <scope>compile</scope>
     </dependency>
 
     <!--Hibernate整合C3P0实现连接池-->
     <dependency>
        <groupId>org.hibernate</groupId>
        <artifactId>hibernate-c3p0</artifactId>
        <version>5.1.0.Final</version>
        <scope>runtime</scope>
        <exclusions>
           <exclusion>
             <groupId>com.mchange</groupId>
             <artifactId>c3p0</artifactId>
           </exclusion>
         </exclusions>
      </dependency>
    
      <dependency>
         <groupId>com.mchange</groupId>
         <artifactId>c3p0</artifactId>
         <version>0.9.5.2</version>
         <scope>runtime</scope>
      </dependency>
      <dependency>
          <groupId>org.hibernate</groupId>
          <artifactId>hibernate-ehcache</artifactId>
          <version>5.1.0.Final</version>
          <scope>runtime</scope>
       </dependency>
        <dependency>
          <groupId>net.sf.ehcache</groupId>
          <artifactId>ehcache</artifactId>
          <version>2.10.1</version>
          <scope>compile</scope>
        </dependency>
</dependencies>
```
persistence.xml：
```
<!--JPA2.1规范要求每一个持久化单元必须有一个名字，不能为空。即persistence-unit name="main"的name不能为空-->
 <persistence-unit name="main"  transaction-type="RESOURCE_LOCAL">
        <provider>org.hibernate.jpa.HibernatePersistenceProvider</provider>
        <properties>
          <!-- 指定连接数据库所用的驱动 -->
          <property name="hibernate.connection.driver_class" value="org.postgresql.Driver"></property>
          <!-- 指定连接数据库的url，hibernate连接的数据库名 -->
          <property name="hibernate.connection.url" value="jdbc:postgresql://128.27.10:8081/test"></property>
          <property name="hibernate.onnection.characterEncoding" value="UTF-8"></property>
          <!-- 指定连接数据库的用户名 -->
          <property name="hibernate.connection.username" value="root"></property>
          <!-- 指定连接数据库的密码 -->
          <property name="hibernate.connection.password" value="123"></property>
          <!-- Connection Pool Provider -->
          <property name="hibernate.connection.provider_class" value="org.hibernate.connection.C3P0ConnectionProvider"></property>
          <!-- 指定连接池里最大连接数 -->
          <property name="hibernate.c3p0.max_size" value="256"></property>
          <!-- 指定连接池里最小连接数 -->
          <property name="hibernate.c3p0.min_size" value="4"></property>
          <!-- 指定连接池里连接的超时时长 -->
          <property name="hibernate.c3p0.timeout" value="4096"></property>
          <!-- 指定连接池里最大缓存多少个Statement对象 -->
          <property name="hibernate.c3p0.max_statements" value="100"></property>
          <property name="hibernate.c3p0.idle_test_period" value="120"></property>
          <property name="hibernate.c3p0.acquire_increment" value="2"></property>
          <property name="hibernate.c3p0.validate" value="true"></property>
          <property name="hibernate.cache.region.factory_class" value="org.hibernate.cache.ehcache.SingletonEhCacheRegionFactory"></property>
          <property name="hibernate.cache.use_second_level_cache" value="true"></property>
          <property name="hibernate.cache.use_query_cache" value="true"></property>
          <!-- 指定数据库方言 -->
          <property name="hibernate.dialect" value="org.hibernate.dialect.PostgreSQL9Dialect"></property>
          <!-- 根据需要自动创建数据表 -->
          <!--<property name="hibernate.hbm2ddl.auto" value="update"></property>-->
          <!-- 显示Hibernate持久化操作所生成的SQL -->
          <property name="hibernate.show_sql" value="true"></property>
          <!-- 将SQL脚本进行格式化后再输出 -->
          <property name="hibernate.format_sql" value="true"></property>
          <property name="hibernate.current_session_context_class" value="managed"></property>
          <property name="javax.persistence.validation.mode" value="none"></property>
        </properties>
    </persistence-unit>
```
### 四、JPA的基本使用
```
public class JPABase(){
    //实体管理器
    protected ThreadLocal<EntityManager> entityManager;
    public JPABase(ThreadLocal<EntityManager> tlemImpl) {
        this.entityManager= tlemImpl;
    }
    //得到当下的entityManager
    public EntityManager currentEM() {
        return (EntityManager)this.entityManager.get();
    }
    //新建一个entityManager
    public EntityManager newEM() {
        return ((EntityManager)this.entityManager.get()).getEntityManagerFactory().createEntityManager();
    }
    //启动事务
    public void begin() throws IllegalStateException {
        ((EntityManager)this.entityManager.get()).getTransaction().begin();
    }
    //更新实体。当实体正在被容器管理时，你可以调用实体的set 方法对数据进行修改，在容器决定flush 时，更新的数据才会同步到数据库。
    public void flush() throws TransactionRequiredException, PersistenceException {
        ((EntityManager)this.entityManager.get()).flush();
    }
    //提交事务
    public void commit() throws IllegalStateException, RollbackException {
        ((EntityManager)this.entityManager.get()).getTransaction().commit();
    }
    //撤消(回滚)当前事务。即撤消事务启动后的所有数据库更新操作，从而不对数据库产生影响
    public void rollback() throws IllegalStateException, PersistenceException {
        ((EntityManager)this.entityManager.get()).getTransaction().rollback();
    }
    //使当前事务只能被撤消
    public void fail() throws IllegalStateException {
        ((EntityManager)this.entityManager.get()).getTransaction().setRollbackOnly();
    }
    //关闭事务
    public void close() throws IllegalStateException {
        ((EntityManager)this.entityManager.get()).close();
        this.entityManager.remove();
    }
     
     //给对象加锁
     public void lock(Object o, LockModeType lock) throws IllegalStateException {
        ((EntityManager)this.entityManager.get()).lock(o, lock);
    }
    //添加实体，相当于add。通常persist之后会调用flush方法将实例插入数据库中
    public void persist(Object o){
         ((EntityManager)this.entityManager.get()).persist(o);
    }
     //addOrUpdate
     public User merge(Object o){
       return ((EntityManager)this.entityManager.get()).merge(o);
    }
    //删除对象
    public void delete(Object o){
         ((EntityManager)this.entityManager.get()).remove(o);
    }
    //返回Query对象,以执行JPQL语句
    public Query query(String jpql) {
        return ((EntityManager)this.entityManager.get()).createQuery(jpql);
    }
 
    //返回Query对象,以执行SQL语句
    public Query createNativeQuery(String sql) {
        return ((EntityManager)this.entityManager.get()).createNativeQuery(sql);
    }
}
```
### 五、JPA的高级查询
首先让我们来了解一下以下类：
> 
CriteriaQuery：查询构造器(查询语句拼装器)。组合要查询的内容和查询的条件。
CriteriaBuiller：CriteriaQuery工厂。
Root：查询根对象,定义查询的From子句中能出现的类型。CriteriaQuery.select(root)就可以拼装出select * from User 这一串。
Predicate：构造条件器。Predicate[] 可以支持多个过滤条件。
构造高级查询上下文：
```
public class CriteriaQueryContext<T> {
    public Class<T> e;
    public CriteriaQuery<T> c;
    public CriteriaBuilder b;
    public Root<T> r;
    ...
}
```
```
public <X> CriteriaQueryContext<X> criteria(Class<X> class) {
        CriteriaQueryContext ctx = new CriteriaQueryContext();
        ctx.e = class;
        ctx.b = ((EntityManager)this.entityManager.get()).getCriteriaBuilder();
        ctx.c = ctx.b.createQuery(class);
        ctx.r = ctx.c.from(class);
        return ctx;
    }
```
**查询条件的使用**
  在criteria 查询中，查询条件通过Predicate 或Expression 实例应用到CriteriaQuery 对象上，**这些条件使用 CriteriaQuery .where 方法应用到CriteriaQuery对象上**。
  CriteriaBuilder 也是作为Predicate 实例的工厂，Predicate对象通过调用CriteriaBuilder 的条件方法（ equal，notEqual， gt， ge，lt， le，between，like等）创建。 Predicate实例也可以用Expression实例的isNull， isNotNull 和in方法获得，复合的Predicate 语句可以使用CriteriaBuilder的and, or等方法构建。
下面的代码片段展示了Predicate 实例检查年龄大于24岁的员工实例:
```
public User getUserById(String userId)
    {
        CriteriaQueryContext<User> c = this.criteria(User.class);
        Predicate condition =  c.b.equal(c.r.<String>get("userid"), userId);
        c.c.where(condition);
        return(
            this.entityManager.createQuery(c.c).getSingleResult();
        );
    }
```
Expression 用在查询语句的select，where和having子句中，该接口有 isNull, isNotNull 和 in方法。
```
CriteriaQueryContext<User> c = this.criteria(User.class);
 c.c.where(c.r.get(User.age).in(20, 24));
```
Criteria Query也允许开发者编写复合谓词，通过该查询可以为多条件测试下面的查询检查两个条件。
```
CriteriaQueryContext<User> c = this.criteria(User.class);
c.c.where(
 criteriaBuilder.and(
  criteriaBuilder.like(c.r.get(User.name), "M%"),
  criteriaBuilder.equal(c.r.get(User.age), 25)
));
```
我们也可以使用Predicate[]来添加多个过滤条件，Predicate[]使用模式为：
```
List<Predicate> predicatesList = new ArrayList<Predicate>();
predicatesList.add(.....Pridicate....)
criteriaQuery.where(predicatesList.toArray(new Predicate[predicatesList.size()]));
```
Predicate[]的demo：
```
List<Predicate> predicatesList = new ArrayList<Predicate>();
Predicate p1 = c.b.in(c.r.get("id")).value(id);
predicatesList.add(p1);
Predicate p2 = c.b.in(c.r.get("name")).value(name);
predicatesList.add(p2);
c.c.where(predicatesList.toArray(new Predicate[predicatesList.size()]));
c.c.orderBy(c.b.asc(c.r.get(age)));
```
