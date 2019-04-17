# eclipse使用maven教程 - 博客堂 - CSDN博客





2017年12月27日 20:41:36[最小森林](https://me.csdn.net/u012052268)阅读数：45118








# eclipse使用maven教程



- [eclipse使用maven教程](#eclipse使用maven教程)- [什么是maven](#0什么是maven)
- [maven下载与安装与配置](#1maven下载与安装与配置)- [1下载](#11下载)
- [2安装](#12安装)
- [3配置环境变量](#13配置环境变量)

- [在eclipse中配置](#2在eclipse中配置)- [1 m2eclipse安装](#21-m2eclipse安装)
- [2配置eclipse](#22配置eclipse)

- [3创建一个maven项目](#3创建一个maven项目)- [1项目的结构](#31项目的结构)
- [2Eclipse中maven常用的命](#32eclipse中maven常用的命)
- [3使用maven命令](#33使用maven命令)






## 0.什么是maven

参考 [https://www.cnblogs.com/whgk/p/7112560.html](https://www.cnblogs.com/whgk/p/7112560.html)

我们开发一个项目，或者做一个小demo，比如用SSH框架，那么我们就必须将SSH框架所用的几十个依赖的jar包依次找出来并手动导入，超级繁琐。　

　　　　上面问题的描述，其实都属于项目与项目之间依赖的问题[A项目使用SSH的所有jar，就说A项目依赖SSH]，人为手动的去解决，很繁琐，也不方便，所以使用maven来帮我们管理

Maven是基于项目对象模型(POM project object model)，可以通过一小段描述信息（配置）来管理项目的构建，报告和文档的软件项目管理工具[百度百科]

　　　　这种又是大白话，如果没明白maven是什么，那么上面这句话跟没说一样，我自己觉得，Maven的核心功能便是合理叙述项目间的依赖关系，通俗点讲，就是通过pom.xml文件的配置获取jar包，而不用手动去添加jar包，而这里pom.xml文件对于学了一点maven的人来说，就有些熟悉了，怎么通过pom.xml的配置就可以获取到jar包呢？pom.xml配置文件从何而来？等等类似问题我们需要搞清楚，如果需要使用pom.xml来获取jar包，那么首先该项目就必须为maven项目，maven项目可以这样去想，就是在java项目和web项目的上面包裹了一层maven，本质上java项目还是java项目，web项目还是web项目，但是包裹了maven之后，就可以使用maven提供的一些功能了(通过pom.xml添加jar包)。

　　　　所以，根据上一段的描述，我们最终的目的就是学会如何在pom.xml中配置获取到我们想要的jar包，在此之前我们就必须了解如何创建maven项目，maven项目的结构是怎样，与普通java,web项目的区别在哪里，还有如何配置pom.xml获取到对应的jar包等等，这里提前了解一下我们如何通过pom.xml文件获取到想要的jar的，具体后面会详细讲解该配置文件。

![image](http://images2015.cnblogs.com/blog/874710/201707/874710-20170703212012128-414258282.png)

为什么通过groupId、artifactId、version三个属性就能定位一个jar包？

　　　　　　　　加入上面的pom.xml文件属于A项目，那么A项目肯定是一个maven项目，通过上面这三个属性能够找到junit对应版本的jar包，那么junit项目肯定也是一个maven项目，junit的maven项目中的pom.xml文件就会有三个标识符，比如像下图这样，然后别的maven项目就能通过这三个属性来找到junit项目的jar包了。所以，在每个创建的maven项目时都会要求写上这三个属性值的。 

![image](http://images2015.cnblogs.com/blog/874710/201707/874710-20170703212837175-1626386029.png)

## 1.maven下载与安装与配置

在最新的Eclipse中自带了maven，我们打开，Windows->Preferences,如果会出现下面的画面： 
![image](http://images.cnitblog.com/i/255323/201404/041555124684257.jpg)
值得注意的是Embedded内置的默认maven版本是我们即将要修改的，应为这个版本有点老了，这直接导致的后果就是在添加dependences的时候，maven的库不是最新的。

### 1.1下载

去maven的官网下载最新版的maven，此时的最新版本为3.5.2,下载地址如下：[http://www-eu.apache.org/dist/maven/maven-3/3.5.2/binaries/apache-maven-3.5.2-bin.zip](http://www-eu.apache.org/dist/maven/maven-3/3.5.2/binaries/apache-maven-3.5.2-bin.zip)。

![image](http://images2015.cnblogs.com/blog/879896/201604/879896-20160422224122257-1646031907.jpg)

解压到本地，安装Maven前，先确保已安装JDK，CMD下输入java -version验证是否已安装JDK，如下为我本机的JDK版本： 
![image](http://images2015.cnblogs.com/blog/879896/201604/879896-20160422224613116-109817766.jpg)
### 1.2安装

解压到本地目录，比如我的环境中，在本地目录C:\Program Files\Java\apache-maven-3.5.2中.

### 1.3配置环境变量

在下面配置一下C:\Program Files\Java\apache-maven-3.5.2\conf目录中的settings.xml文件，更改一下如下配置：　

```xml
<!-- localRepository
   | The path to the local repository maven will use to store artifacts.
   |
   | Default: ${user.home}/.m2/repository  -->
  <localRepository>C:\Program Files\Java\apache-maven-3.5.2\m2\repository</localRepository>
```

如果不改的话就在用户目录。

还有个问题，可能有人说需不需要设置maven的环境变量，可以设置，但是如果只是使用Eclipse的maven的插件而不需要使用命令行，就不用了。反正我有设置。

设置环境变量：

MAVEN_HOME变量设置为C:\Program Files\Java\apache-maven-3.5.2，即下载下来的maven包解压路径；

另外在PATH变量前面增加%MAVEN_HOME%\bin; 
![image](http://images2015.cnblogs.com/blog/879896/201604/879896-20160422225120788-1165144654.jpg)
最后检验配置是否成功：用win键+R，来打开命令行提示符窗口，即Dos界面，输入mvn –version  若出现以下情况说明配置成功![image](http://images2015.cnblogs.com/blog/697835/201512/697835-20151218175625427-628957202.png)

## 2.在eclipse中配置

### 2.1 m2eclipse安装

我本机使用的Eclipse版本已经集成m2eclipse，如果没有集成的话，安装也很简单，有两种方式，

从Eclipse Marketplace上获取m2eclipse并安装：

首先要在Eclipse中安装插件：在Help–>install new software,输入下面地址[http://download.eclipse.org/technology/m2e/releases](http://download.eclipse.org/technology/m2e/releases)，出现如下页面：![image](http://images.cnitblog.com/i/255323/201404/041546144066670.jpg)

然后就像安装其他插件那样一路下一步就OK了。 


这个就是eclipse管理maven的插件。
### 2.2配置eclipse

安装好m2eclipse后，一般不使用eclipse自带的maven，而是使用外部我们自己安装的maven版本，

因为Embedded内置的默认maven版本是我们即将要修改的，这个版本有点老了，这直接导致的后果就是在添加dependences的时候，maven的库不是最新的。

操作：在eclipse上只需作如下配置即可，如下图 
![image](http://images2015.cnblogs.com/blog/879896/201604/879896-20160422231250335-1879951237.jpg)
然后设置用户settings，这是maven下载库的地址。如下图： 
![image](http://images.cnitblog.com/i/255323/201404/041617196406498.jpg)
## 3创建一个maven项目

在实际操作中学习maven及其命令操作。

1在创建向导中多了maven的相关选项：![image](http://images.cnitblog.com/i/255323/201404/041621087187813.jpg)

2 选择默认的工程位置![image](http://images.cnitblog.com/blog/449064/201412/171936158127071.jpg)

3 创建工程，红色是创建普通的maven工程的。粉色是创建web工程的。![image](http://images.cnitblog.com/blog/449064/201412/171938147348302.jpg)

4 填写相关的groupId artifactId version等信息 
![image](http://images.cnitblog.com/blog/449064/201412/171939190941366.jpg)
点击完成就创建好了一个简单的maven工程。

### 3.1项目的结构

maven的工程一般都是如下的结构：

![image](http://dl.iteye.com/upload/attachment/491385/991a0487-74dd-35e4-9ef3-63e8be5a67c9.jpg)

pom.xml：用于定义或者添加jar包的依赖

src-main：用于存放java源文件

src-test：用于存放测试用例。

　　也许工程下还会出现target文件夹，这个是用来生成对应的class文件或发布的jar包。

### 3.2Eclipse中maven常用的命

Eclipse中maven常用的命令， 

　　点击Run As就可以发现几个Maven的命令：

> 
　　Maven Build:


　　这个命令用于编译Maven工程，执行命令后会在target文件夹中的classes中生成对应的class文件。

使用Build的时候，会让你选此次build的目标goal，下附主要的goal选项：(左边这列是步骤名，同时也是phase的名字，右边是对应的goal)：
|步骤名|对应的goal|
|----|----|
|process-resources|resources:resources|
|compile|compiler:compile|
|process-test-resources|resources:testResources|
|test-compile|compiler:testCompile|
|test|surefire:test|
|package|jar:jar|

> 
　　Maven Clean:


　　删除target文件夹，即删除生成的package包以及class等文件。

> 
　　Maven Test:


　　先自动进行编译，在运行所有的测试用例。

> 
　　Maven install:


　　发布生成对应的package包。

　注意：

　　留意上面的命令的作用，build和test都会生成相应的class文件。也就是说，当你新建一个maven工程，或者clean一个maven工程后，如果没有使用这两个命令，直接针对类进行测试，会跑出java.class.notfound的错误。因为此时还没有编译生成class文件，如果有朋友发现这个问题注意一下就好了。（该部分只针对旧版本，新版本如maven3.3是可以自动编译的）

　　只有使用了上面的两个命令后，才能针对某个类进行单元测试。

### 3.3使用maven命令

现在eclipse可以使用自带的run as来进行maven但是对于linux来说还是命令好用，下面通过一个例子演示maven命令的使用。

```
编译：mvn compile　　--src/main/java目录java源码编译生成class （target目录下）

测试：mvn test　　　　--src/test/java 目录编译

清理：mvn clean　　　 --删除target目录，也就是将class文件等删除

打包：mvn package　　--生成压缩文件：java项目#jar包；web项目#war包，也是放在target目录下

安装：mvn install　　　--将压缩文件(jar或者war)上传到本地仓库

部署|发布：mvn deploy　　--将压缩文件上传私服
```

下面我们看项目代码：

一个简单的类com.liuyan.maven.helloword.HelloWorld

```java
package com.liuyan.maven.helloword;

public class HelloWorld {
    public String sayHello() {
        return "hello maven";
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        System.out.println(new HelloWorld().sayHello());
    }

}
```

在此项目的根目录下有一个pom.xml文件，内容是默认的，暂时不用管。

进入控制台，将当前目录cd到与pom.xml同级的文件夹下面（或者使用shift进入）。执行编译命令：
`mvn clean compile`
执行后造成的结果。就是项目的根路径下出现了一个taget文件夹

![image](http://dl.iteye.com/upload/attachment/491387/93df452b-3bef-3236-bbb1-aefefd16f26f.jpg)

里面就是编译后的class类。经理过来说，你需要进行单元测试才能发布出来给大家用。Ok，我们在源码包——src/test/java下面开始编写junit单元测试类。

单元测试代码如下

```java
package com.liuyan.maven.helloword;

import org.junit.Test;
import org.junit.Assert;

public class TestHelloWorld {

    @Test
    public void testSayHello() {
        HelloWorld helloWorld = new HelloWorld();
        Assert.assertEquals(helloWorld.sayHello(), "hello maven");
    }

}
```

之后我们因为使用了junit单元测试，那么就是说我们这个项目依赖了它。修改一下pom.xml文件内容，如下

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.liuyan.maven</groupId>
    <artifactId>MavenDemo</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <dependencies>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.7</version>
            <scope>test</scope>
        </dependency>
    </dependencies>
</project>
```

进入命令行，执行命令
`mvn clean test`
执行后观察一下taget文件夹如下图，多出了test-classes文件夹和另外2个咱们暂时不用去管的文件夹。

之后在观察一下本地的临时仓库C:\Users\liuyan.m2\repository，会多出文件夹junit，下载的版本是4.7。

如果我们想把项目打成jar的形式输出出去呢？在项目根目录下执行
`mvn clean package`
执行后效果如下

![image](http://dl.iteye.com/upload/attachment/491391/61e2d0db-658a-3002-9d08-f387499b4775.jpg)

生成了一个jar包，至于SNAPSHOT是快照的意思，快照就是项目暂时还不稳定的意思。

打包测试后没问题了，想把此项目当做Maven的本地资源仓库，为其他的项目也能提供服务，可以这么做。

执行命令
`mvn clean install`
执行后本地的临时库文件多了你自己的这个项目。这样别的项目需要你这个项目提供服务的时候就可以从本地Mavne库中提取相应的jar了。











