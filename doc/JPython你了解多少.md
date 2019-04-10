# JPython你了解多少

©   Python    /    2017-07-20

概述



可能很多人都听说过 Jython，或者也叫 **JPython**，如果没听过的话，我大概介绍一下，Jyhton 就是在 Java 虚拟机(JVM) 上运行 python 代码的一种语言，代码是用 python 编写的，然后编译成 JVM 可以解析的字节码，运行在 JVM 上。

很明显，Jython 是给 Java程序员 运行 Python 程序用的，如果是 Python程序员 想运行 Java代码 怎么办？方式还是很多的，有 JPype/JEP/JPE 等等，这些项目都可以在 Python运行环境执行 Java代码，那么，本文就以 JPype 为主题，介绍一下如何在 Python代码中运行 Java 代码，包括但不限于直接编写 Java代码，加载jar包 和 执行jar包。

环境介绍

在开始之前，先介绍一下本文使用的 Python 和 Java 运行环境

Java 版本

在本文中，我运行的 Java 版本是 1.8，是 Oracle 的 Hotspot 版，不是 OpenJDK，但是对于本文的内容，OpenJDK 也适用，亲测。

\> java -version

java version "1.8.0_45"

Java(TM) SE Runtime Environment (build 1.8.0_45-b14)

Java HotSpot(TM) 64-Bit Server VM (build 25.45-b02, mixed mode)

Python 版本



在本文中，我使用的 Python 版本为 3.5.2，这里有个很尴尬的问题，Python 不得不吐槽的一点就是版本太乱，2.6, 2.7, 3.4/3.5 这三个版本应该是比较多人用的，但是不兼容！！，而且大部分系统默认依赖的是 2.7，所以当你在项目使用时，一定要注意版本的选择，我目前推荐 3.5。

但是，不用担心，对于本文来说，JPype 是支持 2.7 和 3.5 的，所以，你使用本文的代码是完全可以在 2.7 上跑通的，而且更舒适的是在 2.7 上安装 JPype 比 3.5 上方便多了。

安装依赖

在 Python 3.5 中，要安装 JPype 你不能直接使用 pip3 安装，因为 JPype 默认是为 Python2 编写的（其实是开发那时Python3还未进入主流，并且早在 2009 年就断更了），但是，不要紧张，有专业人士为 Python3 编写了另外一个兼容的版本，所以我们需要从源码安装了，具体步骤为：



\> git clone https://github.com/tcalmant/jpype-py3.git

\> cd jpype-py3

\> python3 setup.py install

然后就会编译安装了，正常的话是可以顺利安装成功地，这样安装也就完事了。



如果编译出错的话，请检查一下你的系统是否缺乏 python3-dev 库或者根本没有安装 C++ 编译工具，如果在 Linux 的话，你可以直接敲这样的命令：



\> sudo apt-get install g++ python3-dev

如果还失败的话，那么可能你需要制定一下 JAVA_HOME，在尝试安装：



这里的 JAVA_HOME 需要根据你的 JDK 情况进行修改，千万不要照抄！！！



\> JAVA_HOME=/usr/lib/jvm/java-1.7.0-openjdk-amd64 **python3** setup.py install

如果还有问题，那我就没办法了，来项目的 GITHUB 提 ISSUE 吧。

安装完了之后，我们就要开始写代码了，下面我将会介绍一些常用的套路，依照这些套路，你应该可以顺利得完成大部分的工作需求了。



本文版权归老男孩教育所有，欢迎转载，转载请注明作者出处。谢谢！

作者：老男孩教育

**首发：http://www.oldboyedu.com/XinXi/**