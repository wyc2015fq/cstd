# 简单商城demo - 建建的博客 - CSDN博客
2018年01月12日 09:18:22[纪建](https://me.csdn.net/u013898698)阅读数：1128
[Spring+Spring MVC+MyBatis](http://www.cnblogs.com/Leo_wl/p/5722023.html)
# [Spring+Spring MVC+MyBatis](http://www.cnblogs.com/best/p/5688040.html)
**目录**
- [一、新建一个基于Maven的Web项目](http://www.cnblogs.com/best/p/5688040.html#_label0)
- [二、创建数据库与表](http://www.cnblogs.com/best/p/5688040.html#_label1)
- [三、添加依赖包](http://www.cnblogs.com/best/p/5688040.html#_label2)
- [四、新建POJO实体层](http://www.cnblogs.com/best/p/5688040.html#_label3)
- [五、新建MyBatis SQL映射层](http://www.cnblogs.com/best/p/5688040.html#_label4)
- [六、JUnit测试数据访问](http://www.cnblogs.com/best/p/5688040.html#_label5)
- [七、完成Spring整合MyBatis配置](http://www.cnblogs.com/best/p/5688040.html#_label6)
- [八、配置web.xml加载Spring容器与MVC](http://www.cnblogs.com/best/p/5688040.html#_label7)
- [九、创建服务层](http://www.cnblogs.com/best/p/5688040.html#_label8)
- [十、完成商品管理功能](http://www.cnblogs.com/best/p/5688040.html#_label9)- [10.1、商品列表与分页](http://www.cnblogs.com/best/p/5688040.html#_lab2_9_0)
- [11.2、删除与多删除功能](http://www.cnblogs.com/best/p/5688040.html#_lab2_9_1)
- [11.3、新增商品功能](http://www.cnblogs.com/best/p/5688040.html#_lab2_9_2)
- [11.4、编辑商品功能](http://www.cnblogs.com/best/p/5688040.html#_lab2_9_3)
- [11.5、上传图片功能](http://www.cnblogs.com/best/p/5688040.html#_lab2_9_4)
- [11.6、日志、首页、样式与最终的控制器](http://www.cnblogs.com/best/p/5688040.html#_lab2_9_5)
- [十二、总结](http://www.cnblogs.com/best/p/5688040.html#_label10)
- [十三、示例下载与预览](http://www.cnblogs.com/best/p/5688040.html#_label11)
与SSH（Struts/Spring/Hibernate/）一样，Spring+SpringMVC+MyBatis也有一个简称SSM，Spring实现业务对象管理，Spring MVC负责请求的转发和视图管理, MyBatis作为数据对象持久化引擎。这样搭配的优点是：轻量、自由度高、Spring与Spring MVC契合度更好。通过一个商品管理示例完成SSM框架的集成，可以将前面学习过的一些内容整合起来，使用到的知识包含：Spring、Spring MVC、MyBatis、JSR303校验、分页、文件上传、路径处理等。
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728112148388-1693437837.png)
# 一、新建一个基于Maven的Web项目
1.1、请勾选“Create a simple project”，创建一个简单的项目，这里不使用模板。也可以使用模板，选择WebApp，如果使用模板这里就不应该勾选。如下图所示：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720100400372-697340630.png)
1.2、填写好包名、项目名，选择打包类型为：war，如下图所示：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720100507310-282337315.png)
1.3、项目创建好后可能会发现有错误，选择项目，右键“属性properties”->"层面Project Facets"->"Java"修改版本号为1.7，默认为1.5或其它版本，先去掉“Dynamic Web Module”保存后再勾选，选择版本为3.0，再按箭头所示操作，步骤如下图所示：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720100715247-506135319.png)
1.4、删除WebContent后会发现项目的pom.xml文件报错，是因为找不到指定位置的web.xml文件引起的。再进入项目的属性，选择“Deployment Assembly”项目部署项，删除“src/test/java”、“src/test/resources”与“WebContent”目录，因为这三项不需要部署出去。
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720100813076-1730636916.png)
1.5、新建完成后发现有错误，是因为没有JavaEE Server Runtime引起的，在项目上右键属性选择“Java Build Path”项，点击“Add Library...”添加引用。也可以不选择Server Runtime可以在Maven中直接引用。目录结构如下所示：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720100917357-166855389.png)
提示：如果您是第一次使用Maven，详细的步骤请查看另一篇随笔：《[Spring整合MyBatis（Maven+MySQL）一](http://www.cnblogs.com/best/p/5638827.html)》。
# 二、创建数据库与表
打开MySQL数据库，创建一个表，这里以goods表为例，一个用于存放商品的表，共4个字段id表示编号，name表示商品名称，picture表示图片，price表示价格。SQL脚本如下：
```
/*
Navicat MySQL Data Transfer
Source Server         : localhost
Source Server Version : 50536
Source Host           : localhost:3306
Source Database       : db1
Target Server Type    : MYSQL
Target Server Version : 50536
File Encoding         : 65001
Date: 2016-07-20 10:13:58
*/
SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `goods`
-- ----------------------------
DROP TABLE IF EXISTS `goods`;
CREATE TABLE `goods` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(200) NOT NULL,
  `price` decimal(10,2) DEFAULT '0.00',
  `picture` varchar(100) DEFAULT 'default.jpg',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of goods
-- ----------------------------
INSERT INTO `goods` VALUES ('1', 'G7 中原G7三合一浓醇咖啡固体饮料1200', '66.50', '1.jpg');
INSERT INTO `goods` VALUES ('2', '百草味东北松子200gx2袋 坚果炒货零', '42.90', '2.jpg');
INSERT INTO `goods` VALUES ('3', '奈津香 桂圆干500gx2袋莆田特产5A桂', '39.90', '3.jpg');
INSERT INTO `goods` VALUES ('4', '益达尊享护齿装草本40粒+冰柠40粒+西', '25.90', '4.jpg');
INSERT INTO `goods` VALUES ('5', '猴坑茶业2016新茶原产地手工太平猴魁特', '168.00', '5.jpg');
INSERT INTO `goods` VALUES ('6', '嘻鱿记 休闲零食 麻辣香辣奶香炭烧 5种', '39.80', '6.jpg');
INSERT INTO `goods` VALUES ('7', '荣业鸿福五分瘦腊肠 香港土特产香肠腊味', '126.80', '7.jpg');
INSERT INTO `goods` VALUES ('8', '蓓琳娜（BELLINA）3L PDO特级初榨橄榄油', '178.00', '8.jpg');
INSERT INTO `goods` VALUES ('10', '荣业鸿福五分瘦腊肠 香港土特产香肠腊味', '30.60', 'b454b44f-868e-4efe-ae17-91e9e6a58390.jpg');
表结构如下所示：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720101213888-397415413.png)
# 三、添加依赖包
项目主要依赖的jar包有Spring核心包、Spring AOP包、Spring MVC包、MyBatis ORM包、MyBatis-Spring适配包、JSTL、JUnit、Log4j2等，具体的pom.xml文件如下：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.zhangguo</groupId>
    <artifactId>SSMall</artifactId>
    <version>0.0.3</version>
    <packaging>war</packaging>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <spring.version>4.3.0.RELEASE</spring.version>
    </properties>
    <dependencies>
        <!--Spring框架核心库 -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <!-- Spring MVC -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <!-- aspectJ AOP 织入器 -->
        <dependency>
            <groupId>org.aspectj</groupId>
            <artifactId>aspectjweaver</artifactId>
            <version>1.8.9</version>
        </dependency>
        <!--mybatis-spring适配器 -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>1.3.0</version>
        </dependency>
        <!--Spring java数据库访问包，在本例中主要用于提供数据源 -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <!--mysql数据库驱动 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.38</version>
        </dependency>
        <!--log4j日志包 -->
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-core</artifactId>
            <version>2.6.1</version>
        </dependency>
        <!-- mybatis ORM框架 -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>3.4.1</version>
        </dependency>
        <!-- JUnit单元测试工具 -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.10</version>
        </dependency>
        <!--c3p0 连接池 -->
        <dependency>
            <groupId>c3p0</groupId>
            <artifactId>c3p0</artifactId>
            <version>0.9.1.2</version>
        </dependency>
        <!-- JSTL -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>jstl</artifactId>
            <version>1.2</version>
        </dependency>
        <!-- Servlet核心包 -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.0.1</version>
            <scope>provided</scope>
        </dependency>
        <!--JSP -->
        <dependency>
            <groupId>javax.servlet.jsp</groupId>
            <artifactId>jsp-api</artifactId>
            <version>2.1</version>
            <scope>provided</scope>
        </dependency>
        <!-- jackson -->
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-core</artifactId>
            <version>2.5.2</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.5.2</version>
        </dependency>
        <!--JSR303 -->
        <dependency>
            <groupId>org.hibernate</groupId>
            <artifactId>hibernate-validator</artifactId>
            <version>5.2.2.Final</version>
        </dependency>
        <!--文件上传 -->
        <dependency>
            <groupId>commons-io</groupId>
            <artifactId>commons-io</artifactId>
            <version>2.4</version>
        </dependency>
        <dependency>
            <groupId>commons-fileupload</groupId>
            <artifactId>commons-fileupload</artifactId>
            <version>1.3.1</version>
        </dependency>
        <!-- FreeMarker -->
        <dependency>
            <groupId>org.freemarker</groupId>
            <artifactId>freemarker</artifactId>
            <version>2.3.23</version>
        </dependency>
    </dependencies>
</project>
如果是第一次依赖相关的包，则需要下载时间，请耐心等待，如果下载失败请手动下载(http://search.maven.org/)后复制到本地的资源库中。依赖后的项目结果如下：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720101655669-1938670290.png)
    ![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720101723013-2044975351.png)
# 四、新建POJO实体层
为了实现与数据库中的books表进行关系映射新建一个Goods商品类，具体代码如下：
```
package com.zhangguo.ssmall.entities;
import java.io.Serializable;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Pattern;
/**
 * 商品实体
 *
 */
public class Goods implements Serializable {
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    /*
     * 编号
     */
    private int id;
    /*
     * 名称
     */
    @Pattern(regexp="^[^><&#]{1,50}$",message="{pattern}")
    @NotNull(message="{notNull}")
    private String name;
    /*
     * 价格
     */
    @Min(value=1,message="必须大于或等于1")
    private double price;
    /*
     * 图片
     */
    private String picture;
    
    public Goods() {
    }
    
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public double getPrice() {
        return price;
    }
    public void setPrice(double price) {
        this.price = price;
    }
    public String getPicture() {
        return picture;
    }
    public void setPicture(String picture) {
        this.picture = picture;
    }
    
    @Override
    public String toString() {
        return "id："+getId()+"，name："+getName()+"，price："+getPrice()+"，picture："+getPicture();
    }
}
为了实现校验，在成员变量上设置了一些注解信息。
# 五、新建MyBatis SQL映射层
这个项目中我们采用接口与xml结合的形式完成关系与对象间的映射，在接口中定义一些数据访问的方法，在xml文件中定义实现数据访问需要的sql脚本。商品数据访问映射接口如下：
```
package com.zhangguo.ssmall.mapper;
import java.util.List;
import org.apache.ibatis.annotations.Param;
import com.zhangguo.ssmall.entities.Goods;
public interface GoodsDAO {
    
    /**
     * 获得商品信息并分页
     */
    public List<Goods> getGoodsPager(@Param("skip") int skip,@Param("size") int size);
    
    /**
     * 获得单个商品通过编号
     */
    public Goods getGoodsById(int id);
    
    /**
     * 获得商品总数
     */
    public int getGoodsCount();
    
    /*
     * 新增加商品
     */
    public int insert(Goods entity);
    
    /**
     * 删除商品
     */
    public int delete(int id);
    
    /**
     * 修改商品
     */
    public int update(Goods entity);
    
}
为MyBatis ORM创建的映射文件GoodsMapper.xml（命名尽量都遵循一个规则，便于扫描，这里约定以实体名+Mapper）如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<!--命名空间应该是对应接口的包名+接口名 -->
<mapper namespace="com.zhangguo.ssmall.mapper.GoodsDAO">
    <!--获得商品信息并分页 -->
    <select id="getGoodsPager" resultType="Goods">
        select
        id,name,price,picture from goods limit #{skip},#{size}
    </select>
    <!-- 获得单个商品通过编号 -->
    <select id="getGoodsById" parameterType="int" resultType="Goods">
        select
        id,name,price,picture from goods where id=#{id}
    </select>
    <!--获得商品总数 -->
    <select id="getGoodsCount" resultType="int">
        select count(*) from goods
    </select>
    <!--新增加商品 -->
    <insert id="insert" parameterType="Goods">
        insert into
        goods(name,price,picture) values(#{name},#{price},#{picture});
    </insert>
    <!-- 删除商品 -->
    <delete id="delete">
        delete from goods where id=#{id}
    </delete>
    <!-- 修改商品 -->
    <update id="update" parameterType="Goods">
        update goods set
        name=#{name},price=#{price},picture=#{picture} where id=#{id}
    </update>
</mapper>
# 六、JUnit测试数据访问
为了保证数据访问正常，使用JUnit进行单元测试，在另一个源代码目录src/test/java下添加一个名为TestGoods的测试用例，编写完成的测试用例如下：
```
package com.zhangguo.ssmall.test;
import java.util.List;
import org.apache.ibatis.session.SqlSession;
import org.junit.Test;
import com.zhangguo.ssmall.entities.Goods;
import com.zhangguo.ssmall.mapper.GoodsDAO;
import junit.framework.Assert;
public class TestGoods{
    @Test
    public void getGoodsPagerTest() {
        int skip=4;
        int size=2;
        SqlSession session=MyBatisUtil.getSession();
        try {
            GoodsDAO bookdao=session.getMapper(GoodsDAO.class);
            List<Goods> goods=bookdao.getGoodsPager(skip, size);
            Assert.assertEquals(2, goods.size());
        } finally {
            session.close();
        }
    }
    @Test
    public void getGoodsByIdTest() {
        SqlSession session=MyBatisUtil.getSession();
        try {
            GoodsDAO bookdao=session.getMapper(GoodsDAO.class);
            Goods goods=bookdao.getGoodsById(1);
            Assert.assertEquals(1, goods.getId());
        } finally {
            session.close();
        }
    }
    
    @Test
    public void getGoodsCountTest() {
        SqlSession session=MyBatisUtil.getSession();
        try {
            GoodsDAO bookdao=session.getMapper(GoodsDAO.class);
            Assert.assertEquals(9, bookdao.getGoodsCount());
        } finally {
            session.close();
        }
    }
    
    @Test
    public void insertTest() {
        SqlSession session=MyBatisUtil.getSession();
        try {
            Goods entity=new Goods();
            entity.setName("正宗无锡阳山水蜜桃新鲜水果水密桃12个6斤装江浙沪皖顺丰包邮");
            entity.setPrice(108);
            entity.setPicture("nopic.jpg");
            GoodsDAO bookdao=session.getMapper(GoodsDAO.class);
            Assert.assertEquals(1, bookdao.insert(entity));
        } finally {
            session.close();
        }
    }
    @Test
    public void deleteTest() {
        SqlSession session=MyBatisUtil.getSession();
        try {
            GoodsDAO bookdao=session.getMapper(GoodsDAO.class);
            Assert.assertEquals(1, bookdao.delete(12));
        } finally {
            session.close();
        }
    }
    @Test
    public void update() {
        SqlSession session=MyBatisUtil.getSession();
        try {
            GoodsDAO bookdao=session.getMapper(GoodsDAO.class);
            Goods entity=bookdao.getGoodsById(12);
            entity.setName("正宗无锡阳山水蜜桃新鲜水果水密桃12个6斤装");
            entity.setPrice(107);
            entity.setPicture("nopicture.jpg");
            Assert.assertEquals(1, bookdao.update(entity));
        } finally {
            session.close();
        }
    }
    
}
MyBatis访问数据库的工具类如下：
```
package com.zhangguo.ssmall.test;
import java.io.InputStream;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
public abstract class MyBatisUtil {
    
    public static SqlSessionFactory getSqlSessionFactory(){
        // 获得环境配置文件流
        InputStream config = MyBatisUtil.class.getClassLoader().getResourceAsStream("MyBatisCfg.xml");
        // 创建sql会话工厂
        SqlSessionFactory factory = new SqlSessionFactoryBuilder().build(config);
        return factory;
    }
    
    //获得会话
    public static SqlSession getSession(){
        return getSqlSessionFactory().openSession(true);
    }
    
    /**
     * 获得得sql会话
     * @param isAutoCommit 是否自动提交，如果为false则需要sqlSession.commit();rollback();
     * @return sql会话
     */
    public static SqlSession getSession(boolean isAutoCommit){
        return getSqlSessionFactory().openSession(isAutoCommit);
    }
    
}
MyBatis配置文件MyBatisCfg.xml如下所示：
```
```
配置文件中使用到了db.properties属性文件，该文件用于存放数据库连接信息，文件内容如下：
```
#mysql
mysql.driver=com.mysql.jdbc.Driver
mysql.url=jdbc:mysql://localhost:3306/db1
mysql.uid=root
mysql.password=root
运行测试，一切正常，测试结果如下： 
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720150208935-1038974786.png)
这里需要注意的是MyBatis配置文件的内容在后面与Spring整合后是会变化的，使用JUnit测试并未使用到Spring框架。
# 七、完成Spring整合MyBatis配置
7.1、在源代码的根目录下修改db.properties文件，用于存放数据库连接信息，文件内容如下：
```
#mysql
mysql.driver=com.mysql.jdbc.Driver
mysql.url=jdbc:mysql://localhost:3306/db1
mysql.uid=root
mysql.password=root
mysql.acquireIncrement=5
mysql.initialPoolSize=10
mysql.minPoolSize=5
mysql.maxPoolSize=20
7.2、在源代码的根目录下新建 applicationContext.xml文件，用于整合MyBatis与Spring，非常关键，具体的内容如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xmlns:p="http://www.springframework.org/schema/p"
    xmlns:aop="http://www.springframework.org/schema/aop" 
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:tx="http://www.springframework.org/schema/tx"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-4.3.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context-4.3.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop-4.3.xsd
        http://www.springframework.org/schema/tx
        http://www.springframework.org/schema/tx/spring-tx-4.3.xsd">
    <!--1 引入属性文件，在配置中占位使用 -->
    <context:property-placeholder location="classpath*:db.properties" />
    <!--2 配置C3P0数据源 -->
    <bean id="datasource" class="com.mchange.v2.c3p0.ComboPooledDataSource"
        destroy-method="close">
        <!--驱动类名 -->
        <property name="driverClass" value="${mysql.driver}" />
        <!-- url -->
        <property name="jdbcUrl" value="${mysql.url}" />
        <!-- 用户名 -->
        <property name="user" value="${mysql.uid}" />
        <!-- 密码 -->
        <property name="password" value="${mysql.password}" />
        <!-- 当连接池中的连接耗尽的时候c3p0一次同时获取的连接数 -->
        <property name="acquireIncrement" value="${mysql.acquireIncrement}"></property>
        <!-- 初始连接池大小 -->
        <property name="initialPoolSize" value="${mysql.initialPoolSize}"></property>
        <!-- 连接池中连接最小个数 -->
        <property name="minPoolSize" value="${mysql.minPoolSize}"></property>
        <!-- 连接池中连接最大个数 -->
        <property name="maxPoolSize" value="${mysql.maxPoolSize}"></property>
    </bean>
    <!--3 会话工厂bean sqlSessionFactoryBean -->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <!-- 配置文件路径 -->
        <property name="configLocation" value="classpath:MyBatisCfg.xml"></property>
        <!-- 数据源 -->
        <property name="dataSource" ref="datasource"></property>
        <!-- sql映射文件路径 -->
        <property name="mapperLocations" value="classpath*:com/zhangguo/ssmall/mapper/*Mapper.xml"></property>
    </bean>
    <!--4 自动扫描对象关系映射 -->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <!--指定会话工厂，如果当前上下文中只定义了一个则该属性可省去 -->
        <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"></property>
        <!-- 指定要自动扫描接口的基础包，实现接口 -->
        <property name="basePackage" value="com.zhangguo.ssmall.mapper"></property>
    </bean>
    <!--5 声明式事务管理 -->
    <!--定义事物管理器，由spring管理事务 -->
    <bean id="transactionManager"
        class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="datasource"></property>
    </bean>
    <!--支持注解驱动的事务管理，指定事务管理器 -->
    <tx:annotation-driven transaction-manager="transactionManager" />
    <!--6 容器自动扫描IOC组件 -->
    <context:component-scan base-package="com.zhangguo.ssmall"></context:component-scan>
    <!--7 aspectj支持自动代理实现AOP功能 -->
    <aop:aspectj-autoproxy proxy-target-class="true"></aop:aspectj-autoproxy>
</beans>
从配置文件中可以看出第3点会话工厂配置中指定了MyBatis配置文件的位置与名称，其实也可以省去，在这里可以通过属性配置好。但个人认为当多个框架整合在一起时最后将配置文件分开，便于修改。修改后的MyBatisCfg.xml文件内容如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <properties resource="db.properties"></properties>
    <settings>
        <!--指定mybatis使用日志组件 -->
        <setting name="logImpl" value="LOG4J2" />
        <!--开启全局的二级缓存 -->
        <setting name="cacheEnabled" value="false" />
        <!--开启延时加载，如果有关联关系，则默认不会获取数据 
        延迟加载的全局开关。当开启时，所有关联对象都会延迟加载。 
        特定关联关系中可通过设置fetchType属性来覆盖该项的开关状态。
        在association中指定fetchType="eager(立即)" 或者 lazy(延迟)
        默认：false
        -->
        <setting name="lazyLoadingEnabled" value="true" />
        <!--true时，对任意延迟属性的调用会使带有延迟加载属性的对象完整加载；
          false，每种属性将会按需加载。
          默认为：true-->
        <setting name="aggressiveLazyLoading" value="false" /> 
    </settings>
    <typeAliases>
        <package name="com.zhangguo.ssmall.entities" />
    </typeAliases>
    <!-- 
    <environments default="development">
        <environment id="development">
            <transactionManager type="JDBC"></transactionManager>
            <dataSource type="POOLED">
                <property name="driver" value="${mysql.driver}" />
                <property name="url" value="${mysql.url}" />
                <property name="username" value="${mysql.uid}" />
                <property name="password" value="${mysql.password}" />
            </dataSource>
        </environment>
    </environments>
    <mappers> 
        <mapper resource="com/zhangguo/ssmall/mapper/GoodsMapper.xml" /> 
    </mappers>-->
</configuration>
中间有一大段注释了，是因为MyBatis-Spring适配器已完成了这部分内容的工作，注释不删除的原因是因为JUnit测试时还要使用，其它也可以使用两个不同的文件。
# 八、配置web.xml加载Spring容器与MVC
修改web.xml文件，注册加载Spring容器所需的监听器；注册Spring MVC前置控制器Servlet，中间还设置了Servlet3.0上传所需的参数；添加了一个全局的编码过滤器。
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns="http://java.sun.com/xml/ns/javaee"
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"
    id="WebApp_ID" version="3.0">
    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>
    <listener>
        <description>Spring容器加载监听器</description>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
    <context-param>
        <description>设置Spring加载时的配置文件位置，默认位置在WEB-INF/lib目录下</description>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath*:applicationContext.xml</param-value>
    </context-param>
    <!--Spring MVC 前置Servlet，中心控制器 -->
    <servlet>
        <servlet-name>springmvc</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <!--Spring MVC配置文件路径 -->
            <param-value>classpath*:springmvc-servlet.xml</param-value>
        </init-param>
        <!-- 启动动优先级，越小越早加载 -->
        <load-on-startup>1</load-on-startup>
        <!--Servlet3.0以上文件上传配置 -->
        <multipart-config>
            <!--上传文件的最大限制5MB -->
            <max-file-size>5242880</max-file-size>
            <!--请求的最大限制20MB -->
            <max-request-size>20971520</max-request-size>
            <!--当文件的大小超过临界值时将写入磁盘 -->
            <file-size-threshold>0</file-size-threshold>
        </multipart-config>
    </servlet>
    <!-- Servlet访问的路径映射，所有的访问都必须经过调度用的前置控制品 -->
    <servlet-mapping>
        <servlet-name>springmvc</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>
    
    <!--编码过滤器 -->
    <filter>
        <filter-name>characterEncodingFilter</filter-name>
        <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
        <init-param>
            <param-name>encoding</param-name>
            <param-value>UTF-8</param-value>
        </init-param>
        <init-param>
            <param-name>forceEncoding</param-name>
            <param-value>true</param-value>
        </init-param>
    </filter>
    <!-- 路径映射 -->
    <filter-mapping>
        <filter-name>characterEncodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
</web-app>
在src/main/java源代码目录下添加applicationContext.xml文件，用于配置Spring，内容在上一节中已列出。
在src/main/java源代码目录下添加Spring MVC配置文件springmvc-servlet.xml，文件内容如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
    xmlns:mvc="http://www.springframework.org/schema/mvc"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
         http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context 
        http://www.springframework.org/schema/context/spring-context-4.3.xsd
        http://www.springframework.org/schema/mvc 
        http://www.springframework.org/schema/mvc/spring-mvc-4.3.xsd">
    <!-- 自动扫描包，实现支持注解的IOC -->
    <context:component-scan base-package="com.zhangguo.ssmall" />
    <!-- Spring MVC不处理静态资源 -->
    <mvc:default-servlet-handler />
    <!-- 支持mvc注解驱动 -->
    <mvc:annotation-driven enable-matrix-variables="true" />
    <!-- 配置映射媒体类型的策略 -->
    <bean
        class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping">
        <property name="removeSemicolonContent" value="false" />
    </bean>
    <!-- 内部视图解析器，JSP与JSTL模板 -->
    <bean
        class="org.springframework.web.servlet.view.InternalResourceViewResolver"
        id="internalResourceViewResolver">
        <!--指定视图渲染类 -->
        <property name="viewClass"
            value="org.springframework.web.servlet.view.JstlView" />
        <!--自动添加到路径中的前缀 -->
        <property name="prefix" value="/WEB-INF/views/jstl" />
        <!--自动添加到路径中的后缀 -->
        <property name="suffix" value=".jsp" />
        <!--设置所有视图的内容类型，如果视图本身设置内容类型视图类可以忽略 -->
        <property name="contentType" value="text/html;charset=UTF-8" />
        <!-- 优先级，越小越前 -->
        <property name="order" value="2" />
    </bean>
    <!-- FreeMarker视图解析器与属性配置 -->
    <bean id="viewResolver"
        class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
        <!--是否启用缓存 -->
        <property name="cache" value="true" />
        <!--自动添加到路径中的前缀 -->
        <property name="prefix" value="" />
        <!--自动添加到路径中的后缀 -->
        <property name="suffix" value=".html" />
        <!--指定视图渲染类 -->
        <property name="viewClass"
            value="org.springframework.web.servlet.view.freemarker.FreeMarkerView" />
        <!-- 设置是否暴露Spring的macro辅助类库，默认为true -->
        <property name="exposeSpringMacroHelpers" value="true" />
        <!-- 是否应将所有request属性添加到与模板合并之前的模型。默认为false。 -->
        <property name="exposeRequestAttributes" value="true" />
        <!-- 是否应将所有session属性添加到与模板合并之前的模型。默认为false。 -->
        <property name="exposeSessionAttributes" value="true" />
        <!-- 在页面中使用${rc.contextPath}就可获得contextPath -->
        <property name="requestContextAttribute" value="rc" />
        <!--设置所有视图的内容类型，如果视图本身设置内容类型视图类可以忽略 -->
        <property name="contentType" value="text/html;charset=UTF-8" />
        <!-- 优先级，越小越前 -->
        <property name="order" value="1" />
    </bean>
    <!-- 配置FreeMarker细节 -->
    <bean id="freemarkerConfig"
        class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
        <!-- 模板路径 -->
        <property name="templateLoaderPath" value="/WEB-INF/views/ftl" />
        <property name="freemarkerSettings">
            <props>
                <!-- 刷新模板的周期，单位为秒 -->
                <prop key="template_update_delay">5</prop>
                <!--模板的编码格式 -->
                <prop key="defaultEncoding">UTF-8</prop>
                <!--url编码格式 -->
                <prop key="url_escaping_charset">UTF-8</prop>
                <!--此属性可以防止模板解析空值时的错误 -->
                <prop key="classic_compatible">true</prop>
                <!--该模板所使用的国际化语言环境选项 -->
                <prop key="locale">zh_CN</prop>
                <!--布尔值格式 -->
                <prop key="boolean_format">true,false</prop>
                <!--日期时间格式 -->
                <prop key="datetime_format">yyyy-MM-dd HH:mm:ss</prop>
                <!--时间格式 -->
                <prop key="time_format">HH:mm:ss</prop>
                <!--数字格式 -->
                <prop key="number_format">0.######</prop>
                <!--自动开启/关闭空白移除，默认为true -->
                <prop key="whitespace_stripping">true</prop>
            </props>
        </property>
    </bean>
    <!--文件上传解析器 -->
    <!--Spring MVC默认不能识别multipart格式的文件内容 -->
    <bean id="multipartResolver"
        class="org.springframework.web.multipart.support.StandardServletMultipartResolver">
    </bean>
</beans>
# 九、创建服务层 
在包com.zhangguo.ssmall.services下添加GoodsService.java文件，该文件是一个服务接口，内容如下：
```
package com.zhangguo.ssmall.services;
import java.util.List;
import com.zhangguo.ssmall.entities.Goods;
/**
 * 商品业务接口
 *
 */
public interface GoodsService {
    //分页
    List<Goods> getGoodsPager(int pageNO, int size);
    //获得单个商品对象
    Goods getGoodsById(int id);
    
    //获得商品总数
    int getGoodsCount();
    //添加
    int insert(Goods entity);
    //删除单个
    int delete(int id);
    //删除多个
    int deletes(int[] ids);
    //更新
    int update(Goods entity);
}
在包com.zhangguo.ssmall.services下添加类GoodsServiceImpl.java，实现接口GoodsService，用于完成商品业务逻辑，由于是示例代码所以比较空；中间使用了两个注解一个是@Service，用于提供给需要服务的类自动装配，当Spring IOC容器启动时被扫描到该类型会自动添加实例到Spring容器中；另一个注解是@Resource用于完成自动装配功能，在Spring容器中找到GoodsDAO类型的对象，代码如下：
```
package com.zhangguo.ssmall.services;
import java.util.List;
import javax.annotation.Resource;
import org.springframework.stereotype.Service;
import com.zhangguo.ssmall.entities.Goods;
import com.zhangguo.ssmall.mapper.GoodsDAO;
/**
 * 商品业务实现
 * 
 */
//自动添加到Spring容器中
@Service
public class GoodsServiceImpl implements GoodsService{
    //自动装配
    @Resource
    GoodsDAO goodsdao;
    
    //分页
    @Override
    public List<Goods> getGoodsPager(int pageNO, int size) {
        int skip=(pageNO-1)*size;
        return goodsdao.getGoodsPager(skip, size);
    }
    //获得单个产品对象
    @Override
    public Goods getGoodsById(int id) {
        return goodsdao.getGoodsById(id);
    }
    
    //获得商品总数
    @Override
    public int getGoodsCount() {
        return goodsdao.getGoodsCount();
    }
    //添加
    @Override
    public int insert(Goods entity) {
        return goodsdao.insert(entity);
    }
    //删除单个
    @Override
    public int delete(int id) {
        return goodsdao.delete(id);
    }
    
    //删除多个
    @Override
    public int deletes(int[] ids) {
        int rows=0;
        for (int id : ids) {
            rows+=delete(id);
        }
        return rows;
    }
    //更新
    @Override
    public int update(Goods entity) {
        return goodsdao.update(entity);
    }
}
# 十、完成商品管理功能
## 10.1、商品列表与分页
定义GoodsController控制器，映射访问路径，需要使用到的商品服务使用自动装配完成，代码如下：
```
package com.zhangguo.ssmall.controllers;
import javax.annotation.Resource;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import com.zhangguo.ssmall.services.GoodsService;
@Controller
@RequestMapping("/goods")
public class GoodsController {
    
    @Resource
    GoodsService goodsService;
    
    /*
     * 产品列表与分页Action
     */
    @RequestMapping("/list")
    public String list(Model model,@RequestParam(required=false,defaultValue="1") int pageNO){
        int size=5;
        model.addAttribute("size",size);
        model.addAttribute("pageNO",pageNO);
        model.addAttribute("count",goodsService.getGoodsCount());
        model.addAttribute("goods", goodsService.getGoodsPager(pageNO, size));
        return "goods/list";
    }
    
}
参数size表示每页记录数，pageNO表示当前页号，处于第几页，count表示总记录数。
在views/jstl/goods目录下添加视图list.jsp页面，页面的内容如下：
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="<c:url value="/styles/main.css"/>"  type="text/css" rel="stylesheet" />
<title>商品管理</title>
</head>
<body>
    <div class="main">
        <h2 class="title"><span>商品管理</span></h2>
        <form action="deletes" method="post">
        <table border="1" width="100%" class="tab">
            <tr>
                <th><input type="checkbox" id="chbAll"></th>
                <th>编号</th>
                <th>产品名</th>
                <th>价格</th>
                <th>类型</th>
                <th>操作</th>
            </tr>
            <c:forEach var="entity" items="${goods}">
                <tr>
                    <th><input type="checkbox" name="id" value="${entity.id}"></th>
                    <td>${entity.id}</td>
                    <td>${entity.name}</td>
                    <td><img src="<c:url value="/images/${entity.picture}"/>" height="40"/></td>
                    <td>${entity.price}</td>
                    <td>
                    <a href="delete/${entity.id}" class="abtn">删除</a>
                    <a href="edit/${entity.id}" class="abtn">编辑</a>
                    </td>
                </tr>
            </c:forEach>
        </table>
        <div id="pager"></div>
        <p>
            <a href="add" class="abtn out">添加</a>
            <input type="submit"  value="批量删除" class="btn out"/>
        </p>
        <p style="color: red">${message}</p>
        <!--分页 -->
        <script type="text/javascript" src="<c:url value="/scripts/jQuery1.11.3/jquery-1.11.3.min.js"/>" ></script>
        <link href="<c:url value="/scripts/pagination22/pagination.css"/>"  type="text/css" rel="stylesheet" />
        <script type="text/javascript" src="<c:url value="/scripts/pagination22/jquery.pagination2.2.js"/>" ></script>
        <script type="text/javascript">
           //初始化分页组件
           var count=${count};
           var size=${size};
           var pageNO=${pageNO};
           $("#pager").pagination(count, {
              items_per_page:size,
               current_page:pageNO-1,
               next_text:"下一页",
               prev_text:"上一页",
               num_edge_entries:2,
               load_first_page:false,
              callback:handlePaginationClick
            });
           
           //回调方法
           function handlePaginationClick(new_page_index, pagination_container){
               location.href="list?pageNO="+(new_page_index+1);
           }
           
           var defaultSrc="<c:url value="/images/default.jpg"/>";
           $(".tab img").bind("error",function(){
               $(this).prop("src",defaultSrc);
           });
        </script>
    </form>
    </div>
</body>
</html>
为了实现分页，添加了一个jQuery插件pagination，该插件的详细参数如下所示：
```
```
测试运行结果：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160720162155122-728764127.png)
页面中有一个简单处理加载图片失败的事件，当图片加载出错时使用default.jpg图，如编号为38的图片就是默认图。
## 11.2、删除与多删除功能
为了实现删除与多删除功能，修改控制器，增加2个action，delete请求处理方法用于删除单个记录，id是路径变量指定要删除的商品编号；pageNO是请求参数，保持状态的目的是为了删除后让页面继续停留在某一页，不过这里有问题的是当某一页的内容只有一条记录里就需要重新计算了；rediredtAttributes是为了保持重定向后的message值。
```
/*
     * 删除单个产品对象Action
     */
    @RequestMapping("/delete/{id}")
    public String delete(Model model,@PathVariable int id,@RequestParam(required=false,defaultValue="1") int pageNO,RedirectAttributes redirectAttributes){
        if(goodsService.delete(id)>0)
        {
            redirectAttributes.addFlashAttribute("message", "删除成功！");
        }else{
            redirectAttributes.addFlashAttribute("message", "删除失败！");
        }
        return "redirect:/goods/list?pageNO="+pageNO;
    }
    
    /*
     * 删除多个产品对象Action
     */
    @RequestMapping("/deletes")
    public String deletes(Model model,@RequestParam int[] id,@RequestParam(required=false,defaultValue="1") int pageNO,RedirectAttributes redirectAttributes){
        //执行删除
        int rows=goodsService.deletes(id);
        if(rows>0)
        {
            redirectAttributes.addFlashAttribute("message", "删除"+rows+"行记录成功！");
        }else{
            redirectAttributes.addFlashAttribute("message", "删除失败！");
        }
        return "redirect:/goods/list?pageNO="+pageNO;
    }
为了配合删除，修改list.jsp页面，修改后的list.jsp页面如下所示：
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="<c:url value="/styles/main.css"/>"  type="text/css" rel="stylesheet" />
<title>商品管理</title>
</head>
<body>
    <div class="main">
        <h2 class="title"><span>商品管理</span></h2>
        <form action="<c:url value="/goods/deletes?pageNO=${pageNO}"/>" method="post">
        <table border="1" width="100%" class="tab">
            <tr>
                <th><input type="checkbox" id="chbAll"></th>
                <th>编号</th>
                <th>产品名</th>
                <th>价格</th>
                <th>类型</th>
                <th>操作</th>
            </tr>
            <c:forEach var="entity" items="${goods}">
                <tr>
                    <th><input type="checkbox" name="id" value="${entity.id}"></th>
                    <td>${entity.id}</td>
                    <td>${entity.name}</td>
                    <td><img src="<c:url value="/images/${entity.picture}"/>" height="40"/></td>
                    <td>${entity.price}</td>
                    <td>
                    <a href="<c:url value="/goods/"/>delete/${entity.id}?pageNO=${pageNO}" class="abtn">删除</a>
                    <a href="<c:url value="/goods/"/>edit/${entity.id}" class="abtn">编辑</a>
                    <a href="<c:url value="/goods/"/>upPicture/${entity.id}" class="abtn">上传</a>
                    </td>
                </tr>
            </c:forEach>
        </table>
        <div id="pager"></div>
        <p>
            <a href="add" class="abtn out">添加</a>
            <input type="submit"  value="批量删除" class="btn out"/>
        </p>
        <p style="color: red">${message}</p>
        <!--分页 -->
        <script type="text/javascript" src="<c:url value="/scripts/jQuery1.11.3/jquery-1.11.3.min.js"/>" ></script>
        <link href="<c:url value="/scripts/pagination22/pagination.css"/>"  type="text/css" rel="stylesheet" />
        <script type="text/javascript" src="<c:url value="/scripts/pagination22/jquery.pagination2.2.js"/>" ></script>
        <script type="text/javascript">
           //初始化分页组件
           var count=${count};
           var size=${size};
           var pageNO=${pageNO};
           $("#pager").pagination(count, {
              items_per_page:size,
               current_page:pageNO-1,
               next_text:"下一页",
               prev_text:"上一页",
               num_edge_entries:2,
               load_first_page:false,
              callback:handlePaginationClick
            });
           
           //回调方法
           function handlePaginationClick(new_page_index, pagination_container){
               location.href="<c:url value="/goods/"/>list?pageNO="+(new_page_index+1);
           }
           
           var defaultSrc="<c:url value="/images/default.jpg"/>";
           $(".tab img").bind("error",function(){
               $(this).prop("src",defaultSrc);
           });
        </script>
    </form>
    </div>
</body>
</html>
运行结果如下所示：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728090825200-771173209.png)
基中的多删除功能可以改进为一次性让数据库删除完成。
## 11.3、新增商品功能
在控制器中添加2个action，一个是add用于完成添加页面展示，一个是addSave用于完成添加保存处理，代码如下：
```
/*
     * 添加商品
     */
    @RequestMapping("/add")
    public String add(Model model){
        model.addAttribute("entity", new Goods());
        return "goods/add";
    }
    
    /*
     * 添加商品保存
     */
    @RequestMapping("/addSave")
    public String addSave(Model model,@ModelAttribute("entity") @Valid Goods entity,BindingResult bindingResult){
        //如果模型中存在错误
        if(!bindingResult.hasErrors()){
            if(goodsService.insert(entity)>0)
            {
                return "redirect:/goods/list";    
            }
        }
        model.addAttribute("entity", entity);
        return "goods/add";
    }
这里有一个问题是因为使用了JSR303校验，当保存对象是需要在参数前注解@ModelAttribute("entity") @Valid，用于激活校验，否则页面将不会有错误展示，非常奇怪的问题；我在第五章中并没有发现该问题。
为了配合Bean Validation，定义的Goods Bean需要注解，内容如下：
```
/*
     * 名称
     */
    @Pattern(regexp="^[^><&#]{1,50}$",message="{pattern}")
    @NotNull(message="{notNull}")
    private String name;
    /*
     * 价格
     */
    @Min(value=1,message="必须大于或等于1")
    private double price;
这里的错误消息来源一个是直接写在注解中，另一个来自消息文件；{pattern}来自消息文件ValidationMessages.properties，在src/main/java目录下新建该文件，文件内容如下：
```
pattern=格式错误
notNull=不允许为空
```
这里需注意的是，默认情况下中文会显示成utf-8编码格式如：
```
pattern=\u683C\u5F0F\u9519\u8BEF
notNull=\u4E0D\u5141\u8BB8\u4E3A\u7A7A
```
为了正常显示，可以安装一个插件，让属性文件支持正常显示中文，插件名称是properties-editor，点击“Helo”->“Marketplace”，搜索插件名称，显示内容如下：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728092428341-409274097.jpg)
点击Install，进入下一步：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728092514934-1634198672.jpg)
完成后在properties文件上右键选择“Open With”，具体步骤如下：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728092657575-1118088095.png)
在views/jstl/goods目录下新增加add.jsp页面，页面内容如下：
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<link href="<c:url value="/styles/main.css" />" type="text/css" rel="stylesheet" />
<title>新增商品</title>
</head>
<body>
    <div class="main">
        <h2 class="title"><span>新增商品</span></h2>
        <form:form action="addSave" modelAttribute="entity">
        <fieldset>
            <legend>商品</legend>
            <p>
                <label for="name">商品名称：</label>
                <form:input path="name" size="50"/>
                <form:errors path="name" cssClass="error"></form:errors>
            </p>
            <p>
                <label for="price">商品价格：</label>
                <form:input path="price"/>
                <form:errors path="price" cssClass="error"></form:errors>
            </p>
            <p>
              <input type="submit" value="保存" class="btn out">
            </p>
        </fieldset>
        </form:form>
        <p style="color: red">${message}</p>
        <form:errors path="*"></form:errors>
        <p>
            <a href="<c:url value="/goods/list" />"  class="abtn out">返回列表</a>
        </p>
    </div>
</body>
</html>
运行结果：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728101751231-2002246102.png)
## 11.4、编辑商品功能
与新增加类似，在控制器下新增两个action，一个用于展示编辑，有一个用于执行编辑后保存，代码如下所示：
```
/*
     * 编辑商品
     */
    @RequestMapping("/edit/{id}")
    public String edit(Model model,@PathVariable int id){
        model.addAttribute("entity", goodsService.getGoodsById(id));
        return "goods/edit";
    }
    
    /*
     * 编辑商品保存
     */
    @RequestMapping("/editSave")
    public String editSave(Model model,@ModelAttribute("entity") @Valid Goods entity,BindingResult bindingResult){
        //如果模型中存在错误
        if(!bindingResult.hasErrors()){
            if(goodsService.update(entity)>0)
            {
                return "redirect:list";    
            }
        }
        model.addAttribute("entity", entity);
        return "/goods/edit";
    }
在views/jstl/goods目录下新增加edit.jsp页面，页面内容如下：
```
<%@page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<link href="<c:url value="/styles/main.css" />" type="text/css"
    rel="stylesheet" />
<title>编辑商品</title>
<base href="<c:url value="/" />" />
</head>
<body>
    <div class="main">
        <h2 class="title">
            <span>编辑商品</span>
        </h2>
        <form:form action="goods/editSave" modelAttribute="entity">
            <fieldset>
                <legend>商品</legend>
                <p>
                    <label for="name">商品名称：</label>
                    <form:input path="name" size="50" />
                    <form:errors path="name" cssClass="error"></form:errors>
                </p>
                <p>
                    <label for="price">商品价格：</label>
                    <form:input path="price" />
                    <form:errors path="price" cssClass="error"></form:errors>
                </p>
                <p>
                    <form:hidden path="picture" />
                    <form:hidden path="id" />
                    <input type="submit" value="保存" class="btn out">
                </p>
            </fieldset>
        </form:form>
        <p style="color: red">${message}</p>
        <form:errors path="*"></form:errors>
        <p>
            <a href="goods/list" class="abtn out">返回列表</a>
        </p>
    </div>
</body>
</html>
运行结果：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728102243716-1990446196.png)
## 11.5、上传图片功能
这里使用Servlet3.0实现文件上传，相关配置已经在前面的配置文件中设置好了，在控制器中增加两个action，代码如下：
```
/**
     * 上传图片
     */
    @RequestMapping("/upPicture/{id}")
    public String upPicture(Model model,@PathVariable int id){
        model.addAttribute("entity", goodsService.getGoodsById(id));
        return "goods/upfile";
    }
    
    /*
     * 上传图片保存
     */
    @RequestMapping("/upPictureSave/{id}")
    public String upPictureSave(Model model,@PathVariable int id,MultipartFile picFile,HttpServletRequest request){
        Goods entity=goodsService.getGoodsById(id);
        //如果选择了文件
        if(picFile!=null){ 
            //如果文件大小不为0
            if(picFile.getSize()>0){
                //获得上传位置
                String path=request.getServletContext().getRealPath("/images");
                //生成文件名
                String filename=UUID.randomUUID().toString()+picFile.getOriginalFilename().substring(picFile.getOriginalFilename().lastIndexOf("."));
                File tempFile=new File(path, filename);
                try {
                    //保存文件
                    picFile.transferTo(tempFile);
                    //更新数据
                    entity.setPicture(filename);
                    goodsService.update(entity);
                    //转向列表页
                    return "redirect:/goods/list";    
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        model.addAttribute("entity", entity);
        return "goods/upfile";
    }
在views/jstl/goods目录下新增加upfile.jsp页面，页面内容如下：
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<link href="<c:url value="/styles/main.css" />" type="text/css"
    rel="stylesheet" />
<title>上传图片</title>
</head>
<body>
    <div class="main">
        <h2 class="title">
            <span>上传图片</span>
        </h2>
        <form action="<c:url value="/goods/upPictureSave/${entity.id}" />" method="post"
            enctype="multipart/form-data">
            <fieldset>
                <legend>商品</legend>
                <p>
                    <label for="name">商品名称：</label> ${entity.name}
                </p>
                <p>
                    <label for="price">商品价格：</label>${entity.price}
                </p>
                <p>
                    <label for="title">商品图片：</label> <input type="file" name="picFile" />
                </p>
                <p>
                    <input type="submit" value="上传" class="btn out">
                </p>
            </fieldset>
            </form>
            <p style="color: red">${message}</p>
            <p>
                <a href="<c:url value="/goods/list" />" class="abtn out">返回列表</a>
            </p>
    </div>
</body>
</html>
运行结果如下：
![](https://images2015.cnblogs.com/blog/63651/201607/63651-20160728102832356-603784110.png)
## 11.6、日志、首页、样式与最终的控制器
为了将MyBatis与Hibernate Validation的日志信息展示在控制中，需要添加log4j2的引用，这部分内容在pom.xml中已配置完成了，另外在项目的根目录下需要添加一个log4j2的配置文件log4j2.xml，内容如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="off" monitorInterval="1800">    
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>        
    </Appenders>
    <Loggers>            
        <Root level="debug">
            <AppenderRef ref="Console" />
        </Root>
    </Loggers>
</Configuration>
在webapp目录下添加index.jsp，首页是这个程序的入口，只完成了转发功能，页面内容如下：
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<jsp:forward page="goods/list"></jsp:forward>
```
所有页面基本都引用了同一个样式表styles/main.css文件，文件内容如下：
```
```
最终的控制器GoodsController.java文件内容如下：
```
package com.zhangguo.ssmall.controllers;
import java.io.File;
import java.util.UUID;
import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;
import com.zhangguo.ssmall.entities.Goods;
import com.zhangguo.ssmall.services.GoodsService;
@Controller
@RequestMapping("/goods")
public class GoodsController {
    
    @Resource
    GoodsService goodsService;
    
    /*
     * 产品列表与分页Action
     */
    @RequestMapping("/list")
    public String list(Model model,@RequestParam(required=false,defaultValue="1") int pageNO){
        int size=5;
        model.addAttribute("size",size);
        model.addAttribute("pageNO",pageNO);
        model.addAttribute("count",goodsService.getGoodsCount());
        model.addAttribute("goods", goodsService.getGoodsPager(pageNO, size));
        return "goods/list";
    }
    
    /*
     * 删除单个产品对象Action
     */
    @RequestMapping("/delete/{id}")
    public String delete(Model model,@PathVariable int id,@RequestParam(required=false,defaultValue="1") int pageNO,RedirectAttributes redirectAttributes){
        if(goodsService.delete(id)>0)
        {
            redirectAttributes.addFlashAttribute("message", "删除成功！");
        }else{
            redirectAttributes.addFlashAttribute("message", "删除失败！");
        }
        return "redirect:/goods/list?pageNO="+pageNO;
    }
    
    /*
     * 删除多个产品对象Action
     */
    @RequestMapping("/deletes")
    public String deletes(Model model,@RequestParam int[] id,@RequestParam(required=false,defaultValue="1") int pageNO,RedirectAttributes redirectAttributes){
        //执行删除
        int rows=goodsService.deletes(id);
        if(rows>0)
        {
            redirectAttributes.addFlashAttribute("message", "删除"+rows+"行记录成功！");
        }else{
            redirectAttributes.addFlashAttribute("message", "删除失败！");
        }
        return "redirect:/goods/list?pageNO="+pageNO;
    }
    
    /*
     * 添加商品
     */
    @RequestMapping("/add")
    public String add(Model model){
        model.addAttribute("entity", new Goods());
        return "goods/add";
    }
    
    /*
     * 添加商品保存
     */
    @RequestMapping("/addSave")
    public String addSave(Model model,@ModelAttribute("entity") @Valid Goods entity,BindingResult bindingResult){
        //如果模型中存在错误
        if(!bindingResult.hasErrors()){
            if(goodsService.insert(entity)>0)
            {
                return "redirect:/goods/list";    
            }
        }
        model.addAttribute("entity", entity);
        return "goods/add";
    }
    
    /*
     * 编辑商品
     */
    @RequestMapping("/edit/{id}")
    public String edit(Model model,@PathVariable int id){
        model.addAttribute("entity", goodsService.getGoodsById(id));
        return "goods/edit";
    }
    
    /*
     * 编辑商品保存
     */
    @RequestMapping("/editSave")
    public String editSave(Model model,@ModelAttribute("entity") @Valid Goods entity,BindingResult bindingResult){
        //如果模型中存在错误
        if(!bindingResult.hasErrors()){
            if(goodsService.update(entity)>0)
            {
                return "redirect:list";    
            }
        }
        model.addAttribute("entity", entity);
        return "/goods/edit";
    }
    
    /**
     * 上传图片
     */
    @RequestMapping("/upPicture/{id}")
    public String upPicture(Model model,@PathVariable int id){
        model.addAttribute("entity", goodsService.getGoodsById(id));
        return "goods/upfile";
    }
    
    /*
     * 上传图片保存
     */
    @RequestMapping("/upPictureSave/{id}")
    public String upPictureSave(Model model,@PathVariable int id,MultipartFile picFile,HttpServletRequest request){
        Goods entity=goodsService.getGoodsById(id);
        //如果选择了文件
        if(picFile!=null){ 
            //如果文件大小不为0
            if(picFile.getSize()>0){
                //获得上传位置
                String path=request.getServletContext().getRealPath("/images");
                //生成文件名
                String filename=UUID.randomUUID().toString()+picFile.getOriginalFilename().substring(picFile.getOriginalFilename().lastIndexOf("."));
                File tempFile=new File(path, filename);
                try {
                    //保存文件
                    picFile.transferTo(tempFile);
                    //更新数据
                    entity.setPicture(filename);
                    goodsService.update(entity);
                    //转向列表页
                    return "redirect:/goods/list";    
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        model.addAttribute("entity", entity);
        return "goods/upfile";
    }
}
# 十二、总结
通个该示例将前面几章的内容整合起来，巩固了前几章的内容；示例中还可以尝试使用FreeMarker视图；示例中没有前端验证都是后台验证，可以使用jQuery扩展插件Validate实现前端校验；有些功能可以结合AJAX完成更加合理；路径是要非常小心的，后台重定向时，前台提交表单的路径，可以使用base标签和c:url。内容比较简单，适合初学，只是希望能起到抛砖引玉、以小见大的作用，谢谢阅读！
# 十三、示例下载与预览
[点击下载](http://files.cnblogs.com/files/best/SSMall.rar)
[github代码下载或预览](https://github.com/zhangguo5/SSMall_MVC6.git)
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
