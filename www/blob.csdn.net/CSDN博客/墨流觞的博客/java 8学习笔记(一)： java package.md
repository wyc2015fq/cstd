# java 8学习笔记(一)： java  package - 墨流觞的博客 - CSDN博客





2018年09月24日 10:44:27[墨氲](https://me.csdn.net/dss_dssssd)阅读数：42标签：[java																[jdk8](https://so.csdn.net/so/search/s.do?q=jdk8&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java学习笔记](https://blog.csdn.net/dss_dssssd/article/category/8080136)








好吧，我要立个flag，每天拿出30分钟学java，学习书籍为 Begining Java 8 fundmentals，这个系列我想一直坚持下去呢！！！

java 代码组成：
- 0 或 1个paxkage声明
- 0,1或多个import声明
- 0,1或多个type生命： class，interface或enum声明

![在这里插入图片描述](https://img-blog.csdn.net/20180924093843443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### package 声明

语法：
$package &lt;your-package-name&gt;$

一个包声明包含package和用户提供的包名；package和包名之间用一个或多个空白符（spaces,tabs,new lines ）隔开；在包声明最后接一个分号(;)表示语句结束。

下面是一个例子：
![在这里插入图片描述](https://img-blog.csdn.net/20180924095142889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一个包的名字可能包含一个或多个部分，在这个例子中，包名由三部分组成：com,jdojo,intro。包名的两部分之间用dot(.)隔开。在一个包中，对于部件（parts）的数量没有限制[there is no limit on the number of parts in a package name].但如果java代码中出现了包声明，则包名中至少有一个部件。下列是一些有效的包声明。

```java
```java
package intro;
package com.jdojo.intro.com
package com.ksharan
package com.jdojo.intro
```
```

为什么要使用包声明呢?

软件包是java类型（class，interface，enum）的逻辑存储库，换句话说，它为相关的java类型提供了一个逻辑分组。【其实我也没太懂】

##### 包存储

一个包可以存储在特定主机的文件系统中，数据库中，或者网络上（host-speifc file system, database, network location）。

在一个文件系统上，包名的每一部分代表主机上的一个目录，比如`com.jdojo.intro`表示windows上的`com\jdojo\intro\<<class File Name>>`,其中在`intro`包含编译过的java源代码。

**注意:** 此时包名只是包含编译过java程序（类文件）的相对路径，并不能显示类文件所处的绝对路径。

所以得需要一个环境变量（environment variables），CLASSPATH来获得包名的绝对路径

包名是可选的，如果你没有选择在java代码中添加包声明，那你的java程序属于哪个逻辑库呢？

一个不包含包声明的java程序（更严格的说，是java type），据说属于一个未命名包(也称默认包)，（A java program(strictly speaking, a java type), which does not have a package declaration, is said to be a part of an unnamed package(also called default package)）.这个后边会再讨论。

java代码区分大小写（case sensitive）,所以关键字package必须是全小写，注意包名也是区分大小写的。

在java程序中，包声明是很简单但也很重要的部分。推荐在代码中总是使用一个包声明。典型而言，包名都是以公司的反向域名（reverse domin name）开头，比如雅虎公司：com.yahpp 。如果你没有自己的域名，想一个独一无二的名字吧。





