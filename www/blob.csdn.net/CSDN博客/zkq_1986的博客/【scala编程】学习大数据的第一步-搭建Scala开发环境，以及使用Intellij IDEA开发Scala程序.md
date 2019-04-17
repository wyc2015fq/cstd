# 【scala编程】学习大数据的第一步-搭建Scala开发环境，以及使用Intellij IDEA开发Scala程序 - zkq_1986的博客 - CSDN博客





2018年01月02日 14:46:30[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1000












- [1为什么要学习Scala语言](http://blog.csdn.net/guiying712/article/details/68947747#1%E4%B8%BA%E4%BB%80%E4%B9%88%E8%A6%81%E5%AD%A6%E4%B9%A0scala%E8%AF%AD%E8%A8%80)
- [2Scala简介](http://blog.csdn.net/guiying712/article/details/68947747#2scala%E7%AE%80%E4%BB%8B)
- [3Scala在Windows系统上的安装及环境配置](http://blog.csdn.net/guiying712/article/details/68947747#3scala%E5%9C%A8windows%E7%B3%BB%E7%BB%9F%E4%B8%8A%E7%9A%84%E5%AE%89%E8%A3%85%E5%8F%8A%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE)- [1安装Java](http://blog.csdn.net/guiying712/article/details/68947747#1%E5%AE%89%E8%A3%85java)- [下载JDK](http://blog.csdn.net/guiying712/article/details/68947747#%E4%B8%8B%E8%BD%BDjdk)
- [配置环境变量](http://blog.csdn.net/guiying712/article/details/68947747#%E9%85%8D%E7%BD%AE%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F)
- [测试Java环境](http://blog.csdn.net/guiying712/article/details/68947747#%E6%B5%8B%E8%AF%95java%E7%8E%AF%E5%A2%83)

- [3安装Scala](http://blog.csdn.net/guiying712/article/details/68947747#3%E5%AE%89%E8%A3%85scala)- [下载Scala](http://blog.csdn.net/guiying712/article/details/68947747#%E4%B8%8B%E8%BD%BDscala)
- [配置环境变量](http://blog.csdn.net/guiying712/article/details/68947747#%E9%85%8D%E7%BD%AE%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F-1)
- [测试Scala环境](http://blog.csdn.net/guiying712/article/details/68947747#%E6%B5%8B%E8%AF%95scala%E7%8E%AF%E5%A2%83)


- [4使用Intellij IDEA写hello world](http://blog.csdn.net/guiying712/article/details/68947747#4%E4%BD%BF%E7%94%A8intellij-idea%E5%86%99hello-world)- [1安装Intellij IDEA](http://blog.csdn.net/guiying712/article/details/68947747#1%E5%AE%89%E8%A3%85intellij-idea)- [下载Intellij IDEA](http://blog.csdn.net/guiying712/article/details/68947747#%E4%B8%8B%E8%BD%BDintellij-idea)
- [配置Intellij IDEA](http://blog.csdn.net/guiying712/article/details/68947747#%E9%85%8D%E7%BD%AEintellij-idea)






# 1、为什么要学习Scala语言？
- 
结合Spark处理大数据 
这是Scala的一个主要应用，而且Spark也是那Scala写的。

- 
Java的脚本语言版 
可以直接写Scala的脚本，也可以在.sh直接使用Scala。

- 
代替Java 
Scala的编程风格更简洁，当然也很可能降低可读性，不过Java语言还是有其优势。


# 2、Scala简介

Scala有几项关键特性表明了它的面向对象的本质。例如，Scala中的每个值都是一个对象，包括基本数据类型（即布尔值、数字等）在内，连函数也是对象。另外，类可以被子类化，而且Scala还提供了基于mixin的组合（mixin-based composition）。

与只支持单继承的语言相比，Scala具有更广泛意义上的类重用。Scala允许定义新类的时候重用“一个类中新增的成员定义（即相较于其父类的差异之处）”。Scala称之为mixin类组合。

Scala还包含了若干函数式语言的关键概念，包括高阶函数（Higher-Order Function）、局部套用（Currying）、嵌套函数（Nested Function）、序列解读（Sequence Comprehensions）等等。

Scala是静态类型的，这就允许它提供泛型类、内部类、甚至多态方法（Polymorphic Method）。另外值得一提的是，Scala被特意设计成能够与Java和.NET互操作。Scala当前版本还不能在.NET上运行（虽然上一版可以-_-b），但按照计划将来可以在.NET上运行。

Scala可以与Java互操作。它用scalac这个编译器把源文件编译成Java的class文件（即在JVM上运行的字节码）。你可以从Scala中调用所有的Java类库，也同样可以从Java应用程序中调用Scala的代码。用David Rupp的话来说，

它也可以访问现存的数之不尽的Java类库，这让（潜在地）迁移到Scala更加容易。这让Scala得以使用为Java编写的巨量的Java类库和框架。

# 3、Scala在Windows系统上的安装及环境配置

Scala 语言是可以运行在Window、Linux、Unix、 Mac OS X等系统上，并且Scala是基于java之上，大量使用java的类库和变量，因此在使用Scala之前必须先安装 Java（>1.5版本），这篇文章只讲基于Windows系统安装Scala及环境配置。

由于这篇文章是在Windows系统中搭建开发环境的，因此需要你了解一些Windows环境变量的知识，方便你理解下面内容；Windows环境变量的知识请查看这篇文章：[Windows环境变量Path、ClassPath、JAVA_HOME的作用](http://blog.csdn.net/guiying712/article/details/51018368)。

## 1）安装Java

### 下载JDK

这个链接是Oracle官网下载JDK的地址[http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)，

![JDK下载地址](https://img-blog.csdn.net/20170304175509459?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

进入下载页面后，请根据你的系统自行下载32位或64位的版本，下载之前请先勾选“ Accept License Agreement ”。下载完成后直接点击你下载下来的文件安装，大家最好安装默认的安装步骤来，每一步都直接点击下一步，直到安装完成。

### 配置环境变量

找到你桌面上的“计算机”图标，点击右键，选择最下面的“属性”,进入系统页面，依次选择”高级系统设置“——-》”高级“——-》”环境变量“。

![这里写图片描述](https://img-blog.csdn.net/20170304180622444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

进入到这里后，我们主要在系统变量这里设置三个属性，分别为 JAVA_HOME、PATH、CLASSPATH，这个三个变量名的大小写都是正确的，当然了如果某个属性已经存在则点击”编辑“，不存在则点击”新建“。

现在开始设置第一个属性”JAVA_HOME“,这个属性在没有配置过JDK时，一般都是没有的：
- 变量名：JAVA_HOME 或者 java_home
- 变量值：C:\Program Files\Java\jdk1.8.0_91 （请选择你之前安装JDK的路径，这个路径是我电脑的）

![这里写图片描述](https://img-blog.csdn.net/20170304181457693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后是第二个属性“PATH”，这个属性一般肯定都是有的：
- 变量名：PATH
- 变量值：C:\Program Files\Java\jdk1.8.0_91\bin;C:\Program Files\Java\jre1.8.0_91\bin （请选择你之前安装JDK的路径，这个路径是我电脑的）

![这里写图片描述](https://img-blog.csdn.net/20170304182446372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后是第三个属性“CLASSPATH”，这个属性一般都是有的：
- 变量名：CLASSPATH
- 变量值：C:\Program Files\Java\jdk1.8.0_91\lib\dt.jar;C:\Program Files\Java\jdk1.8.0_91\lib\tools.jar （请选择你之前安装JDK的路径，这个路径是我电脑的）

![这里写图片描述](https://img-blog.csdn.net/20170304183229081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 测试Java环境

这三步做完后，Java环境就算是配置完了，但是配置完了并不代表JDK就能用，我们还需要测试下： 
点击系统的“开始按钮”，在开始菜单中的搜索框输入 “cmd”，按“回车键”，就会出现我们Windows系统的命令工具；

![cmd.exe](https://img-blog.csdn.net/20170304184130995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这个命令行工具中分别输入以下三个命令：java -version 、java 、 javac，如果出现下图的信息就说明Java环境配置成功了。

![java -version命令](https://img-blog.csdn.net/20170304184658593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![java命令](https://img-blog.csdn.net/20170304184941019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![javac命令](https://img-blog.csdn.net/20170304185010816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3）安装Scala

### 下载Scala

首先点击这个链接: [http://www.scala-lang.org/download/all.html](http://www.scala-lang.org/download/all.html)，进入Scala官网的下载页面，这里需要注意的是，在选择Scala版本是要选择Scala 2.11.0，因为最新的版本在IDEA中运行不起来，会报下面的错误：

![Scala版本的错误](https://img-blog.csdn.net/20170304185623381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下载完成后直接点击你下载下来的文件安装，大家最好安装默认的安装步骤来，每一步都直接点击下一步，直到安装完成。

### 配置环境变量

下载完成后还是需要配置Scala环境变量，同样需要先进入到“系统属性”中：

![这里写图片描述](https://img-blog.csdn.net/20170304185850133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

进入到这里后，我们主要在系统变量这里设置两个属性，分别为 SCALA_HOME、PATH，这个三个变量名的大小写都是正确的，当然了如果某个属性已经存在则点击”编辑“，不存在则点击”新建“。

现在开始设置第一个属性”SCALA_HOME“：
- 变量名：SCALA_HOME 或者 scala_home
- 变量值：C:\Program Files (x86)\scala （请选择你之前安装Scala的路径，这个路径是我电脑的）

![这里写图片描述](https://img-blog.csdn.net/20170304190338230?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后是第二个属性“PATH“：
- 变量名：PATH
- 变量值：C:\Program Files (x86)\scala\bin; （请选择你之前安装Scala的路径，这个路径是我电脑的），这里也可以是这个值：%SCALA_HOME%\bin;，意思是一样的。

![这里写图片描述](https://img-blog.csdn.net/20170304191007001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 测试Scala环境

这两步做完后，Scala环境就算是配置完了，但是配置完了并不代表就能用，我们还需要测试下：

依然打开命令工具，输入以下三个命令：scala，如果出现下图的信息就说明scala环境配置成功了。

![这里写图片描述](https://img-blog.csdn.net/20170304191537755?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 4、使用Intellij IDEA写hello world

Intellij IDEA是jetbrain开发的一个IDE，用于Java、Groovy、Scala等的开发，相对于其他开发工具来说，其对于代码检视、上下文理解、cvs/ant/maven/git的支持都比较完善。

## 1）安装Intellij IDEA

### 下载Intellij IDEA

首先点击这个链接: [https://www.jetbrains.com/idea/download/download-thanks.html](https://www.jetbrains.com/idea/download/download-thanks.html)，进入IntelliJ IDEA官网的下载页面，然后点击 右上角的 Download 按钮，下载完成后直接点击你下载下来的文件安装，大家最好安装默认的安装步骤来，每一步都直接点击下一步，直到安装完成。如果提示让你注册购买这个软件，请自行想办法，不知道如何就去度娘问问。

### 配置Intellij IDEA

安装完成后，打开Intellij IDEA，由于我已经运行过这个软件了，所以会跟你的有点差别，所以请无视左侧的黑色。首先点安装图上所示，选择 Configure 中的 Plugins ，

![Plugins](https://img-blog.csdn.net/20170304193237859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

再次点击下面的 Browse repositories 按钮，

![Browse repositories](https://img-blog.csdn.net/20170304193716961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

进入新页面后，在上方的搜索框中输入 Scala，选择Scala 这个插件，点击右侧边栏中的 Install Plugin 按钮，然后重启Intellij IDEA。

![这里写图片描述](https://img-blog.csdn.net/20170304193936557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

重启后，选择 Configure 中的 Setting 选项，进入Intellij IDEA的设置页面去修改IDE的Encoding，我们要将 IDE Encoding和 Project Encoding 以及最下方 的 Default Encoding for properties files 的属性都修改为 UTF-8 ，注意 ：如果不设置这里，程序将不能运行。然后点击右下方的 Apply 按钮和 OK 按钮，关闭设置页面。

![这里写图片描述](https://img-blog.csdn.net/20170304201727242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后选择 Create New Project 这个选项，进入如下的页面，继续选择 Scala 中的 Scala,然后点击 next ,进入工程配置页面，

![这里写图片描述](https://img-blog.csdn.net/20170304194658911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

请自行填写项目名称，选择项目存放的目录，以及添加之前安装的Java目录以及Scala目录，配置好这四个选项后，直接点击下面的 Finish 按钮，完成项目的创建。

![这里写图片描述](https://img-blog.csdn.net/20170304195427934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果你按照我说的步骤正确配置的话，那么相信你的工程已经创建成功了( Java的JDK以及Scala的SDK都已经正确的显示出来)，例如下图这样:

![这里写图片描述](https://img-blog.csdn.net/20170304195822411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从上图我们可以看到现在还没有源文件，只有一个存放源文件的目录src以及存放工程其他信息的文件，接下来我们需要创建的是入口类，即Object。

将鼠标放在 目录src 上，点击右键依次选择 New ——》 Scala Class ：

![这里写图片描述](https://img-blog.csdn.net/20170304200645641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

创建的是入口类，Name 填写“HelloWorld”，Kind 选择 Object，点击 OK，就将入口类创建成功了。

![这里写图片描述](https://img-blog.csdn.net/20170304200758518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后在新建的HelloWorld类中，输入如下代码：

```
object HelloWorld {

   /* 这是我的第一个 Scala 程序

    * 以下程序将输出'Hello World!' 

    */

   def main(args: Array[String]) {

      println("Hello, world!") // 输出 Hello World

   }

}
```

然后将鼠标放在目录HelloWorld文件上，点击右键，选择 Run “Hello World”：

![这里写图片描述](https://img-blog.csdn.net/20170304203110967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果最后运行结果如下，那么恭喜你，就说明你的第一个Scala程序就算是运行成功了。

![这里写图片描述](https://img-blog.csdn.net/20170304203127717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VpeWluZzcxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果在运作结果的时候出现了错误，请在将本文细读一次，在这篇文章中已经讲解了如何避免两个很严重的问题。

转载自： [http://blog.csdn.net/guiying712/article/details/68947747](http://blog.csdn.net/guiying712/article/details/68947747) ，本文出自:【张华洋的博客】







