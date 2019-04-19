# Maven 核心原理 - 文章 - 伯乐在线
原文出处： [翡青（@翡青jf ）](http://blog.csdn.net/zjf280441589/article/details/53044308)
Maven 是每一位Java工程师每天都会接触的工具, 但据我所知其实很多人对Maven理解的并不深, 只把它当做一个依赖管理工具(下载依赖、打包), Maven很多核心的功能反而没用上. 最近重读 *Maven实战*, 虽然这本书年岁较老(10年出版: 那还是*Hudson*年代), 但绝大部分还是很值得参考的. 本文讲述Maven的核心原理和概念, 因此还是大纲参考了这本书, 但细节大多参考的Maven的官方文档以及网友释出的博客. 本文主要讲解Maven的:
- **坐标与依赖**、
- **仓库**、
- **生命周期与插件**、
- **模块聚合**、
- **模块继承**
等概念, 并通过一个**开发Maven插件**的实例来深入了解Maven的核心机制. 而对于 *如何配置Maven*、*Nexus私服*、*Jenkins持续集成*、*Maven测试*、*构建Web*、*资源过滤*、*自定义Archetype* 等相对简单、讲解繁琐且网上有大量实践案例的内容没有涉及. 本文的目标是希望读者能够通过本文能对Maven核心原理有个相对深入的理解.
## 坐标与依赖
> 
为了能够自动化地解析任何一个Java构件, Maven必须将它们唯一标识, 这就是**依赖管理**的底层基础-**坐标**.
### 坐标
> 
在数学中, 任何一个坐标可以唯一确定一个**“点”**.
Maven 坐标为Java构件引入了秩序, 任何一个构件都必须明确定义自己的坐标, 坐标元素包括`groupId`、`artifactId`、`version`、`packaging`、`classfier`:


```
<groupId>org.apache.commons</groupId>
<artifactId>commons-lang3</artifactId>
<version>3.4</version>
<packaging>jar</packaging>
```
|元素|描述|ext|
|----|----|----|
|groupId|定义当前模块隶属的**实际Maven项目**, 表示方式与Java包类似|**groupId**不应直接对应项目隶属的公司/组织(一个公司/组织下可能会有很多的项目).|
|artifactId|定义实际项目中的一个**Maven模块**|推荐使用项目名作为**artifactId**前缀, 如:**commons-lang3**以**commons**作为前缀(因为Maven打包默认以`artifactId`作为前缀)|
|version|定义当前项目所处版本(如*1.0-SNAPSHOT*、*4.2.7.RELEASE*、*1.2.15*、*14.0.1-h-3* 等)|Maven版本号定义约定: ..-|
|packaging|定义Maven项目打包方式, 通常**打包方式与所生成构件扩展名对应**|有*jar*(默认)、*war*、*pom*、*maven-plugin*等.|
|classifier|用来帮助定义构建输出的一些**附属构件**(如*javadoc*、*sources*)|不能直接定义项目的**classifier**(因为**附属构件不是由项目默认生成**, 须有附加插件的帮助)|
### 依赖
Maven最著名的(也是几乎每个人最先接触到的)就是Maven的依赖管理, 它使得我们不必再到开源项目的官网一个个下载开源组件, 然后再一个个放入**classpath**. 一个依赖声明可以包含如下元素:


```
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-web</artifactId>
    <version>4.2.7.RELEASE</version>
    <type>jar</type>
    <scope>compile</scope>
    <optional>false</optional>
    <exclusions>
        <exclusion>
            <groupId>log4j</groupId>
            <artifactId>log4j</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```
- 依赖传递
Maven依赖传递机制会自动加载我们引入的**依赖包的依赖**, 而不必去手动指定(好拗口(⊙﹏⊙)b : This allows you to avoid needing to discover and specify the libraries that your own dependencies require, and including them automatically).
如: 我们的项目依赖了spring-core, 而spring-core又依赖了commons-logging:
![](http://jbcdn2.b0.upaiyun.com/2016/11/d1b6ca7c28e6a1e8d2b00b67ce0eadac.png)
有了**依赖传递机制**, 在项目中添加了spring-core依赖时就不用再去考虑它依赖了什么, 也不用担心引入多余的依赖. Maven会解析各个直接依赖的POM, 将必要的间接依赖以**传递性依赖**的形式引入到当前目录中(inherits from its parents, or from its dependencies, and so on).
(依赖调节原则: 1. 路径最近者优先; 2. 第一声明者优先.)
更多传递依赖信息可参考: [Dependency Mechanism-Transitive Dependencies](https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html#Transitive_Dependencies).
- 声明一个或者多个项目依赖, 可以包含的元素有:
|元素|描述|Ext|
|----|----|----|
|groupId、artifactId和version|依赖的基本坐标(最最重要)||
|type|依赖的类型, 对应于项目坐标定义的**packaging**|默认**jar**|
|scope|依赖的范围, 用来控制依赖与三种**classpath**(*编译classpath*、*测试classpath*、*运行classpath*)的关系|包含*compile*、*provided*、*runtime*、*test*、*system*和*import* 6种, 详见:[Dependency Scope](https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html#Dependency_Scope).|
|optional|依赖是否可选|假如一个Jar包支持MySQL与Oracle两种DB, 因此其构建时必须添加两类驱动, 但用户使用时只会选择一种DB. 此时对A、B就可使用**optional**可选依赖|
|exclusions|排除传递性依赖|传递性依赖极大的简化了项目依赖的管理, 但也会引入**Jar包版本冲突**等问题, 此时就需要对传递性依赖进行排除. **optional**与**exclusions**详细可参考: [Optional Dependencies and Dependency Exclusions](https://maven.apache.org/guides/introduction/introduction-to-optional-and-excludes-dependencies.html)|
- 依赖管理
Maven提供了**dependency**插件可以对Maven项目依赖查看以及优化, 如`$ mvn dependency:tree`可以查看当前项目的依赖树, 详见`$ mvn dependency:help`.
## Maven 仓库
> 
Maven 中, 任何一个依赖、插件或项目构建的输出, 都可称为**构件**, 而**Maven仓库**就是集中存储这些构件的地方.
### 两类仓库
Maven仓库可简单分成两类: **本地仓库**与**远程仓库**. 当Maven根据坐标寻找构件时, 它会首先检索本地仓库, 如果本地存在则直接使用, 否则去远程仓库下载.
- 本地仓库: 默认地址为***~/.m2/***, 一个构件只有在本地仓库存在之后, 才能由Maven项目使用.
- 远程仓库: 远程仓库又可简单分成两类: **中央仓库**和**私服**.
- 由于原始的本地仓库是空的, Maven必须至少知道一个远程仓库才能在执行命令时下载需要的构件, 中央仓库就是这样一个**默认的远程仓库**.
> 
关于仓库的详细配置可参考: [Settings Reference](https://maven.apache.org/settings.html) 的 *Servers*、*Mirrors*、*Profiles* 等章节.
### 私服
![](http://jbcdn2.b0.upaiyun.com/2016/11/4e299b47fac0e12d8b3e5b047df9173d.png)
私服是一种特殊的远程仓库, 它设在局域网内, 通过代理广域网上的远程仓库, 供局域网内的Maven用户使用.
当需要下载构件时, Maven客户端先向私服请求, 如果私服不存在该构件, 则从外部的远程仓库下载, 并缓存在私服上, 再为客户提供下载服务. 此外, 一些无法从外部仓库下载到的构建也能从本地上传到私服供大家使用(如公司内部二方包、Oracle的JDBC启动等). 为了节省带宽和时间, 一般在公司内部都会架设一台Maven私服, 但将公司内部项目部署到私服还需要对POM做如下配置:


```
<project >
    ...
    <distributionManagement>
        <repository>
            <id>releases</id>
            <url>http://mvn.server.com/nexus/content/repositories/releases/</url>
        </repository>
        <snapshotRepository>
            <id>snapshots</id>
            <url>http://mvn.server.com/nexus/content/repositories/snapshots/</url>
        </snapshotRepository>
    </distributionManagement>
</project>
```
- `repository`表示**发布版本**构件的仓库, `snapshotRepository`代表**快照版本**的仓库.
- `id`为该远程仓库唯一标识, `url`表示该仓库地址.
配置正确后, 执行`$ mvn clean deploy`则可以将项目构建输出的构件部署到对应配置的远程仓库.
> 
注: 往远程仓库部署构件时, 往往需要认证: 需要在*settings.xml*中创建`servers`元素, 其id与仓库的id匹配, 详见: [Password Encryption](https://maven.apache.org/guides/mini/guide-encryption.html#How_to_encrypt_server_passwords).
### 仓库搜索
- 推荐几个可用的Maven仓库搜索服务:
- MVNrepository: [http://mvnrepository.com/](http://mvnrepository.com/)
- Sonatype Nexus: [https://repository.sonatype.org/](https://repository.sonatype.org/)
## 生命周期与插件
> 
Maven 将所有项目的构建过程统一抽象成一套**生命周期**: **项目的清理**、**初始化**、**编译**、**测试**、**打包**、**集成测试**、**验证**、**部署**和**站点生成** … 几乎所有项目的构建,都能映射到这一组生命周期上. 但生命周期是抽象的(Maven的生命周期本身是不做任何实际工作), **任务执行**(如编译源代码)**均交由插件完成**. 其中**每个构建步骤都可以绑定一个或多个插件的目标**,而且Maven为大多数构建步骤都编写并绑定了默认插件.当用户有特殊需要的时候, 也可以配置插件定制构建行为, 甚至自己编写插件.
![](http://jbcdn2.b0.upaiyun.com/2016/11/24dd232e86b7d16e9d78e148c5032345.png)
### 生命周期
Maven 拥有三套相互独立的生命周期: ***clean***、***default*** 和 ***site***, 而每个生命周期包含一些**phase**阶段, 阶段是有顺序的, 并且后面的阶段依赖于前面的阶段. 而三套生命周期相互之间却并没有前后依赖关系, 即调用**site**周期内的某个**phase**阶段并不会对**clean**产生任何影响.
- clean
clean生命周期的目的是清理项目:
![](http://jbcdn2.b0.upaiyun.com/2016/11/7ce3d4f3f6ac950f2b3a54562986d010.png)
执行如`$ mvn clean`;
- default
default生命周期定义了**真正构建**时所需要执行的所有步骤:
![](http://jbcdn2.b0.upaiyun.com/2016/11/264f347df0f6e433c842e12bb44f50e1.png)
执行如`$ mvn clean install`;
- site
site生命周期的目的是建立和发布项目站点: Maven能够基于POM所包含的信息,自动生成一个友好的站点,方便团队交流和发布项目信息
![](http://jbcdn2.b0.upaiyun.com/2016/11/072dc0bae3b601723475413c1096a3b6.png)
执行命令如`$ mvn clean deploy site-deploy`;
> 
更多 Maven 生命周期介绍可以参考: [Introduction to the Build Lifecycle](https://maven.apache.org/guides/introduction/introduction-to-the-lifecycle.html)、[Lifecycles Reference](https://maven.apache.org/ref/3.3.9/maven-core/lifecycles.html).
### 插件
生命周期的阶段**phase**与插件的目标**goal**相互绑定, 用以完成实际的构建任务. 而对于插件本身, 为了能够复用代码,它往往能够完成多个任务, 这些功能聚集在一个插件里,每个功能就是一个目标.
如:`$ mvn compiler:compile`: 冒号前是插件前缀, 后面是该插件目标(即: **maven-compiler-plugin**的**compile**目标).
而该目标绑定了***default***生命周期的**compile**阶段:
![](http://jbcdn2.b0.upaiyun.com/2016/11/5803d871a1a39767f2b826e29a4ddb28.png)
因此, 他们的绑定能够实现项目编译的目的.
#### 内置绑定
为了能让用户几乎不用任何配置就能使用Maven构建项目, Maven 默认为一些核心的生命周期绑定了插件目标, 当用户通过命令调用生命周期阶段时, 对应的插件目标就会执行相应的逻辑.
- clean生命周期阶段绑定
|生命周期阶段|插件目标|
|----|----|
|pre-clean|–|
|clean|maven-clean-plugin:clean|
|post-clean|–|
- default声明周期阶段绑定
|生命周期阶段|插件目标|执行任务|
|----|----|----|
|process-resources|maven-resources-plugin:resources|复制主资源文件到主输出目录|
|compile|maven-compiler-plugin:compile|编译主代码到主输出目录|
|process-test-resources|maven-resources-plugin:testResources|复制测试资源文件到测试输出目录|
|test-compile|maven-compiler-plugin:testCompile|编译测试代码到测试输出目录|
|test|maven-surefire-plugin:test|执行测试用例|
|package|maven-jar-plugin:jar|打jar包|
|install|maven-install-plugin:install|将项目输出安装到本地仓库|
|deploy|maven-deploy-plugin:deploy|将项目输出部署到远程仓库|
注: 上表只列出了打包方式为jar且拥有插件绑定关系的阶段, 其他打包类型生命周期的默认绑定关系可参考: [Built-in Lifecycle Bindings](http://maven.apache.org/guides/introduction/introduction-to-the-lifecycle.html#Built-in_Lifecycle_Bindings)、[Plugin Bindings for default Lifecycle Reference](https://maven.apache.org/ref/3.3.9/maven-core/default-bindings.html).
- site生命周期阶段绑定
|生命周期阶段|插件目标|
|----|----|
|pre-site|–|
|site|maven-site-plugin:site|
|post-site|–|
|site-deploy|maven-site-plugin:deploy|
#### 自定义绑定
除了内置绑定以外, 用户还能够自定义将某个插件目标绑定到生命周期的某个阶段上. 如创建项目的源码包, *maven-source-plugin*插件的*jar-no-fork*目标能够将项目的主代码打包成jar文件, 可以将其绑定到verify阶段上:


```
<build>
    <plugins>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-source-plugin</artifactId>
            <version>3.0.0</version>
            <executions>
                <execution>
                    <id>attach-sources</id>
                    <phase>verify</phase>
                    <goals>
                        <goal>jar-no-fork</goal>
                    </goals>
                </execution>
            </executions>
        </plugin>
    </plugins>
</build>
```
- `executions`下每个`execution`子元素可以用来配置执行一个任务.
> 
关于Maven插件的自定义绑定及其他详细配置可参考: [Guide to Configuring Plug-ins](https://maven.apache.org/guides/mini/guide-configuring-plugins.html).
#### 插件查询
- 在线
- Maven 官方插件
[https://maven.apache.org/plugins/index.html](https://maven.apache.org/plugins/index.html)
- CodeHaus 插件
[http://www.mojohaus.org/plugins.html](http://www.mojohaus.org/plugins.html)
- maven-help-plugin


```
mvn help:describe -Dplugin=org.apache.maven.plugins:maven-source-plugin[:3.0.0] [-Ddetail] [-Dgoal=jar-no-fork]
```
## 聚合与继承
> 
Maven的聚合特性(***aggregation***)能够**使项目的多个模块聚合在一起构建**, 而继承特性(***inheritance***)能够帮助抽取各模块相同的依赖、插件等配置,在简化模块配置的同时, 保持各模块一致.
### 模块聚合
随着项目越来越复杂(需要解决的问题越来越多、功能越来越重), 我们更倾向于**将一个项目划分几个模块并行开发**, 如: 将`feedcenter-push`项目划分为`client`、`core`和`web`三个模块, 而我们又想一次构建所有模块, 而不是针对各模块分别执行`$ mvn`命令. 于是就有了Maven的模块聚合 -> **将`feedcenter-push`作为聚合模块将其他模块聚集到一起构建**:
- 聚合POM
聚合模块POM仅仅是**帮助聚合其他模块构建的工具**, 本身并无实质内容:


```
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
         http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.vdian.feedcenter</groupId>
    <artifactId>feedcenter-push</artifactId>
    <packaging>pom</packaging>
    <version>1.0.0.SNAPSHOT</version>
    <modules>
        <module>feedcenter-push-client</module>
        <module>feedcenter-push-core</module>
        <module>feedcenter-push-web</module>
    </modules>
</project>
```
- 通过在一个打包方式为`pom`的Maven项目中声明任意数量的`module`以实现模块聚合:
- `packaging`: `pom`, 否则无法聚合构建.
- `modules`: 实现聚合的核心,`module`值为被聚合模块相对于聚合POM的**相对路径**, 每个被聚合模块下还各自包含有*pom.xml*、*src/main/java*、*src/test/java*等内容, 离开聚合POM也能够独立构建(注: 模块所处目录最好与其***artifactId***一致).
> 
Tips: 推荐将聚合POM放在项目目录的最顶层, 其他模块作为聚合模块的子目录.
其他关于聚合与**反应堆**介绍可参考: [Guide to Working with Multiple Modules](https://maven.apache.org/guides/mini/guide-multiple-modules.html).
### 模块继承
在面向对象中, 可以通过类继承实现复用. 在Maven中同样也可以创建***POM的父子结构***, 通过**在父POM中声明一些配置供子POM继承**来实现复用与消除重复:
#### 1. 父POM
与聚合类似, 父POM的打包方式也是`pom`, 因此可以继续复用聚合模块的POM(这也是在开发中常用的方式):


```
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
         http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.vdian.feedcenter</groupId>
    <artifactId>feedcenter-push</artifactId>
    <packaging>pom</packaging>
    <version>1.0.0.SNAPSHOT</version>
    <modules>
        <module>feedcenter-push-client</module>
        <module>feedcenter-push-core</module>
        <module>feedcenter-push-web</module>
    </modules>
    <properties>
        <finalName>feedcenter-push</finalName>
        <warName>${finalName}.war</warName>
        <spring.version>4.0.6.RELEASE</spring.version>
        <junit.version>4.12</junit.version>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <warExplodedDirectory>exploded/${warName}</warExplodedDirectory>
    </properties>
    <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-core</artifactId>
                <version>${spring.version}</version>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-beans</artifactId>
                <version>${spring.version}</version>
            </dependency>
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-context</artifactId>
                <version>${spring.version}</version>
            </dependency>
           <dependency>
                <groupId>junit</groupId>
                <artifactId>junit</artifactId>
                <version>${junit.version}</version>
                <scope>test</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>
    <build>
        <pluginManagement>
            <plugins>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-source-plugin</artifactId>
                    <version>3.0.0</version>
                    <executions>
                        <execution>
                            <id>attach-sources</id>
                            <phase>verify</phase>
                            <goals>
                                <goal>jar-no-fork</goal>
                            </goals>
                        </execution>
                    </executions>
                </plugin>
            </plugins>
        </pluginManagement>
    </build>
</project>
```
- `dependencyManagement`: 能让子POM继承父POM的配置的同时, 又能够保证子模块的灵活性: 在父POM`dependencyManagement`元素配置的依赖声明不会实际引入子模块中, 但能够约束子模块`dependencies`下的依赖的使用(子模块只需配置`groupId`与`artifactId`, 见下).
- `pluginManagement`: 与`dependencyManagement`类似, 配置的插件不会造成实际插件的调用行为, 只有当子POM中配置了相关`plugin`元素, 才会影响实际的插件行为.
#### 2. 子POM


```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
         http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <parent>
        <groupId>com.vdian.feedcenter</groupId>
        <artifactId>feedcenter-push</artifactId>
        <version>1.0.0.SNAPSHOT</version>
    </parent>
    <modelVersion>4.0.0</modelVersion>
    <artifactId>feedcenter-push-client</artifactId>
    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-source-plugin</artifactId>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>
```
- 元素继承
可以看到, 子POM中并未定义模块`groupId`与`version`, 这是因为子POM默认会从父POM继承了如下元素:- groupId、version
- dependencies
- developers and contributors
- plugin lists (including reports)
- plugin executions with matching ids
- plugin configuration
- resources
因此所有的***springframework***都省去了`version`、junit还省去了`scope`, 而且插件还省去了`executions`与`configuration`配置, 因为完整的声明已经包含在父POM中.
- 优势: 当依赖、插件的版本、配置等信息在父POM中声明之后, 子模块在使用时就无须声明这些信息, 也就不会出现多个子模块使用的依赖版本不一致的情况, 也就降低了依赖冲突的几率. 另外如果子模块不显式声明依赖与插件的使用, 即使已经在父POM的`dependencyManagement`、`pluginManagement`中配置了, 也不会产生实际的效果.
- 推荐: 模块继承与模块聚合同时进行,这意味着, 你可以为你的所有模块指定一个父工程, 同时父工程中可以指定其余的Maven模块作为它的聚合模块. 但需要遵循以下三条规则:
- 在所有子POM中指定它们的父POM;
- 将父POM的`packaging`值设为`pom`;
- 在父POM中指定子模块/子POM的目录.
> 
注: `parent`元素内还包含一个`relativePath`元素, 用于指定父POM的相对路径, 默认`../pom.xml`.
#### 超级pom-约定优先于配置
任何一个Maven项目都**隐式地继承自超级POM**, 因此**超级POM的大量配置都会被所有的Maven项目继承**, 这些配置也成为了Maven所提倡的约定。


```
<!-- START SNIPPET: superpom -->
<project>
  <modelVersion>4.0.0</modelVersion>
  <!-- 定义了中央仓库以及插件仓库, 均为:https://repo.maven.apache.org/maven2 -->
  <repositories>
    <repository>
      <id>central</id>
      <name>Central Repository</name>
      <url>https://repo.maven.apache.org/maven2</url>
      <layout>default</layout>
      <snapshots>
        <enabled>false</enabled>
      </snapshots>
    </repository>
  </repositories>
  <pluginRepositories>
    <pluginRepository>
      <id>central</id>
      <name>Central Repository</name>
      <url>https://repo.maven.apache.org/maven2</url>
      <layout>default</layout>
      <snapshots>
        <enabled>false</enabled>
      </snapshots>
      <releases>
        <updatePolicy>never</updatePolicy>
      </releases>
    </pluginRepository>
  </pluginRepositories>
  <!-- 依次定义了各类代码、资源、输出目录及最终构件名称格式, 这就是Maven项目结构的约定 -->
  <build>
    <directory>${project.basedir}/target</directory>
    <outputDirectory>${project.build.directory}/classes</outputDirectory>
    <finalName>${project.artifactId}-${project.version}</finalName>
    <testOutputDirectory>${project.build.directory}/test-classes</testOutputDirectory>
    <sourceDirectory>${project.basedir}/src/main/java</sourceDirectory>
    <scriptSourceDirectory>${project.basedir}/src/main/scripts</scriptSourceDirectory>
    <testSourceDirectory>${project.basedir}/src/test/java</testSourceDirectory>
    <resources>
      <resource>
        <directory>${project.basedir}/src/main/resources</directory>
      </resource>
    </resources>
    <testResources>
      <testResource>
        <directory>${project.basedir}/src/test/resources</directory>
      </testResource>
    </testResources>
    <!-- 为核心插件设定版本 -->
    <pluginManagement>
      <!-- NOTE: These plugins will be removed from future versions of the super POM -->
      <!-- They are kept for the moment as they are very unlikely to conflict with lifecycle mappings (MNG-4453) -->
      <plugins>
        <plugin>
          <artifactId>maven-antrun-plugin</artifactId>
          <version>1.3</version>
        </plugin>
        <plugin>
          <artifactId>maven-assembly-plugin</artifactId>
          <version>2.2-beta-5</version>
        </plugin>
        <plugin>
          <artifactId>maven-dependency-plugin</artifactId>
          <version>2.8</version>
        </plugin>
        <plugin>
          <artifactId>maven-release-plugin</artifactId>
          <version>2.3.2</version>
        </plugin>
      </plugins>
    </pluginManagement>
  </build>
  <!-- 定义项目报告输出路径 -->
  <reporting>
    <outputDirectory>${project.build.directory}/site</outputDirectory>
  </reporting>
  <!-- 定义release-profile, 为构件附上源码与文档 -->
  <profiles>
    <!-- NOTE: The release profile will be removed from future versions of the super POM -->
    <profile>
      <id>release-profile</id>
      <activation>
        <property>
          <name>performRelease</name>
          <value>true</value>
        </property>
      </activation>
      <build>
        <plugins>
          <plugin>
            <inherited>true</inherited>
            <artifactId>maven-source-plugin</artifactId>
            <executions>
              <execution>
                <id>attach-sources</id>
                <goals>
                  <goal>jar</goal>
                </goals>
              </execution>
            </executions>
          </plugin>
          <plugin>
            <inherited>true</inherited>
            <artifactId>maven-javadoc-plugin</artifactId>
            <executions>
              <execution>
                <id>attach-javadocs</id>
                <goals>
                  <goal>jar</goal>
                </goals>
              </execution>
            </executions>
          </plugin>
          <plugin>
            <inherited>true</inherited>
            <artifactId>maven-deploy-plugin</artifactId>
            <configuration>
              <updateReleaseInfo>true</updateReleaseInfo>
            </configuration>
          </plugin>
        </plugins>
      </build>
    </profile>
  </profiles>
</project>
<!-- END SNIPPET: superpom -->
```
> 
附: Maven继承与组合的其他信息还可参考: [Introduction to the POM](https://maven.apache.org/guides/introduction/introduction-to-the-pom.html).
## Maven Plugin 开发
> 
几乎100%的场景都不用我们自己开发Maven插件, 但理解插件开发可以使我们更加深入的理解Maven. 下面我们实际开发一个用于统计代码行数的插件 **lc-maven-plugin**.
### 1. 创建plugin项目


```
mvn archetype:generate 
    -DgroupId=com.fq.plugins -DartifactId=lc-maven-plugin -Dversion=0.0.1-SNAPSHOT
    -DarchetypeArtifactId=maven-archetype-plugin 
    -DinteractiveMode=false 
    -DarchetypeCatalog=internal
```
使用`maven-archetype-plugin`**Archetype**可以快速创建一个Maven插件项目(关于Maven Archetype可参考[What is an Archetype](https://maven.apache.org/guides/introduction/introduction-to-archetypes.html) 、[Creating Archetypes](https://maven.apache.org/guides/mini/guide-creating-archetypes.html)[注: 该文档介绍的是Archetype 1.x编写, 2.x内附链接]).
- pom.xml
插件本身也是Maven项目, 特殊之处在于`packaging`方式为`maven-plugin`:


```
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
         http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.fq.plugins</groupId>
    <artifactId>lc-maven-plugins</artifactId>
    <packaging>maven-plugin</packaging>
    <version>0.0.1-SNAPSHOT</version>
    <dependencies>
        <dependency>
            <groupId>com.google.guava</groupId>
            <artifactId>guava</artifactId>
            <version>19.0</version>
        </dependency>
        <dependency>
            <groupId>org.apache.maven</groupId>
            <artifactId>maven-plugin-api</artifactId>
            <version>3.3.3</version>
        </dependency>
        <dependency>
            <groupId>org.apache.maven.plugin-tools</groupId>
            <artifactId>maven-plugin-annotations</artifactId>
            <version>3.3</version>
        </dependency>
    </dependencies>
</project>
```
> 
`maven-plugin` 打包方式能控制Maven为其**在生命周期阶段绑定插件处理的相关目标**.
### 2. 编写目标Mojo
> 
**What is a Mojo?** A mojo is a **M** aven plain **O** ld **J** ava **O** bject. Each mojo is an executable *goal* in Maven, and a plugin is a distribution of one or more related mojos.


Java
```
/**
 * @author jifang
 * <a href='http://www.jobbole.com/members/chchxinxinjun'>@since</a> 16/10/9 上午11:33.
 */
@Mojo(name = "lc", defaultPhase = LifecyclePhase.VERIFY)
public class LCMavenMojo extends AbstractMojo {
    private static final List<String> DEFAULT_FILES = Arrays.asList("java", "xml", "properties");
    @Parameter(defaultValue = "${project.basedir}", readonly = true)
    private File baseDir;
    @Parameter(defaultValue = "${project.build.sourceDirectory}", readonly = true)
    private File srcDir;
    @Parameter(defaultValue = "${project.build.testSourceDirectory}", readonly = true)
    private File testSrcDir;
    @Parameter(defaultValue = "${project.build.resources}", readonly = true)
    private List<Resource> resources;
    @Parameter(defaultValue = "${project.build.testResources}", readonly = true)
    private List<Resource> testResources;
    @Parameter(property = "lc.file.includes")
    private Set<String> includes = new HashSet<>();
    private Log logger = getLog();
    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        if (includes.isEmpty()) {
            logger.debug("includes/lc.file.includes is empty!");
            includes.addAll(DEFAULT_FILES);
        }
        logger.info("includes: " + includes);
        try {
            long lines = 0;
            lines += countDir(srcDir);
            lines += countDir(testSrcDir);
            for (Resource resource : resources) {
                lines += countDir(new File(resource.getDirectory()));
            }
            for (Resource resource : testResources) {
                lines += countDir(new File(resource.getDirectory()));
            }
            logger.info("total lines: " + lines);
        } catch (IOException e) {
            logger.error("error: ", e);
            throw new MojoFailureException("execute failure: ", e);
        }
    }
    private LineProcessor<Long> lp = new LineProcessor<Long>() {
        private long line = 0;
        @Override
        public boolean processLine(String fileLine) throws IOException {
            if (!Strings.isNullOrEmpty(fileLine)) {
                ++this.line;
            }
            return true;
        }
        @Override
        public Long getResult() {
            long result = line;
            this.line = 0;
            return result;
        }
    };
    private long countDir(File directory) throws IOException {
        long lines = 0;
        if (directory.exists()) {
            Set<File> files = new HashSet<>();
            collectFiles(files, directory);
            for (File file : files) {
                lines += CharStreams.readLines(new FileReader(file), lp);
            }
            String path = directory.getAbsolutePath().substring(baseDir.getAbsolutePath().length());
            logger.info("path: " + path + ", file count: " + files.size() + ", total line: " + lines);
            logger.info("\t-> files: " + files.toString());
        }
        return lines;
    }
    private void collectFiles(Set<File> files, File file) {
        if (file.isFile()) {
            String fileName = file.getName();
            int index = fileName.lastIndexOf(".");
            if (index != -1 && includes.contains(fileName.substring(index + 1))) {
                files.add(file);
            }
        } else {
            File[] subFiles = file.listFiles();
            for (int i = 0; subFiles != null && i < subFiles.length; ++i) {
                collectFiles(files, subFiles[i]);
            }
        }
    }
}
```
- `@Parameter`: 配置点, 提供Mojo的可配置参数. 大部分Maven插件及其目标都是可配置的, 通过配置点, 用户可以自定义插件行为:


```
<plugin>
    <groupId>com.fq.plugins</groupId>
    <artifactId>lc-maven-plugins</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <executions>
        <execution>
            <id>lc</id>
            <phase>verify</phase>
            <goals>
                <goal>lc</goal>
            </goals>
            <configuration>
                <includes>
                    <include>java</include>
                    <include>lua</include>
                    <include>json</include>
                    <include>xml</include>
                    <include>properties</include>
                </includes>
            </configuration>
        </execution>
    </executions>
</plugin>
```
- `execute()`: 实际插件功能;
- 异常: `execute()`方法可以抛出以下两种异常:
- `MojoExecutionException`: Maven执行目标遇到该异常会显示 **BUILD FAILURE** 错误信息, 表示在运行期间发生了预期的错误;
- `MojoFailureException`: 表示运行期间遇到了**未预期的错误**, 显示 **BUILD ERROR** 信息.
### 3. 测试&执行
通过`mvn clean install`将插件安装到仓库后, 就可将其配置到实际Maven项目中, 用于统计项目代码了:


```
$ mvn com.fq.plugins:lc-maven-plugins:0.0.1-SNAPSHOT:lc [-Dlc.file.includes=js,lua] [-X]
```
> 
注: 其他关于**Maven Plugin开发**可参考:[Plugin Developers Centre](https://maven.apache.org/plugin-developers/index.html)、[Maven API Reference](https://maven.apache.org/developers/mojo-api-specification.html).
- 参考 & 扩展
[Maven实战](http://weibo.com/3319050953/Ec1WpFpcc?type=comment#_rnd1476162026812)[持续部署, 并不简单](http://coolshell.cn/articles/7657.html)[maven.apache.org](https://maven.apache.org/index.html)[Guide to creating assemblies](https://maven.apache.org/guides/mini/guide-assemblies.html)[Maven入门指南（一）](http://ifeve.com/maven-1/)[用 Maven 做项目构建](https://www.ibm.com/developerworks/cn/java/j-lo-maven/)[项目管理: Maven 让事情变得简单](https://www.ibm.com/developerworks/cn/java/j-maven/)[Maven开始逃离 XML 阵营](http://www.infoq.com/cn/news/2015/03/maven-polyglot)[Apache Maven JDeps插件3.0.0版本发布](http://www.infoq.com/cn/news/2015/11/maven-jdeps-plugin-released?utm_source=news_about_maven&utm_medium=link&utm_campaign=maven)[Maven实战（六）——Gradle，构建工具的未来？](http://www.infoq.com/cn/news/2011/04/xxb-maven-6-gradle)[Maven实战（七）——常用Maven插件介绍（上）](http://www.infoq.com/cn/news/2011/04/xxb-maven-7-plugin)[Maven实战（八）——常用Maven插件介绍（下）](http://www.infoq.com/cn/news/2011/05/xxb-maven-8-plugin)[Maven实战（九）——打包的技巧](http://www.infoq.com/cn/news/2011/06/xxb-maven-9-package)[Gradle 2.0发布：简单、快速、支持Java 8](http://www.infoq.com/cn/news/2014/07/gradle-2-0)
