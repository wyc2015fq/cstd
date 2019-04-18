# SpringMVC + spring3.1.1 + hibernate4.1.0 集成及常见问题总结 - z69183787的专栏 - CSDN博客
2013年12月05日 10:42:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1103
个人分类：[SpringMvc																[Spring-知识																[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)](https://blog.csdn.net/z69183787/article/category/2175163)](https://blog.csdn.net/z69183787/article/category/2176803)
## [下载地址](http://sishuok.com/forum/upload/2012/2/25/struts2spring3hibernate4.rar)
## 一 开发环境
1、动态web工程
2、部分依赖
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- hibernate-release-4.1.0.Final.zip  
- hibernate-validator-4.2.0.Final.jar  
- spring-framework-3.1.1.RELEASE-with-docs.zip  
- proxool-0.9.1.jar  
- log4j 1.2.16
- slf4j -1.6.1
- mysql-connector-java-5.1.10.jar  
- hamcrest 1.3.0RC2  
- ehcache 2.4.3
3、为了方便学习，暂没有使用maven构建工程
## 二 工程主要包括内容
1、springMVC + spring3.1.1 + hibernate4.1.0集成
2、通用DAO层 和 Service层
3、二级缓存 Ehcache
4、REST风格的表现层
5、通用分页（两个版本）
５.1、首页 上一页,下一页 尾页 跳转
５.2、上一页 1 2 3 4 5 下一页
6、数据库连接池采用proxool
7、spring集成测试    
8、表现层的 java validator框架验证（采用hibernate-validator-4.2.0实现）
9、视图采用JSP，并进行组件化分离
## 三 TODO LIST  将本项目做成脚手架方便以后新项目查询
1、Service层进行AOP缓存（缓存使用Memcached实现）
2、单元测试（把常见的桩测试、伪实现、模拟对象演示一遍 区别集成测试）
3、监控功能
后台查询hibernate二级缓存 hit/miss率功能      
   后台查询当前服务器状态功能（如 线程信息、服务器相关信息）
4、spring RPC功能
5、spring集成 quartz 进行任务调度
6、spring集成 java mail进行邮件发送
7、DAO层将各种常用框架集成进来（方便查询）
8、把工作中经常用的东西 融合进去，作为脚手架，方便以后查询
## 四 集成重点及常见问题
1、spring-config.xml 配置文件：
1.1、该配置文件只加载除表现层之外的所有bean，因此需要如下配置：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- <context:component-scan base-package="cn.javass">  
-     <context:exclude-filter type="annotation" expression="org.springframework.stereotype.Controller"/>  
- </context:component-scan>  
通过exclude-filter 把所有 @Controller注解的表现层控制器组件排除
1.2、国际化消息文件配置
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- <!-- 国际化的消息资源文件 -->  
-     <bean id="messageSource"class="org.springframework.context.support.ReloadableResourceBundleMessageSource">  
-         <property name="basenames">  
-             <list>  
-                 <!-- 在web环境中一定要定位到classpath 否则默认到当前web应用下找  -->  
-                 <value>classpath:messages</value>  
-             </list>  
-         </property>  
-         <property name="defaultEncoding" value="UTF-8"/>  
-         <property name="cacheSeconds" value="60"/>  
-     </bean>  
此处basenames内一定是 classpath:messages ，如果你写出“messages”，将会到你的web应用的根下找 即你的messages.properties一定在 web应用/messages.propertis。
1.3、hibernate的sessionFactory配置 需要使用org.springframework.orm.hibernate4.LocalSessionFactoryBean，其他都是类似的，具体看源代码。
1．4、<aop:aspectj-autoproxy expose-proxy="true"/> 实现@AspectJ注解的，默认使用AnnotationAwareAspectJAutoProxyCreator进行AOP代理，它是BeanPostProcessor的子类，在容器启动时Bean初始化开始和结束时调用进行AOP代理的创建，因此只对当容器启动时有效，使用时注意此处。
1.5、声明式容器管理事务
建议使用声明式容器管理事务，而不建议使用注解容器管理事务（虽然简单），但太分布式了，采用声明式容器管理事务一般只对service层进行处理。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- <tx:advice id="txAdvice" transaction-manager="txManager">  
-     <tx:attributes>  
-         <tx:method name="save*" propagation="REQUIRED" />  
-         <tx:method name="add*" propagation="REQUIRED" />  
-         <tx:method name="create*" propagation="REQUIRED" />  
-         <tx:method name="insert*" propagation="REQUIRED" />  
-         <tx:method name="update*" propagation="REQUIRED" />  
-         <tx:method name="merge*" propagation="REQUIRED" />  
-         <tx:method name="del*" propagation="REQUIRED" />  
-         <tx:method name="remove*" propagation="REQUIRED" />  
-         <tx:method name="put*" propagation="REQUIRED" />  
-         <tx:method name="use*" propagation="REQUIRED"/>  
-         <!--hibernate4必须配置为开启事务 否则 getCurrentSession()获取不到-->  
-         <tx:method name="get*" propagation="REQUIRED" read-only="true" />  
-         <tx:method name="count*" propagation="REQUIRED" read-only="true" />  
-         <tx:method name="find*" propagation="REQUIRED" read-only="true" />  
-         <tx:method name="list*" propagation="REQUIRED" read-only="true" />  
-         <tx:method name="*" read-only="true" />  
-     </tx:attributes>  
- </tx:advice>  
- <aop:config expose-proxy="true">  
-     <!-- 只对业务逻辑层实施事务 -->  
-     <aop:pointcut id="txPointcut" expression="execution(* cn.javass..service..*.*(..))" />  
-     <aop:advisor advice-ref="txAdvice" pointcut-ref="txPointcut"/>  
- </aop:config>  
此处一定注意 使用 hibernate4，在不使用OpenSessionInView模式时，在使用getCurrentSession()时会有如下问题：
当有一个方法list 传播行为为Supports，当在另一个方法getPage()（无事务）调用list方法时会抛出org.hibernate.HibernateException: No Session found for current thread 异常。
这是因为getCurrentSession()在没有session的情况下不会自动创建一个，不知道这是不是Spring3.1实现的bug，欢迎大家讨论下。
因此最好的解决方案是使用REQUIRED的传播行为。
二、spring-servlet.xml：
2.1、表现层配置文件，只应加装表现层Bean，否则可能引起问题。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- <!-- 开启controller注解支持 -->  
- <!-- 注：如果base-package=cn.javass 则注解事务不起作用-->  
- <context:component-scan base-package="cn.javass.demo.web.controller">  
-     <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>  
- </context:component-scan>  
此处只应该加载表现层组件，如果此处还加载dao层或service层的bean会将之前容器加载的替换掉，而且此处不会进行AOP织入，所以会造成AOP失效问题（如事务不起作用），再回头看我们的1.4讨论的。
2.2、<mvc:view-controller path="/" view-name="forward:/index"/> 表示当访问主页时自动转发到index控制器。
2.3、静态资源映射
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- <!-- 当在web.xml 中   DispatcherServlet使用     <url-pattern>/</url-pattern> 映射时，能映射静态资源 -->  
- <mvc:default-servlet-handler/>  
- <!-- 静态资源映射 -->  
- <mvc:resources mapping="/images/**" location="/WEB-INF/images/" />  
- <mvc:resources mapping="/css/**" location="/WEB-INF/css/" />  
- <mvc:resources mapping="/js/**" location="/WEB-INF/js/" />  
以上是配置文件部分，接下来来看具体代码。
三、通用DAO层Hibernate4实现
为了减少各模块实现的代码量，实际工作时都会有通用DAO层实现，以下是部分核心代码：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- publicabstractclass BaseHibernateDao<M extends java.io.Serializable, PK extends java.io.Serializable> implements IBaseDao<M, PK> {  
- 
- protectedstaticfinal Logger LOGGER = LoggerFactory.getLogger(BaseHibernateDao.class);  
- 
- privatefinal Class<M> entityClass;  
- privatefinal String HQL_LIST_ALL;  
- privatefinal String HQL_COUNT_ALL;  
- privatefinal String HQL_OPTIMIZE_PRE_LIST_ALL;  
- privatefinal String HQL_OPTIMIZE_NEXT_LIST_ALL;  
- private String pkName = null;  
- 
- @SuppressWarnings("unchecked")  
- public BaseHibernateDao() {  
- this.entityClass = (Class<M>) ((ParameterizedType) getClass().getGenericSuperclass()).getActualTypeArguments()[0];  
-         Field[] fields = this.entityClass.getDeclaredFields();  
- for(Field f : fields) {  
- if(f.isAnnotationPresent(Id.class)) {  
- this.pkName = f.getName();  
-             }  
-         }  
- 
-         Assert.notNull(pkName);  
- //TODO @Entity name not null
-         HQL_LIST_ALL = "from " + this.entityClass.getSimpleName() + " order by " + pkName + " desc";  
-         HQL_OPTIMIZE_PRE_LIST_ALL = "from " + this.entityClass.getSimpleName() + " where " + pkName + " > ? order by " + pkName + " asc";  
-         HQL_OPTIMIZE_NEXT_LIST_ALL = "from " + this.entityClass.getSimpleName() + " where " + pkName + " < ? order by " + pkName + " desc";  
-         HQL_COUNT_ALL = " select count(*) from " + this.entityClass.getSimpleName();  
-     }  
- 
- @Autowired
- @Qualifier("sessionFactory")  
- private SessionFactory sessionFactory;  
- 
- public Session getSession() {  
- //事务必须是开启的，否则获取不到
- return sessionFactory.getCurrentSession();  
-     }  
- ……  
- }  
Spring3.1集成Hibernate4不再需要HibernateDaoSupport和HibernateTemplate了，直接使用原生API即可。
四、通用Service层代码 此处省略，看源代码，有了通用代码后CURD就不用再写了。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- @Service("UserService")  
- publicclass UserServiceImpl extends BaseService<UserModel, Integer> implements UserService {  
- 
- privatestaticfinal Logger LOGGER = LoggerFactory.getLogger(UserServiceImpl.class);  
- 
- private UserDao userDao;  
- 
- @Autowired
- @Qualifier("UserDao")  
- @Override
- publicvoid setBaseDao(IBaseDao<UserModel, Integer> userDao) {  
- this.baseDao = userDao;  
- this.userDao = (UserDao) userDao;  
-     }  
- 
- 
- 
- @Override
- public Page<UserModel> query(int pn, int pageSize, UserQueryModel command) {  
- return PageUtil.getPage(userDao.countQuery(command) ,pn, userDao.query(pn, pageSize, command), pageSize);  
-     }  
- }  
- 
五、表现层 Controller实现
采用SpringMVC支持的REST风格实现，具体看代码，此处我们使用了java Validator框架 来进行 表现层数据验证
在Model实现上加验证注解
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- @Pattern(regexp = "[A-Za-z0-9]{5,20}", message = "{username.illegal}") //java validator验证（用户名字母数字组成，长度为5-10）
- private String username;  
- 
- @NotEmpty(message = "{email.illegal}")  
- @Email(message = "{email.illegal}") //错误消息会自动到MessageSource中查找
- private String email;  
- 
- @Pattern(regexp = "[A-Za-z0-9]{5,20}", message = "{password.illegal}")  
- private String password;  
- 
- @DateFormat( message="{register.date.error}")//自定义的验证器
- private Date registerDate;  
在Controller中相应方法的需要验证的参数上加@Valid即可
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- @RequestMapping(value = "/user/add", method = {RequestMethod.POST})  
- public String add(Model model, @ModelAttribute("command") @Valid UserModel command, BindingResult result)  
六、Spring集成测试
使用Spring集成测试能很方便的进行Bean的测试，而且使用@TransactionConfiguration(transactionManager = "txManager", defaultRollback = true)能自动回滚事务，清理测试前后状态。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2625.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2625.html#)[打印](http://sishuok.com/forum/blogPost/list/2625.html#)
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration(locations = {"classpath:spring-config.xml"})  
- @Transactional
- @TransactionConfiguration(transactionManager = "txManager", defaultRollback = true)  
- publicclass UserServiceTest {  
- 
-     AtomicInteger counter = new AtomicInteger();  
- 
- @Autowired
- private UserService userService;  
-     ……    
- }  
其他部分请直接看源码，欢迎大家讨论。

补充spring3.1.1源代码分析当 传播行为为 Support时报 org.hibernate.HibernateException: No Session found for current thread 异常：
spring3.1开始 不提供（没有这个东西了）Hibernate4的 DaoSupport和Template，，而是直接使用原生的Hibernate4 API 
如在 Hibernate3中 HibernateTemplate中有如下代码 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- protected Session getSession() {  
- if (isAlwaysUseNewSession()) {  
- return SessionFactoryUtils.getNewSession(getSessionFactory(), getEntityInterceptor());  
-         }  
- elseif (isAllowCreate()) {//默认是true，也就是即使你的传播行为是Supports也一定会有session存在的
- return SessionFactoryUtils.getSession(  
-                     getSessionFactory(), getEntityInterceptor(), getJdbcExceptionTranslator());  
-         }  
- elseif (SessionFactoryUtils.hasTransactionalSession(getSessionFactory())) {  
- return SessionFactoryUtils.getSession(getSessionFactory(), false);  
-         }  
- else {  
- try {  
- return getSessionFactory().getCurrentSession();  
-             }  
- catch (HibernateException ex) {  
- thrownew DataAccessResourceFailureException("Could not obtain current Hibernate Session", ex);  
-             }  
-         }  
-     }  
但我们使用的是Hibernate4原生API，使用SpringSessionContext获取session，而这个isAllowCreate选项默认为false 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-  * Retrieve the Spring-managed Session for the current thread, if any.
-  */
- public Session currentSession() throws HibernateException {  
- try {  
- return (org.hibernate.classic.Session) SessionFactoryUtils.doGetSession(this.sessionFactory, false);//最后的false即是
-     }  
- catch (IllegalStateException ex) {  
- thrownew HibernateException(ex.getMessage());  
-     }  
- }  
SessionFactoryUtils类 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicstatic Session doGetSession(SessionFactory sessionFactory, boolean allowCreate)  
- throws HibernateException, IllegalStateException {  
- 
- return doGetSession(sessionFactory, null, null, allowCreate);  
- }  
可否认为这是集成Hibernate4的bug，或者采用OpenSessionInView模式解决或使用Required传播行为。

原创内容，转载请注明私塾在线【[http://sishuok.com/forum/blogPost/list/2625.html](http://sishuok.com/forum/blogPost/list/2625.html#7523)】
