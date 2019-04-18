# maven 配置详解 - z69183787的专栏 - CSDN博客
2016年05月17日 14:06:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：942
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)
MAVEN2的配置文件有两个settings.xml和pom.xml
settings.xml：保存的是本地所有项目所共享的全局配置信息，默认在maven安装目录的conf目录下，如果没有安装maven而是用的m2eclipse插件，那么也可以把这个配置文件放在.m2目录下
pom.xml：描述项目的配置文件，放在每个项目的根目录下
**settings.xml**
localRepository：本地仓库位置，默认在.m2/repository/，可以人为更改
offline：离线开关，是否每次构建都从远程仓库下载，默认false
servers：对应POM文件的distributionManagement元素里定义id,和登陆服务器的用户名、密码
mirrors：定义仓库镜像，将仓库地址指向自定义仓库地址(id：新的镜像ID，name：镜像名称，url：镜像地址，mirrorOf：以那个地址做镜像，默认为central)
proxies：设置HTTP代理
parent     给出父项目的位置，如果存在上一级父项目的话。如果没有特别指出的话，值就是父项目对于当前项目而言。位置是一个 as a group ID, artifact ID 和version元素的组合。
modelVersion     描述这个POM文件遵从哪个版本的项目描述符.
groupId     针对一个项目的普遍唯一识别符。通常用一个完全正确的包的名字来与其他项目的类似名字来进行区分（比如：org.apache.maven)
artifactId     在给定groupID 的group里面为artifact 指定的标识符是唯一的 artifact 代表的是被制作或者被一个project应用的组件。对于Maven项目的artifact 的例子包括：一些JARs, 原代码以及二进制的发布以及WARs. 
packaging     这个项目生产出来的artifact 类型，举个例子 jar war  pom Plugins 能够创建他们自己的包，包括包的类型，所以这个列表不可能包含所有可能的类型 
name     当前项目的全称
version     当前项目产生的artifact的当前版本
description     当前项目的一个细节描述，当需要描述这个项目的时候被Maven所用，比如在web 站点中。 这个元素能够被指定为CDATA 类型，允许在描述器中HTML的tags, 并不鼓励用空白文本来表示。 如果你需要去修改生成的web 站点的索引页，你能够用你自己的索引来代替自动生成的文本。 
url     当前项目的主页的URL 
prerequisites     描述当前项目的编译环境的先决条件 
issueManagement     当前项目的发布管理信息。
ciManagement     当前项目的连续集成信息。
inceptionYear     当前项目开始的年份, 用4位数字描述. 涉及到介绍情况时用作提供版权信息 
mailingLists     包含的信息包括邮件列表
developers     描述当前的项目的开发人员的信息
contributors     描述对当前项目有贡献的人员的信息，不特指开发人员
licenses     这个元素描述了当前项目的所有的许可文件。每一个许可文件用一个许可元素来描述，然后描述额外的元素. 通常只列出适用于这个项目的许可文件以及适用于 依赖的非licenses。如果多个licenses都列出来了，那么假设这个用户选择其中的所需的，而不是接受所有的许可文件。 
scm     指定当前项目中的版本控制工具，比如CVS, Subversion, 等等。
organization     这个元素描述这个项目所属组织的各种属性的描述。这些属性应用于文档创建的时候 (版权通知和链接). 
build     创建项目时必须的信息。
profiles     本地项目编译档案文件时的列表，被激活时会修改build的过程 
modules     模块 (有时被叫做子项目)作为当前项目的一部分.每一个被列出来的子模块都指向包含这个模块的目录文件的相对路径 
repositories     发现依赖和扩展的远程资源库
pluginRepositories     发现plugins 的远程资源库的列表，主要是为了编译和报告
dependencies     这个元素描述了所有与当前项目相关的依赖.这些依赖被用作创建一个编译时的路径. 他们被自动的从资源库中下在下来依据当前项目的定义。如需更多信息，参看 the dependency mechanism 
reports     Deprecated.禁止适用。现在的版本中被 Maven所忽略掉。
reporting     这个元素包括报告的plugins 的指定，用作Maven生成站点的自动生成报告.这些报告将会运行当用户执行mvn site. 所有的报告将会包括在浏览器的导航栏中。 
dependencyManagement     缺省的依赖信息将会从这个元素中继承。这些依赖在这一部分中被不立刻被解决的。当一个源于这个POM的元素描述了一个依赖根据匹配的 groupId 和artifactId,这个部分的版本和其他值用作那些还没有指定的依赖。
distributionManagement     对于一个项目分配的信息允许对于远程web服务器和资源库的site和artifacts配置。
--------------------------------------------------------------------------------------------------------
什么是pom?
    pom作为项目对象模型。通过xml表示maven项目，使用pom.xml来实现。主要描述了项目：包括配置文件；开发者需要遵循的规则，缺陷管理系统，组织和licenses，项目的url，项目的依赖性，以及其他所有的项目相关因素。
