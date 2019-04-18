# Maven基础-默认中央仓库 - z69183787的专栏 - CSDN博客
2014年11月25日 16:02:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1828
当我们创建一个简单的Maven项目后(只需要在pom.xml配置好依赖)，运行mvn clean install就可以把项目构建好，不需要我们手工下载任何jar，这全靠中央仓库的存在，它会自动从仓库下载。这个仓库的定义是在*${M2_HOME}/lib/maven-2.0.10-uber.jar* 里面。你可以在里面找到*\org\apache\maven\project\pom-4.0.0.xml* 这个文件，在这个文件里面定义了默认中央仓库的地址：
Xml代码  ![收藏代码](http://sunxboy.iteye.com/images/icon_star.png)
- <repositories>
- <repository>
- <id> central</id>
- <name> Maven Repository Switchboard</name>
- <layout> default</layout>
- <url> http://repo1.maven.org/maven2</url>
- <snapshots>
- <enabled> false</enabled>
- </snapshots>
- </repository>
- </repositories>
如果你想覆盖中央仓库的默认地址，那么这里我们就会使用的镜像了，还在setting.xml里面配置：
Xml代码  ![收藏代码](http://sunxboy.iteye.com/images/icon_star.png)
- <settings>
- …     
- <mirrors>
- <mirror>
- <id> maven-net-cn</id>
- <name> Maven China Mirror</name>
- <url> http://maven.net.cn/content/groups/public/</url>
- <mirrorOf> central</mirrorOf>
- </mirror>
- </mirrors>
- …     
- </settings>
<mirrorOf>,表示只为central仓库做镜像，如果想为所有的仓库做镜像那么可以改为：
<mirrorOf>*</mirrorOf>
