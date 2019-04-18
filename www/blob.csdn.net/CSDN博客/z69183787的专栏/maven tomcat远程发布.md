# maven tomcat远程发布 - z69183787的专栏 - CSDN博客
2014年05月15日 14:56:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4347
maven tomcat远程发布
我表示这个插件在tomcat6下是可以运行的mvn tocmat:redeploy
```java
<build>
		<directory>target</directory><!-- 这是打包的文件的路径。默认是该工程的根目录。 -->
		<plugins><!-- tomcat 的插件 -->
			<plugin>
				<groupId>org.codehaus.mojo</groupId>
                <artifactId>tomcat-maven-plugin</artifactId>
				<configuration>
					<!-- <path>/mavenwebdemo</path> -->
					<url>http://192.168.2.102:8080/manager</url>
					<server>tomcat</server>
					<path>/SSHMJ-FRANK</path>
				</configuration>
			</plugin>
		</plugins>
	</build>
```
 很高兴经过几小时后的折腾 ， 可以将maven远程发布到tomcat7的过程写下来
```java
<url>http://mvnrepository.com/</url>
	<repositories>
		<repository>
			<id>people.apache.snapshots</id>
			<url>http://mvnrepository.com/</url>
			<releases>
				<enabled>false</enabled>
			</releases>
			<snapshots>
				<enabled>true</enabled>
			</snapshots>
		</repository>
	</repositories>
	
	<pluginRepositories>
		<pluginRepository>
			<id>apache.snapshots</id>
			<name>Apache Snapshots</name>
			<url>http://mvnrepository.com/</url>
			<releases>
				<enabled>false</enabled>
			</releases>
			<snapshots>
				<enabled>true</enabled>
			</snapshots>
		</pluginRepository>
	</pluginRepositories>
	
	<build>
		<plugins>
			<plugin>
				<groupId>org.apache.tomcat.maven</groupId>
				<artifactId>tomcat7-maven-plugin</artifactId>
				<version>2.1</version>  
				<configuration>
					<url>http://192.168.2.102:8080/manager/text</url>
					<server>tomcat</server>
					<path>/SSHMJ-FRANK</path>
					<username>qq</username>
    				<password>123456</password>
				</configuration>
			</plugin>
		</plugins>
	</build>
```
 看到红色的server没 ， 哪里其实就是部署的角色的配置
在maven的配置文件中setting.xml
```java
<server>
      <id>tomcat</id>
      <username>qq</username>
      <password>123456</password>
    </server>
```
 那么这个用户就是发布的用户了 ， 用户就要设置角色tomcat-user.xml
```java
<role rolename="manager"/>
	<role rolename="admin"/>
	<role rolename="manager-gui"/>
	<role rolename="manager-script"/>
	<role rolename="tomcat"/>
	<role rolename="role1"/>
	<user username="admin" password="123456" roles="admin,manager,manager-gui"/>
	<user username="qq" password="123456" roles="manager-script,tomcat,role1,admin,manager,manager-gui"/>
```
 看到没 ，管他什么事情，我以后就用qq用户了 ， 他什么都能干。
这样的我的文章的搜索率可能低点
我加两个异常[INFO]
tomcatManager status code:403, ReasonPhrase:Prohibido
```java
[INFO] tomcatManager status code:403, ReasonPhrase:Prohibido
```
[INFO]
tomcatManager status code:401, ReasonPhrase:Unauthorized
```java
[INFO] tomcatManager status code:401, ReasonPhrase:Unauthorized
```
 看到没 ， 反正就是你没有权限 ， 按照我说的 ， tomcat-user.xml配置用户最大权限
 ， 配置了还报错那是因为你tomcat服务没有重启的缘故
这样这下面的问题终于解决了
http://stackoverflow.com/questions/13336816/tomcat7-maven-plugin-tomcatmanager-status-code403-reasonphraseforbbiden
http://stackoverflow.com/questions/14023836/now-getting-401-unauthorized-in-jenkins-when-deploying-artifact-to-archiva-maven
