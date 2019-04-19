# Windows版maven的安装配置 - Big Smile - CSDN博客
2017年02月23日 20:55:30[王啸tr1912](https://me.csdn.net/tr1912)阅读数：515
        这几天研究项目，恰好要用到maven的库，所以就简单的了解了一下maven的配置，我们一起来探究一下。
# 一、maven
         什么是maven呢？
         Maven项目对象模型(POM)，可以通过一小段描述信息来管理项目的构建，报告和文档的软件项目管理工具。
Maven 除了以程序构建能力为特色之外，还提供高级项目管理工具。由于 Maven 的缺省构建规则有较高的可重用性，所以常常用两三行 Maven 构建脚本就可以构建简单的项目。
         上面说了这么多，大家肯定也什么都不知道对吧，其实简单来说，maven这个工具可以对你的项目进行管理，包括创建，包支持，测试等等功能，而我现在主要用的就是一个包支持的功能，就是把maven当做一个包仓库的功能。
         官方网站：[https://maven.apache.org/](https://maven.apache.org/)
# 二、在Windows中配置
        官方下载地址：[https://maven.apache.org/download.cgi](https://maven.apache.org/download.cgi)
        其实下载maven并不是什么难事，直接在官方网站下载就是了，但是由于中国的网络限制，所以尽量还是找网上分享的资源比较快。
         下载完成之后我们就得到了apache-maven-3.3.9.zip这样的一个压缩文件包，找到一个纯英文的路径直接解压这个包，然后进行我们的配置：
首先找到系统的环境变量设置：
![](https://img-blog.csdn.net/20170223202532525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后在系统变量中添加我们刚解压完的maven文件的目录为MAVEN_HOME：
![](https://img-blog.csdn.net/20170223202750491?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)   其中变量值就是maven解压之后的目录。
然后我们要在系统变量Path中添加maven的bin路径：%MAVEN_HOME%\bin
我这里直接用的maven_home的值。
        这样maven的环境变量就配置好了，然后我们win+r输入cmd打开命令提示符，输入mvn -v，如果有如下显示，代表我们的maven环境配置成功了。
![](https://img-blog.csdn.net/20170223203144059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**仓库的配置：**
        由于我们要把maven当做一个jar包的仓库用，所以可能会下载很多的jar包到电脑上，maven默认配置仓库为“users/你的用户名/.m2”下，所以如果你的C盘不是很宽裕的话，可以提前更改仓库的配置，具体如下：
       首先到解压maven的目录下进入maven文件夹下的conf文件夹，我们会看到一个settings.xml文件，这个就是maven的一个配置文件，用文本编辑工具打开这个文件，我们可以看到他自己的默认配置文件：
```
<?xml version="1.0" encoding="UTF-8"?>
<settings xmlns="http://maven.apache.org/SETTINGS/1.0.0" 
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
          xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0 http://maven.apache.org/xsd/settings-1.0.0.xsd">
  <!-- localRepository
   | The path to the local repository maven will use to store artifacts.
   |
   | Default: ${user.home}/.m2/repository
  <localRepository>/path/to/local/repo</localRepository>
  -->
  <!-- interactiveMode
   | This will determine whether maven prompts you when it needs input. If set to false,
   | maven will use a sensible default value, perhaps based on some other setting, for
   | the parameter in question.
   |
   | Default: true
  <interactiveMode>true</interactiveMode>
  -->
  <!-- offline
   | Determines whether maven should attempt to connect to the network when executing a build.
   | This will have an effect on artifact downloads, artifact deployment, and others.
   |
   | Default: false
  <offline>false</offline>
  -->
  <!-- pluginGroups
   | This is a list of additional group identifiers that will be searched when resolving plugins by their prefix, i.e.
   | when invoking a command line like "mvn prefix:goal". Maven will automatically add the group identifiers
   | "org.apache.maven.plugins" and "org.codehaus.mojo" if these are not already contained in the list.
   |-->
  <pluginGroups>
    <!-- pluginGroup
     | Specifies a further group identifier to use for plugin lookup.
    <pluginGroup>com.your.plugins</pluginGroup>
    -->
  </pluginGroups>
  <!-- proxies
   | This is a list of proxies which can be used on this machine to connect to the network.
   | Unless otherwise specified (by system property or command-line switch), the first proxy
   | specification in this list marked as active will be used.
   |-->
  <proxies>
    <!-- proxy
     | Specification for one proxy, to be used in connecting to the network.
     |
    <proxy>
      <id>optional</id>
      <active>true</active>
      <protocol>http</protocol>
      <username>proxyuser</username>
      <password>proxypass</password>
      <host>proxy.host.net</host>
      <port>80</port>
      <nonProxyHosts>local.net|some.host.com</nonProxyHosts>
    </proxy>
    -->
  </proxies>
  <!-- servers
   | This is a list of authentication profiles, keyed by the server-id used within the system.
   | Authentication profiles can be used whenever maven must make a connection to a remote server.
   |-->
  <servers>
    <!-- server
     | Specifies the authentication information to use when connecting to a particular server, identified by
     | a unique name within the system (referred to by the 'id' attribute below).
     | 
     | NOTE: You should either specify username/password OR privateKey/passphrase, since these pairings are 
     |       used together.
     |
    <server>
      <id>deploymentRepo</id>
      <username>repouser</username>
      <password>repopwd</password>
    </server>
    -->
    
    <!-- Another sample, using keys to authenticate.
    <server>
      <id>siteServer</id>
      <privateKey>/path/to/private/key</privateKey>
      <passphrase>optional; leave empty if not used.</passphrase>
    </server>
    -->
  </servers>
  <!-- mirrors
   | This is a list of mirrors to be used in downloading artifacts from remote repositories.
   | 
   | It works like this: a POM may declare a repository to use in resolving certain artifacts.
   | However, this repository may have problems with heavy traffic at times, so people have mirrored
   | it to several places.
   |
   | That repository definition will have a unique id, so we can create a mirror reference for that
   | repository, to be used as an alternate download site. The mirror site will be the preferred 
   | server for that repository.
   |-->
  <mirrors>
    <!-- mirror
     | Specifies a repository mirror site to use instead of a given repository. The repository that
     | this mirror serves has an ID that matches the mirrorOf element of this mirror. IDs are used
     | for inheritance and direct lookup purposes, and must be unique across the set of mirrors.
     |
    <mirror>
      <id>mirrorId</id>
      <mirrorOf>repositoryId</mirrorOf>
      <name>Human Readable Name for this Mirror.</name>
      <url>http://my.repository.com/repo/path</url>
    </mirror>
     -->
  </mirrors>
  
  <!-- profiles
   | This is a list of profiles which can be activated in a variety of ways, and which can modify
   | the build process. Profiles provided in the settings.xml are intended to provide local machine-
   | specific paths and repository locations which allow the build to work in the local environment.
   |
   | For example, if you have an integration testing plugin - like cactus - that needs to know where
   | your Tomcat instance is installed, you can provide a variable here such that the variable is 
   | dereferenced during the build process to configure the cactus plugin.
   |
   | As noted above, profiles can be activated in a variety of ways. One way - the activeProfiles
   | section of this document (settings.xml) - will be discussed later. Another way essentially
   | relies on the detection of a system property, either matching a particular value for the property,
   | or merely testing its existence. Profiles can also be activated by JDK version prefix, where a 
   | value of '1.4' might activate a profile when the build is executed on a JDK version of '1.4.2_07'.
   | Finally, the list of active profiles can be specified directly from the command line.
   |
   | NOTE: For profiles defined in the settings.xml, you are restricted to specifying only artifact
   |       repositories, plugin repositories, and free-form properties to be used as configuration
   |       variables for plugins in the POM.
   |
   |-->
  <profiles>
    <!-- profile
     | Specifies a set of introductions to the build process, to be activated using one or more of the
     | mechanisms described above. For inheritance purposes, and to activate profiles via <activatedProfiles/>
     | or the command line, profiles have to have an ID that is unique.
     |
     | An encouraged best practice for profile identification is to use a consistent naming convention
     | for profiles, such as 'env-dev', 'env-test', 'env-production', 'user-jdcasey', 'user-brett', etc.
     | This will make it more intuitive to understand what the set of introduced profiles is attempting
     | to accomplish, particularly when you only have a list of profile id's for debug.
     |
     | This profile example uses the JDK version to trigger activation, and provides a JDK-specific repo.
    <profile>
      <id>jdk-1.4</id>
      <activation>
        <jdk>1.4</jdk>
      </activation>
      <repositories>
        <repository>
          <id>jdk14</id>
          <name>Repository for JDK 1.4 builds</name>
          <url>http://www.myhost.com/maven/jdk14</url>
          <layout>default</layout>
          <snapshotPolicy>always</snapshotPolicy>
        </repository>
      </repositories>
    </profile>
    -->
    <!--
     | Here is another profile, activated by the system property 'target-env' with a value of 'dev',
     | which provides a specific path to the Tomcat instance. To use this, your plugin configuration
     | might hypothetically look like:
     |
     | ...
     | <plugin>
     |   <groupId>org.myco.myplugins</groupId>
     |   <artifactId>myplugin</artifactId>
     |   
     |   <configuration>
     |     <tomcatLocation>${tomcatPath}</tomcatLocation>
     |   </configuration>
     | </plugin>
     | ...
     |
     | NOTE: If you just wanted to inject this configuration whenever someone set 'target-env' to
     |       anything, you could just leave off the <value/> inside the activation-property.
     |
    <profile>
      <id>env-dev</id>
      <activation>
        <property>
          <name>target-env</name>
          <value>dev</value>
        </property>
      </activation>
      <properties>
        <tomcatPath>/path/to/tomcat/instance</tomcatPath>
      </properties>
    </profile>
    -->
  </profiles>
  <!-- activeProfiles
   | List of profiles that are active for all builds.
   |
  <activeProfiles>
    <activeProfile>alwaysActiveProfile</activeProfile>
    <activeProfile>anotherAlwaysActiveProfile</activeProfile>
  </activeProfiles>
  -->
</settings>
```
        其中省略了头解释，我们可以看到里面大部分的东西都是注释的，我们需要的只是最前面的本地库的配置：<localRepository>/path/to/local/repo</localRepository>
        然后我们在这个标签中的路径替换为我们想要建库的路径即可，注意这里仓库所指向的文件夹必须存在，否则会出错。
# 三、在eclipse中设置maven本地库
        打开myeclipse或者eclipse，进入Window/Preferences，在右面找到maven这个选项（如果没有找到说明你的IDE版本比较老），点击User Settings：
![](https://img-blog.csdn.net/20170223204712457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        然后看到User Settings的设置，点击他的Browse把他指向你自己安装的maven的settings文件的地方，然后下面的Repository会自己出现，然后点击apply就可以了。
![](https://img-blog.csdn.net/20170223205227837?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        IDE会自己更新项目所需要的代码库到本地代码库，或者你可以自己下载一个仓库添加到你设置仓库的地方就可以了，你的项目就可以正常编译了，具体项目中引用maven中的包，是靠的pom文件，所以学好pom文件是很重要的。 
# 四 、总结
       maven其实算是比较好配置的工具了，他还有很多的用处等着我去发现，不过他作为一个仓库来说确实很是好用。
