# SSM 配置 - z69183787的专栏 - CSDN博客
2017年01月22日 18:06:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：438
pom.xml（parent）：
```
<properties>
        <repo.internal.snapshots.url>http://xxxxx</repo.internal.snapshots.url>
        <repo.internal.releases.url>http://xxxxx</repo.internal.releases.url>
        <repo.external.url>http://xxxxx</repo.external.url>
        <repo.proxy.url>http://xxxxx</repo.proxy.url>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <useDefaultDelimiters>false</useDefaultDelimiters>
        <xxxxx.version>1.0.0-SNAPSHOT</xxxxx.version>
        <java.version>1.6</java.version>
        <env>qa</env>
        <javax.servlet.version>3.1.0</javax.servlet.version>
        <spring.version>4.1.8.RELEASE</spring.version>
        <aspectj.version>1.8.9</aspectj.version>
        <lombok.version>1.14.8</lombok.version>
        <slf4j.version>1.7.10</slf4j.version>
        <log4j2.version>2.3</log4j2.version>
        <disruptor.version>3.3.4</disruptor.version>
        <gson.version>2.4</gson.version>
        <jackson.version>2.4.4</jackson.version>
        <joda.version>2.3</joda.version>
        <quartz.version>2.2.1</quartz.version>
        <httpclient.version>4.5</httpclient.version>
        <asm.version>3.3.1</asm.version>
        <cglib.version>3.1</cglib.version>
        <mybatis.version>3.2.8</mybatis.version>
     <mybatis-spring.version>1.2.2</mybatis-spring.version>
     <mysql.version>5.1.34</mysql.version>
    </properties>
    <build>
        <!--<finalName>emidas-coupon</finalName>-->
        <resources>
            <resource>
                <directory>src/main/resources</directory>
                <filtering>true</filtering>
            </resource>
            <resource>
                <directory>src/main/java</directory>
                <includes>
                    <include>**/*.properties</include>
                    <include>**/*.xml</include>
                </includes>
            </resource>
        </resources>
        <pluginManagement>
            <plugins>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-compiler-plugin</artifactId>
                    <version>2.3.2</version>
                    <configuration>
                        <source>${java.version}</source>
                        <target>${java.version}</target>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-source-plugin</artifactId>
                    <version>2.2</version>
                    <executions>
                        <execution>
                            <id>attach-sources</id>
                            <goals>
                                <goal>jar</goal>
                            </goals>
                        </execution>
                    </executions>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-resources-plugin</artifactId>
                    <version>2.6</version>
                    <configuration>
                        <encoding>${project.build.sourceEncoding}</encoding>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-war-plugin</artifactId>
                    <version>2.4</version>
                    <configuration>
                        <warName>${project.artifactId}-${env}-${project.version}</warName>
                    </configuration>
                </plugin>
                <plugin>
                    <groupId>org.mybatis.generator</groupId>
                    <artifactId>mybatis-generator-maven-plugin</artifactId>
                    <version>1.3.2</version>
                    <configuration>
                        <verbose>true</verbose>
                        <overwrite>true</overwrite>
                        <configurationFile>src/main/resources/config/mybatis/generatorConfig.xml</configurationFile>
                    </configuration>
                </plugin>
            </plugins>
        </pluginManagement>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-source-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
```
pom.xml：
```
<dependencies>
        <dependency>
            <groupId>xxxxx</groupId>
            <artifactId>xxxxx</artifactId>
            <version>${xxxxx.version}</version>
        </dependency>
	<!-- mybitis-->
     <dependency>
       <groupId>org.mybatis</groupId>
       <artifactId>mybatis</artifactId>
       <version>${mybatis.version}</version>
     </dependency>
     <dependency>
       <groupId>org.mybatis</groupId>
       <artifactId>mybatis-spring</artifactId>
       <version>${mybatis-spring.version}</version>
      </dependency>
      <!-- mysql -->
      <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
        <version>${mysql.version}</version>
      </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>${javax.servlet.version}</version>
        </dependency>
        <dependency>
            <groupId>commons-collections</groupId>
            <artifactId>commons-collections</artifactId>
            <version>3.2.1</version>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
            <version>3.3.2</version>
        </dependency>
        <!-- spring -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <!-- <version>3.1.1.RELEASE</version> -->
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
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-expression</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aop</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-orm</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.aspectj</groupId>
            <artifactId>aspectjrt</artifactId>
            <version>${aspectj.version}</version>
        </dependency>
        <dependency>
            <groupId>org.aspectj</groupId>
            <artifactId>aspectjweaver</artifactId>
            <version>${aspectj.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
            <scope>test</scope>
            <exclusions>
                <exclusion>
                    <groupId>junit</groupId>
                    <artifactId>junit</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>org.testng</groupId>
            <artifactId>testng</artifactId>
            <version>6.7</version>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.10</version>
            <scope>test</scope>
        </dependency>
        <!-- log4j2-->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>jcl-over-slf4j</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>jul-to-slf4j</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-api</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-core</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-slf4j-impl</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-1.2-api</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>com.lmax</groupId>
            <artifactId>disruptor</artifactId>
            <version>${disruptor.version}</version>
        </dependency>
        <!-- log4j2-->
        <dependency>
            <groupId>org.quartz-scheduler</groupId>
            <artifactId>quartz</artifactId>
            <version>${quartz.version}</version>
        </dependency>
        <!-- asm cglib -->
        <dependency>
            <groupId>asm</groupId>
            <artifactId>asm</artifactId>
            <version>${asm.version}</version>
        </dependency>
        <dependency>
            <groupId>cglib</groupId>
            <artifactId>cglib</artifactId>
            <version>${cglib.version}</version>
        </dependency>
        <!-- joda-->
        <dependency>
            <groupId>joda-time</groupId>
            <artifactId>joda-time</artifactId>
            <version>${joda.version}</version>
        </dependency>
        <!-- jackson-->
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-core</artifactId>
            <version>${jackson.version}</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>${jackson.version}</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-annotations</artifactId>
            <version>${jackson.version}</version>
        </dependency>
        <!-- gson-->
        <dependency>
            <groupId>com.google.code.gson</groupId>
            <artifactId>gson</artifactId>
            <version>${gson.version}</version>
        </dependency>
        <!-- http client -->
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
            <version>${httpclient.version}</version>
        </dependency>
    </dependencies>
    <build>
        <finalName>${project.artifactId}-${env}-${project.version}</finalName>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-resources-plugin</artifactId>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-war-plugin</artifactId>
                <configuration>
                    <warName>${project.artifactId}-${env}-${project.version}</warName>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.mybatis.generator</groupId>
                <artifactId>mybatis-generator-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
```
Spring
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx"
	xsi:schemaLocation="
    http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.1.xsd
    http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-4.1.xsd
       http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-4.1.xsd
    http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.1.xsd">
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
       <property name="basePackage" value="xxxxx.dao.mapper" />
        <!--<property name="sqlSessionFactoryBeanName" value="sqlSessionFactory" />-->
    </bean>
    <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource" init-method="init" destroy-method="close">
        <property name="driverClass" value="${jdbc.driverClassName}" />
         <property name="jdbcUrl" value="${jdbc.url}" />
        <property name="user" value="${jdbc.username}" />
       <property name="password" value="${jdbc.password}" />
        <property name="minPoolSize" value="2"/>
        <property name="maxPoolSize" value="30"/>
        <property name="initialPoolSize" value="2"/>
        <property name="maxIdleTime" value="1800"/>
        <property name="idleConnectionTestPeriod" value="60"/>
        <property name="acquireRetryAttempts" value="3"/>
        <property name="acquireRetryDelay" value="300"/>
        <property name="maxStatements" value="0"/>
        <property name="maxStatementsPerConnection" value="100"/>
        <property name="numHelperThreads" value="6"/>
        <property name="maxAdministrativeTaskTime" value="5"/>
        <property name="preferredTestQuery" value="SELECT 1"/>
    </bean>
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <!--dataource-->
        <property name="dataSource" ref="dataSource"/>
        <!--Mapper files-->
        <property name="mapperLocations" value="classpath*:config/mybatis/local/*.xml" />
        <property name="typeAliasesPackage" value="xxxxx.dao.entity"/>
        <property name="configLocation" value="classpath:config/mybatis/mybatis-configuration.xml" />
    </bean>
    <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <property name="dataSource">
            <ref bean="dataSource" />
        </property>
    </bean>
    <!-- 开启AOP监听 只对当前配置文件有效 -->
    <aop:aspectj-autoproxy expose-proxy="true"/>
    <!-- 开启注解事务 只对当前配置文件有效 -->
    <tx:annotation-driven transaction-manager="txManager"/>
    <bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource"/>
    </bean>
    <tx:advice id="txAdvice" transaction-manager="txManager">
        <tx:attributes>
            <tx:method name="get*" read-only="true"/>
            <tx:method name="select*" read-only="true"/>
            <tx:method name="load*" read-only="true"/>
            <tx:method name="*" propagation="REQUIRED"/>
        </tx:attributes>
    </tx:advice>
    <!--expression="execution(* xxxxx.service..*.*(..)) or-->
    <!--execution(* xxxxx.remote..*.*(..))"/>-->
    <aop:config expose-proxy="true">
        <!-- 只对业务逻辑层实施事务 -->
        <aop:pointcut id="txPointcut"
                      expression="execution(* xxxxx.service..*.*(..))"/>
        <aop:advisor advice-ref="txAdvice" pointcut-ref="txPointcut"/>
    </aop:config>
