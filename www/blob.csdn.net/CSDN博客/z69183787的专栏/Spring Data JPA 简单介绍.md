# Spring Data JPA 简单介绍 - z69183787的专栏 - CSDN博客
2014年04月02日 19:05:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2053
**背景**
考虑到公司应用中数据库访问的多样性和复杂性，目前正在开发UDSL（统一数据访问层），开发到一半的时候，偶遇[SpringData](http://www.springsource.org/spring-data) 工程。发现两者的思路惊人的一致。
于是就花了点时间了解SpringData，可能UDSL II期会基于SpringData做扩展
**SpringData相关资料**
介绍：针对关系型数据库，KV数据库，Document数据库，Graph数据库，Map-Reduce等一些主流数据库，采用统一技术进行访问，并且尽可能简化访问手段。
目前已支持的数据库有（主要）：MongoDB，Neo4j，Redis，Hadoop，JPA等
SpringData官方资料（强烈推荐，文档非常详细）
SpringData主页：[http://www.springsource.org/spring-data](http://www.springsource.org/spring-data)
SpringDataJPA 指南文档：[http://static.springsource.org/spring-data/data-jpa/docs/current/reference/html/](http://static.springsource.org/spring-data/data-jpa/docs/current/reference/html/)  （非常详细）
SpringDataJPA Examples： https://github.com/SpringSource/spring-data-jpa-examples （非常详细的例子）
**Spring-Data-Jpa简介**
Spring Data Jpa 极大简化了数据库访问层代码，只要3步，就能搞定一切
1. 编写Entity类，依照JPA规范，定义实体
2. 编写Repository接口，依靠SpringData规范，定义数据访问接口（注意，只要接口，不需要任何实现）
3. 写一小陀配置文件 （Spring Scheme配置方式极大地简化了配置方式）
下面，我依赖Example中的例子，简单地介绍下以上几个步骤
User.java
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)
 1/**
 2 * User Entity Sample
 3 * 
 4 * @author <a href="mailto:li.jinl@alibaba-inc.com">Stone.J</a> Aug 25, 2011
 5*/
 6  @Entity
 7publicclass  User  extends  AbstractPersistable < Long >  {
 8
 9privatestaticfinallong  serialVersionUID  =   - 2952735933715107252L ;
10
11      @Column(unique  =   true )
12private  String            username;
13private  String            firstname;
14private  String            lastname;
15
16public  String getUsername() {
17return  username;
18      }
19
20publicvoid  setUsername(String username) {
21this .username  =  username;
22      }
23
24public  String getFirstname() {
25return  firstname;
26      }
27
28publicvoid  setFirstname(String firstname) {
29this .firstname  =  firstname;
30      }
31
32public  String getLastname() {
33return  lastname;
34      }
35
36publicvoid  setLastname(String lastname) {
37this .lastname  =  lastname;
38      }
39
没什么技术，JPA规范要求怎么写，它就怎么写
Repository.java
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)
 1/**
 2 * User Repository Interface.
 3 * 
 4 * @author <a href="mailto:li.jinl@alibaba-inc.com">Stone.J</a> Aug 25, 2011
 5*/
 6publicinterface  SimpleUserRepository  extends  CrudRepository < User, Long > , JpaSpecificationExecutor <User >  {
 7
 8public  User findByTheUsersName(String username);
 9
10public  List < User >  findByLastname(String lastname);
11
12      @Query( " select u from User u where u.firstname = ? " )
13public  List < User >  findByFirstname(String firstname);
14
15      @Query( " select u from User u where u.firstname = :name or u.lastname = :name " )
16public  List < User >  findByFirstnameOrLastname(@Param( " name " ) String name);
17
18
需要关注它继承的接口，我简单介绍几个核心接口
Repository： 仅仅是一个标识，表明任何继承它的均为仓库接口类，方便Spring自动扫描识别
CrudRepository： 继承Repository，实现了一组CRUD相关的方法
PagingAndSortingRepository： 继承CrudRepository，实现了一组分页排序相关的方法
JpaRepository： 继承PagingAndSortingRepository，实现一组JPA规范相关的方法
JpaSpecificationExecutor： 比较特殊，不属于Repository体系，实现一组JPA Criteria查询相关的方法
不需要写任何实现类，Spring Data Jpa框架帮你搞定这一切。
Spring Configuration
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)
 1  < beans > 
 2       < bean id = " entityManagerFactory "   class = "org.springframework.orm.jpa.LocalContainerEntityManagerFactoryBean " > 
 3           < property name = " dataSource "  ref = " dataSource "   /> 
 4           < property name = " jpaVendorAdapter " > 
 5               < bean  class = " org.springframework.orm.jpa.vendor.HibernateJpaVendorAdapter " > 
 6                   < property name = " generateDdl "  value = " true "   /> 
 7                   < property name = " database "  value = " HSQL "   /> 
 8               </ bean > 
 9           </ property > 
10           < property name = " persistenceUnitName "  value = " jpa.sample "   /> 
11       </ bean > 
12
13       < bean id = " transactionManager "   class = " org.springframework.orm.jpa.JpaTransactionManager " > 
14           < property name = " entityManagerFactory "  ref = " entityManagerFactory "   /> 
15       </ bean > 
16
17       < jdbc:embedded - database id = " dataSource "  type = " HSQL "   /> 
18
19
20       < jpa:repositories base - package = " org.springframework.data.jpa.example.repository.simple "   /> 
21  </ beans>
核心代码只要配置一行：<jpa:repositories base-package="org.springframework.data.jpa.example.repository.simple" />即可。上面的仅仅是数据源，事务的配置而已。
至此，大功告成，即可运行
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)
 1/**
 2 * Intergration test showing the basic usage of {@link SimpleUserRepository}.
 3 * 
 4 * @author <a href="mailto:li.jinl@alibaba-inc.com">Stone.J</a> Aug 25, 2011
 5*/
 6  @RunWith(SpringJUnit4ClassRunner. class )
 7  @ContextConfiguration(locations  =   " classpath:simple-repository-context.xml " )
 8  @Transactional
 9publicclass  SimpleUserRepositorySample {
10
11      @Autowired
12      SimpleUserRepository repository;
13      User                 user;
14
15      @Before
16publicvoid  setUp() {
17          user  =   new  User();
18          user.setUsername( " foobar " );
19          user.setFirstname( " firstname " );
20          user.setLastname( " lastname " );
21      }
22
23// crud方法测试
24      @Test
25publicvoid  testCrud() {
26          user  =  repository.save(user);
27          assertEquals(user, repository.findOne(user.getId()));
28      }
29
30// method query测试
31      @Test
32publicvoid  testMethodQuery()  throws  Exception {
33          user  =  repository.save(user);
34          List < User >  users  =  repository.findByLastname( " lastname " );
35          assertNotNull(users);
36          assertTrue(users.contains(user));
37      }
38
39// named query测试
40      @Test
41publicvoid  testNamedQuery()  throws  Exception {
42          user  =  repository.save(user);
43          List < User >  users  =  repository.findByFirstnameOrLastname( " lastname " );
44          assertTrue(users.contains(user));
45      }
46
47// criteria query测试
48      @Test
49publicvoid  testCriteriaQuery()  throws  Exception {
50          user  =  repository.save(user);
51          List < User >  users  =  repository.findAll( new  Specification < User > () {
52
53              @Override
54public  Predicate toPredicate(Root < User >  root, CriteriaQuery <?>  query, CriteriaBuilder cb) {
55return  cb.equal(root.get( " lastname " ),  " lastname " );
56              }
57          });
58          assertTrue(users.contains(user));
59      }
60
其中，写操作相对比较简单，我不做详细介绍，针对读操作，我稍微描述下：
Method Query: 方法级别的查询，针对 `findBy` , `find` , `readBy` , `read`, 
```
getBy等前缀的方法，解析方法字符串，生成查询语句，其中支持的关键词有：
```
![](http://www.blogjava.net/images/blogjava_net/stone2083/java/mq.png)
Named Query： 针对一些复杂的SQL，支持原生SQL方式，进行查询，保证性能
Criteria Query： 支持JPA标准中的Criteria Query
备注：
本文只是简单介绍SpringDataJpa功能，要深入了解的同学，建议直接传送到[官方网站](http://www.springsource.org/spring-data/jpa)