快速察看：
xml 代码
<project>
<modelVersion>4.0.0modelVersion>
<groupId>...<groupId>
<artifactId>...<artifactId>
<version>...<version>
<packaging>...<packaging>
<dependencies>...<dependencies>
<parent>...<parent>
<dependencyManagement>...<dependencyManagement>
<modules>...<modules>
<properties>...<properties>
<build>...<build>
<reporting>...<reporting>
<name>...<name>
<description>...<description>
<url>...<url>
<inceptionYear>...<inceptionYear>
<licenses>...<licenses>
<organization>...<organization>
<developers>...<developers>
<contributors>...<contributors>
<issueManagement>...<issueManagement>
<ciManagement>...<ciManagement>
<mailingLists>...<mailingLists>
<scm>...<scm>
<prerequisites>...<prerequisites>
<repositories>...<repositories>
<pluginRepositories>...<pluginRepositories>
<distributionManagement>...<distributionManagement>
<profiles>...<profiles>
<project>
基本内容：
POM包括了所有的项目信息。
maven 相关：
pom定义了最小的maven2元素，允许groupId,artifactId,version。所有需要的元素
groupId:项目或者组织的唯一标志，并且配置时生成的路径也是由此生成，如org.codehaus.mojo生成的相对路径为：/org/codehaus/mojo
artifactId: 项目的通用名称
version:项目的版本
packaging: 打包的机制，如pom, jar, maven-plugin, ejb, war, ear, rar, par
classifier: 分类
**POM关系**：
主要为依赖，继承，合成
依赖关系：
xml 代码
<dependencies>
    <dependency>
      <groupId>junit<groupId>
      <artifactId>junit<artifactId>
      <version>4.0<version>
      <type>ja<rtype>
      <scope>test<scope>
      <optional>true<optional>
    <dependency>
    ...
<dependencies>
groupId, artifactId, version:描述了依赖的项目唯一标志
可以通过以下方式进行安装：
使用以下的命令安装：
mvn install:install-file –Dfile=non-maven-proj.jar –DgroupId=some.group –DartifactId=non-maven-proj –Dversion=1
创建自己的库,并配置，使用deploy:deploy-file
设置此依赖范围为system，定义一个系统路径。不提倡。
type:相应的依赖产品包形式，如jar，war
scope:用于限制相应的依赖范围，包括以下的几种变量：
compile ：默认范围，用于编译
provided：类似于编译，但支持你期待jdk或者容器提供，类似于classpath
runtime:在执行时，需要使用
test:用于test任务时使用
system:需要外在提供相应得元素。通过systemPath来取得
systemPath: 仅用于范围为system。提供相应的路径
optional: 标注可选，当项目自身也是依赖时。用于连续依赖时使用
**独占性**外在告诉maven你只包括指定的项目，不包括相关的依赖。此因素主要用于解决版本冲突问题
xml 代码
<dependencies>
    <dependency>
      <groupId>org.apache.maven<groupId>
      <artifactId>maven-embedder<artifactId>
      <version>2.0<version>
      <exclusions>
        <exclusion>
          <groupId>org.apache.maven<groupId>
          <artifactId>maven-core<artifactId>
        <exclusion>
      <exclusions>
    <dependency>
