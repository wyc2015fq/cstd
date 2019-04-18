# Intellij IDEA 14中使用MyBatis-generator 自动生成MyBatis代码 - z69183787的专栏 - CSDN博客
2015年06月19日 11:53:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：27444
Intellij IDEA 14 作为Java IDE 神器，接触后发现，非常好用，对它爱不释手，打算离开eclipse和myeclipse，投入Intellij IDEA的怀抱。
     然而在使用的过程中会发现Intellij IDEA也有一些不尽如意的地方，难免会有些不爽：Intellij IDEA 的插件库远不及eclipse的丰富。 mybatis-generator在eclipse中有专门的插件，而没有开发出Intellij IDEA能够使用的插件。
    不过不用灰心，如果你的项目是使用maven组织的，那么我们可以在Intellij IDEA中使用 mybatis-generator-maven-plugin插件来完成MyBatis model 和Mapper文件的自动生成。
#### STEP 0.在Intellij IDEA创建maven项目（本过程比较简单，略）
#### STEP 1. 在maven项目的pom.xml 添加mybatis-generator-maven-plugin 插件
> 
**[html]**[view
 plain](http://blog.csdn.net/sunny243788557/article/details/45166397#)[copy](http://blog.csdn.net/sunny243788557/article/details/45166397#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/590183/fork)
- <build>
- <finalName>xxx</finalName>
- <plugins>
- <plugin>
- <groupId>org.mybatis.generator</groupId>
- <artifactId>mybatis-generator-maven-plugin</artifactId>
- <version>1.3.2</version>
- <configuration>
- <verbose>true</verbose>
- <overwrite>true</overwrite>
- </configuration>
- </plugin>
- </plugins>
- </build>
#### STEP 2. 在maven项目下的src/main/resources 目录下建立名为 generatorConfig.xml的配置文件，作为mybatis-generator-maven-plugin 插件的执行目标，模板如下：
> 
**[html]**[view
 plain](http://blog.csdn.net/sunny243788557/article/details/45166397#)[copy](http://blog.csdn.net/sunny243788557/article/details/45166397#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/590183/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE generatorConfiguration  
-         PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"  
-         "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
- <generatorConfiguration>
- <!--导入属性配置 -->
- <propertiesresource="generator.properties"></properties>
- 
- <!--指定特定数据库的jdbc驱动jar包的位置 -->
- <classPathEntrylocation="${jdbc.driverLocation}"/>
- 
- <contextid="default"targetRuntime="MyBatis3">
- 
- 
- <!-- optional，旨在创建class时，对注释进行控制 -->
- <commentGenerator>
- <propertyname="suppressDate"value="true"/>
- </commentGenerator>
- 
- 
- <!--jdbc的数据库连接 -->
- <jdbcConnectiondriverClass="${jdbc.driverClass}"connectionURL="${jdbc.connectionURL}"userId="${jdbc.userId}"password="${jdbc.password}">
- </jdbcConnection>
- 
- 
- 
- <!-- 非必需，类型处理器，在数据库类型和java类型之间的转换控制-->
- <javaTypeResolver>
- <propertyname="forceBigDecimals"value="false"/>
- </javaTypeResolver>
- 
-         <!-- Model模型生成器,用来生成含有主键key的类，记录类 以及查询Example类  
-             targetPackage     指定生成的model生成所在的包名  
-             targetProject     指定在该项目下所在的路径  
-         -->
- <javaModelGeneratortargetPackage="org.louis.hometutor.po"targetProject="src/main/java">
- <!-- 是否对model添加 构造函数 -->
- <propertyname="constructorBased"value="true"/>
- 
- <!-- 是否允许子包，即targetPackage.schemaName.tableName -->
- <propertyname="enableSubPackages"value="false"/>
- 
- <!-- 建立的Model对象是否 不可改变  即生成的Model对象不会有 setter方法，只有构造方法 -->
- <propertyname="immutable"value="true"/>
- 
- <!-- 给Model添加一个父类 -->
- <propertyname="rootClass"value="com.foo.louis.Hello"/>
- 
- <!-- 是否对类CHAR类型的列的数据进行trim操作 -->
- <propertyname="trimStrings"value="true"/>
- </javaModelGenerator>
- 
- <!--Mapper映射文件生成所在的目录 为每一个数据库的表生成对应的SqlMap文件 -->
- <sqlMapGeneratortargetPackage="org.louis.hometutor.domain"targetProject="src/main/java">
- <propertyname="enableSubPackages"value="false"/>
- </sqlMapGenerator>
- 
- 
-         <!-- 客户端代码，生成易于使用的针对Model对象和XML配置文件 的代码  
- type="ANNOTATEDMAPPER",生成Java Model 和基于注解的Mapper对象  
- type="MIXEDMAPPER",生成基于注解的Java Model 和相应的Mapper对象  
- type="XMLMAPPER",生成SQLMap XML文件和独立的Mapper接口  
-         -->
- <javaClientGeneratortargetPackage="com.foo.tourist.dao"targetProject="src/main/java"type="MIXEDMAPPER">
- <propertyname="enableSubPackages"value=""/>
-             <!--  
-                     定义Maper.java 源代码中的ByExample() 方法的可视性，可选的值有：  
-                     public;  
-                     private;  
-                     protected;  
-                     default  
-                     注意：如果 targetRuntime="MyBatis3",此参数被忽略  
-              -->
- <propertyname="exampleMethodVisibility"value=""/>
-             <!--  
-                                            方法名计数器  
-               Important note: this property is ignored if the target runtime is MyBatis3.  
-              -->
- <propertyname="methodNameCalculator"value=""/>
- 
- <!--
-                                                 为生成的接口添加父接口
-              -->
- <propertyname="rootInterface"value=""/>
- 
- </javaClientGenerator>
- 
- 
- 
- <tabletableName="lession"schema="louis">
- 
-             <!-- optional   , only for mybatis3 runtime  
-                  自动生成的键值（identity,或者序列值）  
-                如果指定此元素，MBG将会生成<selectKey>元素，然后将此元素插入到SQL Map的<insert> 元素之中  
-                sqlStatement 的语句将会返回新的值  
-                如果是一个自增主键的话，你可以使用预定义的语句,或者添加自定义的SQL语句. 预定义的值如下:  
-                   Cloudscape    This will translate to: VALUES IDENTITY_VAL_LOCAL()  
-                   DB2:      VALUES IDENTITY_VAL_LOCAL()  
-                   DB2_MF:       SELECT IDENTITY_VAL_LOCAL() FROM SYSIBM.SYSDUMMY1  
-                   Derby:        VALUES IDENTITY_VAL_LOCAL()  
-                   HSQLDB:   CALL IDENTITY()  
-                   Informix:     select dbinfo('sqlca.sqlerrd1') from systables where tabid=1
-                   MySql:        SELECT LAST_INSERT_ID()  
-                   SqlServer:    SELECT SCOPE_IDENTITY()  
-                   SYBASE:   SELECT @@IDENTITY  
-                   JDBC:     This will configure MBG to generate code for MyBatis3 suport of JDBC standard generated keys. This is a database independent method of obtaining the value from identity columns.  
-                   identity: 自增主键  If true, then the column is flagged as an identity column and the generated <selectKey> element will be placed after the insert (for an identity column). If false, then the generated <selectKey> will be placed before the insert (typically for a sequence).  
- 
-             -->
- <generatedKeycolumn=""sqlStatement=""identity=""type=""/>
- 
- 
-             <!-- optional.  
-                     列的命名规则：  
-                     MBG使用 <columnRenamingRule> 元素在计算列名的对应 名称之前，先对列名进行重命名，  
-                     作用：一般需要对BUSI_CLIENT_NO 前的BUSI_进行过滤  
-                     支持正在表达式  
-                      searchString 表示要被换掉的字符串  
-                      replaceString 则是要换成的字符串，默认情况下为空字符串，可选  
-             -->
- <columnRenamingRulesearchString=""replaceString=""/>
- 
- 
- 
-             <!-- optional.告诉 MBG 忽略某一列  
-                     column，需要忽略的列  
-                     delimitedColumnName:true ,匹配column的值和数据库列的名称 大小写完全匹配，false 忽略大小写匹配  
-                     是否限定表的列名，即固定表列在Model中的名称  
-             -->
- <ignoreColumncolumn="PLAN_ID"delimitedColumnName="true"/>
- 
- 
-             <!--optional.覆盖MBG对Model 的生成规则  
-                  column: 数据库的列名  
-                  javaType: 对应的Java数据类型的完全限定名  
-                  在必要的时候可以覆盖由JavaTypeResolver计算得到的java数据类型. For some databases, this is necessary to handle "odd" database types (e.g. MySql's unsigned bigint type should be mapped to java.lang.Object).  
-                  jdbcType:该列的JDBC数据类型(INTEGER, DECIMAL, NUMERIC, VARCHAR, etc.)，该列可以覆盖由JavaTypeResolver计算得到的Jdbc类型，对某些数据库而言，对于处理特定的JDBC 驱动癖好 很有必要(e.g. DB2's LONGVARCHAR type should be mapped to VARCHAR for iBATIS).  
-                  typeHandler:  
- 
-             -->
- <columnOverridecolumn=""javaType=""jdbcType=""typeHandler=""delimitedColumnName=""/>
- 
- </table>
- </context>
- </generatorConfiguration>
这里使用了外置的配置文件generator.properties，可以将一下属性配置到properties文件之中，增加配置的灵活性：
**[plain]**[view
 plain](http://blog.csdn.net/sunny243788557/article/details/45166397#)[copy](http://blog.csdn.net/sunny243788557/article/details/45166397#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/590183/fork)
- jdbc.driverLocation=D:\\maven\\com\\oracle\\ojdbc14\\10.2.0.4.0\\ojdbc14-10.2.0.4.0.jar  
- jdbc.driverClass=oracle.jdbc.driver.OracleDriver  
- jdbc.connectionURL=jdbc:oracle:thin:@//localhost:1521/XE  
- jdbc.userId=LOUIS  
- jdbc.password=123456  
项目目录如下：
![技术分享](https://img-blog.csdn.net/20150127125526859)

#### STEP 3. 在Intellij IDEA添加一个“Run运行”选项，使用maven运行mybatis-generator-maven-plugin插件 ：
> 
![技术分享](https://img-blog.csdn.net/20150127114624571)
> 
![技术分享](https://img-blog.csdn.net/20150127114637987)
之后弹出运行配置框，为当前配置配置一个名称，这里其名为"generator",然后在 “Command line” 选项中输入“mybatis-generator:generate 
 -e”
这里加了“-e ”选项是为了让该插件输出详细信息，这样可以帮助我们定位问题。
> 
![技术分享](https://img-blog.csdn.net/20150127114644399)
![技术分享](https://img-blog.csdn.net/20150127115339141)
如果添加成功，则会在run 选项中有“generator” 选项，如下：
![技术分享](https://img-blog.csdn.net/20150127115619156)
点击运行，然后不出意外的话，会在控制台输出：
**[plain]**[view
 plain](http://blog.csdn.net/sunny243788557/article/details/45166397#)[copy](http://blog.csdn.net/sunny243788557/article/details/45166397#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/590183/fork)
- C:\Java\jdk1.7.0_71\bin\java -Dmaven.home=D:\software\apache-maven-3.0.5-bin -Dclassworlds.conf=D:\software\apache-maven-3.0.5-bin\bin\m2.conf -Didea.launcher.port=7533 "-Didea.launcher.bin.path=D:\applications\JetBrains\IntelliJ IDEA 14.0.2\bin" -Dfile.encoding=UTF-8 -classpath "D:\software\apache-maven-3.0.5-bin\boot\plexus-classworlds-2.4.jar;D:\applications\JetBrains\IntelliJ IDEA 14.0.2\lib\idea_rt.jar" com.intellij.rt.execution.application.AppMain org.codehaus.classworlds.Launcher -Didea.version=14.0.2 -s D:\software\apache-maven-3.0.5-bin\conf\settings.xml mybatis-generator:generate -e  
- [INFO] Error stacktraces are turned on.  
- [INFO] Scanning for projects...  
- [INFO]                                                                           
- [INFO] ------------------------------------------------------------------------  
- [INFO] Building hometutor Maven Webapp 1.0-SNAPSHOT  
- [INFO] ------------------------------------------------------------------------  
- [INFO]   
- [INFO] --- mybatis-generator-maven-plugin:1.3.2:generate (default-cli) @ hometutor ---  
- [INFO] Connecting to the Database  
- [INFO] Introspecting table louis.lession  
- log4j:WARN No appenders could be found for logger (org.mybatis.generator.internal.db.DatabaseIntrospector).  
- log4j:WARN Please initialize the log4j system properly.  
- log4j:WARN See http://logging.apache.org/log4j/1.2/faq.html#noconfig for more info.  
- [INFO] Generating Example class for table LOUIS.LESSION  
- [INFO] Generating Record class for table LOUIS.LESSION  
- [INFO] Generating Mapper Interface for table LOUIS.LESSION  
- [INFO] Generating SQL Map for table LOUIS.LESSION  
- [INFO] Saving file LessionMapper.xml  
- [INFO] Saving file LessionExample.java  
- [INFO] Saving file Lession.java  
- [INFO] Saving file LessionMapper.java  
- [WARNING] Root class com.foo.louis.Hello cannot be loaded, checking for member overrides is disabled for this class   
- [WARNING] Existing file E:\sources\tutor\src\main\java\org\louis\hometutor\po\Lession.java was overwritten  
- [WARNING] Existing file E:\sources\tutor\src\main\java\com\foo\tourist\dao\LessionMapper.java was overwritten  
- [INFO] ------------------------------------------------------------------------  
- [INFO] BUILD SUCCESS  
- [INFO] ------------------------------------------------------------------------  
- [INFO] Total time: 2.334s  
- [INFO] Finished at: Tue Jan 27 12:04:08 CST 2015  
- [INFO] Final Memory: 8M/107M  
- [INFO] ------------------------------------------------------------------------  
- 
- Process finished with exit code 0  
好，看到BUILD SUCCESS，则大功告成，如果有错误的话，由于添加了-e 选项，会把具体的详细错误信息打印出来的，根据错误信息修改即可
