# Maven最佳实践：Maven仓库 - 续 - z69183787的专栏 - CSDN博客
2016年06月08日 10:44:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：485
**分发构件至远程仓库**
mvn install 会将项目生成的构件安装到本地Maven仓库，mvn deploy 用来将项目生成的构件分发到远程Maven仓库。本地Maven仓库的构件只能供当前用户使用，在分发到远程Maven仓库之后，所有能访问该仓库的用户都能使用你的构件。
我们需要配置POM的distributionManagement来指定Maven分发构件的位置，如下：
Xml代码 
- <settings>
-   ...    
- <servers>
- <server>
- <id>nexus-releases</id>
- <username>admin</username>
- <password>admin123</password>
- </server>
- <server>
- <id>nexus-snapshots</id>
- <username>admin</username>
- <password>admin123</password>
- </server>
- </servers>
-   ...    
- </settings>
需要注意的是，settings.xml中server元素下id的值必须与POM中repository或snapshotRepository下id的值完全一致。将认证信息放到settings下而非POM中，是因为POM往往是它人可见的，而settings.xml是本地的。
## 小结
本文介绍了Maven仓库，它是什么？本地仓库，远程仓库，中央仓库具体是指什么？并介绍了如何在POM中配置项目层次的仓库，在settings中配置用户层次的仓库，以及mirror。本文还介绍了如何安装构件到本地仓库，如何分发构件至仓库。
