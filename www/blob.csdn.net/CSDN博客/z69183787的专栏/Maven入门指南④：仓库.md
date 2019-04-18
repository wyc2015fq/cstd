# Maven入门指南④：仓库 - z69183787的专栏 - CSDN博客
2016年05月17日 11:46:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：360
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)

1 . 仓库简介
　　没有 Maven 时，项目用到的 .jar 文件通常需要拷贝到 /lib 目录，项目多了，拷贝的文件副本就多了，占用磁盘空间，且难于管理。Maven 使用一个称之为仓库的目录，根据构件的坐标统一存储这些构件的唯一副本，在项目中通过依赖声明，可以方便的引用构件。
2 . 仓库的布局
　　构件都有唯一的坐标，Maven 根据坐标管理构件的存储。如以下对 spring-orm-3.2.0 的存储：
![](http://images.cnitblog.com/i/293735/201406/130240024366894.png)
文件路径对应了：groupId/artifactId/version/artifactId-version.packaging
3 . 仓库的分类
　　Maven 仓库分为本地仓库和远程仓库，寻找构件时，首先从本地仓库找，找不到则到远程仓库找，再找不到就报错；在远程仓库中找到了，就下载到本地仓库再使用。中央仓库是 Maven 核心自带的远程仓库，默认地址：http://repo1.maven.org/maven2。除了中央仓库，还有其它很多公共的远程仓库。私服是架设在本机或局域网中的一种特殊的远程仓库，通过私服可以方便的管理其它所有的外部远程仓库。
![](http://images.cnitblog.com/i/293735/201406/130307021083775.png)
3 . 1 . 本地仓库
　　Maven 本地仓库默认地址为：${user.home}/.m2/repository。
![](http://images.cnitblog.com/i/293735/201406/130325391247688.png)
　　通过修改 %MAVEN_HOME%/conf/settings.xml （或者：${user.home}/.m2/settings.xml，针对当前用户（推荐））配置文件可以更改本地仓库的位置。
![](http://images.cnitblog.com/i/293735/201406/130326142955376.png)
3 . 2 . 中央仓库
　　安装完 Maven ，本地仓库几乎是空的，这时需要从远程仓库下载所需构件。Maven 配置了一个默认的远程仓库，即中央仓库，找到 %MAVEN_HOME%/lib/maven-model-builder-3.2.1.jar，打开 org/apache/maven/model/pom-4.0.0.xml 超级POM：
![](http://images.cnitblog.com/i/293735/201406/130407543745688.png)
3 . 3 . 在项目中添加其他远程仓库
　　当中央仓库找不到所需的构件时，我们可以配置 pom.xml ，添加其它的远程仓库。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <repositories>
 2     <repository>
 3         <id>Sonatype</id>
 4         <name>Sonatype Repository</name>
 5         <url>http://repository.sonatype.org/content/groups/public/</url>
 6         <layout>default</layout>
 7         <releases>
 8             <enabled>true</enabled>
 9         </releases>
10         <snapshots>
11             <enabled>false</enabled>
12         </snapshots>
13     </repository>
14 </repositories>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
其中 id 必须唯一，若不唯一，如设置为 central 将覆盖中央仓库的配置。
3 . 4 . 镜像仓库
　　镜像仓库可以理解为仓库的副本，从仓库中可以找到的构件，从镜像仓库中也可以找到。比如针对中央仓库 http://repo1.maven.org/maven2 ，在中国有它的镜像仓库，这样我们直接访问镜像仓库，更快更稳定。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <settings>
 2     ...
 3     <mirrors>
 4         <mirror>
 5             <id>maven.net.cn</id>
 6             <name>central mirror in china</name>
 7             <url>http://maven.net.cn/content/groups/public</url>
 8             <mirrorOf>central</mirrorOf>    <!--表明为central中央仓库配置镜像仓库-->
 9         </mirror>
10     </mirrors>
11     ...
12 </settings>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　其中，<mirrorOf> 指明了为哪个仓库配置镜像，可以使用通配符如：<mirrorOf>*</mirrorOf>，或者 <mirrorOf>repo1,repo2</mirrorOf> 等进行匹配。一旦配置了镜像，所有针对原仓库的访问将转到镜像仓库的访问，原仓库将不再能直接访问，即使镜像仓库不稳定或停用。在搭建私服的时候，我们通常为所有仓库设置镜像为私服地址，通过私服对所有仓库进行统一管理。
3 . 5 . 常用的仓库搜索地址
- [http://search.maven.org/](http://search.maven.org/)
![](http://images.cnitblog.com/i/293735/201406/130437570451062.png)
- [https://repository.sonatype.org/](https://repository.sonatype.org/#welcome)
![](http://images.cnitblog.com/i/293735/201406/130439280145638.png)
- [http://mvnrepository.com/](http://mvnrepository.com/)
![](http://images.cnitblog.com/i/293735/201406/130440344994959.png)