<dependencies>
表示项目maven-embedder需要项目maven-core，但我们不想引用maven-core
**继承关系**
另一个强大的变化,maven带来的是项目继承。主要的设置：
定义父项目
xml 代码
<project>
<modelVersion>4.0.0<modelVersion>
<groupId>org.codehaus.mojo<groupId>
<artifactId>my-parent<artifactId>
<version>2.0version>
<packaging>pom<packaging>
<project>
packaging 类型，需要pom用于parent和合成多个项目。我们需要增加相应的值给父pom，用于子项目继承。主要的元素如下：
依赖型
开发者和合作者
插件列表
报表列表
插件执行使用相应的匹配ids
插件配置
子项目配置
xml 代码
<project>
<modelVersion>4.0.0<modelVersion>
<parent>
    <groupId>org.codehaus.mojo<groupId>
    <artifactId>my-parent<artifactId>
    <version>2.0<version>
    <relativePath>../my-parent<relativePath>
<parent>
<artifactId>my-project<artifactId>
<project>
relativePath可以不需要，但是用于指明parent的目录，用于快速查询。
dependencyManagement：
用于父项目配置共同的依赖关系，主要配置依赖包相同因素，如版本，scope。
**合成（或者多个模块）**
    一个项目有多个模块，也叫做多重模块，或者合成项目。
如下的定义：
xml 代码
<project>
<modelVersion>4.0.0<modelVersion>
<groupId>org.codehaus.mojo<groupId>
<artifactId>my-parent<artifactId>
<version>2.0<version>
<modules>
    <module>my-project1<module>
    <module>my-project2<module>
<modules>
<project>
build 设置
    主要用于编译设置，包括两个主要的元素，build和report
build
    主要分为两部分，基本元素和扩展元素集合
注意：包括项目build和profile build
xml 代码
<project>
<build>...<build>
<profiles>
    <profile>
      <build>...<build>
    <profile>
<profiles>
<project>
**基本元素**
xml 代码
<build>
<defaultGoal>install<defaultGoal>
<directory>${basedir}/targetdirectory>
<finalName>${artifactId}-${version}finalName>
<filters>
    <filter>filters/filter1.properties<filter>
<filters>
...
<build>
defaultGoal: 定义默认的目标或者阶段。如install
directory: 编译输出的目录
finalName: 生成最后的文件的样式
filter: 定义过滤，用于替换相应的属性文件，使用maven定义的属性。设置所有placehold的值
**资源(resources)**
    你项目中需要指定的资源。如spring配置文件,log4j.properties
