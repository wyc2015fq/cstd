# 项目开发框架-SSM - 菜鸟路上的小白 - CSDN博客





2018年04月24日 10:36:10[somnus_小凯](https://me.csdn.net/u012486840)阅读数：204










# 1.Spring

无需多言，作为开源届数一数二的典例，项目开发中无处不在；
核心IOC容器，用来装载bean（java中的类）-用Spring的IOC容器来管理Bean的生命周期，有了这样一种机制，我们就可以不用在代码中去重复的做new操作。
aop，面向切面编程，spring中最主要的是用于事务方面的使用。

# 2.Spring MVC

作用于web层，相当于controller，与struts中的action一样，都是用来处理用户请求的。同时，相比于struts2来说，更加细粒度，它是基于方法层面的，而struts是基于类层面的。

![](https://user-gold-cdn.xitu.io/2017/10/28/ab2648fd768293a6fa61959ba1dbbc40?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)


# 3.MyBatis

MyBatis 是一款优秀的持久层框架，它支持定制化 SQL、存储过程以及高级映射。MyBatis 避免了几乎所有的 JDBC 代码和手动设置参数以及获取结果集。MyBatis 可以使用简单的 XML 或注解来配置和映射原生信息，将接口和 Java 的 POJOs(Plain Old Java Objects,普通的 Java对象)映射成数据库中的记录。[来自：[www.mybatis.org/mybatis-3/z…](https://link.juejin.im?target=http%3A%2F%2Fwww.mybatis.org%2Fmybatis-3%2Fzh%2Findex.html)]

**他人总结**
- Hibernate功能强大，数据库无关性好，O/R映射能力强，如果你对Hibernate相当精通，而且对Hibernate进行了适当的封装，那么你的项目整个持久层代码会相当简单，需要写的代码很少，开发速度很快，非常爽。 
- Hibernate的缺点就是学习门槛不低，要精通门槛更高，而且怎么设计O/R映射，在性能和对象模型之间如何权衡取得平衡，以及怎样用好Hibernate方面需要你的经验和能力都很强才行。 
- MYBATIS入门简单，即学即用，提供了数据库查询的自动对象绑定功能，而且延续了很好的SQL使用经验，对于没有那么高的对象模型要求的项目来说，相当完美。 
- MYBATIS的缺点就是框架还是比较简陋，功能尚有缺失，虽然简化了数据绑定代码，但是整个底层数据库查询实际还是要自己写的，工作量也比较大，而且不太容易适应快速数据库修改。
# 4.SSM框架整合
本项目将以购物为背景，主要包括商品信息及库存【因为想顺便学习一下事务的处理】、订单信息。下面将从数据库创建、项目结构说明、配置文件、业务代码等方面进行一步步说明。
## 4.1 数据库创建
1.商品表
```bash
CREATE TABLE `goods` (
`goods_id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '商品ID',
`goodsname` varchar(100) NOT NULL COMMENT '商品名称',
`number` int(11) NOT NULL COMMENT '商品库存',
 PRIMARY KEY (`goods_id`)
) ENGINE=InnoDB AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8 COMMENT='商品表'
```
初始化表数据
```bash
INSERT INTO `goods` (`goods_id`, `goodsname`, `number`)
VALUES (1001, 'SN卫衣', 15)
```
2.订单表
```bash
CREATE TABLE `orderinfo` (
`order_id` varchar(20) NOT NULL COMMENT '订单编号',
`goods_id` bigint(18) NOT NULL COMMENT '商品ID',
`user_id` bigint(10) NOT NULL COMMENT '用户ID',
`order_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '下单时间' ,
 PRIMARY KEY (`order_id`),
 INDEX `idx_order_id` (`order_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='订单表'
```
OK，至此表结构及初始化数据构建完成，下面说下基于Mavan的项目结构。
## 项目结构说明
因为项目是使用maven来管理jar包的，先来贴一下，pom.xml的配置
- 
pom.xml
为了避免学习小伙伴崇尚拿来主义【也就是去除了xmlns之类的东西】，这里只放项目依赖的jar包的dependencies；本案例将本着“需则用”的原则，避免在网上看到的各种乱七八糟的依赖都丢进来的情况，造成资源浪费和干扰阅读。

```bash
<dependencies>
      <!-- 单元测试 -->
      <dependency>
          <groupId>junit</groupId>
          <artifactId>junit</artifactId>
          <version>4.11</version>
      </dependency>

      <!-- 1.日志 slf4j-->
      <dependency>
          <groupId>ch.qos.logback</groupId>
          <artifactId>logback-classic</artifactId>
          <version>1.1.1</version>
      </dependency>

      <!-- 2.数据库连接驱动 -->
      <dependency>
          <groupId>mysql</groupId>
          <artifactId>mysql-connector-java</artifactId>
          <version>5.1.37</version>
          <scope>runtime</scope>
      </dependency>
      <!-- 2.数据库连接池 -->
      <dependency>
          <groupId>c3p0</groupId>
          <artifactId>c3p0</artifactId>
          <version>0.9.1.2</version>
      </dependency>

      <!-- 3.MyBatis 以及 spring-mybatis -->
      <dependency>
          <groupId>org.mybatis</groupId>
          <artifactId>mybatis</artifactId>
          <version>3.3.0</version>
      </dependency>
      <dependency>
          <groupId>org.mybatis</groupId>
          <artifactId>mybatis-spring</artifactId>
          <version>1.2.3</version>
      </dependency>

      <!-- 4.Servlet 相关依赖 -->
      <dependency>
          <groupId>taglibs</groupId>
          <artifactId>standard</artifactId>
          <version>1.1.2</version>
      </dependency>
      <dependency>
          <groupId>jstl</groupId>
          <artifactId>jstl</artifactId>
          <version>1.2</version>
      </dependency>
      <dependency>
          <groupId>com.fasterxml.jackson.core</groupId>
          <artifactId>jackson-databind</artifactId>
          <version>2.5.4</version>
      </dependency>
      <dependency>
          <groupId>javax.servlet</groupId>
          <artifactId>javax.servlet-api</artifactId>
          <version>3.1.0</version>
      </dependency>

      <!-- 5.Spring -->

      <!-- 5.1 Spring核心 ：core bean context -->
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-core</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-beans</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>
      <!-- 5.2 Spring jdbc依赖，事务依赖 -->
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-jdbc</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-tx</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>
      <!-- 5.3 Spring web依赖>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-web</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-webmvc</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>
      <!-- 5.4 Spring test -->
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-test</artifactId>
          <version>4.1.7.RELEASE</version>
      </dependency>

      <!-- 6.redis客户端:Jedis【不使用的话可以直接去除】 -->
      <dependency>
          <groupId>redis.clients</groupId>
          <artifactId>jedis</artifactId>
          <version>2.7.3</version>
      </dependency>
      <dependency>
          <groupId>com.dyuproject.protostuff</groupId>
          <artifactId>protostuff-core</artifactId>
          <version>1.0.8</version>
      </dependency>
      <dependency>
          <groupId>com.dyuproject.protostuff</groupId>
          <artifactId>protostuff-runtime</artifactId>
          <version>1.0.8</version>
      </dependency>

      <!-- 7.工具类 -->
      <dependency>
          <groupId>commons-collections</groupId>
          <artifactId>commons-collections</artifactId>
          <version>3.2</version>
      </dependency>
  </dependencies>
```

*项目结构图



![](https://user-gold-cdn.xitu.io/2017/10/28/2c7ad9c13a987f35dec09b1aa58d8eb8?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
*src/test/java：用于junit的测试类*src/main/java:
   dao:数据库处理
   service:业务处理
   enums:项目枚举
   mapper:dao中方法对应mybatis映射文件，Sql就在这里面
   web：控制器，controller
   entity:项目中的实体类，如：商品类和订单类


## 配置文件
- 
jdbc.properties

```bash
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://serverName:port/dbname?useUnicode=true&characterEncoding=utf8
jdbc.username=[填写自己的数据库用户名]
jdbc.password=[填写自己的数据库登录密码]
```
- logback.xml
这里直接用的是控制台输出，如果是生产环境，可以根据具体的需求进行配置。
```bash
<?xml version="1.0" encoding="UTF-8"?>
<configuration debug="true">
<appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
ch.qos.logback.classic.encoder.PatternLayoutEncoder -->
   <encoder>
       <pattern>%d{HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</pattern>
   </encoder>
</appender>
<root level="debug">
   <appender-ref ref="STDOUT" />
</root>
</configuration>
```

- 
mybatis-config
这里主要是MyBaties全局配置文件的配置，可以将一些类的别名、主键自增配置、驼峰命名规则配置等。

```bash
<configuration>
<!-- 配置全局属性 -->
<settings>
   <!-- 使用jdbc的getGeneratedKeys获取数据库自增主键值 -->
   <setting name="useGeneratedKeys" value="true" />

   <!-- 使用列别名替换列名 默认:true -->
   <setting name="useColumnLabel" value="true" />

   <!-- 开启驼峰命名转换:Table{create_time} -> Entity{createTime} -->
   <setting name="mapUnderscoreToCamelCase" value="true" />
</settings>
</configuration>
```

- 
spring 相关配置文件
为了更加清晰的了解spring各个组件的作用，这里将数据源的配置、事务配置和视图解析器的配置分开来。
**spring-dao.xml**
这里面主要就是spring配置整合mybatis的具体过程，具体包括：
1.引入数据库配置文件
2.配置数据源【数据库连接池】
3.配置SqlSessionFactory对象
4.配置扫描Dao接口包，动态实现Dao接口，注入到spring容器中

```bash
<!-- 1.配置数据库相关参数properties的属性：${url} -->
<context:property-placeholder location="classpath:jdbc.properties" />

<!-- 2.数据库连接池 -->
<bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
   <!-- 配置连接池属性 -->
   <property name="driverClass" value="${jdbc.driver}" />
   <property name="jdbcUrl" value="${jdbc.url}" />
   <property name="user" value="${jdbc.username}" />
   <property name="password" value="${jdbc.password}" />

   <!-- c3p0连接池的私有属性 -->
   <property name="maxPoolSize" value="30" />
   <property name="minPoolSize" value="10" />
   <!-- 关闭连接后不自动commit -->
   <property name="autoCommitOnClose" value="false" />
   <!-- 获取连接超时时间 -->
   <property name="checkoutTimeout" value="10000" />
   <!-- 当获取连接失败重试次数 -->
   <property name="acquireRetryAttempts" value="2" />
</bean>

<!-- 3.配置SqlSessionFactory对象 -->
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
   <!-- 注入数据库连接池 -->
   <property name="dataSource" ref="dataSource" />
   <!-- 配置MyBaties全局配置文件:mybatis-config.xml -->
   <property name="configLocation" value="classpath:mybatis-config.xml" />
   <!-- 扫描entity包 使用别名 -->
   <property name="typeAliasesPackage" value="com.glmapper.framerwork.entity" />
   <!-- 扫描sql配置文件:mapper需要的xml文件 -->
   <property name="mapperLocations" value="com.glmapper.framerwork.mapper/*.xml" />
</bean>

<!-- 4.配置扫描Dao接口包，动态实现Dao接口，注入到spring容器中 -->
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
   <!-- 注入sqlSessionFactory -->
   <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory" />
   <!-- 给出需要扫描Dao接口包 -->
   <property name="basePackage" value="com.glmapper.framerwork.dao" />
</bean>
```

- spring-service
实际的开发过程中事务一般都是在service层进行操作。因此用一个单独的spring-service.xml来进行事务的相关的配置
```bash
<!-- 扫描service包下所有使用注解的类型 -->
<context:component-scan base-package="com.glmapper.framerwork.service" />
<!-- 配置事务管理器 -->
<bean id="transactionManager"
   class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
   <!-- 注入数据库连接池 -->
   <property name="dataSource" ref="dataSource" />
</bean>
<!-- 配置基于注解的声明式事务 -->
<tx:annotation-driven transaction-manager="transactionManager" />
```

- 
spring-web.xml
配置SpringMVC；需要说明一下，一般我们在实际的开发过程中，会配置json2map解析。这里没有用到就不贴出来，读者可以自行网上搜索一波。

```bash
<!-- 1.开启SpringMVC注解模式 -->
<mvc:annotation-driven />
<!-- 2.静态资源默认servlet配置
   (1)加入对静态资源的处理：js,css,图片等
   (2)允许使用"/"做整体映射
-->
<mvc:default-servlet-handler/>

<!-- 3.配置视图解析器ViewResolver -->
<bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView" />
    <property name="prefix" value="/WEB-INF/jsp/" />
    <property name="suffix" value=".jsp" />
</bean>

<!-- 4.扫描web相关的bean -->
<context:component-scan base-package="com.glmapper.framerwork.web" />
```

- web.xml
```bash
<!-- 编码过滤器 -->  
<filter>  
   <filter-name>encodingFilter</filter-name>  
   <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>  
   <async-supported>true</async-supported>  
   <init-param>  
       <param-name>encoding</param-name>  
       <param-value>UTF-8</param-value>  
   </init-param>  
</filter>  
<filter-mapping>  
   <filter-name>encodingFilter</filter-name>  
   <url-pattern>/*</url-pattern>  
</filter-mapping>  
<!-- Spring监听器 -->  
<listener>  
   <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>  
</listener>  
<!-- 防止Spring内存溢出监听器 -->  
<listener>  
   <listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>  
</listener> 
<!-- 配置DispatcherServlet -->
<servlet>
   <servlet-name>mvc-dispatcher</servlet-name>
   <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
   <!-- 配置springMVC需要加载的配置文件
       spring-dao.xml,spring-service.xml,spring-web.xml
       Mybatis - > spring -> springmvc
    -->
   <init-param>
       <param-name>contextConfigLocation</param-name>
       <param-value>classpath:spring/spring-*.xml</param-value>
   </init-param>
</servlet>
<servlet-mapping>
   <servlet-name>mvc-dispatcher</servlet-name>
   <!-- 默认匹配所有的请求 -->
   <url-pattern>/</url-pattern>
</servlet-mapping>
```
至此，所有的配置文件结束，下面将进行具体的代码环节
## 业务代码
这里mapper中的xml文件就不贴了，自行脑补。。。。
- 实体类：包括商品和订单


**商品类**

```bash
/**
 * 商品信息类
 * @author glmapper
 *
 */
public class Goods {
    private long goodsId;// 商品ID
    private String goodsName;// 商品名称
    private int number;// 商品库存

    public long getGoodsId() {
        return goodsId;
    }
    public void setGoodsId(long goodsId) {
        this.goodsId = goodsId;
    }
    public String getGoodsName() {
        return goodsName;
    }
    public void setGoodsName(String goodsName) {
        this.goodsName = goodsName;
    }
    public int getNumber() {
        return number;
    }
    public void setNumber(int number) {
        this.number = number;
    }
}
```

**订单类**

```bash
/**
 * 订单信息类
 * @author glmapper
 *
 */
public class OrderInfo {
    private String orderId;//订单ID
    private long goodsId;//商品ID
    private long userId;//用户ID
    private Date orderTime;//下单时间
    public String getOrderId() {
        return orderId;
    }
    public void setOrderId(String orderId) {
        this.orderId = orderId;
    }
    public long getGoodsId() {
        return goodsId;
    }
    public void setGoodsId(long goodsId) {
        this.goodsId = goodsId;
    }
    public long getUserId() {
        return userId;
    }
    public void setUserId(long userId) {
        this.userId = userId;
    }
    public Date getOrderTime() {
        return orderTime;
    }
    public void setOrderTime(Date orderTime) {
        this.orderTime = orderTime;
    }
}
```
- 商品dao

```bash
public interface GoodsDao {

    /**
     * 通过ID查询单件商品信息
     * 
     * @param id
     * @return
     */
    Goods queryById(long id);

    /**
     * 查询所有商品信息
     * 
     * @param offset 查询起始位置
     * @param limit 查询条数
     * @return
     */
    List<Goods> queryAll(@Param("offset") int offset, @Param("limit") int limit);

    /**
     * 减少商品库存
     * 
     * @param bookId
     * @return 如果影响行数等于>1，表示更新的记录行数
     */
    int reduceNumber(long goodsId);

}
```
- 
订单dao

```bash
public interface OrderInfoDao {

  /**
   * 插入订单记录
   * 
   * @param OrderInfo orderInfo
   * @return 插入的行数
   */
  int insertOrderInfo(OrderInfo orderInfo);

  /**
   * 通过主键查询订单记录，返回订单实体 
   * @param orderId
   * @return
   */
  OrderInfo queryByOrderId(String orderId);
}
```

- 
下单服务接口orderService

```bash
@Service("orderService")
public class OrderServiceImpl implements OrderService {
  //log生成器
  private Logger logger = LoggerFactory.getLogger(OrderServiceImpl.class);

  // 注入dao依赖【商品dao，订单dao】
  @Autowired
  private GoodsDao goodsDao;
  @Autowired
  private OrderInfoDao orderInfoDao;

  @Override
  public Goods getById(long goodsId) {
      // TODO Auto-generated method stub
      return goodsDao.queryById(goodsId);
  }

  @Override
  public List<Goods> getList(int offset,int limit) {
      // TODO Auto-generated method stub
      return goodsDao.queryAll(offset, limit);
  }

  @Override
  @Transactional
  public OrderInfo buyGoods(long goodsId, long userId) {
      //扣减库存，插入订单 =一个事务  如果失败则执行回滚
      try {
          // 减库存
          int update = goodsDao.reduceNumber(goodsId);
          if (update <= 0) {// 库存不足
              throw new NoNumberException("no number");
          } else {
              // 执行预约操作
              OrderInfo orderInfo=new OrderInfo();
              orderInfo.setGoodsId(goodsId);
              orderInfo.setUserId(userId);
              orderInfo.setOrderTime(new Date());
              String orderId=getRandomOrderId(goodsId);
              orderInfo.setOrderId(orderId);
              int insert = orderInfoDao.insertOrderInfo(orderInfo);
              if (insert <= 0) {// 重复预约
                  throw new RepeatAppointException("repeat appoint");
              } else {// 预约成功
                  return orderInfo;
              }
          }
      } catch (Exception e) {
          //这里可以丰富下具体的返回信息
          logger.error("下单失败");
      } 
      return null;
  }

  private String getRandomOrderId(long goodsId) {
      SimpleDateFormat dateFormater = new SimpleDateFormat("yyyyMMddhhmmss");
      String prefix=dateFormater.format(new Date());
      String goodsIdStr=goodsId+"";
      String temp="";
      for (int i = 0; i < 6; i++) {
          Random random=new Random(goodsIdStr.length()-1);
          temp+=goodsIdStr.charAt(random.nextInt());
      }
      return prefix+temp;
  }
}
```

OK，至此所有核心代码及配置文件罗列完毕；【mapper中的xml和具体的controller就不贴了，相信大家对这个也不陌生。本文主要意图在于梳理下自己学习中的一些点，SSM框架在实际的应用开发中还会有很多其他的开源技术结合进来，如：quartz,redis等。当前本文的列子就是一个空壳子，以备参考吧】



作者：glmapper
链接：https://juejin.im/post/59f3ef5c51882534aa0691b2
来源：掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



