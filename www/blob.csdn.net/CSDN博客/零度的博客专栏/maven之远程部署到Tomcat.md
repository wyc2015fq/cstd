# maven之远程部署到Tomcat - 零度的博客专栏 - CSDN博客
2016年07月14日 16:36:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：971
1、设置Tomcat用户角色，使该用户支持页面部署应用，修改tomcat-user.xml
```
<role rolename="manager"/>
  <role rolename="admin"/>
  <role rolename="tomcat"/>
  <role rolename="manager-gui"/>
  <role rolename="manager-script"/>
  <user username="admin" password="admin" roles="manager,admin,manager-gui,manager-script"/>
  <user username="test" password="test" roles="tomcat"/>
```
2、在maven的配置文件setting.xml中增加server
```
<server>
  <id>mytomcat7</id>
  <username>admin</username>
  <password>admin</password>
</server>
```
3、pom.xml
tomcat7:
```
<build>  
        <plugins>  
            <plugin>  
                <groupId>org.apache.tomcat.maven</groupId>  
                <artifactId>tomcat7-maven-plugin</artifactId>  
                <version>2.1</version>    
                <configuration>  
                    <url>http://192.168.14.19:8080/manager/text</url>  
                    <server>mytomcat7</server>  
                    <path>/SSM</path>  
                    <username>admin</username>  
                    <password>admin</password>  
                </configuration>  
            </plugin>  
        </plugins>  
</build>
```
另附tomcat6：
```
<build>  
        <directory>target</directory><!-- 这是打包的文件的路径。默认是该工程的根目录。 -->  
        <plugins><!-- tomcat 的插件 -->  
            <plugin>  
                <groupId>org.codehaus.mojo</groupId>  
                <artifactId>tomcat-maven-plugin</artifactId>  
                <configuration>                       
                    <url>http://192.168.14.19:8080/manager</url>  
                    <server>mytomcat6</server>  
                    <path>/SSM</path>  
                </configuration>  
            </plugin>  
        </plugins>  
</build>
```
