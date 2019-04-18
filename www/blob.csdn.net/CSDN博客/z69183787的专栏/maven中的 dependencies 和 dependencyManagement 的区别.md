# maven中的 dependencies 和 dependencyManagement 的区别 - z69183787的专栏 - CSDN博客
2014年03月27日 12:35:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1273
今天我在配置 sellercenter 的接口测试环境的时候，发现一些依赖的写法不太一致：
比如有的依赖的<scope>是写在子项目中的 <dependencies> 下的<dependency> 标签中，
而有的依赖的<scope>是写在父项目中的<dependencyManagement> 中 。
我知道前一种写法是对的，而后一种写法却不知道对不对了，从网上查了下，没有找到非常确切的答案，于是自己验证了一把。
把验证过程给大家说下，大家也可以自己练手。
首先新建三个项目，Parent作为父项目、projectA、projectB作为子项目。
在父项目Parent中依赖项如下：
<dependencies>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<version>4.8.1</version>
<scope>test</scope>
</dependency>
</dependencies>
在子项目projectA、projectB中没有写任何依赖，在projectA 下运行命令 mvn help:effective-pom,会发现A下面有 junit 4.8.1的依赖。
如果我把 父项目Parent 中的依赖修改如下：
**<dependencyManagement>**<dependencies>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
<version>4.8.1</version>
<scope>test</scope>
</dependency>
</dependencies>
**</dependencyManagement>**
子项目ProjectA、projectB下面还是没有任何依赖项，在projectA 下运行命令 mvn help:effective-pom,会发现A下面 **没有** junit 4.8.1的依赖。
如果我在projectA 下添加junit的依赖：
<dependencies>
<dependency>
<groupId>junit</groupId>
<artifactId>junit</artifactId>
</dependency>
</dependencies>
再在projectA 下运行命令 mvn help:effective-pom,会发现A下面有了 junit 4.8.1的依赖，并且scope为 test。
那么经过验证，scope写在子项目中的<dependencies> 下的<dependency>中，或是写在父项目中的<dependencyManagement>中，都是可以的。
但有一点需要注意，dependencies 和 dependencyManagement 的区别在于：
前者，即使在子项目中不写该依赖项，那么子项目仍然会从父项目中继承该依赖项。
后者，如果在子项目中不写该依赖项，那么子项目中是不会从父项目继承该依赖项的；只有在子项目中写了该依赖项，才会从父项目中继承该项，并且version 和 scope 都读取自 父pom。