xml 代码
<project>
<build>
    ...
    <resources>
      <resource>
        <targetPath>META-INF/plexus<targetPath>
        <filtering>falsefiltering>
        <directory>${basedir}/src/main/plexus<directory>
        <includes>
          <include>configuration.xml<include>
        <includes>
        <excludes>
          <exclude>**/*.properties<exclude>
        <excludes>
      <resource>
    <resources>
    <testResources>
      ...
    <testResources>
    ...
<build>
<project>
resources: resource的列表，用于包括所有的资源
targetPath: 指定目标路径，用于放置资源，用于build
filtering: 是否替换资源中的属性placehold
directory: 资源所在的位置
includes: 样式，包括那些资源
excludes: 排除的资源
testResources: 测试资源列表
**插件**
在build时，执行的插件，比较有用的部分，如使用jdk 5.0编译等等
xml 代码
<project>
<build>
    ...
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins<groupId>
        <artifactId>maven-jar-plugin<artifactId>
        <version>2.0<version>
        <extensions>false<extensions>
        <inherited>true<inherited>
        <configuration>
          <classifier>test<classifier>
        <configuration>
        <dependencies>...<dependencies>
        <executions>...<executions>
      <plugin>
    <plugins>
<build>
<project>
extensions: true or false，是否装载插件扩展。默认false
inherited: true or false，是否此插件配置将会应用于poms，那些继承于此的项目
configuration: 指定插件配置
dependencies: 插件需要依赖的包
executions: 用于配置execution目标，一个插件可以有多个目标。
如下：
xml 代码
<plugin>
        <artifactId>maven-antrun-plugin<artifactId>
        <executions>
          <execution>
            <id>echodirid>
            <goals>
              <goal>run<goal>
            <phase>verify<phase>
            <inherited>false<inherited>
            <configuration>
              <tasks>
                <echo>Build Dir: ${project.build.directory}<echo>
              <tasks>
            <configuration>
          <execution>
        <executions>
     <plugin>
说明：
id:规定execution 的唯一标志
goals: 表示目标
phase: 表示阶段，目标将会在什么阶段执行
inherited: 和上面的元素一样，设置false maven将会拒绝执行继承给子插件
configuration: 表示此执行的配置属性
**插件管理**    pluginManagement：插件管理以同样的方式包括插件元素，用于在特定的项目中配置。所有继承于此项目的子项目都能使用。主要定义插件的共同元素
扩展元素集合
主要包括以下的元素：
Directories
用于设置各种目录结构，如下：
xml 代码
<build>
    <sourceDirectory>${basedir}/src/main/java<sourceDirectory>
    <scriptSourceDirectory>${basedir}/src/main/scripts<scriptSourceDirectory>
    <testSourceDirectory>${basedir}/src/test/java<testSourceDirectory>
    <outputDirectory>${basedir}/target/classes<outputDirectory>
    <testOutputDirectory>${basedir}/target/test-classes<testOutputDirectory>
    ...
<build>
**Extensions**
表示需要扩展的插件，必须包括进相应的build路径。
xml 代码
<project>
<build>
    ...
    <extensions>
      <extension>
        <groupId>org.apache.maven.wagon<groupId>
        <artifactId>wagon-ftp<artifactId>
        <version>1.0-alpha-3<version>
      <extension>
    <extensions>
    ...
<build>
<project>
**Reporting**
    用于在site阶段输出报表。特定的maven 插件能输出相应的定制和配置报表。
xml 代码
<reporting>
    <plugins>
      <plugin>
        <outputDirectory>${basedir}/target/siteoutputDirectory>
        <artifactId>maven-project-info-reports-pluginartifactId>
        <reportSets>
          <reportSet>reportSet>
        reportSets>
      plugin>
    plugins>
reporting>
**Report Sets**
    用于配置不同的目标，应用于不同的报表
xml 代码
<reporting>
    <plugins>
      <plugin>
        ...
        <reportSets>
          <reportSet>
            <id>sunlinkid>
            <reports>
              <report>javadoc<report>
            <inherited>truei<nherited>
            <configuration>
              <links>
                <link>http://java.sun.com/j2se/1.5.0/docs/api/<link>
            <configuration>
          <reportSet>
        <reportSets>
      <plugin>
    <plugins>
<reporting>
name:项目除了artifactId外，可以定义多个名称
description: 项目描述
url: 项目url
inceptionYear:创始年份
Licenses
xml 代码
<licenses>
<license>
    <name>Apache 2name>
    <url>http://www.apache.org/licenses/LICENSE-2.0.txt<url>
    <distribution>repodistribution>
    <comments>A business-friendly OSS license<comments>
<license>
<licenses>
**Organization配置组织信息**
xml 代码
<organization>
    <name>Codehaus Mojoname>
    <url>http://mojo.codehaus.org<url>
organization>
**Developers配置开发者信息**
xml 代码
<developers>
    <developer>
      <id>eric<id>
      <name>Eric<name>
      <email>eredmond@codehaus.org<email>
      <url>http://eric.propellors.net<url>
      <organization>Codehausorganization>
      <organizationUrl>http://mojo.codehaus.orgorganization<Url>
      <roles>
        <role>architect<role>
        <role>developer<role>
      <roles>
      <timezone>-6timezone>
      <properties>
        <picUrl>http://tinyurl.com/prv4tpic<Url>
      <properties>
    <developer>
<developers>
Contributors
xml 代码
<contributors>
   <contributor>
     <name>Noelle<name>
     <email>some.name@gmail.com<email>
     <url>http://noellemarie.com<url>
     <organization>Noelle Marie<organization>
     <organizationUrl>http://noellemarie.com<organizationUrl>
     <roles>
       <role>tester<role>
     <roles>
     <timezone>-5<timezone>
     <properties>
       <gtalk>some.name@gmail.com<gtalk>
     <properties>
   <contributor>
<contributors>
**环境设置**
**Issue Management**
    定义相关的bug跟踪系统，如bugzilla,testtrack,clearQuest等
xml 代码
<issueManagement>
    <system>Bugzilla<system>
    <url>http://127.0.0.1/bugzillau<rl>
<issueManagement>
**Continuous Integration Management连续整合管理，基于triggers或者timings**
xml 代码
<ciManagement>
   <system>continuum<system>
   <url>http://127.0.0.1:8080/continuum<url>
   <notifiers>
     <notifier>
       <type>mail<type>
       <sendOnError>true<sendOnError>
       <sendOnFailure>true<sendOnFailure>
       <sendOnSuccess>false<sendOnSuccess>
       <sendOnWarning>false<sendOnWarning>
       <configuration><address>continuum@127.0.0.1<address><configuration>
     <notifier>
   <notifiers>
<ciManagement>
**Mailing Lists**
xml 代码
<mailingLists>
   <mailingList>
     <name>User List<name>
     <subscribe>user-subscribe@127.0.0.1<subscribe>
     <unsubscribe>user-unsubscribe@127.0.0.1un<subscribe>
     <post>user@127.0.0.1<post>
     <archive>http://127.0.0.1/user/<archive>
     <otherArchives>
       <otherArchive>http://base.google.com/base/1/127.0.0.1<otherArchive>
     <otherArchives>
   <mailingList>
<mailingLists>
**SCM软件配置管理，如cvs 和svn**
xml 代码
<scm>
    <connection>scm:svn:http://127.0.0.1/svn/my-project<connection>
    <developerConnection>scm:svn:https://127.0.0.1/svn/my-project<developerConnection>
    <tag>HEAD<tag>
    <url>http://127.0.0.1/websvn/my-project<url>
<scm>
**Repositories**
配置同setting.xml中的开发库
Plugin Repositories
配置同 repositories
Distribution Management
用于配置分发管理，配置相应的产品发布信息,主要用于发布，在执行mvn deploy后表示要发布的位置
**1 配置到文件系统**
xml 代码
<distributionManagement>
<repository>
<id>proficio-repository<id>
<name>Proficio Repository<name>
<url>file://${basedir}/target/deploy<url>
<repository>
<distributionManagement>
**2 使用ssh2配置**
xml 代码
<distributionManagement>
<repository>
<id>proficio-repository<id>
<name>Proficio Repository<name>
<url>scp://sshserver.yourcompany.com/deploy<url>
<repository>
<distributionManagement>
**3 使用sftp配置**
xml 代码
<distributionManagement>
<repository>
<id>proficio-repositoryi<d>
<name>Proficio Repository<name>
<url>sftp://ftpserver.yourcompany.com/deploy<url>
<repository>
<distributionManagement>
**4 使用外在的ssh配置**
    编译扩展用于指定使用wagon外在ssh提供，用于提供你的文件到相应的远程服务器。
xml 代码
<distributionManagement>
<repository>
<id>proficio-repository<id>
<name>Proficio Repository<name>
<url>scpexe://sshserver.yourcompany.com/deploy<url>
<repository>
<distributionManagement>
<build>
<extensions>
<extension>
<groupId>org.apache.maven.wagon<groupId>
<artifactId>wagon-ssh-external<artifactId>
<version>1.0-alpha-6<version>
<extension>
<extensions>
<build>
**5 使用ftp配置**
xml 代码
<distributionManagement>
<repository>
<id>proficio-repository<id>
<name>Proficio Repository<name>
<url>ftp://ftpserver.yourcompany.com/deploy<url>
<repository>
<distributionManagement>
<build>
<extensions>
<extension>
<groupId>org.apache.maven.wagongroupId>
<artifactId>wagon-ftpartifactId>
<version>1.0-alpha-6version>
<extension>
<extensions>
<build>
repository 对应于你的开发库，用户信息通过settings.xml中的server取得
**Profiles**类似于settings.xml中的profiles，增加了几个元素，如下的样式：
xml 代码
<profiles>
    <profile>
      <id>test<id>
      <activation>...<activation>
      <build>...<build>
      <modules>...<modules>
      <repositories>...<repositories>
      <pluginRepositories>...<pluginRepositories>
      <dependencies>...<dependencies>
      <reporting>...r<eporting>
      <dependencyManagement>...<dependencyManagement>
      <distributionManagement>...<distributionManagement>
    <profile>
<profiles>
设置项目主函数：
<plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-shade-plugin</artifactId>
                <version>1.2.1</version>
                <executions>
                    <execution>
                        <phase>package</phase>
                        <goals>
                            <goal>shade</goal>
                        </goals>
                        <configuration>
                            <transformers>
                                <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
                                    <mainClass>cn.vicky.mainapp.App</mainClass>
                                </transformer>
                            </transformers>
                        </configuration>
                    </execution>
                </executions>
            </plugin>
