# 用maven构建java project - z69183787的专栏 - CSDN博客
2014年03月27日 16:09:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：691
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)
### 为什么要用Maven
Maven是一个用于项目代码管理的开源产品，主页[http://maven.apache.org](http://maven.apache.org/)。在项目中使用maven的好处有：统一项目成员的开发环境，统一管理项目类库，就不必每人都手动添加项目开发需要的包了，设置项目间的继承、依赖关系，编译、打包、发布项目，生成项目信息站点，利用插件自动执行junit，checkstyle，pmd测试和检查，并生成报告在项目信息站点上。
### Maven的工作原理
如果以上种种都不够吸引你在项目中使用maven的话，那么可以就此打住，不必往下看了。下面要说的是maven的工作原理：maven主要通过每个项目制定的pom.xml文件来管理项目的各种信息和特性。POM是Project Obejct Model的缩写，maven把每个项目看成一个对象，通过pom.xml来定义这个对象的属性。每个项目都有自己的pom.xml，这样项目间的继承和依赖在pom.xml就表现为对象的继承和依赖，这点特别适合大型项目的部署。Maven还内置了一个super pom，所有的pom都继承自这个super
 pom，这样即使项目的pom.xml里看起来没有定义什么特性，这个项目也能执行基本的build逻辑。而当项目定义了自己的build逻辑时，就会自动覆盖父类POM的相应逻辑。
### POM构造
一个基本的pom.xml文件模版如下：
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.cdc</groupId>
  <artifactId>CDCProject</artifactId>
  <packaging>jar</packaging>
  <version>1.0-SNAPSHOT</version>
  <name>CDCProject</name>     
<build>
         <directory>target</directory>
         <sourceDirectory>src</sourceDirectory>
         <testSourceDirectory>test</testSourceDirectory>
         <outputDirectory>build</outputDirectory>
         <testOutputDirectory>build/test-classes</testOutputDirectory>               
         <resources>
               <resource>
                    <directory>src</directory>
                    <excludes>
                         <exclude>**/*.java</exclude>
                    </excludes>
               </resource>
         </resources>            
     </build> 
<repositories>
     </repositories>
     <dependencies>    
     </dependencies>
     <properties>
     </properties>
</project>
其中黑色部分的是需要根据项目情况自己定义的内容。简单来说，project根节点下有build，dependencies, repositories等节点内容，其中build指定项目的构造信息及build逻辑，dependencies存放项目依赖的jar包的信息，repositories指定下载这些包的仓库地址。除了这些，还可以添加licenses，organization，issueManagement，distributionManagemen，pluginrepositories等元素指定项目其他相关信息，具体请参考[http://maven.apache.org/ref/current/maven-model/maven.html](http://maven.apache.org/ref/current/maven-model/maven.html)。
### Maven在项目中的使用
对POM Model有了一定了解之后，我们就可以开始maven开发项目管理之旅了。
首先要安装配置maven。Maven安装配置的文章网络上一搜一大把 ，这里我就不赘述了，最重要的是把.M2变量指定到本地的repository目录上就可以了。
### 构建项目目录
安装好maven后，我们就可以用mvn archetype:create命令在workspace下创建一个项目目录。如要创建一个名为myapp的项目名，它有一个名为com.mycompany.app的包，则应在dos下进入workspace目录，运行 mvn archetype:create -DgroupId=com.mycompany.app -DartifactId=myapp ，这就生成了一个项目目录，有如下内容：
+src
  +main
    +java
      +com
        +mycompany
          +app
  +test
    +java
      +com
        +mycompany
          +app
**pom.xml**
如果觉得这个目录结构太简单了，不能满足项目需要，可以使用Appfuse插件生成更有针对性地目录结构。下面是生成各种配置类型的目录结构所使用的maven命令（要求2.0.6以上版本）：
|**配置类型**|**命  令**|
|----|----|
|**JSF Basic**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-basic-jsf -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**Spring MVC Basic**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-basic-spring -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**Struts 2 Basic**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-basic-struts -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**Tapestry Basic**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-basic-tapestry -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**JSF Modular**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-modular-jsf -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**Spring MVC Modular**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-modular-spring -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**Struts 2 Modular**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-modular-struts -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**Tapestry Modular**|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-modular-tapestry -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
|**Core**(仅后端)|mvn archetype:create -DarchetypeGroupId=org.appfuse.archetypes -DarchetypeArtifactId=appfuse-core -DremoteRepositories=[http://static.appfuse.org/releases](http://static.appfuse.org/releases)-DarchetypeVersion=2.0 -DgroupId=com.mycompany.app -DartifactId=myproject|
当然，Appfuse的妙用肯定不止这些，它的代码生成工具可以生成绝大部分我们需要的代码，如 dao 类，service 类，增删改查的 web 页面、配置文件等，有兴趣的同学可以自己研究一下。
### 修改POM文件
自动生成的POM文件肯定是有一些未明确的地方，这就需要我们手动修改了，指定项目的其他信息，build规则，依赖包信息和下载地址等等。
首先我们看看，pom.xml都还提供了哪些用于描述项目信息的元素：
<project>
  <!—在继承的情况下 描述上层POM -->
  <parent>
    <artifactId>...</artifactId>
    <groupId>...</groupId>
    <version>...</version>
  </parent>
  <!-- POM Module 版本-->
  <modelVersion>4.0.0</modelVersion>
  <groupId>...</groupId>
  <artifactId>...</artifactId>
<version>...</version>
  <!--在子类Module 存在的情况下，packaging设置为pom，利用 <modules> 连接子Module；否则packaging设置为jar，无需<modules> 元素 -->
<modules>
<module>...</module>
     <module>...</module>
</modules>
<packaging>pom</packaging>
<!—描述项目的开发管理系统-->
  <issueManagement>
    <system/>
    <url/>
  </issueManagement>
<!—描述项目的continuous integration system-->
  <ciManagement>
    ...
  </ciManagement>
<!—描述项目的版本控制工具，如cvs，svn等-->
  <scm>
    ...
</scm>
<!—用于指定所有子项目共同依赖的包-->
  <dependencyManagement>
    <dependencies>
      <dependency>
      ...
      </dependency>
    </dependencies>
  </dependencyManagement>
<!—用于指定部署Application 的地址-->
  <distributionManagement>
    <repository>
    ...
    </repository>
</distributionManagement>
<!—用于项目所使用的属性的定义，使用时就可以用${destdir}来取得/EP的值，并且此属性可以被继承了该POM的子POM取得-->
<properties>
<destdir>/EP</destdir>
...
</properties>
<!—用于指定 Source 文件位置，Resource 位置，编译好的 Class 文件位置等-->
  <build>
  ...
  </build>
<!—用于指定相应phase触发时执行的build动作-->
<profiles>
    ...
</profiles>
<!—用于指定下载第三方包的远程仓库地址-->
  <repositories>
    <repository>
    ...
    </repository>
  </repositories>
<!—用于指定下载第三方插件的远程仓库地址-->
  <pluginRepositories>
    <pluginRepository>
    ...
    </pluginRepository>
  </pluginRepositories>
<!—用于指定项目所依赖的第三方包的信息-->
  <dependencies>
    <dependency>
    ...
    </dependency>
  </dependencies>
<!—用于指定项目自动生成报告的内容-->
  <reporting>
  ...
  </reporting>
</project>
以build之前的大多数信息都会体现在用maven生成的项目站点上。下面我们来看看build的定义。maven的很多功能都是由插件提供的，这也不例外。
<build>
<!—- 似乎没什么必要重新指定，生产的项目站点html会放在这个目录下 -->
         <directory>target</directory>
<!—- SourceDirectory 和testDirectory 默认使用的是标准目录结构，即src/main/java和src/test/java，如果不是使用标准目录结构则需要重新指定 -->
<sourceDirectory>src</sourceDirectory>
         <testSourceDirectory>test</testSourceDirectory>
<!—- 编译后的class存放的地址 -->
         <outputDirectory>build</outputDirectory>
         <testOutputDirectory>build/test-classes</testOutputDirectory>      
<!—- 指定项目的source目录 -->
<resources>
               <resource>
                    <directory>src</directory>
                    <excludes>
                         <exclude>**/*.java</exclude>
                    </excludes>
               </resource>
         </resources>
         <testResources>
            <testResource>
                <directory>src/test/java</directory>
                <excludes>
                    <exclude>**/*.java</exclude>
                </excludes>
            </testResource>
        </testResources>
        <plugins>
            <!—用ant插件执行build任务 -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-antrun-plugin</artifactId>
                <executions>
                    <execution>
<!—- id可以随便取，不重复就行，phase必须是指 mvn 命令名称 -->
                        <id>clean</id>
                        <phase>clean</phase>
<!—- 插件使用时的配置信息 -->
                        <configuration>
                            <tasks>
<!—- 以下任务描述的标记语法和ant的一样 -->
                                <delete>
                                    <fileset dir="${destdir}/WebApp/WEB-INF/">
                                        <include name="**/**"/>
                                    </fileset>                                 
                                </delete>
                            </tasks>
                        </configuration>
                        <goals>
                            <goal>run</goal>
                        </goals>
                    </execution>
                    <execution>
                        <id>compile</id>
                        <phase>compile</phase>
                        <configuration>
                            <tasks>
                                <native2ascii src="src/resources"
                                              dest="${project.build.outputDirectory}/resources" encoding="UTF-8"
                                              includes="**/*.base"  ext=".properties" />
                                <delete>
                                    <fileset dir="${project.build.outputDirectory}/resources">
                                        <include name="**/*.base"/>
                                    </fileset>                                 
                                </delete>                             
                            </tasks>
                        </configuration>
                        <goals>
                            <goal>run</goal>
                        </goals>
                    </execution>
                </executions>
<!—- 指定插件执行时所依赖的包 -->
                <dependencies>
                    <dependency>
                        <groupId>com.sun</groupId>
                        <artifactId>tools</artifactId>
                        <scope>system</scope>
                        <version>1.5</version>
                        <systemPath>${java.home}/../lib/tools.jar</systemPath>
                    </dependency>
                    <dependency>
                        <groupId>ant</groupId>
                        <artifactId>ant-nodeps</artifactId>
                        <version>1.6.5</version>
                        <scope>provided</scope>
                    </dependency>
                </dependencies>
            </plugin>
            <!-- 编译用的插件，利用这它在低版本的jdk的情况下也可以编译-->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>1.5</source>
                    <target>1.5</target>
                </configuration>
            </plugin>
</plugins>
</build>
接着，再来看看maven最具特色的jar包管理。pom.xml文件中根元素下的dependencies元素就是用于描述项目所依赖的类库信息的，格式如下：
<dependencies>
         <dependency>
              <groupId>javax.servlet</groupId>
              <artifactId>jstl</artifactId>
              <version>1.1.2</version>
              <scope>provided</scope>
          </dependency>
         ...
</dependencies>
其中<scope> 参数指定了该类库什么时候使用，有以下 5 种：
l        compile : 默认的范围 ，在编译时装载Class Path .
l        provided : 和 compile 类似， 显示由 JDK 或 container 提供.
l        runtime : 在编译时不需要，但是Application 执行时必要. 测试或Application 执行时装载 ClassPath ，但是编译时就不装载 ClassPath.
l        test : 一般状态下是没有必要的，测试编译，或是测试时必要的.
l        system : 除了以明细标记 jar 文件以外和 provided 类似.
Maven对第三方包的管理类是以一种集中管理的方式存在的。maven目录下的repository目录可以看成一个第三方包的仓库，以名字和版本号为路径存放各种jar包及其pom.xml文件。也就是说maven的类库管理实际上是把每个jar报都当成一个 POM来看，这样当项目要使用某个包时，只要在项目的pom.xml文件中指定这个包的POM所定义的artifactId和版本信息就可以了，不必像以前那样把包复制到项目文件夹中。当机器上有多个项目时，就可以节省了不少复制来复制去的包所占用的空间，为项目瘦身了不少。开发时的classpath也可以根据pom.xml来生成，这就绝对统一了开发人员间所用到的第三方包的版本。
当生成classpath或者编译项目时，Maven都先在本地repository里groupId， artifactId，version指定的路径下下载必要的 Library，如上面这个例子的包就会在repository\javax\servlet\jstl\1.1.2\下下载 。而网络上也有一些服务器提供这样的repository，如[http://www.ibiblio.org/maven2](http://www.ibiblio.org/maven2)和ttp://maven-plugins.sourceforge.net。如果本地仓库上没有，maven就会连接到这样的远程仓库的相应目录进行下载。这样就需要指定远程仓库的地址了：
<repositories>
         <repository>
              <id>internal</id>
              <name>Maven Repository Switchboard</name>
              <layout>default</layout>
              <url>http://109.52.20.29/internal</url>
         </repository>
         <repository>
              <id>central</id>
              <name>Maven Plugin Repository Mirror</name>
              <url>http://www.ibiblio.org/maven2</url>
              <layout>default</layout>
         </repository>
         <repository>
              <id>sourceforge</id>
              <name>Sourceforge Plugin Repository Mirror</name>
              <url>http://maven-plugins.sourceforge.net</url>
              <layout>default</layout>
         </repository>     
     </repositories>
项目常用的包在远程仓库上一般都有，但是公司内部的包要想通过远程仓库让开发人员下载的话，就需要建立一个内网的repository，也根据名字和版本号放上要用的包及其pom.xml文件。安装第三方包的命令是：
mvn install:install-file -DgroupId=javax.transaction -DartifactId=jta -Dversion=1.0.1B -Dpackaging=jar -Dfile=jta.jar
其中Dfile指定目前这个包的存放路径，这样就会自动根据groupId，artifactId和version在本地repository下构建路径和pom.xml。
如果内网的远程仓库jar包比较全的话，就把它放在repositories节点下的第一个，毕竟内网的下载速度是要快些的嘛。
### 添加工程属性
mvn archetype:create命令生成的项目目录还不能称之为工程，因为这个目录下还缺少.project文件。如果是用eclipse开发的话，就在工程目录下执行mvn  eclipse: eclipse。上一步我们修改好这个目录下的pom.xml后，maven就可以根据这个文件定义的dependencies来生成.classpath和 .project文件了。到此项目的构建就算完成了，开发人员就可以用这个环境进行开发了。
### 生成项目站点
用mvn site命令可以根据项目的信息生成项目站点，其中就包括了我们之前在pom.xml里设置的项目相关信息和reporting元素定义的测试和检查的报告。maven 提供的针对如 junit ， checkstyle ， pmd 等的插件，都能够方便的进行测试和检查并能直接生成报告。具体使用可以在 [http://maven.apache.org/plugins/index.html](http://maven.apache.org/plugins/index.html) 找到。
<reporting>  
         <plugins>
              <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-site-plugin</artifactId>
              </plugin>
              <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>
                       maven-project-info-reports-plugin
                   </artifactId>
                   <inherited>true</inherited>
                   <reportSets />
              </plugin>
              <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-checkstyle-plugin</artifactId>
                   <configuration>
                       <configLocation>
                            config/sun_checks.xml
                       </configLocation>
                   </configuration>
              </plugin>
              <plugin>
                   <groupId>org.apache.maven.plugins</groupId>
                   <artifactId>maven-pmd-plugin</artifactId>
              </plugin>
         </plugins>
     </reporting>
### 其他mvn 命令
开发过程中，可能用到的其他mvn 命令有：
l         validate : 检查 Project 里Setting 的信息是否可用。
l         compile : 编译源代码。
l         test : 用junit测试编译好的 Source Code 。
l         package  : 把编译好的源代码打包。
l         integration-test : 根据环境打包，并进行集成测试。
l         verify : 检查package 正确执行与否，可否使用，packaging 条件是否齐全。
l         install : 把package打好的包发布到本地 repository 。这样别的项目就可以使用这个包了。
l         deploy : 把package打好的包发布到远程仓库。这样其他开发人员就可以下载这个包了。
