# maven 开发 ejb ear - z69183787的专栏 - CSDN博客
2015年11月24日 13:45:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：703
## 基于EJB开发业务简介
在复杂的企业IT架构中 web模块主要用途其实在展现层，它接收到用户的请求后委托后端的 ejb 完成业务逻辑（将业务交由ejb处理，可方便地获得保障业务事务及分布式事务、多客户端调用等好处），然后将ejb的处理结果以可视化形式展现给用户，如此的分层架构可以方便地对ejb业务层实施可复用和分布式。
ejb通常都是jar文件，web通常是war文件，一般都是独立部署，但是，有的时候我们也希望将某些ejb和web打包的一起部署，这就是企业应用ear。
**maven 支持 ear 的管理**。
假设要完成的ear中包行一个ejb和一个web，名称分别为：ejb3和webdemo，两个模块独立开发完成后执行 install 安装到本地maven库，建立一个ear模块（其实就是普通的maven项目，只是设置pom打包类型<packaging>ear</packaging>），如果要开发一个ejb，那么就设置<packaging>ejb</packaging>。
## 1.开发一个ejb
名称ejb3，其pom大致为：
**[html]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/44805141#)[copy](http://blog.csdn.net/conquer0715/article/details/44805141#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <projectxmlns="http://maven.apache.org/POM/4.0.0"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <parent>
- <artifactId>eden</artifactId>
- <groupId>com.conquer.eden</groupId>
- <version>1.0</version>
- </parent>
- <modelVersion>4.0.0</modelVersion>
- 
- <packaging>ejb</packaging>
- 
- <artifactId>ejb3</artifactId>
- 
- <dependencies>
- <dependency>
- <groupId>javax.ejb</groupId>
- <artifactId>ejb-api</artifactId>
- <version>3.0</version>
- <scope>provided</scope>
- </dependency>
- <!--<dependency>-->
- <!--<groupId>org.eclipse.persistence</groupId>-->
- <!--<artifactId>javax.persistence</artifactId>-->
- <!--<version>2.0.5</version>-->
- <!--<scope>provided</scope>-->
- <!--</dependency>-->
- </dependencies>
- <build>
- <finalName>${artifactId}</finalName>
- <plugins>
- <plugin>
- <!--<groupId>org.apache.maven.plugins</groupId>-->
- <artifactId>maven-ejb-plugin</artifactId>
- <version>2.5</version>
- <configuration>
- <ejbVersion>3.0</ejbVersion>
- 
- <!--<filterDeploymentDescriptor>true</filterDeploymentDescriptor>-->
- 
- <!-- this is false by default -->
- <generateClient>true</generateClient>
- <!--用下面的方式进行 客户端生成定制-->
- <clientIncludes>
- <!-- 包含 test 下面的所有文件和文件夹 -->
- <!--<clientInclude>test/**</clientInclude>-->
- <!-- 包含 com/example 下面所有的文件，PS：不含文件夹-->
- <!--<clientInclude>com/example/*</clientInclude>-->
- </clientIncludes>
- <clientExcludes>
- <!-- 不包含 com/example 下的所有文件-->
- <!--<clientExclude>com/example/*</clientExclude>-->
- <!-- this will exclude all files and directories with the name sparrow under com/jack -->
- <!--<clientExclude>com/jack/**/sparrow</clientExclude>-->
- </clientExcludes>
- </configuration>
- </plugin>
- </plugins>
- </build>
- </project>
以上的pom文件需要注意的点：
1.打包类型：
**<packaging>ejb</packaging>**2.EJB插件
**<artifactId>maven-ejb-plugin</artifactId>**3.是否生成EJB客户端jar
**<generateClient>true</generateClient>**关于<generateClient>的介绍：
**<generateClient>**用于生成简单版的ejb客户端，通常只包含客户端使用到的接口而不包含实现类，
他的默认排除规则是：
- **/*Bean.class
- **/*CMP.class
- **/*Session.class
- **/package.html
所以加入我们的实现类是以Bean结尾的话，在打包成的*-client.jar中会自动去除，
当然，该规则也可以定制，可以定制需要添加和排除的文件，如下：
**[html]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/44805141#)[copy](http://blog.csdn.net/conquer0715/article/details/44805141#)
- <configuration>
- <clientIncludes>
- <!-- this will include all files and directories under com/foo/bar -->
- <clientInclude>com/foo/bar/**</clientInclude>
- <!-- this will include all files and directories under com/foo/acme -->
- <clientInclude>com/foo/acme/**</clientInclude>
- <!-- this will include all files under com/example -->
- <clientInclude>com/example/*</clientInclude>
- </clientIncludes>
- <clientExcludes>
- <!-- this will exclude all files under com/example -->
- <clientExclude>com/example/*</clientExclude>
-      <!-- this will exclude all files and directories with the name  
-           sparrow under com/jack -->
- <clientExclude>com/jack/**/sparrow</clientExclude>
- </clientExcludes>
- </configuration>
最后：在模块中添加此client的依赖时候，其类型要指定为ejb-client，如
<dependency>
    <groupId>com.conquer.eden</groupId>
    <artifactId>xaejb</artifactId>
    <version>1.0</version>
    <!--<classifier>client</classifier>-->
    **<type>ejb-client</type>**
