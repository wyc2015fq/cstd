# 为Maven项目配置多个源文件夹 - z69183787的专栏 - CSDN博客
2015年10月06日 19:20:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7797
Maven 为我们提供了一致的项目目录配置（源文件夹、资源文件夹等），在自动构建项目时， Maven 会按照这个配置来执行操作（编译源文件、拷贝资源文件）。
当 Maven 规约的默认配置不能够满足我们的需求时，我们就要动手修改项目的 Maven 配置，让我们的项目能够与 Maven 很好地协同工作。这里讨论的需求有：
- 配置多个源文件夹来管理我们项目的模块
- 配置多个资源文件夹来管理我们项目模块的资源文件
- 有些遗留代码的一些资源文件是放在源文件夹下的，为了不改变原有项目结构，我希望 Maven 能够从源文件夹下读取资源文件
- 我想将项目模块的源文件和资源文件放置在同一个文件夹下，以方便开发与维护
**1. 配置项目的pom文件（Maven配置文件）**
**1.1 Maven 默认的源文件夹及资源文件夹配置**
Maven 默认的源文件夹及资源文件夹的配置代码如下：
Xml代码  ![收藏代码](http://casheen.iteye.com/images/icon_star.png)
- <build>
-    ...  
- <sourceDirectory> src/main/java </sourceDirectory>
- <testSourceDirectory> src/test/java </testSourceDirectory>
- <resources>
- <resource>
- <directory> src/main/resources </directory>
- </resource>
- </resources>
- <testResources>
- <testResource>
- <directory> src/test/resources </directory>
- </testResource>
- </testResources>
-    ...  
- </build>
在 eclipse 下，项目源文件夹及资源文件夹展现成这样：
![](http://dl.iteye.com/upload/attachment/175931/3c61ed50-669c-3e72-98ca-8458ab224227.jpg)
**1.2 配置多个资源文件夹**
从上面 Maven 的默认配置来看，源文件夹和测试源文件夹都只能配置一个，要配置多个源文件夹比较困难。这里我们先展示如何配置多个资源文件夹。
有人会认为，在 eclipse 下将这些文件夹都设置为源文件夹不就 OK 了吗？其实不然， Maven 在构建的时候并不会去读取任何 IDE 的配置信息，所以他不会知道我们在 eclipse 下指定的源文件夹，也不会将这些源文件夹下的代码编译打包。
配置多个资源文件夹的代码：
Xml代码  ![收藏代码](http://casheen.iteye.com/images/icon_star.png)
- <build>
-    ...  
- <resources>
- <resource>
- <directory> src/main/resources </directory>
- </resource>
- <resource>
- <directory> src/labs/resources </directory>
- </resource>
- </resources>
-    ...  
- </build>
配置好以后， IDE （ eclipse ）不会识别我们的配置，我们需要更新项目的配置：
![](http://dl.iteye.com/upload/attachment/175933/b4bb4f27-7419-3a60-a5b8-5bed484e946e.jpg)
更新后， 在 eclipse 下项目源文件夹及资源文件夹展现成这样（多了一个资源文件夹）：
![](http://dl.iteye.com/upload/attachment/175935/2aee7afc-5d0d-3975-9252-eda5f209155b.jpg)
**1.3 配置读取源文件夹里的资源文件**
为了让 Maven 能够从源文件夹下读取资源文件（或将所有资源文件配置到源文件夹下），我们可以这样配置：
Xml代码  ![收藏代码](http://casheen.iteye.com/images/icon_star.png)
- <build>
-    ...  
- <resources>
- <resource>
- <directory> src/main/resources </directory>
- </resource>
- <resource>
- <directory> src/labs/resources </directory>
- </resource>
- <resource>
- <directory> src/main/java </directory>
- <includes>
- <include> **/*.hbm.xml </include>
- </includes>
- </resource>
- <resource>
- <directory> src/labs/java </directory>
- <includes>
- <include> **/*.hbm.xml </include>
- </includes>
- </resource>
- </resources>
-    ...  
- </build>
这样，不仅 src/main/resources 和 src/labs/resources 这两个目录下的文件会作为资源文件被打包，src/main/java 和 src/labs/java 目录下的所有 .hbm.xml 文件也都会作为资源文件被打包。否则， Maven 构建时不会打包这两个目录下的 .hbm.xml 文件，导致运行时因为找不到资源文件而抛异常。
**1.4 配置多个源文件夹**
前面讲过， Maven 默认只支持一个源文件夹，为了给项目配置多个源文件夹，我们需要用到一个 Maven 插件，目的是在 Maven 构建生命周期内为项目添加源文件夹：
Xml代码  ![收藏代码](http://casheen.iteye.com/images/icon_star.png)
- <build>
-     ...  
- <plugins>
-         ...   
- <!-- build-helper-maven-plugin, 设置多个源文件夹 -->
- <plugin>
- <groupId>org.codehaus.mojo</groupId>
- <artifactId>build-helper-maven-plugin</artifactId>
- <version>1.4</version>
- <executions>
- <execution>
- <id>add-source</id>
- <phase>generate-sources</phase>
- <goals>
- <goal>add-source</goal>
- </goals>
- <configuration>
- <sources>
- <source>${basedir}/src/labs/java</source>
- <!-- 我们可以通过在这里添加多个source节点，来添加任意多个源文件夹 -->
- </sources>
- </configuration>
- </execution>
- </executions>
- </plugin>
-         ...   
- </plugins>
-     ...   
- </build>
更新项目配置（操作同2.2）后，在eclipse下项目源文件夹及资源文件夹展现成这样（多了一个源文件夹）：
![](http://dl.iteye.com/upload/attachment/175937/886ed938-eb9f-35fb-a26f-e6867c209949.jpg)
