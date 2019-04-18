# Maven入门指南⑤：使用Nexus搭建Maven私服 - z69183787的专栏 - CSDN博客
2016年05月17日 11:47:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1615
1 . 私服简介
　　私服是架设在局域网的一种特殊的远程仓库，目的是代理远程仓库及部署第三方构件。有了私服之后，当 Maven 需要下载构件时，直接请求私服，私服上存在则下载到本地仓库；否则，私服请求外部的远程仓库，将构件下载到私服，再提供给本地仓库下载。
![](http://images.cnitblog.com/i/293735/201406/162358583957595.png)![](http://images.cnitblog.com/i/293735/201406/162359096298261.png)
　　我们可以使用专门的 Maven 仓库管理软件来搭建私服，比如：[Apache Archiva](http://archiva.apache.org/index.cgi)，[Artifactory](http://www.jfrog.com/home/v_artifactory_opensource_overview/)，[Sonatype
 Nexus](http://www.sonatype.org/nexus/)。这里我们使用 Sonatype Nexus。
2 . 安装Nexus
2 . 1 . 下载Nexus
　　Nexus 专业版是需要付费的，这里我们下载开源版 Nexus OSS。Nexus 提供两种安装包，一种是包含 Jetty 容器的 bundle 包，另一种是不包含容器的 war 包。下载地址：[http://www.sonatype.org/nexus/go](http://www.sonatype.org/nexus/go)。
![](http://images.cnitblog.com/i/293735/201406/170019176148773.png)
2 . 2 . 使用bundle安装包安装Nexus
解压安装包nexus-2.8.1-bundle.zip，打开命令提示符，进入/nexus-2.8.1-01目录，键入nexus命令（为方便启动和退出Nexus，可将bin目录添加到环境变量）：
![](http://images.cnitblog.com/i/293735/201406/170035210368403.png)
执行 nexus install 将Nexus安装为Windows服务。可将服务启动方式设为手动，以后通过 nexus start 即可启动Nexus ，通过 nexus stop 退出Nexus：
![](http://images.cnitblog.com/i/293735/201406/170041209893372.png)
![](http://images.cnitblog.com/i/293735/201406/170042505821809.png)
打开浏览器，访问：http://localhost:8081/nexus/：
![](http://images.cnitblog.com/i/293735/201406/170047243957928.png)
点击右上角 Log In，使用用户名：admin ，密码：admin123 登录，可使用更多功能：
![](http://images.cnitblog.com/i/293735/201406/170053363173719.png)
3 . Nexus预置的仓库
点击左侧 Repositories 链接，查看 Nexus 内置的仓库：
![](http://images.cnitblog.com/i/293735/201406/170120024571827.png)
Nexus 的仓库分为这么几类：
- hosted 宿主仓库：主要用于部署无法从公共仓库获取的构件（如 oracle 的 JDBC 驱动）以及自己或第三方的项目构件；
- proxy 代理仓库：代理公共的远程仓库；
- virtual 虚拟仓库：用于适配 Maven 1；
- group 仓库组：Nexus 通过仓库组的概念统一管理多个仓库，这样我们在项目中直接请求仓库组即可请求到仓库组管理的多个仓库。
![](http://images.cnitblog.com/i/293735/201406/170144310676162.png)
4 . 添加代理仓库
以 Sonatype 为例，添加一个代理仓库，用于代理 Sonatype 的公共远程仓库。点击菜单 Add - Proxy Repository ：
![](http://images.cnitblog.com/i/293735/201406/170154149117195.png)
填写Repository ID - sonatype；Repository Name - Sonatype Repository；
Remote Storage Location - http://repository.sonatype.org/content/groups/public/ ，save 保存：
![](http://images.cnitblog.com/i/293735/201406/170157250366180.png)
将添加的 Sonatype 代理仓库加入 Public Repositories 仓库组。选中 Public Repositories，在 Configuration 选项卡中，将 Sonatype Repository 从右侧 Available Repositories 移到左侧 Ordered Group Repositories，save 保存：
![](http://images.cnitblog.com/i/293735/201406/170204512706741.png)
5 . 搜索构件
为了更好的使用 Nexus 的搜索，我们可以设置所有 proxy 仓库的 Download Remote Indexes 为 true，即允许下载远程仓库索引。
![](http://images.cnitblog.com/i/293735/201406/170212351148114.png)
索引下载成功之后，在 Browse Index 选项卡下，可以浏览到所有已被索引的构件信息，包括坐标、格式、Maven 依赖的 xml 代码：
![](http://images.cnitblog.com/i/293735/201406/170217538798966.png)
有了索引，我们就可以搜索了：
![](http://images.cnitblog.com/i/293735/201406/170226067237333.png)
6 . 配置Maven使用私服
私服搭建成功，我们就可以配置 Maven 使用私服，以后下载构件、部署构件，都通过私服来管理。
在 settings.xml 文件中，为所有仓库配置一个镜像仓库，镜像仓库的地址即私服的地址（这儿我们使用私服公共仓库组 Public Repositories 的地址）：
![](http://images.cnitblog.com/i/293735/201406/170233181767611.png)
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    <mirrors>
            <mirror>
                <id>central</id>
                <mirrorOf>*</mirrorOf> <!-- * 表示让所有仓库使用该镜像--> 
                <name>central-mirror</name> 
                <url>http://localhost:8081/nexus/content/groups/public/</url>
            </mirror> 
    </mirrors>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
更多高级特性不再介绍（我也还不会）。