</beans>
```
mybatis config:
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <settings>
        <setting name="logImpl" value="LOG4J2" />
        <!-- 对于批量更新操作缓存SQL以提高性能 -->
        <!-- defaultExecutorType设置为BATCH有个缺陷就是无法获取update、delete返回的行数 -->
        <!-- <setting name="defaultExecutorType" value="BATCH" />-->
        <!-- default <setting name="defaultExecutorType" value="SIMPLE" />-->
        <!--<setting name="useGeneratedKeys" value="true"/> <!– 自动生成主键 –>-->
    </settings>
    <plugins>
        <plugin interceptor="xxxxx.dao.plugin.page.PageInterceptor">
            <property name="dialectClass" value="xxxxx.dialect.MySQLDialect"/>
        </plugin>
    </plugins>
</configuration>
```
mapper xml:（type可用 alias简称，即上述typeAlias映射的entity文件夹，简名默认为 首字母大写类名）
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="xxxxx.dao.mapper.PrizeMapper" >
  <resultMap id="BaseResultMap" type="Prize" >
    <id column="ID" property="ID" jdbcType="INTEGER" />
    <result column="PackageId" property="packageId" jdbcType="INTEGER" />
    <result column="OperatorId" property="operatorId" jdbcType="INTEGER" />
    <result column="CreateTime" property="createTime" jdbcType="TIMESTAMP" />
    <result column="UpdateTime" property="updateTime" jdbcType="TIMESTAMP" />
  </resultMap>
  <sql id="Base_Column_List" >
    ID, PackageId, OperatorId, CreateTime, UpdateTime
  </sql>
  <select id="selectByPrimaryKey" resultMap="BaseResultMap" parameterType="java.lang.Integer" >
    select 
    <include refid="Base_Column_List" />
    from XXX_Prize
    where ID = #{ID,jdbcType=INTEGER} AND Status = 1
  </select>
  <delete id="deleteByPrimaryKey" parameterType="java.lang.Integer" >
    delete from XXX_Prize
    where ID = #{ID,jdbcType=INTEGER}
  </delete>
  <insert id="insert" parameterType="Prize" >
    <selectKey resultType="java.lang.Integer" keyProperty="ID" order="AFTER" >
      SELECT LAST_INSERT_ID()
    </selectKey>
    insert into XXX_Prize (PackageId,OperatorId, CreateTime, UpdateTime)
    values (#{packageId,jdbcType=INTEGER}, #{operatorId,jdbcType=INTEGER}, NOW(),NOW())
  </insert>
</mapper>
```
mapper java：
```java
package xxxxx.dao.mapper;
import org.apache.ibatis.annotations.Param;
public interface PrizeMapper {
    int deleteByPrimaryKey(Integer ID);
    int insert(Prize record);
    int insertSelective(Prize record);
    int deleteById(@Param(value = "prizeId") List<Integer> prizeId);
}
```
log4j2.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN" >
    <properties>
        <property name="LOG_HOME">/xxxxx/</property>
    </properties>
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n" />
        </Console>
        <RollingRandomAccessFile name="infoLog" fileName="${LOG_HOME}/app.log"
                                 filePattern="${LOG_HOME}/info.%d{yyyy-MM-dd}.log.gz" append="true">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="warnLog" fileName="${LOG_HOME}/warn.log"
                                 filePattern="${LOG_HOME}/warn.%d{yyyy-MM-dd}.log.gz" append="true">
            <Filters>
                <ThresholdFilter level="error" onMatch="DENY" onMismatch="NEUTRAL"/>
                <ThresholdFilter level="warn" onMatch="ACCEPT" onMismatch="DENY"/>
            </Filters>
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="errorLog" fileName="${LOG_HOME}/error.log"
                                 filePattern="${LOG_HOME}/error.%d{yyyy-MM-dd}.log.gz" append="true">
            <Filters>
                <ThresholdFilter level="ERROR" onMatch="ACCEPT" onMismatch="DENY"/>
            </Filters>
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
        </RollingRandomAccessFile>
    </Appenders>
    <Loggers>
        <!-- 第三方的软件日志级别 -->
        <AsyncLogger name="org.springframework" level="info" additivity="true">
            <AppenderRef ref="warnLog"/>
            <AppenderRef ref="errorLog"/>
        </AsyncLogger>
        <!-- 应用的日志级别 -->
        <AsyncLogger name="xxxxx" level="info" additivity="true">
            <AppenderRef ref="infoLog"/>
            <AppenderRef ref="warnLog"/>
            <AppenderRef ref="errorLog"/>
        </AsyncLogger>
        <!--<logger name="java.sql.PreparedStatement" level="debug" additivity="true">-->
            <!--<AppenderRef ref="Console"/>-->
        <!--</logger>-->
        <!--<logger name="xxxxx.dao.mapper" level="debug" additivity="false">-->
            <!--<AppenderRef ref="Console"/>-->
        <!--</logger>-->
        <!-- 剩余其他的日志级别 -->
        <asyncRoot level="info" includeLocation="true">
            <AppenderRef ref="infoLog" />
            <AppenderRef ref="Console" />
        </asyncRoot>
    </Loggers>
</Configuration>
```
//引入
@Autowired
    private PrizeMapper prizeMapper;
