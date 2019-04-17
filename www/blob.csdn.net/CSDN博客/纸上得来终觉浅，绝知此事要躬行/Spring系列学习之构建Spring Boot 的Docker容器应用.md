# Spring系列学习之构建Spring Boot 的Docker容器应用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月21日 16:00:28[boonya](https://me.csdn.net/boonya)阅读数：64








英文原文：[https://spring.io/guides/gs/spring-boot-docker/](https://spring.io/guides/gs/spring-boot-docker/)

**目录**

[你要构建什么](#%E4%BD%A0%E8%A6%81%E6%9E%84%E5%BB%BA%E4%BB%80%E4%B9%88)

[你需要什么](#%E4%BD%A0%E9%9C%80%E8%A6%81%E4%BB%80%E4%B9%88)

[如何完成本指南](#%E5%A6%82%E4%BD%95%E5%AE%8C%E6%88%90%E6%9C%AC%E6%8C%87%E5%8D%97)

[Gradle构建](#Gradle%E6%9E%84%E5%BB%BA)

[Maven构建](#Maven%E6%9E%84%E5%BB%BA)

[IDE构建](#IDE%E6%9E%84%E5%BB%BA)

[设置一个Spring Boot应用程序](#%E8%AE%BE%E7%BD%AE%E4%B8%80%E4%B8%AASpring%20Boot%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F)

[应用容器化](#%E5%BA%94%E7%94%A8%E5%AE%B9%E5%99%A8%E5%8C%96)

[使用Maven构建Docker镜像](#%E4%BD%BF%E7%94%A8Maven%E6%9E%84%E5%BB%BADocker%E9%95%9C%E5%83%8F)

[使用Gradle构建Docker镜像](#%E4%BD%BF%E7%94%A8Gradle%E6%9E%84%E5%BB%BADocker%E9%95%9C%E5%83%8F)

[Push推送镜像之后](#Push%E6%8E%A8%E9%80%81%E9%95%9C%E5%83%8F%E4%B9%8B%E5%90%8E)

[使用Spring Profiles配置](#%E4%BD%BF%E7%94%A8Spring%20Profiles%E9%85%8D%E7%BD%AE)

[在Docker容器中调试应用程序](#%E5%9C%A8Docker%E5%AE%B9%E5%99%A8%E4%B8%AD%E8%B0%83%E8%AF%95%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F)

[总结](#%E6%80%BB%E7%BB%93)

[更多](#%E6%9B%B4%E5%A4%9A)

本指南将指导您构建运行Spring Boot应用程序的[Docker](https://docker.com/)镜像的过程。

## 你要构建什么


[Docker](https://docker.com/)是一个具有“社交”方面的Linux容器管理工具包，允许用户发布容器图像并使用其他人发布的容器图像。 Docker镜像是运行容器化过程的配方，在本指南中，我们将为简单的Spring启动应用程序构建一个。

## 你需要什么
-      大约15分钟
-      最喜欢的文本编辑器或IDE
-      JDK 1.8或更高版本
-      Gradle 4+或Maven 3.2+
-      您还可以将代码直接导入IDE：[Spring Tool Suite (STS)](https://spring.io/guides/gs/sts)[IntelliJ IDEA](https://spring.io/guides/gs/intellij-idea/)

如果您不使用Linux计算机，则需要虚拟化服务器。 通过安装VirtualBox，Mac的boot2docker等其他工具可以为您无缝管理。 访问[VirtualBox的下载站点](https://www.virtualbox.org/wiki/Downloads)，选择适合您机器的版本。 下载并安装。 不要担心实际运行它。


您还需要Docker，它只能在64位计算机上运行。 有关为计算机设置Docker的详细信息，请参阅[https://docs.docker.com/installation/#installation](https://docs.docker.com/installation/#installation)。 在继续之前，请验证您是否可以从shell运行docker命令。 如果您使用的是boot2docker，则需要先运行它。 



## 如何完成本指南



与大多数Spring[入门指南](https://spring.io/guides)一样，您可以从头开始并完成每个步骤，或者您可以绕过您已熟悉的基本设置步骤。 无论哪种方式，您最终都会使用工作代码。


要从头开始，请继续使用[Gradle构建](https://spring.io/guides/gs/spring-boot-docker/#scratch)。


要跳过基础知识，请执行以下操作： 
-      下载并解压缩本指南的源存储库，或使用Git克隆它：
`git clone https://github.com/spring-guides/gs-spring-boot-docker.git`-      cd到gs-spring-boot-docker/initial
-      跳转到[设置Spring Boot应用程序](https://spring.io/guides/gs/spring-boot-docker/#initial)。

 完成后，您可以根据gs-spring-boot-docker/complete中的代码检查结果。

### Gradle构建

首先，设置一个基本的构建脚本。 在使用Spring构建应用程序时，您可以使用任何您喜欢的构建系统，但此处包含了使用Gradle和Maven所需的代码。 如果您不熟悉这两者，请参阅使用Gradle构建Java项目或使用Maven构建Java项目。
**创建目录结构**

```
└── src
    └── main
        └── java
            └── hello
```

在您选择的项目目录中，创建以下子目录结构; 例如，在* nix系统上使用mkdir -p src / main / java / hello：

**创建Gradle构建文件**


下面是最初的Gradle构建文件。

*build.gradle*

```
buildscript {
    repositories {
        mavenCentral()
    }
    dependencies {
        classpath("org.springframework.boot:spring-boot-gradle-plugin:2.0.5.RELEASE")
    }
}

apply plugin: 'java'
apply plugin: 'eclipse'
apply plugin: 'idea'
apply plugin: 'org.springframework.boot'
apply plugin: 'io.spring.dependency-management'

bootJar {
    baseName = 'gs-spring-boot-docker'
    version =  '0.1.0'
}

repositories {
    mavenCentral()
}

sourceCompatibility = 1.8
targetCompatibility = 1.8

dependencies {
    compile("org.springframework.boot:spring-boot-starter-web")
    testCompile("org.springframework.boot:spring-boot-starter-test")
}
```

[Spring Boot gradle插件](https://docs.spring.io/spring-boot/docs/current/gradle-plugin/reference/html)提供了许多方便的功能：
-      它收集类路径上的所有jar并构建一个可运行的“über-jar”，这使得执行和传输服务更加方便。
-      它搜索public static void main（）方法以标记为可运行的类。
-      它提供了一个内置的依赖项解析器，它设置版本号以匹配[Spring Boot依赖项](https://github.com/spring-projects/spring-boot/blob/master/spring-boot-project/spring-boot-dependencies/pom.xml)。 您可以覆盖任何您希望的版本，但它将默认为Boot的所选版本集。 

### Maven构建

首先，设置一个基本的构建脚本。 在使用Spring构建应用程序时，您可以使用任何您喜欢的构建系统，但此处包含了使用Maven所需的代码。 如果您不熟悉Maven，请参阅使用Maven构建Java项目。
**创建目录结构**

```
└── src
    └── main
        └── java
            └── hello
```

在您选择的项目目录中，创建以下子目录结构; 例如，在* nix系统上使用mkdir -p src/main/java/hello：

*`pom.xml`*

```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>org.springframework</groupId>
    <artifactId>gs-spring-boot-docker</artifactId>
    <version>0.1.0</version>

    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.0.5.RELEASE</version>
    </parent>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>


    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
```

[Spring Boot Maven插件](https://docs.spring.io/spring-boot/docs/current/maven-plugin)提供了许多方便的功能：


-      它收集类路径上的所有jar并构建一个可运行的“über-jar”，这使得执行和传输服务更加方便。
-      它搜索public static void main（）方法以标记为可运行的类。
-      它提供了一个内置的依赖项解析器，它设置版本号以匹配[Spring Boot依赖项](https://github.com/spring-projects/spring-boot/blob/master/spring-boot-project/spring-boot-dependencies/pom.xml)。 您可以覆盖任何您希望的版本，但它将默认为Boot的所选版本集。

### IDE构建

阅读如何将本指南直接导入[Spring Tool Suite](https://spring.io/guides/gs/sts/)。


阅读[IntelliJ IDEA](https://spring.io/guides/gs/intellij-idea)中如何使用本指南。

## 设置一个Spring Boot应用程序

现在您可以创建一个简单的应用程序

*src/main/java/hello/Application.java*

```java
package hello;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
@RestController
public class Application {

    @RequestMapping("/")
    public String home() {
        return "Hello Docker World";
    }

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }

}
```

该类被标记为@SpringBootApplication和@RestController，这意味着Spring MVC可以使用它来处理Web请求。 @RequestMapping 映射 / 到 home（）方法，它只发送一个'Hello World'响应。 main（）方法使用Spring Boot的SpringApplication.run（）方法来启动应用程序。


现在我们可以在没有Docker容器的情况下运行应用程序（即在主机OS中）。


如果您使用的是Gradle，请执行：

`./gradlew build && java -jar build /libs/gs-spring-boot-docker-0.1.0.jar`
如果您使用的是Maven，请执行：

`./mvnw package && java -jar target/gs-spring-boot-docker-0.1.0.jar`
并转到[localhost:8080](http://localhost:8080)以查看您的“Hello Docker World”消息。

## 应用容器化

Docker有一个简单的[Dockerfile](https://docs.docker.com/reference/builder/)文件格式，用于指定图像的“图层”。 那么让我们继续在Spring Boot项目中创建一个Dockerfile：
*Dockerfile*

```
FROM openjdk:8-jdk-alpine
VOLUME /tmp
ARG JAR_FILE
COPY ${JAR_FILE} app.jar
ENTRYPOINT ["java","-Djava.security.egd=file:/dev/./urandom","-jar","/app.jar"]
```

这个Dockerfile非常简单，但是你需要运行一个没有多余装饰的Spring Boot应用程序：只需要Java和一个JAR文件。 项目JAR文件作为“app.jar”添加到容器中，然后在ENTRYPOINT中执行。

我们添加了一个指向“/tmp”的VOLUME（映射路径），因为这是Spring Boot应用程序默认为Tomcat创建工作目录的地方。 效果是在主机“/var/lib/docker”下创建一个临时文件，并将其链接到“/tmp”下的容器。 对于我们在此处编写的简单应用程序，此步骤是可选的，但如果需要在文件系统中实际编写，则对于其他Spring Boot应用程序可能是必需的。

为了减少Tomcat的启动时间，我们添加了一个指向“/dev/urandom”的系统属性作为源。 如果您使用Tomcat（或任何其他Web服务器）的“标准”版本，则对于更新版本的Spring Boot，这不是必需的。

**为了充分利用Spring Boot jar文件中依赖项和应用程序资源之间的清晰分离，我们将使用稍微不同的Dockerfile实现：**

*Dockerfile*

```
FROM openjdk:8-jdk-alpine
VOLUME /tmp
ARG DEPENDENCY=target/dependency
COPY ${DEPENDENCY}/BOOT-INF/lib /app/lib
COPY ${DEPENDENCY}/META-INF /app/META-INF
COPY ${DEPENDENCY}/BOOT-INF/classes /app
ENTRYPOINT ["java","-cp","app:app/lib/*","hello.Application"]
```

这个Dockerfile有一个DEPENDENCY参数，指向我们已经解压缩jar的目录。 如果我们做对了，它已经包含一个BOOT-INF/lib目录，其中包含依赖项jars，以及一个带有应用程序类的BOOT-INF / classes目录。 请注意，我们正在使用应用程序自己的主类hello.Application（这比使用jar启动器提供的间接更快）。

如果您使用的是boot2docker，则需要在使用Docker命令行或使用构建工具执行任何操作之前先运行它（它运行一个守护进程来处理虚拟机中的工作）。

要构建镜像，您可以从社区使用Maven或Gradle的一些工具（非常感谢[Palantir](https://github.com/palantir/gradle-docker)和[Spotify](https://github.com/spotify/dockerfile-maven)提供这些工具）。

### 使用Maven构建Docker镜像

在Maven pom.xml中，您应该添加一个这样的新插件（有关更多选项，请参阅插件文档）::

*pom.xml*

```
<properties>
   <docker.image.prefix>springio</docker.image.prefix>
</properties>
<build>
    <plugins>
        <plugin>
            <groupId>com.spotify</groupId>
            <artifactId>dockerfile-maven-plugin</artifactId>
            <version>1.4.9</version>
            <configuration>
                <repository>${docker.image.prefix}/${project.artifactId}</repository>
            </configuration>
        </plugin>
    </plugins>
</build>
```

配置指定了1个必需的东西：具有映像名称的存储库，最终将作为springio/gs-spring-boot-docker。


其他一些属性是可选的：
-      jar将要解压缩的目录的名称，将Maven配置公开为docker的构建参数。 可以使用插件配置的<buildArgs />指定它。
-      图像标记，如果未指定则最终为“最新”。 它可以使用<tag/>元素设置

在继续执行以下步骤（使用Docker的CLI工具）之前，请通过键入docker ps确保Docker正常运行。 如果收到错误消息，可能无法正确设置某些内容。 用Mac？ 将$（boot2docker shellinit 2> /dev/null）添加到.bash_profile（或类似的env-setting配置文件）的底部并刷新shell以确保配置了正确的环境变量。

为确保在创建docker镜像之前解压缩jar，我们为依赖项插件添加一些配置：
*pom.xml*

```
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-dependency-plugin</artifactId>
    <executions>
        <execution>
            <id>unpack</id>
            <phase>package</phase>
            <goals>
                <goal>unpack</goal>
            </goals>
            <configuration>
                <artifactItems>
                    <artifactItem>
                        <groupId>${project.groupId}</groupId>
                        <artifactId>${project.artifactId}</artifactId>
                        <version>${project.version}</version>
                    </artifactItem>
                </artifactItems>
            </configuration>
        </execution>
    </executions>
</plugin>
```

您可以使用命令行构建标记的docker镜像，如下所示：
`$ ./mvnw install dockerfile:build`
你可以用./mvnw dockerfile:push将图像推送到dockerhub。

您不必将新创建的Docker镜像推送到实际运行它。 此外，如果您不是Dockerhub上“springio”组织的成员，“push”命令将失败。 将构建配置和命令行更改为您自己的用户名而不是“springio”，以使其实际工作。

您可以通过将其添加到插件配置来使dockerfile:push在安装或部署生命周期阶段自动运行。

*pom.xml*

```
<executions>
	<execution>
		<id>default</id>
		<phase>install</phase>
		<goals>
			<goal>build</goal>
			<goal>push</goal>
		</goals>
	</execution>
</executions>
```

### 使用Gradle构建Docker镜像

如果您使用的是Gradle，则需要添加如下新插件：
*build.gradle*

```
buildscript {
    ...
    dependencies {
        ...
        classpath('gradle.plugin.com.palantir.gradle.docker:gradle-docker:0.13.0')
    }
}

group = 'springio'

...
apply plugin: 'com.palantir.docker'

task unpack(type: Copy) {
    dependsOn bootJar
    from(zipTree(tasks.bootJar.outputs.files.singleFile))
    into("build/dependency")
}
docker {
    name "${project.group}/${bootJar.baseName}"
    copySpec.from(tasks.unpack.outputs).into("dependency")
    buildArgs(['DEPENDENCY': "dependency"])
}
```

配置指定了4件事：
-      解压jar文件的任务
-      图像名称（或标记）是从jar文件属性设置的，最终将作为springio / gs-spring-boot-docker
-      解压缩的jar文件的位置，我们可以在Dockerfile中对其进行硬编码
-      docker指向jar文件的构建参数

您可以构建标记的docker镜像，然后使用Gradle将其推送到远程存储库：
`$ ./gradlew build docker`


### Push推送镜像之后



“docker push”将失败（除非您是Dockerhub中“springio”组织的一部分），但是如果您更改配置以匹配您自己的docker ID，那么它应该会成功，并且您将有一个新的标记，部署 图片。

您不必注册docker或发布任何东西来运行docker镜像。 您仍然拥有本地标记的镜像，您可以像这样运行它：

```
$ docker run -p 8080:8080 -t springio/gs-spring-boot-docker
....
2015-03-31 13:25:48.035  INFO 1 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8080 (http)
2015-03-31 13:25:48.037  INFO 1 --- [           main] hello.Application                        : Started Application in 5.613 seconds (JVM running for 7.293)
```

然后可以在[http://localhost:8080](http://localhost:8080)上访问该应用程序（访问它并显示“Hello Docker World”）。 要确保整个过程真正有效，请将前缀从“springio”更改为其他内容（例如$ {env.USER}），然后再从构建到docker运行再次查看。

Mac与boot2docker一起使用时，通常会在启动时看到类似的内容：

```
Docker client to the Docker daemon, please set:
    export DOCKER_CERT_PATH=/Users/gturnquist/.boot2docker/certs/boot2docker-vm
    export DOCKER_TLS_VERIFY=1
    export DOCKER_HOST=tcp://192.168.59.103:2376
```

要查看应用程序，您必须访问DOCKER_HOST中的IP地址而不是localhost。 在这种情况下，[http://192.168.59.103:8080](http://192.168.59.103:8080)，VM的面向公众的IP。

当它运行时，您可以在容器列表中看到，例如：

```
$ docker ps
CONTAINER ID        IMAGE                                   COMMAND                  CREATED             STATUS              PORTS                    NAMES
81c723d22865        springio/gs-spring-boot-docker:latest   "java -Djava.secur..."   34 seconds ago      Up 33 seconds       0.0.0.0:8080->8080/tcp   goofy_brown
```

并再次关闭它你可以停靠与上面列表中的容器ID停止 docker stop ${container_id}（你的将是不同的）：

```
$ docker stop 81c723d22865
81c723d22865
```

如果您愿意，还可以在完成后删除容器（它在/var/lib/docker下的文件系统中保存）：
`$ docker rm 81c723d22865`
### 使用Spring Profiles配置

使用Spring配置文件运行刚刚创建的Docker镜像就像将环境变量传递给Docker run命令一样简单
`$ docker run -e "SPRING_PROFILES_ACTIVE=prod" -p 8080:8080 -t springio/gs-spring-boot-docker`
或
`$ docker run -e "SPRING_PROFILES_ACTIVE=dev" -p 8080:8080 -t springio/gs-spring-boot-docker`


### 在Docker容器中调试应用程序

要调试应用程序，可以使用[JPDA Transport](https://docs.oracle.com/javase/8/docs/technotes/guides/jpda/conninv.html#Invocation)。 所以我们将容器视为远程服务器。 要启用此功能，请在JAVA_OPTS变量中传递Java代理设置，并在容器运行期间将代理程序的端口映射到localhost。 使用[Docker for Mac](https://www.docker.com/products/docker#/mac)存在限制，因为我们无法通过IP访问容器而不[使用黑魔法](https://github.com/docker/for-mac/issues/171)。
`$ docker run -e "JAVA_OPTS=-agentlib:jdwp=transport=dt_socket,address=5005,server=y,suspend=n" -p 8080:8080 -p 5005:5005 -t springio/gs-spring-boot-docker`
## 总结

恭喜！ 您刚刚为Spring Boot应用程序创建了一个Docker容器！ 默认情况下，Spring Boot应用程序在容器内的端口8080上运行，我们使用命令行上的“-p”将其映射到主机上的同一端口。

## 更多

以下指南也可能有所帮助：
- 
[Serving Web Content with Spring MVC](https://spring.io/guides/gs/serving-web-content/)

- 
[Building an Application with Spring Boot](https://spring.io/guides/gs/spring-boot/)


想要撰写新指南或为现有指南做出贡献？ 查看我们的[贡献指南](https://github.com/spring-guides/getting-started-guides/wiki)。

所有指南均附有代码的ASLv2许可证，以及[Attribution，NoDerivatives](https://creativecommons.org/licenses/by-nd/3.0/)创作公共许可证。



