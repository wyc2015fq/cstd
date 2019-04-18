# Maven学习八：多模块依赖 - z69183787的专栏 - CSDN博客
2014年03月27日 16:08:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1469
使用Maven以后，可以更方便的进行构件化开发，一般项目中存在多模块，它们的关系，包括父子关系以及依赖关系，都可以通过pom文件的配置来实现的。
## 1. 父子关系
      一个多模块项目通过一个父POM 引用一个或多个子模块来定义。
      a. 在父目录中分别创建子项目
      b. 父项目，通过以下配置，将子项目关联：
如下：
- **<****modules****>**
- **<****module****>**myapp-client**</****module****>**
- **<****module****>**myapp-core**</****module****>**
- **<****module****>**myapp-webapp**</****module****>**
- **</****modules****>**
     其中：在modules元素中，每个modules 元素对应了一个 父目录下的子目录。Maven会去这些子目录中寻找pom.xml 文件，并且在构建mydemo时会将这些子模块包含到要构建的项目中。
    另外，通过配置<packaging>pom</packaging> 表示这个父项目不像之前的项目那样创建一个JAR 或者一个WAR，它仅仅是一个引用其它Maven 项目的POM。pom.xml 中下一部分列出了项目的子模块。
    c. 在子项目的POM中添加父项目关联：
如下：
- **<****parent****>**
- **<****groupId****>**com.china.demo**</****groupId****>**
- **<****artifactId****>**myapp-parent**</****artifactId****>**
- **<****version****>**1.0**</****version****>**
- **</****parent****>**
## 2. 依赖关系
    如果项目间存在依赖关系，如项目A依赖项目B，则需要在项目A的pom增加了对B的依赖节点<dependency>，表示A项目中需要B的jar包。