</dependency>
官网参考：
http://maven.apache.org/plugins/maven-ejb-plugin/
http://maven.apache.org/plugins/maven-ejb-plugin/examples/generating-ejb-client.html
ejb的java代码：
**[java]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/44805141#)[copy](http://blog.csdn.net/conquer0715/article/details/44805141#)
- publicinterface TestDAOEJB {  
-     String insertData(String data) throws Exception;  
-     String insertData2(String data) throws Exception;  
-     String insertData3(String data) throws Exception;  
- }  
**[java]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/44805141#)[copy](http://blog.csdn.net/conquer0715/article/details/44805141#)
- import javax.ejb.*;  
- import javax.naming.Context;  
- import javax.naming.InitialContext;  
- import javax.sql.DataSource;  
- import java.io.IOException;  
- import java.sql.Connection;  
- import java.sql.Statement;  
- import java.util.Properties;  
- 
- //@Stateless
- //@Stateless(mappedName = "TestDAOBean")
- @Stateful
- @Remote(value = TestDAOEJB.class)  
- publicclass TestDAOBean implements TestDAOEJB {  
- 
- privatestatic String otherNode1;  
- privatestatic String otherNode2;  
- static {  
- try {  
-             Properties properties = new Properties();  
-             properties.load(TestDAOBean.class.getResourceAsStream("/config.properties"));  
-             otherNode1 = properties.getProperty("otherNode1");  
-             otherNode2 = properties.getProperty("otherNode2");  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- /**
-      * CREATE TABLE wangpl(name VARCHAR2(200))
-      */
- 
- @Override
- @TransactionAttribute(TransactionAttributeType.REQUIRED)//这是默认值，可以省略，或修改为其它事务策略
- public String insertData(String data) throws Exception {  
-         String returnString = "insertData-OK";  
-         Connection con = null;  
-         Statement statement = null;  
- try {  
-             DataSource dataSource = (DataSource) new InitialContext().lookup("DataSource1");  
-             con = dataSource.getConnection();  
-             statement = con.createStatement();  
-             String sql = "INSERT INTO wangpl VALUES('" + data + ":insertData')";  
-             statement.execute(sql);  
-         } finally {  
- // close xxx
-         }  
- 
- // 调用远程节点
-         Properties p = new Properties();  
-         p.put(Context.INITIAL_CONTEXT_FACTORY, "com.xxx.RemoteInitialContextFactory");  
-         p.put(Context.PROVIDER_URL, otherNode1);  
-         InitialContext ctx = new InitialContext(p);  
-         TestDAOEJB remote = (TestDAOEJB) ctx.lookup("TestDAOBeanRemote");  
-         remote.insertData2(data + "1");  
- 
- return returnString;  
-     }  
- @Override
- @TransactionAttribute(TransactionAttributeType.SUPPORTS)  
- public String insertData2(String data) throws Exception {  
-         String returnString = "insertData2-OK";  
-         Connection con = null;  
-         Statement statement = null;  
- try {  
-             DataSource dataSource = (DataSource) new InitialContext().lookup("DataSource1");  
-             con = dataSource.getConnection();  
-             statement = con.createStatement();  
-             String sql = "INSERT INTO wangpl VALUES('" + data + ":insertData2')";  
-             statement.execute(sql);  
-         } finally {  
- // close xxx
-         }  
- 
- // 调用远程ejb
-         Properties p = new Properties();  
-         p.put(Context.INITIAL_CONTEXT_FACTORY, "com.xxx.RemoteInitialContextFactory");  
-         p.put(Context.PROVIDER_URL, otherNode2);  
-         InitialContext ctx = new InitialContext(p);  
-         TestDAOEJB remote = (TestDAOEJB) ctx.lookup("TestDAOBeanRemote");  
-         remote.insertData3(data + "1");  
- 
- if (data.equals("11")) {  
- thrownew RuntimeException("************* Runtime Exception **************");  
-         }  
- return returnString;  
-     }  
- @TransactionAttribute(TransactionAttributeType.REQUIRES_NEW)  
- public String insertData3(String data) throws Exception {  
-         String returnString = "insertData3-OK";  
-         Connection con = null;  
-         Statement statement = null;  
- try {  
-             DataSource dataSource = (DataSource) new InitialContext().lookup("DataSource1");  
-             con = dataSource.getConnection();  
-             statement = con.createStatement();  
-             String sql = "INSERT INTO wangpl VALUES('" + data + ":insertData3')";  
-             statement.execute(sql);  
-         } finally {  
- // close xxx
-         }  
- 
- if (data.equals("211")) {  
- thrownew RuntimeException("************* Runtime Exception **************");  
-         }  
- 
- return returnString;  
-     }  
- }  
## 2.开发web模块
其pom大致如下：
**[html]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/44805141#)[copy](http://blog.csdn.net/conquer0715/article/details/44805141#)
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
- <parent>
- <artifactId>eden</artifactId>
- <groupId>com.conquer.eden</groupId>
- <version>1.0</version>
- </parent>
- <modelVersion>4.0.0</modelVersion>
- <artifactId>webdemo</artifactId>
- <packaging>war</packaging>
- <name>webdemo</name>
- <build>
- <finalName>${artifactId}</finalName>
- </build>
- <dependencies>
- <dependency>
- <groupId>javax.servlet</groupId>
- <artifactId>javax.servlet-api</artifactId>
- <version>3.1.0</version>
- <scope>provided</scope>
- </dependency>
- <dependency>
- <groupId>com.conquer.eden</groupId>
- <artifactId>ejb3</artifactId>
- <version>1.0</version>
- <!--<classifier>client</classifier>-->
- <type>ejb-client</type>
- </dependency>
- </dependencies>
- </project>
以上pom，要注意的是添加ejb的client依赖，指明类型是：<type>ejb-client</type>
web层业务代码 ，可以使用*** @EJB  @Resource 的注解方式，或者手工： new InitialContext(xx)***来访问ejb。
## 3.开发ear
其pom文件为：
**[html]**[view
 plain](http://blog.csdn.net/conquer0715/article/details/44805141#)[copy](http://blog.csdn.net/conquer0715/article/details/44805141#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <projectxmlns="http://maven.apache.org/POM/4.0.0"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <parent>
- <artifactId>eden</artifactId>
- <groupId>com.conquer.eden</groupId>
- <version>1.0</version>
- </parent>
- <modelVersion>4.0.0</modelVersion>
- <packaging>ear</packaging>
- 
- <artifactId>ear</artifactId>
- 
- <dependencies>
- <dependency>
- <groupId>com.conquer.eden</groupId>
- <artifactId>ejb3</artifactId>
- <version>1.0</version>
- <type>ejb</type>
- </dependency>
- <dependency>
- <groupId>com.conquer.eden</groupId>
- <artifactId>webdemo</artifactId>
- <version>1.0</version>
- <type>war</type>
- </dependency>
- </dependencies>
- 
- <build>
- <finalName>${artifactId}</finalName>
- <plugins>
- <plugin>
- <artifactId>maven-ear-plugin</artifactId>
- <version>2.10</version>
- <configuration>
- <!--<generateModuleId>true</generateModuleId>-->
- 
- <!--指定打包范围，避免无用jar混乱加入-->
- <!--<packagingIncludes>META-INF/**,**/acme-*.jar,**/acme-*.war</packagingIncludes>-->
- <!--<packagingExcludes></packagingExcludes>-->
- 
- <!--打包指定目录lib-->
- <defaultLibBundleDir>lib/</defaultLibBundleDir>
- <!--将多个war的共享包提取到父级别-->
- <skinnyWars>true</skinnyWars>
- 
- <!--<includeLibInApplicationXml>true</includeLibInApplicationXml>-->
- 
- <!--jboss 定制-->
- <!--<jboss>-->
- <!--<version>5</version>-->
- <!--<module-order>strict</module-order>-->
- <!--<unauthenticated-principal>guest</unauthenticated-principal>-->
- <!--<loader-repository>com.foo:loader=foo-application-1.0.ear</loader-repository>-->
- <!--</jboss>-->
- 
- <modules>
- 
- <!--jarModule：用于添加第三方库，配合includeInApplicationXml，可使其像一个开发的模块-->
- <jarModule>
- <!--<groupId>com.conquer.eden</groupId>-->
- <!--<artifactId>ejb3</artifactId>-->
- <!--<bundleFileName>ejb3.jar</bundleFileName>-->
- 
- <!--<includeInApplicationXml>true</includeInApplicationXml>-->
- 
- <!--<bundleDir>APP-INF/lib</bundleDir> 只更改位置，名称保持不变-->
- <!--<uri>APP-INF/lib/anotherName-1.2.3.jar</uri> 可定制位置和名称，需要指定名称-->
- <!--<excluded>true</excluded>-->
- 
- <!--<moduleId>ejb3-id</moduleId>-->
- </jarModule>
- <ejbModule>
- <groupId>com.conquer.eden</groupId>
- <artifactId>ejb3</artifactId>
- <bundleFileName>ejb3.jar</bundleFileName>
- </ejbModule>
- <webModule>
- <groupId>com.conquer.eden</groupId>
- <artifactId>webdemo</artifactId>
- <bundleFileName>webdemo.war</bundleFileName>
- 
- <!--<moduleId>webdemo-id</moduleId>-->
- </webModule>
- </modules>
- <!--<security>-->
- <!--<security-role id="SecurityRole_1234">-->
- <!--<role-name>manager</role-name>-->
- <!--</security-role>-->
- <!--<security-role id="SecurityRole_5678">-->
- <!--<description>My cool description</description>-->
- <!--<role-name id="RoleName_12">teller</role-name>-->
- <!--</security-role>-->
- <!--</security>-->
- 
- <!--<version>6</version>-->
- <!--<env-entries>-->
- <!--<env-entry>-->
- <!--<description>A complete entry.</description>-->
- <!--<env-entry-name>complete</env-entry-name>-->
- <!--<env-entry-type>java.lang.Integer</env-entry-type>-->
- <!--<env-entry-value>4</env-entry-value>-->
- <!--</env-entry>-->
- <!--<env-entry>-->
- <!--<env-entry-name>no-type</env-entry-name>-->
- <!--<env-entry-value>4</env-entry-value>-->
- <!--</env-entry>-->
- <!--<env-entry>-->
- <!--<env-entry-name>no-value</env-entry-name>-->
- <!--<env-entry-type>java.lang.String</env-entry-type>-->
- <!--</env-entry>-->
- <!--</env-entries>-->
- 
- </configuration>
- </plugin>
- </plugins>
- </build>
- </project>
以上pom要注意的是：
1.添加的两个依赖一个是ejb，一个是war包，指明的类型分别是：
<type>**ejb**</type>和<type>**war**</type>2.EAR的打包插件
**<artifactId>maven-ear-plugin</artifactId>**
3.添加的ejb和war需要分别指明：<ejbModule> 和 <webModule> 并且都可以定制打进ear的名称，通过：**<bundleFileName>**
**执行ear模块的package目标或执行ear模块maven的ear:ear可完成 ear的打包工作。**
关于 maven-ear-plugin ，官网参考：http://maven.apache.org/plugins/maven-ear-plugin/
