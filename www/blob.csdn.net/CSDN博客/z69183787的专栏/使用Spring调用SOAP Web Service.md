# 使用Spring调用SOAP Web Service - z69183787的专栏 - CSDN博客
2016年12月06日 11:12:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5662
调用SOAP web service
本指南将指导你使用Spring调用一个基于SOAP的web service的整个过程。
指南内容
你将构建一个客户端，使用SOAP用来从远端的基于WSDL的web service获取天气数据。请访问http://wiki.cdyne.com/index.php/CDYNE_Weather进一步获取该天气服务的信息。
该服务根据邮编返回天气预测。你可以使用自己的邮编。
准备事项
大约15分钟
钟爱的编辑器或IDE
JDK1.6或更高版本
Gradle 1.11+ 或 Maven 3.0+
你也可以直接参阅该指南导入代码，或通过Spring工具集（Spring Tool Suite，简称STS)通过网页浏览代码，从而帮助你学习该章节内容。[源码](http://www.2cto.com/ym)下载地址:https://github.com/spring-guides/gs-consuming-web-service.git。
如何完成该指南
如同大多数的示例教程一样，你可以从头开始并完成每个步骤，或者你也可以跳过已经熟悉的基础章节。无论怎样，最终你要得到可以工作的代码。
想从头开始，请移动到使用Gradle构建章节。
想跳过基础部分，请做以下事情：
下载并解压该向导的源代码，或者使用Git复制一份： git clone https://github.com/spring-guides/gs-consuming-web-service.git
切换到gs-consuming-web-service/initial
跳到基于WSDL生成领域对象章节。
当完成后，你可以使用gs-consuming-web-service/complete目录中的代码检查你的结果。
使用Gradle构建
首先你要设置一个基本的build脚本。当构建Spring应用程序时，你可以使用任何构建[系统](http://www.2cto.com/os/)，但是这里只包括了使用Maven和Gradle的代码。如果你两者都不熟悉，请访问使用Gradle构建[Java](http://www.2cto.com/kf/ware/Java/)项目或使用Maven构建Java项目。
创建目录结构
在你选择的存放项目的目录中，创建如下的子目录结构。例如，在*nix系统中使用mkdir -p src/main/java/hello。
1
2
3
4
└── src
    └── main
        └── java
            └── hello
创建Gradle 构建文件
下面是一个初始的Gradle build文件。
build.gradle
configurations {
    jaxb
}
buildscript {
    repositories {
        maven { url "http://repo.spring.io/libs-release" }
        mavenLocal()
        mavenCentral()
    }
    dependencies {
        classpath("org.springframework.boot:spring-boot-gradle-plugin:1.1.6.RELEASE")
    }
}
apply plugin: 'java'
apply plugin: 'eclipse'
apply plugin: 'idea'
apply plugin: 'spring-boot'
repositories {
    mavenLocal()
    mavenCentral()
    maven { url 'http://repo.spring.io/libs-release' }
}
// tag::wsdl[]
task genJaxb {
    ext.sourcesDir = "${buildDir}/generated-sources/jaxb"
    ext.classesDir = "${buildDir}/classes/jaxb"
    ext.schema = "http://wsf.cdyne.com/WeatherWS/Weather.asmx?wsdl"
    outputs.dir classesDir
    doLast() {
        project.ant {
            taskdef name: "xjc", classname: "com.sun.tools.xjc.XJCTask",
                    classpath: configurations.jaxb.asPath
            mkdir(dir: sourcesDir)
            mkdir(dir: classesDir)
            xjc(destdir: sourcesDir, schema: schema,
                    package: "hello.wsdl") {
                arg(value: "-wsdl")
                produces(dir: sourcesDir, includes: "**/*.java")
            }
            javac(destdir: classesDir, source: 1.6, target: 1.6, debug: true,
                    debugLevel: "lines,vars,source",
                    classpath: configurations.jaxb.asPath) {
                src(path: sourcesDir)
                include(name: "**/*.java")
                include(name: "*.java")
            }
            copy(todir: classesDir) {
                fileset(dir: sourcesDir, erroronmissingdir: false) {
                    exclude(name: "**/*.java")
                }
            }
        }
    }
}
// end::wsdl[]
dependencies {
    compile("org.springframework.boot:spring-boot-starter")
    compile("org.springframework.ws:spring-ws-core")
    compile(files(genJaxb.classesDir).builtBy(genJaxb))
    jaxb "com.sun.xml.bind:jaxb-xjc:2.1.7"
}
jar {
    from genJaxb.classesDir
}
task wrapper(type: Wrapper) {
    gradleVersion = '1.11'
}
task afterEclipseImport {
    dependsOn genJaxb
}
Spring Boot gradle插件提供了很多便利的特性：
将classpath中的所有jar包构建单个可运行的jar包，从而更容易执行和传播服务。
搜索public static void main（）方法并标记为可运行的类。
提供了一个内置的依赖管理器，设置依赖版本以匹配Spring Boot依赖。你可以覆盖为任何你希望的版本，但默认会使用Boot选择的版本。
使用Maven构建
首先你需要设置一个基本的构建脚本。你可以使用任何构建系统来构建Spring应用程序，但这里包含了Maven的代码。如果你对Maven不熟，请访问使用Maven构建Java项目。
创建目录结构
在你选择的存放项目的目录中，创建如下的子目录结构。例如，在*nix系统中使用mkdir -p src/main/java/hello。
1
2
3
4
└── src
    └── main
        └── java
            └── hello
pom.xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>org.springframework</groupId>
    <artifactId>gs-consuming-web-service</artifactId>
    <version>0.1.0</version>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>1.1.6.RELEASE</version>
    </parent>
    <properties>
        <!-- use UTF-8 for everything -->
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
    </properties>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.ws</groupId>
            <artifactId>spring-ws-core</artifactId>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>2.3.2</version>
            </plugin>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
            <!-- tag::wsdl[] -->
            <plugin>
                <groupId>org.jvnet.jaxb2.maven2</groupId>
                <artifactId>maven-jaxb2-plugin</artifactId>
                <executions>
                    <execution>
                        <goals>
                            <goal>generate</goal>
                        </goals>
                    </execution>
                </executions>
                <configuration>
                    <schemaLanguage>WSDL</schemaLanguage>
                    <generatePackage>hello.wsdl</generatePackage>
                    <forceRegenerate>true</forceRegenerate>
                    <schemas>
                        <schema>
                            <url>http://wsf.cdyne.com/WeatherWS/Weather.asmx?wsdl</url>
                        </schema>
                    </schemas>
                </configuration>
            </plugin>
            <!-- end::wsdl[] -->
        </plugins>
    </build>
    <repositories>
        <repository>
            <id>spring-releases</id>
            <name>Spring Releases</name>
            <url>http://repo.spring.io/libs-release</url>
        </repository>
    </repositories>
    <pluginRepositories>
        <pluginRepository>
            <id>spring-releases</id>
            <url>http://repo.spring.io/libs-release</url>
        </pluginRepository>
    </pluginRepositories>
</project>
注意：你可能注意到我们指定了maven-complier-plugin的版本。通常并不推荐这样做。这里主要是为了解决我们的CI系统默认运行在该插件的早期版本（java5之前）的一个问题。
Spring Boot Maven插件提供了很多便利的特性：
将classpath中的所有jar包构建单个可运行的jar包，从而更容易执行和传播服务。
搜索public static void main（）方法并标记为可运行的类。
提供了一个内置的依赖管理器，设置依赖版本以匹配Spring Boot依赖。你可以覆盖为任何你希望的版本，但默认会使用Boot选择的版本。





