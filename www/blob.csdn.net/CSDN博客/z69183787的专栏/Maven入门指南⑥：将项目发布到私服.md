# Maven入门指南⑥：将项目发布到私服 - z69183787的专栏 - CSDN博客
2016年05月17日 11:01:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1549
# [Maven入门指南⑥：将项目发布到私服](http://www.cnblogs.com/luotaoyeah/p/3817465.html)
1 . 修改私服中仓库的部署策略
　　Release版本的项目应该发布到Releases仓库中，对应的，Snapshot版本应该发布到Snapshots仓库中。Maven根据pom.xml文件中版本号<version>节点的属性是否包含-SNAPSHOT，来判断该项目是否是snapshot版本。如果是snapshot版本，在执行mvn deploy部署命令时，maven会自动将项目发布到Snapshots仓库。要发布项目，首先需要将Releases仓库和Snapshots仓库的“Deployment
 Policy”设置为“Allow Redeploy”：
![](http://images.cnitblog.com/i/293735/201406/302236089652297.png)
2 . 配置项目的部署仓库
在pom.xml中分别对Release版本和Snapshot版本配置部署仓库，其中id唯一，url分别对应私服中Releases和Snapshots仓库的Repository Path：
![](http://images.cnitblog.com/i/293735/201406/302252426059391.png)
<uniqueVersion>表示是否为Snapshot版本分配一个包含时间戳的构建号，效果如下：
![](http://images.cnitblog.com/i/293735/201406/302304059962571.png)
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    <distributionManagement>
        <snapshotRepository>
            <id>user-snapshot</id>
            <name>User Porject Snapshot</name>
            <url>http://localhost:8081/nexus/content/repositories/snapshots/</url>
            <uniqueVersion>true</uniqueVersion>
        </snapshotRepository>
        <repository>
            <id>user-release</id>
            <name>User Porject Release</name>
            <url>http://localhost:8081/nexus/content/repositories/releases/</url>
        </repository>
    </distributionManagement>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
3 . 启用Deployment用户
Nexus默认有三个用户，其中Deployment用户用于部署项目：
![](http://images.cnitblog.com/i/293735/201406/302255399651911.png)
Deployment用户默认密码为deployment123，右键菜单可修改或重置密码：
![](http://images.cnitblog.com/i/293735/201406/302257306847235.png)
在settings.xml中分别为上面配置的部署仓库配置server，其中id需要分别对应上面的部署仓库id：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    <servers>
        <server>
            <id>user-release</id>
            <username>deployment</username>
            <password>deployment123</password>
        </server>
        <server>
            <id>user-snapshot</id>
            <username>deployment</username>
            <password>deployment123</password>
        </server>
        …        
    </servers>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4 . 发布项目
　　右键pom.xml - Run As - 2 Maven build...
![](http://images.cnitblog.com/i/293735/201406/302309511055471.png)
发布成功后，在私服的仓库中就能看到了：
![](http://images.cnitblog.com/i/293735/201406/302311224183467.png)
5 . 在Nexus中手动上传项目构件
　　在Nexus仓库的Artifact Upload选项卡中，填写相关信息，可以手动的方式上传项目构件：
![](http://images.cnitblog.com/i/293735/201406/302318276523412.png)
