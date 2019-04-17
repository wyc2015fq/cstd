# java 8 学习笔记（二）: java import - 墨流觞的博客 - CSDN博客





2018年09月25日 00:06:14[墨氲](https://me.csdn.net/dss_dssssd)阅读数：24








在java源代码中，import声明是可选的。那为何要在java中引入import声明呢？

it makes your life easier, 可以少写一些代码，并且使你的代码更整洁和易读。

在import声明里，你会告知java编译器，你从某一特定的软件包里引入一个或多个类。无论何时你用一个类型（类， 接口或枚举），你都需要使用完全限定名（fully qualified name），而 import后，则可以使用简单的名字，关于（fully qualified name ）和（simple name）后边会有介绍

下面是两个包声明：

```java
```java
import com.jdodo.intro.Account
 import com.jdojo.util.*
```
```

在classes and objects 会更加详细的介绍 import，在这只是简单的介绍import声明各部分的含义。
- 以关键字import 开始
- 第二部分有两部分组成
- 你想使用的类所在的软件包（package）
- 一个类名或者星号（*）来说明你想使用软件包中的一个或多个类

- 最后以一个分号（；）结束

按照此规则介绍上述两个import声明
- 想使用软件包com.jdojo.introde 一个叫Account的类
- 可以使用任何在som.jdojo.util包中的class、interface和enum

比如你想使用com.jdojo.commom包中的Person类，你需要以下两种声明中的一种

```java
```java
import com.jdojo.commom.Person

import com.jdojo.commom.*
```
```

注意一下两种import声明不能使用在com和com.jdojo中的类

```java
```java
import com.jdojo.intro.Account
import com.jdojo.intro.*
```
```



