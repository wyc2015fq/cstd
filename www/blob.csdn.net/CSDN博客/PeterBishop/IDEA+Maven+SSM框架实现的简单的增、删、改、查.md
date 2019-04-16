# IDEA+Maven+SSM框架实现的简单的增、删、改、查 - PeterBishop - CSDN博客





2018年08月27日 14:51:03[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：414








## 选用SSM框架的原因

在目前的企业级Java应用中，Spring框架是必须的。Struts2框架与Spring的整合问题日益凸显，而Spring MVC作为新一代的MVC框架，因其可以与Spring框架无缝整合的特性收到了越来越多的欢迎。Hibernate框架在面对需要存储过程或者复杂SQL时显得力不从心，不能提供高效的数据库控制。而Mybatis框架作为持久层的框架，虽然需要自己编写SQL语句，但是其对高并发高响应的支持，以及对动态SQL和动态绑定的支持使其脱颖而出。

因此SSM框架（Spring + Spring MVC +Mybatis）逐渐取代了之前广泛使用的SSH框架（Spring + Struts2 + Hibernate），成为了目前使用最多的框架。

本文使用IDEA，利用Maven管理项目，整合SSM框架搭建论文管理系统，实现简单的增、删、改、查功能。步骤详细，讲解丰富，适合新手入门。

## 开发工具
- IntelliJ IDEA Ultimate 2017.2.2
- Apache-tomcat-9.0.5
- JDK 1.8.0_121
- MySQL 5.7
- Maven 3.3.9

## 工程结构

最终完成后的工程结构： 
![](https://img-blog.csdn.net/20180408142331816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面开始详细的讲解项目的开发过程。

## IDEA+Maven搭建项目骨架

**1. 新建Maven项目：**

点击File -> New -> Project -> Maven -> 勾选 Create from archetype -> 选择 maven-archetype-webapp (**注意：此处不要错选成上面的cocoom-22-archetype-webapp**) 
![这里写图片描述](https://img-blog.csdn.net/20180408142648814?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180408143032200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在弹出的new project 选项卡中填写GroupId和Artifactid，其中GroupID是项目组织唯一的标识符，实际对应JAVA的包的结构，是main目录里java的目录结构，ArtifactID是项目的唯一的标识符，实际对应项目的名称，就是项目根目录的名称。对于入门练习，这两项可以随意填写。

![这里写图片描述](https://img-blog.csdn.net/20180408143314944?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

之后点击next 选择Maven版本**（其中IDEA 专业版自带Maven，也可以选择自己下载的maven）**。之后填写项目名称和项目地址，完成后点击Finish，完成项目骨架的创建。 
![这里写图片描述](https://img-blog.csdn.net/20180408143643717?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

.

**2. 在新建的项目中添加所需要的文件/文件夹**

创建之后的项目如图所示，我们需要在这之上新建一些目录。 
![这里写图片描述](https://img-blog.csdn.net/20180408144048497?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在项目的根目录下新建target文件夹，系统自动将其设置为“Excluded” 
![这里写图片描述](https://img-blog.csdn.net/2018040814424415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在src/main目录下新建Directory：“java”，并将其设置为“Source Root”（即：此项目默认的代码文件源目录） 
![这里写图片描述](https://img-blog.csdn.net/20180408144453387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018040816091184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在刚才新建的java文件下新建“com”包，再在com包下新建四个包，分别命名为：pojo,service,dao,controller。（如果出现下图中所示的包名重叠的情况，可以点击图中所示的图标，将“Hide empty middle package取消掉”） 
![这里写图片描述](https://img-blog.csdn.net/20180408145026346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面新建的四个包：pojo,service,dao,controller，其所存放的分别是：
- pojo: 存放自定义的java类。如：paper类，user类，book类等，每个类的属性设为private，并提供public属性的getter/setter方法让外界访问
- service：定义接口，包含系统所提供的功能。（之后还会在service包下再新建impl包）。
- dao：定义接口，包含与数据库进行交互的功能。
- controller：控制器，负责接收页面请求，转发和处理。

在resource包下新建Directory：“mapper”（**用于存放xxxMapper.xml文件**）和“spring”（**用于存放spring-xxx.xml配置文件**），新建文件：“jdbc.properties”（**mysql数据库配置文件**）,”log4j.properties”（**日志输出配置文件**）,”mybatis-config.xml”（**mybatis框架配置文件**）。 
![这里写图片描述](https://img-blog.csdn.net/20180408150253880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在web-inf目录下新建“jsp”包，存放xxx.jsp显示界面。 

补充之后的项目目录如下图所示，至此项目骨架搭建完毕，开始写代码实现增删改查功能。 
![这里写图片描述](https://img-blog.csdn.net/20180408150328491?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**3. 自定义Paper类**

在这之前要对项目的pom.xml文件进行配置，在此文件中配置项目所需要的jar包。

pom.xml

```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com</groupId>
    <artifactId>first</artifactId>
    <packaging>war</packaging>
    <version>1.0-SNAPSHOT</version>
    <name>first Maven Webapp</name>
    <url>http://maven.apache.org</url>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
        <spring.version>5.0.3.RELEASE</spring.version>
        <mybatis.version>3.4.4</mybatis.version>
    </properties>
    <dependencies>
        <!-- 单元测试 -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>3.8.1</version>
            <scope>test</scope>
        </dependency>
        <!-- 第一部分：Spring 配置-->
        <!-- Spring core -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <!-- Spring DAO -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <!-- Spring mvc -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <!-- 第二部分：Servlet web -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.0.1</version>
            <scope>provided</scope>
        </dependency>
        <dependency>
            <groupId>javax.servlet.jsp</groupId>
            <artifactId>jsp-api</artifactId>
            <version>2.2</version>
            <scope>provided</scope>
        </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>jstl</artifactId>
            <version>1.2</version>
        </dependency>
        <dependency>
            <groupId>taglibs</groupId>
            <artifactId>standard</artifactId>
            <version>1.1.2</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.9.4</version>
        </dependency>
        <!-- 第三部分：数据库和mybatis -->
        <!-- 数据库 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.38</version>
        </dependency>
        <!-- 数据库连接池 -->
        <dependency>
            <groupId>com.mchange</groupId>
            <artifactId>c3p0</artifactId>
            <version>0.9.5.2</version>
        </dependency>
        <!-- MyBatis -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>${mybatis.version}</version>
        </dependency>
        <!-- mybatis-spring整合包 -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>1.3.1</version>
        </dependency>
        <!-- 第四部分：日志 -->
        <!-- 实现slf4j接口并整合 -->
        <dependency>
            <groupId>ch.qos.logback</groupId>
            <artifactId>logback-classic</artifactId>
            <version>1.1.1</version>
        </dependency>
    </dependencies>
    <build>
        <finalName>first</finalName>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

本项目所用的数据库为MySQL 5.7 建表语句如下：

```
SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `paper`
-- ----------------------------
DROP TABLE IF EXISTS `paper`;
CREATE TABLE `paper` (
  `paper_id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'paperID',
  `name` varchar(100) NOT NULL COMMENT 'paper名称',
  `number` int(11) NOT NULL COMMENT 'paper数量',
  `detail` varchar(200) NOT NULL COMMENT 'paper描述',
  PRIMARY KEY (`paper_id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 COMMENT='paper表';

-- ----------------------------
-- Records of paper
-- ----------------------------
INSERT INTO `paper` VALUES ('1', '机器学习', '2', 'mlmlmlml');
INSERT INTO `paper` VALUES ('2', '深度学习', '3', 'dldldl');
INSERT INTO `paper` VALUES ('3', '大数据', '4', 'bdbdbd');
```
- **始写java代码：**
- 在pojo包中新建Paper.java

```java
package com.pojo;

public class Paper {
    private long paperId;
    private String paperName;
    private int paperNum;
    private String paperDetail;

    public long getPaperId() {
        return paperId;
    }

    public void setPaperId(long paperId) {
        this.paperId = paperId;
    }

    public String getPaperName() {
        return paperName;
    }

    public void setPaperName(String paperName) {
        this.paperName = paperName;
    }

    public int getPaperNum() {
        return paperNum;
    }

    public void setPaperNum(int paperNum) {
        this.paperNum = paperNum;
    }

    public String getPaperDetail() {
        return paperDetail;
    }

    public void setPaperDetail(String paperDetail) {
        this.paperDetail = paperDetail;
    }

}
```

其中getter/setter方法可以在定义完私有属性之后使用IDEA提供的快捷键：ALT+Insert 来实现快速添加 
![这里写图片描述](https://img-blog.csdn.net/20180408150941909?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180408150959790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 在service包中新建**接口** PaperService：

```java
package com.service;

import com.pojo.Paper;

import java.util.List;

public interface PaperService {
    int addPaper(Paper paper);

    int deletePaperById(long id);

    int updatePaper(Paper paper);

    Paper queryById(long id);

    List<Paper> queryAllPaper();
}
```
- 在service包下新建impl包，并在此包中新建PaperServiceImpl.java

```java
package com.service.impl;

import com.pojo.Paper;
import com.dao.PaperDao;
import com.service.PaperService;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class PaperServiceImpl implements PaperService {
    @Autowired
    private PaperDao paperDao;

    @Override
    public int addPaper(Paper paper) {
        return paperDao.addPaper(paper);
    }

    @Override
    public int deletePaperById(long id) {
        return paperDao.deletePaperById(id);
    }

    @Override
    public int updatePaper(Paper paper) {
        return paperDao.updatePaper(paper);
    }

    @Override
    public Paper queryById(long id) {
        return paperDao.queryById(id);
    }

    @Override
    public List<Paper> queryAllPaper() {
        return paperDao.queryAllPaper();
    }

}
```
- 在dao包中新建**接口** PaperDao.java

```java
package com.dao;

import com.pojo.Paper;

import java.util.List;

public interface PaperDao {
    int addPaper(Paper paper);

    int deletePaperById(long id);

    int updatePaper(Paper paper);

    Paper queryById(long id);

    List<Paper> queryAllPaper();


}
```
- 在controller包中新建 PaperController.java

```java
package com.controller;

import com.pojo.Paper;
import com.service.PaperService;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
@RequestMapping("/paper")
public class PaperController {
    @Autowired
    private PaperService paperService;

    @RequestMapping("/allPaper")
    public String list(Model model) {
        List<Paper> list = paperService.queryAllPaper();
        model.addAttribute("list", list);
        return "allPaper";
    }

    @RequestMapping("toAddPaper")
    public String toAddPaper() {
        return "addPaper";
    }

    @RequestMapping("/addPaper")
    public String addPaper(Paper paper) {
        paperService.addPaper(paper);
        return "redirect:/paper/allPaper";
    }

    @RequestMapping("/del/{paperId}")
    public String deletePaper(@PathVariable("paperId") Long id) {
        paperService.deletePaperById(id);
        return "redirect:/paper/allPaper";
    }

    @RequestMapping("toUpdatePaper")
    public String toUpdatePaper(Model model, Long id) {
        model.addAttribute("paper", paperService.queryById(id));
        return "updatePaper";
    }

    @RequestMapping("/updatePaper")
    public String updatePaper(Model model, Paper paper) {
        paperService.updatePaper(paper);
        paper = paperService.queryById(paper.getPaperId());
        model.addAttribute("paper", paper);
        return "redirect:/paper/allPaper";
    }
}
```
- 在resource/mapper包下新建 PaperMapper.xml（mybatis框架mapper代理开发配置文件）

```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.dao.PaperDao">
    <resultMap type="Paper" id="paperResultMap" >
        <id property="paperId" column="paper_id"/>
        <result property="paperName" column="name"/>
        <result property="paperNum" column="number"/>
        <result property="paperDetail" column="detail"/>
    </resultMap>
    <insert id="addPaper" parameterType="Paper">
        INSERT INTO paper(paper_id,name,number,detail) VALUE (#{paperId},#{paperName}, #{paperNum}, #{paperDetail})
    </insert>

    <delete id="deletePaperById" parameterType="long">
        DELETE FROM paper WHERE paper_id=#{paperID}
    </delete>

    <update id="updatePaper" parameterType="Paper">
        UPDATE paper
        SET NAME = #{paperName},NUMBER = #{paperNum},detail = #{paperDetail}
        WHERE  paper_id = #{paperId}
    </update>

    <select id="queryById" resultType="Paper" parameterType="long">
        SELECT paper_id,name,number,detail
        FROM paper
        WHERE paper_id=#{paperId}
    </select>
    <select id="queryAllPaper" resultMap="paperResultMap">
        SELECT paper_id,name,number,detail
        FROM paper
    </select>

</mapper>
```
- 在resource/spring包下分别新建配置文件：spring-dao.xml , spring-mvc.xml , spring-service.xml

![这里写图片描述](https://img-blog.csdn.net/20180408153824995?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

spring-dao.xml：（spring-mybatis整合配置文件）

```
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
    http://www.springframework.org/schema/beans/spring-beans.xsd
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context.xsd">
    <!-- 配置整合mybatis过程 -->
    <!-- 1.配置数据库相关参数properties的属性：${url} -->
    <context:property-placeholder location="classpath:jdbc.properties"/>

    <!-- 2.数据库连接池 -->
    <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
        <!-- 配置连接池属性 -->
        <property name="driverClass" value="${jdbc.driver}"/>
        <property name="jdbcUrl" value="${jdbc.url}"/>
        <property name="user" value="${jdbc.username}"/>
        <property name="password" value="${jdbc.password}"/>

        <!-- c3p0连接池的私有属性 -->
        <property name="maxPoolSize" value="30"/>
        <property name="minPoolSize" value="10"/>
        <!-- 关闭连接后不自动commit -->
        <property name="autoCommitOnClose" value="false"/>
        <!-- 获取连接超时时间 -->
        <property name="checkoutTimeout" value="10000"/>
        <!-- 当获取连接失败重试次数 -->
        <property name="acquireRetryAttempts" value="2"/>
    </bean>

    <!-- 3.配置SqlSessionFactory对象 -->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <!-- 注入数据库连接池 -->
        <property name="dataSource" ref="dataSource"/>
        <!-- 配置MyBaties全局配置文件:mybatis-config.xml -->
        <property name="configLocation" value="classpath:mybatis-config.xml"/>
        <!-- 扫描pojo包 使用别名 -->
        <property name="typeAliasesPackage" value="com.pojo"/>
        <!-- 扫描sql配置文件:mapper需要的xml文件 -->
        <property name="mapperLocations" value="classpath:mapper/*.xml"/>
    </bean>

    <!-- 4.配置扫描Dao接口包，动态实现Dao接口，注入到spring容器中 -->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <!-- 注入sqlSessionFactory -->
        <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"/>
        <!-- 给出需要扫描Dao接口包 -->
        <property name="basePackage" value="com.dao"/>
    </bean>
</beans>
```

spring-mvc.xml：（spring mvc配置文件）

```
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
    http://www.springframework.org/schema/beans/spring-beans.xsd
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context.xsd
    http://www.springframework.org/schema/mvc
    http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd">
    <!-- 配置SpringMVC -->
    <!-- 1.开启SpringMVC注解模式 -->
    <!-- 简化配置：
        (1)自动注册DefaultAnootationHandlerMapping,AnotationMethodHandlerAdapter
        (2)提供一些列：数据绑定，数字和日期的format @NumberFormat, @DateTimeFormat, xml,json默认读写支持
    -->
    <mvc:annotation-driven />

    <!-- 2.静态资源默认servlet配置
        (1)加入对静态资源的处理：js,gif,png
        (2)允许使用"/"做整体映射
     -->
    <mvc:default-servlet-handler/>

    <!-- 3.配置jsp 显示ViewResolver -->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="viewClass" value="org.springframework.web.servlet.view.JstlView" />
        <property name="prefix" value="/WEB-INF/jsp/" />
        <property name="suffix" value=".jsp" />
    </bean>

    <!-- 4.扫描web相关的bean -->
    <context:component-scan base-package="com.controller" />
</beans>
```

spring-service.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
    http://www.springframework.org/schema/beans/spring-beans.xsd
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context.xsd
    http://www.springframework.org/schema/tx
    http://www.springframework.org/schema/tx/spring-tx.xsd">
    <!-- 扫描service包下所有使用注解的类型 -->
    <context:component-scan base-package="com.service" />

    <!-- 配置事务管理器 -->
    <bean id="transactionManager"
          class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <!-- 注入数据库连接池 -->
        <property name="dataSource" ref="dataSource" />
    </bean>
    <!-- 配置基于注解的声明式事务 -->
    <tx:annotation-driven transaction-manager="transactionManager" />
</beans>
```
- 在jdbc.properties文件中配置mysql数据库（**这里的数据库名、表名，用户名，密码一定要记得修改！！！**）。

```
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://127.0.0.1:3306/papersystem?useUnicode=true&characterEncoding=utf8
jdbc.username=root
jdbc.password=12345678
```
- 在log4j.properties文件中配置日志设定

```
log4j.rootLogger=ERROR, stdout
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%5p [%t] - %m%n
```
- 在mybatis-config.xml文件中配置mybtis框架

```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
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
- 配置web.xml文件

```
<!DOCTYPE web-app PUBLIC
                "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
                "http://java.sun.com/dtd/web-app_2_3.dtd" >

<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee
                      http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
         version="3.1" metadata-complete="true">
<display-name>Archetype Created Web Application</display-name>
<servlet>
    <servlet-name>dispatcher</servlet-name>
    <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
    <!-- 配置springMVC需要加载的配置文件
        spring-dao.xml,spring-service.xml,spring-mvc.xml
        Mybatis - > spring -> springmvc
     -->
    <init-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath:spring/spring-*.xml</param-value>
    </init-param>
</servlet>
<servlet-mapping>
    <servlet-name>dispatcher</servlet-name>
    <!-- 默认匹配所有的请求 -->
    <url-pattern>/</url-pattern>
</servlet-mapping>
    <filter>
        <filter-name>encodingFilter</filter-name>
        <filter-class>
            org.springframework.web.filter.CharacterEncodingFilter
        </filter-class>
        <init-param>
            <param-name>encoding</param-name>
            <param-value>utf-8</param-value>
        </init-param>
    </filter>

    <filter-mapping>
        <filter-name>encodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
</web-app>
```
- 修改index.jsp文件

index.jsp

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
         pageEncoding="UTF-8" %>
<%
    pageContext.setAttribute("path", request.getContextPath());
%>
<!DOCTYPE HTML>
<html>
<head>
    <title>首页</title>
    <style type="text/css">
        a {
            text-decoration: none;
            color: black;
            font-size: 18px;
        }

        h3 {
            width: 180px;
            height: 38px;
            margin: 100px auto;
            text-align: center;
            line-height: 38px;
            background: deepskyblue;
            border-radius: 4px;
        }
    </style>
</head>
<body>
<div class="container">
    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="page-header">
                <h1>
                    基于SSM框架的管理系统：简单实现增、删、改、查。
                </h1>
            </div>
        </div>
    </div>
</div>
<br><br>
<h3>
    <a href="${path }/paper/allPaper">点击进入管理页面</a>
</h3>
</body>
</html>
```
- 在WEB-INF/jsp包下分别新建addPaper,jsp , appPaper.jsp , updatePaper.jsp

allPaper.jsp：所有论文显示界面

```
<%--
  Created by IntelliJ IDEA.
  User: lenovo
  Date: 2018/4/6
  Time: 16:57
  To change this template use File | Settings | File Templates.
--%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<% String appPath = request.getContextPath(); %>
<html>
<head>
    <title>Paper列表</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- 引入 Bootstrap -->
    <link href="https://cdn.bootcss.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
<div class="container">
    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="page-header">
                <h1>
                    基于SSM框架的管理系统：简单实现增、删、改、查。
                </h1>
            </div>
        </div>
    </div>

    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="page-header">
                <h1>
                    <small>论文列表 —— 显示所有论文</small>
                </h1>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-4 column">
            <a class="btn btn-primary" href="${path}/paper/toAddPaper">新增</a>
        </div>
    </div>
    <div class="row clearfix">
        <div class="col-md-12 column">
            <table class="table table-hover table-striped">
                <thead>
                <tr>
                    <th>论文编号</th>
                    <th>论文名字</th>
                    <th>论文数量</th>
                    <th>论文详情</th>
                    <th>操作</th>
                </tr>
                </thead>
                <tbody>
                <c:forEach var="paper" items="${requestScope.get('list')}" varStatus="status">
                    <tr>
                        <td>${paper.paperId}</td>
                        <td>${paper.paperName}</td>
                        <td>${paper.paperNum}</td>
                        <td>${paper.paperDetail}</td>
                        <td>
                            <a href="${path}/paper/toUpdatePaper?id=${paper.paperId}">更改</a> |
                            <a href="<%=appPath%>/paper/del/${paper.paperId}">删除</a>
                        </td>
                    </tr>
                </c:forEach>
                </tbody>
            </table>
        </div>
    </div>
</div>
```

addPaper,jsp：论文添加页面

```
<%--
  Created by IntelliJ IDEA.
  User: lenovo
  Date: 2018/4/7
  Time: 16:45
  To change this template use File | Settings | File Templates.
--%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    String path = request.getContextPath();
    String basePath = request.getScheme() + "://"
            + request.getServerName() + ":" + request.getServerPort()
            + path + "/";
%>
<html>
<head>
    <title>新增论文</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- 引入 Bootstrap -->
    <link href="https://cdn.bootcss.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
<div class="container">
    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="page-header">
                <h1>
                    基于SSM框架的管理系统：简单实现增、删、改、查。
                </h1>
            </div>
        </div>
    </div>

    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="page-header">
                <h1>
                    <small>新增论文</small>
                </h1>
            </div>
        </div>
    </div>
    <form action="" name="userForm">
        论文名称：<input type="text" name="paperName"><br><br><br>
        论文数量：<input type="text" name="paperNum"><br><br><br>
        论文详情：<input type="text" name="paperDetail"><br><br><br>
        <input type="button" value="添加" onclick="addPaper()">
    </form>

    <script type="text/javascript">
        function addPaper() {
            var form = document.forms[0];
            form.action = "<%=basePath %>paper/addPaper";
            form.method = "post";
            form.submit();
        }
    </script>
</div>
```

updatePaper.jsp：论文更改界面

```
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    String path = request.getContextPath();
    String basePath = request.getScheme() + "://"
            + request.getServerName() + ":" + request.getServerPort()
            + path + "/";
%>
<html>
<head>
    <title>修改论文</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- 引入 Bootstrap -->
    <link href="https://cdn.bootcss.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
<div class="container">
    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="page-header">
                <h1>
                    基于SSM框架的管理系统：简单实现增、删、改、查。
                </h1>
            </div>
        </div>
    </div>

    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="page-header">
                <h1>
                    <small>修改论文</small>
                </h1>
            </div>
        </div>
    </div>

    <form action="" name="userForm">
        <input type="hidden" name="paperId" value="${paper.paperId}"/>
        论文名称：<input type="text" name="paperName" value="${paper.paperName}"/>
        论文数量：<input type="text" name="paperNum" value="${paper.paperNum}"/>
        论文详情：<input type="text" name="paperDetail" value="${paper.paperDetail }"/>
        <input type="button" value="提交" onclick="updatePaper()"/>
    </form>
    <script type="text/javascript">
        function updatePaper() {
            var form = document.forms[0];
            form.action = "<%=basePath %>paper/updatePaper";
            form.method = "post";
            form.submit();
        }
    </script>
</div>
```

**4. 利用tomcat进行项目部署**
- 下载 安装 tomcat：[tomcat 9 下载地址](https://tomcat.apache.org/download-90.cgi)
- IDEA配置tomcat： 

	点击上方的 Run 选项，选择Edit Configuration

![这里写图片描述](https://img-blog.csdn.net/20180408153940535?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选择default -> tomcat -> local 选择下载安装到本地的tomcat服务器的地址

![这里写图片描述](https://img-blog.csdn.net/20180408154110416?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

切换到Deployment选项页 点击 + 号选择 Arctifact ，

![这里写图片描述](https://img-blog.csdn.net/20180408154406461?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

添加 项目名:war exploded 打包 
![这里写图片描述](https://img-blog.csdn.net/20180408154415778?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后在Application context 中选择 空白 那一项，点击 Apply 应用。 
![这里写图片描述](https://img-blog.csdn.net/20180408154428398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 最后切换到index.jsp界面，右键运行项目

![这里写图片描述](https://img-blog.csdn.net/20180408154609651?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**5. 运行界面**

![这里写图片描述](https://img-blog.csdn.net/20180408155013656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

论文列表界面： 
![这里写图片描述](https://img-blog.csdn.net/20180408155101549?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

新增论文界面： 
![这里写图片描述](https://img-blog.csdn.net/20180408155118116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

论文修改界面： 
![这里写图片描述](https://img-blog.csdn.net/20180408155131740?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2toeHU2NjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

至此，利用IDEA+Maven+SSM框架实现的简单的增、删、改、查功能已经构建完毕！



