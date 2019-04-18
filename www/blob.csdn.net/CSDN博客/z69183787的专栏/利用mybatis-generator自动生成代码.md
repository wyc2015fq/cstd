# 利用mybatis-generator自动生成代码 - z69183787的专栏 - CSDN博客
2015年06月19日 11:39:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2185
[mybatis-generator](https://github.com/mybatis/generator)有三种用法：命令行、eclipse插件、maven插件。个人觉得maven插件最方便，可以在eclipse/intellij
 idea等ide上可以通用。
下面是从官网上的截图：(不过官网[www.mybatis.org](http://www.mybatis.org/) 最近一段时间,好象已经挂了)
![](http://images.cnitblog.com/blog/27612/201501/081227432658706.png)
一、在pom.xml中添加plugin
```
<plugin>
    <groupId>org.mybatis.generator</groupId>
    <artifactId>mybatis-generator-maven-plugin</artifactId>
    <version>1.3.2</version>
    <configuration>
        <configurationFile>src/main/resources/mybatis-generator/generatorConfig.xml</configurationFile>
        <verbose>true</verbose>
        <overwrite>true</overwrite>
    </configuration>
    <executions>
        <execution>
            <id>Generate MyBatis Artifacts</id>
            <goals>
                <goal>generate</goal>
            </goals>
        </execution>
    </executions>
    <dependencies>
        <dependency>
            <groupId>org.mybatis.generator</groupId>
            <artifactId>mybatis-generator-core</artifactId>
            <version>1.3.2</version>
        </dependency>
    </dependencies>
</plugin>
```
其中generatorConfig.xml的位置，大家根据实际情况自行调整
二、generatorConfig.xml配置文件
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <?xml version="1.0" encoding="UTF-8"?>
 2 <!DOCTYPE generatorConfiguration
 3         PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
 4         "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
 5 
 6 <generatorConfiguration>
 7     <classPathEntry
 8             location="C:/Oracle/Middleware/wlserver_10.3/server/lib/ojdbc6.jar"/>
 9     <context id="my" targetRuntime="MyBatis3">
10         <commentGenerator>
11             <property name="suppressDate" value="false"/>
12             <property name="suppressAllComments" value="true"/>
13         </commentGenerator>
14 
15         <jdbcConnection driverClass="oracle.jdbc.driver.OracleDriver"
16                         connectionURL="jdbc:oracle:thin:@172.20.16.***:1521:CARGO" userId="***"
17                         password="***"/>
18 
19         <javaModelGenerator targetPackage="ctas.test.entity"
20                             targetProject="D:/yangjm/Code/CTAS/JAVAEE/CTAS2CCSP/src/main/java">
21             <property name="enableSubPackages" value="true"/>
22             <property name="trimStrings" value="true"/>
23         </javaModelGenerator>
24 
25         <sqlMapGenerator targetPackage="ctas.test.entity.xml"
26                          targetProject="D:/yangjm/Code/CTAS/JAVAEE/CTAS2CCSP/src/main/java">
27             <property name="enableSubPackages" value="true"/>
28         </sqlMapGenerator>
29 
30         <javaClientGenerator targetPackage="ctas.test.mapper"
31                              targetProject="D:/yangjm/Code/CTAS/JAVAEE/CTAS2CCSP/src/main/java" type="XMLMAPPER">
32             <property name="enableSubPackages" value="true"/>
33         </javaClientGenerator>
34 
35         <!--<table tableName="T_FEE_AGTBILL" domainObjectName="FeeAgentBill"
36                enableCountByExample="false" enableUpdateByExample="false"
37                enableDeleteByExample="false" enableSelectByExample="false"
38                selectByExampleQueryId="false"/>-->
39 
40         <table tableName="CTAS_FEE_BASE" domainObjectName="FeeBase"
41                enableCountByExample="false" enableUpdateByExample="false"
42                enableDeleteByExample="false" enableSelectByExample="false"
43                selectByExampleQueryId="false">
44             <!--<columnRenamingRule searchString="^D_"
45                                 replaceString=""/>-->
46         </table>
47 
48     </context>
49 </generatorConfiguration>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
几个要点：
a) 因为生成过程中需要连接db，所以第3行指定了驱动jar包的位置
b) 15-17行为连接字符串
c) 19-33行指定生成“entity实体类、mybatis映射xml文件、mapper接口”的具体位置
d) 40-46行为具体要生成的表，如果有多个表，复制这一段，改下表名即可
三、使用方式
mvn mybatis-generator:generate
如果是在intellij 环境，直接鼠标点击即可
![](http://images.cnitblog.com/blog/27612/201501/081223382503659.jpg)
最后给出目录结构图：
![](http://images.cnitblog.com/blog/27612/201501/081226229375280.jpg)
最后给一些小技巧：
a) 建表时，字段名称建议用"_"分隔多个单词，比如:AWB_NO、REC_ID...，这样生成的entity，属性名称就会变成漂亮的驼峰命名，即：awbNo、recId
b)oracle中，数值形的字段，如果指定精度，比如Number(12,2)，默认生成entity属性是BigDecimal型 ，如果不指定精度，比如:Number(9)，指默认生成的是Long型
c)oracle中的nvarchar/nvarchar2，mybatis-generator会识别成Object型，建议不要用nvarchar2，改用varchar2
