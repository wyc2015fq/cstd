# generator自动生成mybatis的xml配置、model、map - z69183787的专栏 - CSDN博客
2016年04月29日 09:40:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1194
generator自动生成mybatis的xml配置、model、map等信息：
1、下载mybatis-generator-core-1.3.2.jar包，解压
       找到lib下的需要jar包。
2、编写genertor的xml文件，名下：generator.xml
Xml代码  ![收藏代码](http://jadethao.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE generatorConfiguration  
-   PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"  
-   "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
- 
- <generatorConfiguration>
- <!-- classPathEntry:数据库的JDBC驱动的jar包地址-->
- <classPathEntrylocation="E:\oracle\product\10.2.0\db_1\jdbc\lib\ojdbc14.jar"/>
- <contextid="DB2Tables"targetRuntime="MyBatis3">
- <commentGenerator>
- <!-- 是否去除自动生成的注释 true：是 ： false:否 -->
- <propertyname="suppressAllComments"value="true"/>
- <!--数据库连接的信息：驱动类、连接地址、用户名、密码 -->
- </commentGenerator>
- <jdbcConnectiondriverClass="oracle.jdbc.driver.OracleDriver"
- connectionURL="jdbc:oracle:thin:@198.17.1.1:1521:ORCL"
- userId="unuser"
- password="password">
- </jdbcConnection>
-     <!--  默认false，把JDBC DECIMAL 和 NUMERIC 类型解析为 Integer   
-          true，把JDBC DECIMAL 和 NUMERIC 类型解析为java.math.BigDecimal   
-      -->
- <javaTypeResolver>
- <propertyname="forceBigDecimals"value="false"/>
- </javaTypeResolver>
- <!-- targetProject:自动生成代码的位置 -->
- <javaModelGeneratortargetPackage="com.soft.model"targetProject="E:\WebWorkSpace\workspace_js\downAttachdemo\src">
- <!-- enableSubPackages:是否让schema作为包的后缀 -->
- <propertyname="enableSubPackages"value="true"/>
- <!-- 从数据库返回的值被清理前后的空格  -->
- <propertyname="trimStrings"value="true"/>
- </javaModelGenerator>
- 
- <sqlMapGeneratortargetPackage="sqlmap"targetProject="E:\WebWorkSpace\workspace_js\downAttachdemo\conf">
- <propertyname="enableSubPackages"value="false"/>
- </sqlMapGenerator>
- 
- <javaClientGeneratortype="XMLMAPPER"targetPackage="com.soft.mapping"targetProject="E:\WebWorkSpace\workspace_js\downAttachdemo\src">
- <propertyname="enableSubPackages"value="true"/>
- </javaClientGenerator>
- <!-- tableName:用于自动生成代码的数据库表；domainObjectName:对应于数据库表的javaBean类名 -->
- <tableschema="untodo"tableName="mocha_t_app"domainObjectName="MochaTodoApp">
- 
- </table>
- </context>
- 
- </generatorConfiguration>
table其他属性：
enableCountByExample="false" 
enableUpdateByExample="false"
enableDeleteByExample="false" 
enableSelectByExample="false"
selectByExampleQueryId="false"
schema即为数据库名, tableName为对应的数据库表, domainObjectName是要生成的实体类, 
如果想要mapper配置文件加入sql的where条件查询, 可以将enableCountByExample等设为true, 
这样就会生成一个对应domainObjectName的Example类, enableCountByExample等设为false时, 
就不会生成对应的Example类了.
如果table里边不配置property，默认字段都生成为类属性。
<ignoreColumn column="FRED" />//忽略字段
<columnOverride column="LONG_VARCHAR_FIELD" jdbcType="VARCHAR" />//无论字段是什么类型，生成的类属性都是varchar。
3、运行有四种：命令生成（最简单）、Java生成、ant生成、maven生成。这里说两种，有兴趣其余的可以在mybatis官网去学习。
1）、运行-》cmd->java - jar jar包的文件路径  -configfile  generator.xml的文件路径  -overwrite 命令。
如下：
Java代码  ![收藏代码](http://jadethao.iteye.com/images/icon_star.png)
- java -jar E:\Websoft\mybaits\mybatis-generator-core-1.3.2\lib\mybatis-generator-core-1.3.2.jar -configfile E:\WebWorkSpace\workspace_js\downAttachdemo\src\com\mochasoft\down\generator.xml -overwrite  
成功时输出：MyBatis Generator finished successfully.
2）、java运行关键代码：
Java代码  ![收藏代码](http://jadethao.iteye.com/images/icon_star.png)
- List<String> warnings = new ArrayList<String>();  
- boolean overwrite = true;  
-   File configFile = new File("generatorConfig.xml");  
-   ConfigurationParser cp = new ConfigurationParser(warnings);  
-   Configuration config = cp.parseConfiguration(configFile);  
-   DefaultShellCallback callback = new DefaultShellCallback(overwrite);  
-   MyBatisGenerator myBatisGenerator = new MyBatisGenerator(config, callback, warnings);  
-   myBatisGenerator.generate(null);  
其实Java运行，细分可以分两种，还有一种可以去官网学习。
4、生成代码之后，根据自己的实际项目架构，可以对生成的代码进行适当的修改，如把数据库管理交有spring等等。
