# 使用 MAVEN 进行不同环境的服务配置文件管理 - z69183787的专栏 - CSDN博客
2016年11月29日 20:36:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1395
在实际用中，我们的Java服务通常要部署在不同的环境中，并且每个环境有只适用于本环境的配置文件。 例如测试环境需要连接测试数据库，生产环境又会在不同的机房进行服务部署，需要连接本机房的数据库从库。 同时，每个环境的配置又有很多相同的部分，比如cache的超时时间等。本文介绍如何使用maven进行多个部署环境的配置文件管理。
对于多个运行环境的配置文件管理，比较原始的方法是为每一个部署环境创建一个配置文件：
```
resources
├── prod_beijing_config.properties
├── prod_shanghai_config.properties
├── dev_config.properties
└── config.properties
```
在上面的例子中我们配置了两个正式环境的配置文件，分别是北京机房和上海机房；配置了一个开发环境的配置文件，还有一个本地运行使用的配置文件。 在读取配置文件的时候，通过系统变量进行选择某一个配置文件或者在部署脚本中将不同机房的配置文件按照部署的环境不同进行上传。
这样做的问题有： 1. 实现复杂，需要在Java程序或者在部署脚本中进行处理 2. 各个部署环境公用的配置要维护多份。例如需要新增一个和部署环境无关的配置项，需要在所有的配置文件中依次添加，稍有不慎酒会造成部署失败。
下面介绍一种使用maven进行配置文件管理的方法，可以做到： 1. 加载配置文件的过程对应用程序透明，不管在哪个环境中都只需要加载 config.properties 2. 公共配置项在一个文件中维护
首先修改项目的config.properties，将需要根据运行环境不同取不同值的配置项目用变量代替，例如：
```
mysql.host = ${mysql.host}
mysql.port = ${mysql.port}
cache.size = 1000
```
然后在项目的根目录上创建一个 profiles 目录，存放所有运行环境的配置文件。
```
.
├── README.md
├── pom.xml
├── profiles
│   ├── beijing_config.properties
│   ├── shanghai_config.properties
│   └── dev_config.properties
├── src
│   ├── main
│   │   ├── java
│   │   │   └── com
```
注意这里每个机房的配置文件中只包含需要根据运行环境不同取不同值的配置项：
```
$ cat profiles/beijing_config.properties
mysql.host = beijing.mysql.domain
mysql.port = 4534
$ cat profiles/shanghai_config.properties
mysql.host = shanghai.mysql.domain
mysql.port = 4534
$ cat profiles/dev_config.properties
mysql.host = test.mysql.host
mysql.port = 4534
```
最关键的部分是pom.xml的配置：
```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>demo</groupId>
    <artifactId>demo</artifactId>
    <packaging>war</packaging>
    <version>1.0-SNAPSHOT</version>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
    </properties>
    ....
    <profiles>
        <profile>
            <id>dev</id>
            <activation>
                <activeByDefault>true</activeByDefault>
            </activation>
            <properties>
                <build.profile.id>dev</build.profile.id>
            </properties>
        </profile>
        <profile>
            <id>beijing</id>
            <properties>
                <build.profile.id>beijing</build.profile.id>
            </properties>
        </profile>
        <profile>
            <id>shanghai</id>
            <properties>
                <build.profile.id>shanghai</build.profile.id>
            </properties>
        </profile>
    </profiles>
    <build>
        ...
        <filters>
            <filter>profiles/${build.profile.id}_config.properties</filter>
        </filters>
        <resources>
            <resource>
                <directory>src/main/resources/</directory>
                <filtering>true</filtering>
            </resource>
        </resources>
        ...
    </build>
</project>
```
在这个pom文件中，我们首先定义了三个profile，dev profile 中将 build.profile.id 的值设置为 dev，beijing profile 中将 build.profile.id 的值设置为 beijing，shanghai profile 中将 build.profile.id 的值设置为 shanghai。 同时，这个配置中还指定了资源文件的位置是 src/main/resources/，
 在生成这些资源文件的时候使用 profiles/${build.profile.id}_config.properties 中定义的变量进行内容的替换。
在打包的时候，如果我们执行：`mvn package -D beijing` ， 这时首先会找到beijing的profile，将build.profile.id赋值为beijing， 这样定义变量的文件名就变成了 profiles/beijing_config.properties， 将config.properties 打入war包的时候会从
 profiles/beijing_config.properties 中读取 `${mysql.host}` 和 `${mysql.port}` 的值。最后war包中的 config.properties 就变成了：
```
mysql.host = beijing.mysql.domain
mysql.port = 4534
cache.size = 1000
```
当没有指定 -D 选项的时候，由于 dev 这个环境配置了 `<activeByDefault>true</activeByDefault>` ，所以会被默认启用。
