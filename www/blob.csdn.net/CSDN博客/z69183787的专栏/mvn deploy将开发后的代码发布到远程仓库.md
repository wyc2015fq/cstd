# mvn deploy将开发后的代码发布到远程仓库 - z69183787的专栏 - CSDN博客
2018年07月03日 13:47:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：119
[https://blog.csdn.net/ChenQin091017/article/details/77755052](https://blog.csdn.net/ChenQin091017/article/details/77755052)
### 出现场景
在日常开发工作中，会将自己开发的项目发布到nexus，便于项目组中的其他人员能够`dependency`你开发的jar包，引用你的代码。
### 配置方法
- 在自己开发的项目的`pom.xml`文件中增加下面的配置
```xml
<!-- 发布位置配置 -->
    <distributionManagement>
        <repository>
            <id>releases</id>
            <name>Internal Release Repository</name>
            <url>http://172.16.88.47:8081/nexus/content/repositories/releases/
            </url>
        </repository>
        <snapshotRepository>
            <id>snapshots</id>
            <name>Internal Snapshot Repository</name>
            <url>http://172.16.88.47:8081/nexus/content/repositories/snapshots/
            </url>
        </snapshotRepository>
    </distributionManagement>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
其中`id`的值为你本地maven文件`apache-maven-3.3.9\conf`下的`setting.xml`中的`id`,如下代码所示
```xml
<servers>
    <!--使用nexus搭建的测试私服-->
        <server>  
          <id>releases</id>  
          <username>username**</username>  
          <password>password**</password>  
        </server>
        <server>
          <id>snapshots</id>
          <username>username**</username>
          <password>password**</password>
        </server>
  </servers>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 
- 最后只需要到你的项目文件下，优雅的执行`mvn deploy`即可 
> 
另外在使用maven进行编译，经常会有跳过测试进行编译的需求，只需要执行`mvn clean install -Dmaven.test.skip=true`
